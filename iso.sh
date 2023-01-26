#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/newtonix.kernel isodir/boot/newtonix.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "newtonix" {
	multiboot /boot/newtonix.kernel
}
EOF
grub-mkrescue -o newtonix.iso isodir
