#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "all_elf_func.h"

void initElf(Elf32_info *elf,FILE *file){
	fread(&elf->header,1,sizeof(elf->header),file);
	//if big endian then swap
	if(elf->header.e_ident[EI_DATA]==2){		
		setup_little_endian(elf);
	}
	elf->section = malloc(sizeof(Elf32_Shdr) * swap_uint16(elf->header.e_shnum));
	lire_Section_table(elf,file);
	init_strtab(elf,file);
	declaSymtab(elf);
	init_symtable(elf,file);
	lire_Symbol_table(elf,file);
	
}	

void declaSymtab(Elf32_info *elf){
	int i;
	int sechnum = elf->header.e_shnum;
	for(i=0;i<sechnum;i++){
		if(elf->section[i].sh_type==SHT_SYMTAB)
		elf->symtab = malloc(sizeof(Elf32_Sym) * (elf->section[i].sh_size/elf->section[i].sh_entsize));
	}
}
void declaReltab(Elf32_info *elf,int indice_rel){
	int relsize=elf->section[indice_rel].sh_size;
	int nbrel=relsize/elf->section[indice_rel].sh_entsize;
	elf->reltab = malloc(sizeof(Elf32_Rel) * nbrel);
}

void init_strtab(Elf32_info *elf,FILE *file){

	int shoff=elf->header.e_shoff;
	int shstrndx=elf->header.e_shstrndx;
	int shentsize=elf->header.e_shentsize;

	Elf32_Shdr strtab;

	fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    	fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);//get the string table header

    	fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
		elf->strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    	fread(elf->strtable, sizeof(char), swap_uint32(strtab.sh_size), file);	
}


void init_symtable(Elf32_info *elf,FILE *file){
	int indice_str;
	int shoff=elf->header.e_shoff;
	int shentsize=elf->header.e_shentsize;
	Elf32_Shdr sym_strtab;
	indice_str=get_indice_str(*elf);

	fseek(file, shoff + indice_str*shentsize, SEEK_SET);
    fread(&sym_strtab, sizeof(char), sizeof(Elf32_Shdr), file);//symble string table
    fseek(file, swap_uint32(sym_strtab.sh_offset), SEEK_SET);
	elf->symtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(sym_strtab.sh_size));
    fread(elf->symtable, sizeof(char), swap_uint32(sym_strtab.sh_size), file);	
}

void setup_little_endian(Elf32_info *elf){

	elf->header.e_type = swap_uint16(elf->header.e_type);
	elf->header.e_machine = swap_uint16(elf->header.e_machine);
	elf->header.e_version = swap_uint32(elf->header.e_version);
	elf->header.e_entry = swap_uint32(elf->header.e_entry);
	elf->header.e_phoff = swap_uint32(elf->header.e_phoff);
	elf->header.e_shoff = swap_uint32(elf->header.e_shoff);
	elf->header.e_flags = swap_int32(elf->header.e_flags);
	elf->header.e_ehsize = swap_uint16(elf->header.e_ehsize);
	elf->header.e_phentsize = swap_uint16(elf->header.e_phentsize);
	elf->header.e_phnum = swap_uint16(elf->header.e_phnum);
	elf->header.e_shentsize = swap_uint16(elf->header.e_shentsize);
	elf->header.e_shnum = swap_uint16(elf->header.e_shnum);
	elf->header.e_shstrndx = swap_uint16(elf->header.e_shstrndx);
	
}
