#include "myelf.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
	Elf32_Ehdr header;
	int i,n;
	FILE *file;
	file=fopen(argv[1],"r");
	if(file==NULL){
		printf("erreur.\n");
		exit(1);
	}
	else{
		fread(&header,1,sizeof(header),file);
/*e_ident[]*/
		printf("Magic number\t");
		for(i=0;i<16;i++){
			printf("%02x ",header.e_ident[i]);
		}
		printf("\nClasse\t\t");
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
		printf("\nDonnee\t\t");
		switch(header.e_ident[EI_DATA]){
			case 0: printf("Invalid");
					break;
			case 1: printf("2's complement values ,little endian");
					break;
			case 2: printf("2's complement values ,big endian");
					break;
		}
		printf("\nVersion\t\t");
		if(header.e_ident[EI_VERSION]==EV_CURRENT)
			printf("%d(current)",header.e_ident[EI_VERSION]);
		printf("\nOS/ABI\t\t");
		switch(header.e_ident[EI_OSABI]){
			case 0:printf("UNIX System V ABI");
				break;
			case 3:printf("linux");
				break;

		}
/*e_type*/
		printf("\nType\t\t");
		switch(header.e_type>>8){
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

		n=(header.e_type>>8)|(header.e_type<<8);
		printf("\nType\t\t%hx",n);
		n=(header.e_machine>>8)|(header.e_machine<<8);
		printf("\nmachine\t\t %x",n);
		switch(n){
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
			case 28:printf("ARM");
					break;
			default:printf("unknown");
					break;
		}
		printf("\nobjet file version\t%#02x",header.e_version);
		printf("\nadresse du point d'entree\t%#02x",header.e_entry);
		printf("\ndebut des en-tetes de programme\t %2x",header.e_phoff);
		printf("\ndebut des en-tetes de section\t %d(bytes)",header.e_shoff);
		printf("\nfanions\t %#02x\n",header.e_flags);
		printf("\ntaille de cet en-tete\t %d(bytes)",header.e_ehsize);
		printf("\nprogram header table entry size\t %d(bytes)",header.e_phentsize);
		printf("\nprogral header table entry count\t%d",header.e_phnum);
		printf("\nsection header table entry size\t%d(bytes)",header.e_shentsize);
		printf("\nsection header table entry count\t%d",header.e_shnum);
		printf("\nsection header string table index\t%d\n",header.e_shstrndx);
		
	}
	return 0;
}
