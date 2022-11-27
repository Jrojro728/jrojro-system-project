clang Kernel.c -nostdlib -ffreestanding -o Kernel.elf -e main
cp /home/jrojro/src/JrojroSystemKernel/Kernel.elf /home/jrojro/src/ovmf/esp/Kernel.elf
qemu_ovmf