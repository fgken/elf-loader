gcc -static test.c -o test.o
#cp vmlinux test.o
objcopy -I binary -O elf64-x86-64 -B i386:x86-64 test.o test.elf
gcc -c -g -O0 elf-loader.c
gcc -g -O0 elf-loader.o test.elf
