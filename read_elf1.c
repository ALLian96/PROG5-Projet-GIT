#include "myelf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "conversion.h"


int main(int argc,char* argv[]){
	Elf32_Ehdr header;
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
	else if(strcmp(argv[2],"-S")==0){

		fread(&header,1,sizeof(header),file);
		int sechnum=swap_uint16(header.e_shnum);
		int shoff=swap_uint32(header.e_shoff);
		int shstrndx=swap_uint16(header.e_shstrndx);
		int shentsize=swap_uint16(header.e_shentsize);
		int shflag;
		Elf32_Shdr section;		
		Elf32_Shdr strtab;
		char* sec_type="";
		char* flag="";
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
				int j=0;
				if(shflag> SHF_EXECINSTR){
					shflag=shflag-SHF_EXECINSTR;
					flag[j]="X";
					j++;
				}
				if(shflag> SHF_ALLOC){
					shflag=shflag-SHF_ALLOC;
					flag[j]="A";
					j++;
				}
				if(shflag> SHF_WRITE){
					shflag=shflag-SHF_WRITE;
					flag[j]="W";
					j++;
				}	
			}	
			printf("[%2d] ",i);
			printf("%-20.20s", strtable+swap_uint32(section.sh_name));
        		printf("%s ",sec_type);        	
        		printf("%08x ",  swap_uint32(section.sh_addr));
        		printf("%06x ",  swap_uint32(section.sh_offset));
        		printf("%06x ",  swap_uint32(section.sh_size));
			printf("%02x ",  swap_uint32(section.sh_entsize));
			printf("%3d ",  swap_uint32(section.sh_flags));
        		printf("%2d ",  swap_uint32(section.sh_link));
        		printf("%2d ",  swap_uint32(section.sh_info));
        		printf("%2d \n",  swap_uint32(section.sh_addralign));
        		
   		}


	}


	return 0;
}



			
