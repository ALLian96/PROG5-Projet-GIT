CC=gcc 
CFLAGS=-Wall -Werror

read_elf: read_elf_func.o read_elf.o
	gcc -g -o $@ $^

read_elf_func.o: read_elf_func.h myelf.h conversion.h
read_elf.o: read_elf_func.h myelf.h 

clean:
	rm -f read_elf read_elf_func.o read_elf.o