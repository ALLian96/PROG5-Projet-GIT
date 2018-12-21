#include "myelf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "conversion.h"
/*
void afficher_table_section(Elf32_Ehdr header,Elf32_Shdr* tab_sec,FILE *file){

		fread(&header,1,sizeof(header),file);
		int sechnum=swap_uint16(header.e_shnum);
		int shoff=swap_uint32(header.e_shoff);
		int shstrndx=swap_uint16(header.e_shstrndx);
		int shentsize=swap_uint16(header.e_shentsize);
		int shflag;
		Elf32_Shdr section;		
		Elf32_Shdr strtab;
		char* sec_type="";
    	fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    	fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);//get the string table header
    	fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
    	unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    	fread(strtable, sizeof(char), swap_uint32(strtab.sh_size), file);	

		printf("Il y a %d en-tetes de sections, debutant a l'adresse de decalage %#x\n\n",swap_uint16(header.e_shnum),swap_uint32(header.e_shoff));
		printf("En-tetes de section:\n");
		printf("[Nr] Nom                 Type           Adr      Decal  Taille ES Fan LN Inf Al\n");
		fseek(file,swap_uint32(header.e_shoff),SEEK_SET);
		for(i=0;i<sechnum;i++){
			char* flag=malloc(sizeof(char)*4);
			fread(&section,sizeof(char),sizeof(Elf32_Shdr),file);
			shflag=swap_uint32(section.sh_flags);
			switch(swap_uint32(section.sh_type)){
				case SHT_NULL    :sec_type="NULL          ";
					break;
				case SHT_PROGBITS:sec_type="PROGBITS      ";
					break;
				case SHT_SYMTAB  :sec_type="SYMTAB        ";
					break;
				case SHT_STRTAB  :sec_type="STRTAB        ";
					break;
				case SHT_RELA    :sec_type="RELA          ";
					break;
				case SHT_HASH    :sec_type="HASH          ";
					break;
				case SHT_DYNAMIC :sec_type="DYNAMIC       ";
					break;
				case SHT_NOTE    :sec_type="NOTE          ";
					break;
				case SHT_NOBITS  :sec_type="NOBITS        ";
					break;
				case SHT_REL     :sec_type="REL           ";
					break;
				case SHT_SHLIB   :sec_type="SHLIB         ";
					break;
				case SHT_DYNSYM  :sec_type="DYNSYM        ";
					break;
				case SHT_NUM     :sec_type="NUM           ";
					break;
				case SHT_ARM_ATTRIBUTES  :sec_type="ARM_ATTRIBUTES";
			}
			while(shflag!=0){
				if(shflag>=SHF_MASKPROC){
					shflag=shflag-SHF_MASKPROC;
					strcat(flag,"M");
				}
				if(shflag>=SHF_EXECINSTR){
					shflag=shflag-SHF_EXECINSTR;
					strcat(flag,"X");
				}
				if(shflag>=SHF_ALLOC){-
					shflag=shflag-SHF_ALLOC;
					strcat(flag,"A");
				}
				if(shflag>=SHF_WRITE){
					shflag=shflag-SHF_WRITE;
					strcat(flag,"W");
				}	
			}	
			printf("[%2d] ",i);
			printf("%-20.20s", strtable+swap_uint32(section.sh_name));
        	printf("%s ",sec_type);        	
        	printf("%08x ",swap_uint32(section.sh_addr));
        	printf("%06x ",  swap_uint32(section.sh_offset));
        	printf("%06x ",  swap_uint32(section.sh_size));
		printf("%02x ",  swap_uint32(section.sh_entsize));
		printf("%3s ",flag);
        	printf("%2d ",  swap_uint32(section.sh_link));
        	printf("%2d ",  swap_uint32(section.sh_info));
        	printf("%2d \n",  swap_uint32(section.sh_addralign));
        	free(flag);
   		}

		free(strtable);
}
*/

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
		Elf32_Shdr strtab;
		Elf32_Sym symtable;
		
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
		}                       		//get section:.symtab
		fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    		fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);
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
	}




