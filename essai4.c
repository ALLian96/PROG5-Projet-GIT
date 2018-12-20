#include "myelf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "conversion.h"


int main(int argc,char* argv[]){
	Elf32_Ehdr header;
	Elf32_Shdr *tab_sec;
	int i,n;
	FILE *file;
	file=fopen(argv[1],"r");
	if(argc<3){
		printf("il manque argument.\n");
	}
	if(file==NULL){
		printf("erreur.\n");
		exit(1);
	}
    	
	else if(strcmp(argv[2],"-s")==0){
		fread(&header,1,sizeof(header),file);
		int sechnum=swap_uint16(header.e_shnum);
		int shoff=swap_uint32(header.e_shoff);
		int shstrndx=swap_uint16(header.e_shstrndx);
		int shentsize=swap_uint16(header.e_shentsize);
		int i=0,indice_symtab,read;

		Elf32_Shdr symtab;
		Elf32_Shdr strtab;//table of name string
		Elf32_Sym symtable;//
		
		char* symbole_type="";
		char* symbole_bind="";
		tab_sec=malloc(sechnum*sizeof(Elf32_Shdr));
		fseek(file,shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){
			fread(&tab_sec[i],sizeof(char),sizeof(Elf32_Shdr),file);
			if(swap_uint32(tab_sec[i].sh_type)==SHT_SYMTAB){
				symtab=tab_sec[i];
				indice_symtab=i;
			}
			if(swap_uint32(tab_sec[i].sh_type)==SHT_STRTAB && i!=shstrndx){
				strtab=tab_sec[i];
			}
		}                       		//get section:.symtab
/*			fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    		fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);*/
    		fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
    		unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    		fread(strtable, sizeof(char), swap_uint32(strtab.sh_size), file);//get the string table header
			fseek(file, swap_uint32(symtab.sh_offset), SEEK_SET);
    		read=fread(&symtable, sizeof(Elf32_Sym), 1, file); //get symtable
			printf("num:\tnom\t\t\tvaleur\ttype\t\tlien\tindice\n");
			i=0;
		while(read!=0){
			printf("%d:",i);
			printf("\t%-20.20s", strtable+swap_uint32(symtable.st_name));
			printf("%08x",swap_uint32(symtable.st_value));
			
			switch(ELF32_ST_TYPE(symtable.st_info)){
				case STT_NOTYPE : symbole_type="STT_NOTYPE";
					break;
				case STT_OBJECT : symbole_type="STT_OBJECT";
					break;
				case STT_FUNC : symbole_type="STT_FUNC";
					break;
				case STT_SECTION : symbole_type="STT_SECTION";
					break;
				case STT_FILE : symbole_type="STT_FILE";
					break;
				case STT_LOPROC : symbole_type="STT_LOPROC";
					break;
				case STT_HIPROC : symbole_type="STT_HIPROC";
					break;
			}
			printf("\t%s ",symbole_type);
			switch(ELF32_ST_BIND(symtable.st_info)){
				case STB_LOCAL : symbole_bind= "STB_LOCAL ";
					break;
				case STB_GLOBAL : symbole_bind="STB_GLOBAL";
					break;
				case STB_WEAK : symbole_bind=  "STB_WEAK  ";
					break;
				case STB_LOPROC : symbole_bind="STB_LOPROC";
					break;
				case STB_HIPROC : symbole_bind="STB_HIPROC";
					break;
			}	
			printf("\t%s ",symbole_bind);
			int indice = swap_uint16(symtable.st_shndx);
			if(indice>=0 && indice<=sechnum){
				printf(" %d",indice);
			}
			else{
				printf(" ABS");
			}
			printf("\n");

			read=fread(&symtable, sizeof(Elf32_Sym), 1, file); //get symtable
			i++;	
		}
	free(strtable);
	}
	

fclose(file);
return 0;
}
