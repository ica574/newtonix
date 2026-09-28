# Introduction

*Newtonix* is an elementary operating system built from tutorials across the internet. I am working on it primarily to learn more about low-level programming, however, it may be developed further into a mature operating system in the future. The design is meant to be UNIX-like, for stability and portability purposes.

## Target Platform

The operating system is primarily designed for *i686* processors and *legacy BIOS* systems, therefore, it is to be built using a cross-compiler, and a legacy version of *GRUB* as a bootloader.

### Pre-requisites

To build *Newtonix*, it is important to have a version of ```i686-elf-gcc```, and ```i686-elf-as``` installed. These could be made by downloading and compiling source files from the *GCC*, and *Binutils* websites, however, a quicker way would be to install these from *Debian* repositories. 

It is also important to have ```grub-pc-bin```, especially if you have a *UEFI* system. ```xorriso``` and ```qemu``` are needed to create a disc image and emulate *i686* on your host computer.

***NOTE:*** If installed from *Debian* repositories, a *symlink* must be created in the host's ```/bin``` from the 'apt-installed' cross-compilers ```i686-linux-elf-gcc```, and ```i686-linux-elf-as``` to ```i686-elf-gcc```, and ```i686-elf-as```. This is necessary to avoid issues with the *make* script.

### Building and Running

The simplest local workflow requires only Docker:

```sh
make setup   # Build the reusable toolchain container once
make build   # Compile the kernel and create newtonix.iso
make run     # Boot with serial output; press Ctrl-C to stop
make run-vga # Optional VGA/curses display
make clean
```

`make run` prints kernel serial output in the terminal. Press `Ctrl-C` to stop
QEMU. `make run-vga` is available for inspecting the VGA console, but its
terminal key handling varies between platforms.
With a native i686 cross-compiler, GRUB, xorriso, and QEMU installation, the
same build graph can be used directly with `make newtonix.iso`.

## Objectives

It would be splendid to have a version of *GCC* ported to *Newtonix*, however, this must succeed in getting all fundamental system functions like memory management in working order. After this is accomplished, the OS can then be ported to more modern platforms, such as *x86_64*, and *aarch64*, with *UEFI* firmware. Initially, however, the system will be designed on more simple, legacy hardware.

## Sources

Here are some of the websites I'm using to build this project:

* [OSDev Wiki](https://wiki.osdev.org/)
* [The little book about OS development](https://littleosbook.github.io/)
* [Bona Fide OS Developer](http://www.osdever.net/)
