CC=gcc 
CFLAGS= -g -Wall -Werror

read_elf: read_elf_func.o read_elf.o conversion.o
	gcc -o $@ $^

conversion.o:conversion.h
read_elf_func.o: read_elf_func.h myelf.h conversion.h
read_elf.o: read_elf_func.h myelf.h conversion.h

clean:
	rm -f read_elf read_elf_func.o read_elf.o
