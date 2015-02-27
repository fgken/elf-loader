target_file = ARGV[0]
puts target_file

puts `mips64-linux-gcc -c -march=octeon -mips64r2 -nostdlib -mabi=64 elf_loader.c -o elf_loader.o`

f = open("program.c", "w")

f.puts <<"EOS"
unsigned char program[#{File.size(target_file)}] = { 1, 2, 3 };
EOS

f.close

puts `mips64-linux-gcc -c -march=octeon -mips64r2 -nostdlib -mabi=64 program.c -o program.o`

puts `gcc overwrite.c -o overwrite`

puts `./overwrite program.o #{target_file} 64`

puts `mips64-linux-gcc -march=octeon -mips64r2 -nostdlib -mabi=64 -emain elf_loader.o program.o`

