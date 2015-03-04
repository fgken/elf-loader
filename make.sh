set -e

echo "Build ELF Loader with "$1

cp $1 elfimage
mips64-linux-objcopy -I binary -O elf64-tradbigmips -B mips:octeon elfimage elfimage.o
mips64-linux-gcc -g -O0 -march=octeon -mips64r2 -mno-abicalls -nostdlib -mabi=64 -Telf64btsmip.x head.S elf_loader.c elfimage.o -o $1.elf
mips64-linux-objcopy -O binary $1.elf $1.bin
rm elfimage
