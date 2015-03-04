gcc -static test.c -o program
#cp vmlinux test.o
objcopy -I binary -O elf64-x86-64 -B i386:x86-64 program program.elf
gcc -c -g -O0 elf_loader.c
gcc -g -O0 elf_loader.o program.elf

mips64-linux-objcopy -v -I binary -O elf64-tradbigmips -B mips:octeon LICENSE tmptmptmp
mips64-linux-gcc -O0 -march=octeon -nostdlib -emain elf_loader.o tmptmptmp -Telf64btsmip.x -o result
mips64-linux-gcc -g -O0 -march=octeon -mips64r2 -mno-abicalls -nostdlib -mabi=64 -mno-explicit-relocs -Telf64btsmip.x head.S elf_loader.c -o result
