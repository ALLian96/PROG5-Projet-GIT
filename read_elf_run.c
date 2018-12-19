#include "myelf.h"
#include <stdio.h>
#include <stdlib.h>

#define BigtoLittle16(A) ((((__u16)(A) & 0xff00) >> 8) | (((__u16)(A) & 0x00ff) << 8))
#define BigtoLittle32(A) ((((__u32)(A) & 0xff000000) >> 24) | (((__u32)(A) & 0x00ff0000) >> 8) | \
             (((__u32)(A) & 0x0000ff00) << 8) | (((__u32)(A) & 0x000000ff) << 24))

int main(int argc,char* argv[]){
	Elf32_Ehdr header;
	int i,n;
	FILE *file;
	file=fopen(argv[1],"r");
	if(file==NULL){
		printf("erreur.\n");
		exit(1);
	}	else{
		fread(&header,1,sizeof(header),file);
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
			case 1: printf("2's complement values, little endian");
					break;
			case 2: printf("2's complement values, big endian");
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
		switch(BigtoLittle16(header.e_type)){
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
		switch(BigtoLittle16(header.e_machine)){
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
		printf("\nObjet file version            \t%#02x",BigtoLittle32(header.e_version));
		printf("\nAdresse du point d'entree     \t%#02x",BigtoLittle32(header.e_entry));
		printf("\nDebut des en-tetes de programme\t%d(octets)",BigtoLittle32(header.e_phoff));
		printf("\nDebut des en-tetes de section \t%d(octets)",BigtoLittle32(header.e_shoff));
		printf("\nFanions                       \t%#02x",BigtoLittle32(header.e_flags));
		printf("\nTaille de cet en-tete         \t%d(bytes)",BigtoLittle16(header.e_ehsize));
		printf("\nTaille de l'en-tete du programme\t%d(bytes)",BigtoLittle16(header.e_phentsize));
		printf("\nNombre d'en-tete du programme  \t%d",BigtoLittle16(header.e_phnum));
		printf("\nTaille des en-tetes de section\t%d(bytes)",BigtoLittle16(header.e_shentsize));
		printf("\nNombre d'en-tetes de section  \t%d",BigtoLittle16(header.e_shnum));
		printf("\nTable d'indexes des chaines d'en-tete de section\t%d\n",BigtoLittle16(header.e_shstrndx));
		
	}


// PARTIE 2 ----------------------------------

	Elf32_Shdr *shdr = malloc(sizeof(Elf32_Shdr) * BigtoLittle16(header.e_shnum));
	int num=0;
	int err_fread=0;

	if(file){
		printf("%li", ftell(file));
		int err_fseek = fseek(file, BigtoLittle32(header.e_shoff), SEEK_SET);
		printf(" | %li\n", ftell(file));
		if(err_fseek!=0){
			printf("Erreur recherche table des sections\n");
		}
		for(num=0;num < BigtoLittle16(header.e_shnum) ; num++){
			err_fread = fread(shdr,sizeof(Elf32_Shdr), BigtoLittle16(header.e_shnum), file);
			if(err_fread!=BigtoLittle16(header.e_shnum)){
				printf("Erreur lecture table des sections (%d / %d read)\n", num+1, BigtoLittle16(header.e_shnum));
				/*exit(0);*/
			}
		}
	
	
	}	
	
	for(num=0;num < BigtoLittle16(header.e_shnum) ; num++){
		printf("[%d]", num);
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_name));
		printf("\t%#02x  ", BigtoLittle32(shdr[num].sh_type));
		printf("\t%#02x  ", BigtoLittle32(shdr[num].sh_addr));
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_offset));
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_size));
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_entsize));
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_flags));
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_link));
		printf("\t%d  ", BigtoLittle32(shdr[num].sh_info));
		printf("\t%d  \n", BigtoLittle32(shdr[num].sh_addralign)); 
}


}