//header
	else if(strcmp(argv[2],"-h")==0){
		fread(&header,1,sizeof(Elf32_Ehdr),file);
/*e_ident[]*/
		printf("Magic number\t");
		for(i=0;i<16;i++){
			printf("%02x ",header.e_ident[i]);
		}
		printf("\nClasse            \t\t");
		for(i=EI_MAG1;i<=EI_MAG3;i++){
			printf("%c",header.e_ident[i]);
		}
		switch(header.e_ident[EI_CLASS]){
			case 0: printf("Invalid");
					break;
			case 1: printf("32");
					break;
			case 2: printf("64");
					break;
		}
		printf("\nDonnee            \t\t");
		switch(header.e_ident[EI_DATA]){
			case 0: printf("Invalid");
					break;
			case 1: printf("2's complement values ,little endian");
					break;
			case 2: printf("2's complement values ,big endian");
					break;
		}
		printf("\nVersion          \t\t");
		if(header.e_ident[EI_VERSION]==EV_CURRENT)
			printf("%d(current)",header.e_ident[EI_VERSION]);
		printf("\nOS/ABI           \t\t");
		switch(header.e_ident[EI_OSABI]){
			case 0:printf("UNIX System V ABI");
				break;
			case 3:printf("linux");
				break;

		}
/*e_type*/
		printf("\nType             \t\t");
		switch(swap_uint16(header.e_type)){
			case ET_NONE: printf("No file");
					break;
			case ET_REL: printf("REL");
					break;
			case ET_EXEC: printf("EXEC");
					break;
			case ET_DYN: printf("DYN");
					break;
			case ET_CORE: printf("CORE");
					break;
			case ET_LOPROC: printf("LOPROC");
					break;
			case ET_HIPROC: printf("HIPROC");
					break;
		}
		printf("\nmachine          \t\t");
		switch(swap_uint16(header.e_machine)){
			case EM_NONE: printf("No machine");
					break;
			case EM_M32: printf("AT&T WE 32100");
					break;
			case EM_SPARC: printf("SUN SPARC");
					break;
			case EM_386: printf("Intel 80386");
					break;
			case EM_68K: printf("Motorola m68k family ");
					break;
			case EM_88K: printf("Motorola m88k family");
					break;
			case EM_860: printf("Intel 80860");
					break;
			case EM_ARM:printf("ARM");
					break;
			default:printf("unknown");
					break;
		}
		printf("\nObjet file version            \t%#02x",swap_uint32(header.e_version));
		printf("\nAdresse du point d'entree     \t%#02x",swap_uint32(header.e_entry));
		printf("\nDebut des en-tetes de programme\t%d(octets)",swap_uint32(header.e_phoff));
		printf("\nDebut des en-tetes de section \t%d(octets)",swap_uint32(header.e_shoff));
		printf("\nFanions                       \t%#02x",swap_uint32(header.e_flags));
		printf("\nTaille de cet en-tete         \t%d(bytes)",swap_uint16(header.e_ehsize));
		printf("\nTaille de l'en-tete du programme\t%d(bytes)",swap_uint16(header.e_phentsize));
		printf("\nNombre d'en-tete du programme  \t%d",swap_uint16(header.e_phnum));
		printf("\nTaille des en-tetes de section\t%d(bytes)",swap_uint16(header.e_shentsize));
		printf("\nNombre d'en-tetes de section  \t%d",swap_uint16(header.e_shnum));
		printf("\nTable d'indexes des chaines d'en-tete de section\t%d\n",swap_uint16(header.e_shstrndx));
		
	}

	//table de sections
	
/*	else if(strcmp(argv[2],"-S")==0){
	afficher_table_section(header,file);	
 
	}

*/

	/*else if(strcmp(argv[2],"-x")==0){
		if(argc<4){
			printf("il manque d'argument\n");
		}
		else{
			fread(&header,1,sizeof(Elf32_Ehdr),file);
			int sechnum=swap_uint16(header.e_shnum);//nbr de sections
			int shoff=swap_uint32(header.e_shoff);//debut
			int shstrndx=swap_uint16(header.e_shstrndx);//indice de string head table
			int shentsize=swap_uint16(header.e_shentsize);//size
			int shflag;
			Elf32_Shdr section;		
			Elf32_Shdr strtab;



		}

	}
*/

	return 0;
}
