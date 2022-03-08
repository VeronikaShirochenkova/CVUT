#!/bin/sh
git clone https://git.busybox.net/busybox   # clone busybox from git
cd busybox/                                 # go to busybox folder
make defconfig                              # configuration
make                                        # compilation
cd ../                                      # go back

git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git  # clone Linux kernel codes
cd linux/                                                                   # go to Linux folder
make defconfig                                                              # configuration
make -j$(nproc)                                                             # compilation
cd ../                                                                      # go back

cd busybox/                                 # go to busybox folder
make install                                # install busybox file system

mkdir _install/lib                                  # make directory
mkdir _install/lib64                                # make directory
cp /lib/x86_64-linux-gnu/libm.so.6 /lib/x86_64-linux-gnu/libresolv.so.2 /lib/x86_64-linux-gnu/libc.so.6 /lib64/ld-linux-x86-64.so.2 _install/lib
cp /usr/lib64/ld-linux-x86-64.so.2 _install/lib64   # copy libraries

# create a RAM disk
mkdir _install/{dev,etc,proc,sys}                           
sudo cp -a /dev/tty? _install/dev                           
ln -s bin/busybox _install/init
(cd _install; find . | cpio -o -H newc | gzip) > ramdisk

cd ../                                                                          # go back
qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage -initrd busybox/ramdisk  # run emulator
