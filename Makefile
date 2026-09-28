TOOLCHAIN_IMAGE := newtonix-toolchain:local
DOCKER_RUN := docker run --rm --platform linux/amd64 \
	-v "$(CURDIR):/work" -w /work $(TOOLCHAIN_IMAGE)

HOST ?= $(shell ./scripts/default-host.sh)
HOSTARCH := $(shell ./scripts/target-triplet-to-arch.sh $(HOST))

PREFIX := /usr
BOOTDIR := /boot
LIBDIR := $(PREFIX)/lib
INCLUDEDIR := $(PREFIX)/include
SYSROOT := $(CURDIR)/sysroot

AR := $(HOST)-ar
AS := $(HOST)-as
CC := $(HOST)-gcc --sysroot=$(SYSROOT)
CFLAGS := -O2 -g -fno-pie
CPPFLAGS :=
LDFLAGS := -no-pie -Wl,--build-id=none

ifneq ($(filter %-elf,$(HOST)),)
CC += -isystem=$(INCLUDEDIR)
endif

export HOST AR AS CC CFLAGS CPPFLAGS LDFLAGS
export PREFIX BOOTDIR LIBDIR INCLUDEDIR SYSROOT

HEADER_SOURCES := $(shell find libc/include kernel/include -type f)
LIBC_SOURCES := $(shell find libc -type f \( -name '*.c' -o -name '*.S' \))
KERNEL_SOURCES := $(shell find kernel -type f \( -name '*.c' -o -name '*.S' -o -name '*.ld' \))

.PHONY: setup build run run-vga clean clean-internal

setup:
	docker build --platform linux/amd64 \
		-f Dockerfile.toolchain -t $(TOOLCHAIN_IMAGE) .

build: setup
	$(DOCKER_RUN) make --no-print-directory HOST=i686-linux-gnu newtonix.iso

run: build
	docker run --rm -it --platform linux/amd64 \
		-v "$(CURDIR):/work" -w /work $(TOOLCHAIN_IMAGE) \
		qemu-system-i386 -cdrom newtonix.iso -boot d \
		-display none -serial stdio -no-reboot

run-vga: build
	docker run --rm -it --platform linux/amd64 \
		-v "$(CURDIR):/work" -w /work $(TOOLCHAIN_IMAGE) \
		qemu-system-i386 -cdrom newtonix.iso -boot d \
		-display curses -serial none -no-reboot

build/.headers.stamp: $(HEADER_SOURCES) libc/Makefile kernel/Makefile
	mkdir -p build
	$(MAKE) -C libc DESTDIR=$(SYSROOT) install-headers
	$(MAKE) -C kernel DESTDIR=$(SYSROOT) install-headers
	touch $@

sysroot/usr/lib/libk.a: build/.headers.stamp $(LIBC_SOURCES) libc/Makefile libc/arch/$(HOSTARCH)/make.config
	$(MAKE) -C libc DESTDIR=$(SYSROOT) install-libs

kernel/newtonix.kernel: sysroot/usr/lib/libk.a $(KERNEL_SOURCES) kernel/Makefile kernel/arch/$(HOSTARCH)/make.config
	$(MAKE) -C kernel newtonix.kernel
	mkdir -p $(SYSROOT)$(BOOTDIR)
	cp $@ $(SYSROOT)$(BOOTDIR)/newtonix.kernel

newtonix.iso: kernel/newtonix.kernel grub.cfg
	mkdir -p isodir/boot/grub
	cp kernel/newtonix.kernel isodir/boot/newtonix.kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

clean: setup
	$(DOCKER_RUN) make --no-print-directory HOST=i686-linux-gnu clean-internal

clean-internal:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	rm -rf -- build sysroot isodir newtonix.iso
