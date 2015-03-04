gcc -static test.c -o program
#cp vmlinux test.o
objcopy -I binary -O elf64-x86-64 -B i386:x86-64 program program.elf
gcc -c -g -O0 elf_loader.c
gcc -g -O0 elf_loader.o program.elf
