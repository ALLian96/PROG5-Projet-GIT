#include <stdio.h>
#include <stdlib.h>
#include "elf_structs.h"
void main(int argc, char** argv){

	FILE* f=fopen(argv[1],"rb");
	Elf32_Shdr *elf_sec_table = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr)*11)/* elfheader.e_shnum*/;
	int num=0;

	Elf32_Ehdr elf_head;
	fread(&elf_head, sizeof(Elf64_Ehdr), 1, f);
/* elfheader  elfheader.e_shnum  */
	if(f){
		int err_fseek = fseek(f, elfheader.e_shoff, SEEK_SET);
		if(err_fseek==0){
			printf("Erreur recherche table des sections\n");
		}
		int err_fread = fread(elf_sec_table, sizeof(Elf32_Shdr) * 11/* elfheader.e_shnum*/, 1, f);
		if(err_fread==0){
			printf("Erreur lecture table des sections\n");
		}
	}
	for(num=0;num < 11 /* elfheader.e_shnum*/ ; num++){
		printf("[%d]", num);
		printf("%hx  ", elf_sec_table[num].sh_name);
		printf("%hx  ", elf_sec_table[num].sh_type);
		printf("%hx  ", elf_sec_table[num].sh_addr);
		printf("%hx  ", elf_sec_table[num].sh_offset);
		printf("%hx  ", elf_sec_table[num].sh_size);
		printf("%hx  ", elf_sec_table[num].sh_entsize);
		printf("%hx  ", elf_sec_table[num].sh_flags);
		printf("%hx  ", elf_sec_table[num].sh_link);
		printf("%hx  ", elf_sec_table[num].sh_info);
		printf("%hx  \n", elf_sec_table[num].sh_addralign); 

	}


}


