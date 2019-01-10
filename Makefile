CC=gcc 
CFLAGS= -g -Wall -Werror

read_elf: read_elf_func.o read_elf.o conversion.o init_elf_func.o aff_elf_func.o
	gcc -o $@ $^

conversion.o:conversion.h
init_elf_func.o: all_elf_func.h myelf.h conversion.h
read_elf_func.o: all_elf_func.h myelf.h conversion.h
aff_elf_func.o: all_elf_func.h myelf.h conversion.h
read_elf.o: all_elf_func.h myelf.h conversion.h

clean:
	rm -f read_elf read_elf_func.o read_elf.o
