#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "read_elf_func.h"
#include "conversion.h"


void affiche_Magic(Elf32_Ehdr header){
    for(int i=0;i<16;i++){
	printf("%02x ",header.e_ident[i]);
     }
}

void affiche_Classe(Elf32_Ehdr header){

		for(int i=EI_MAG1;i<=EI_MAG3;i++){
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

}

void affiche_DATA(Elf32_Ehdr header){
                

		switch(header.e_ident[EI_DATA]){
			case 0: printf("Invalid");
					break;
			case 1: printf("2's complement, little endian");
					break;
			case 2: printf("2's complement, big endian");
					break;
		}

}
void affiche_Version(Elf32_Ehdr header){
		if(header.e_ident[EI_VERSION]==EV_CURRENT)
			printf("%d(current)\n",header.e_ident[EI_VERSION]);
		printf("\tOS/ABI:\t\t\t");
		switch(header.e_ident[EI_OSABI]){
			case 0:printf("UNIX System V ABI\n");
				break;
			case 3:printf("linux\n");
				break;
		}

}
void affiche_Type(Elf32_Ehdr header){
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

}

void affiche_Machine(Elf32_Ehdr header){

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

}

void affiche_header(Elf32_Ehdr header){
                printf("ELF Header:\n");
 		/*e_ident[]*/
		printf("\tMagic number:\t\t");
                affiche_Magic(header);
                printf("\n");
		printf("\tClasse:\t\t\t");
                affiche_Classe(header);
                printf("\n");
		printf("\tDonnee:\t\t\t");
                affiche_DATA(header);
                printf("\n");
		printf("\tVersion:\t\t\t");
                affiche_Version(header);
		/*e_type*/
		printf("\tType:\t\t\t");
                affiche_Type(header);
                printf("\n");
                 /* e_machine */
		printf("\tmachine:\t\t\t");
                affiche_Machine(header);

		int W=0; // Debug purposes
		int ait=W; // Debug purposes
		ait+=ait;

                printf("\n\tObjet file version:            \t\t\t%#02x\n",swap_uint32(header.e_version));
		printf("\tAdresse du point d'entree:     \t\t\t%#02x\n",swap_uint32(header.e_entry));
		printf("\tDebut des en-tetes de programme:\t\t\t%d(octets)\n",swap_uint32(header.e_phoff));
		printf("\tDebut des en-tetes de section: \t\t\t%d(octets)\n",swap_uint32(header.e_shoff));
		printf("\tFlags:                       \t\t\t%#02x\n",swap_int32(header.e_flags));
		printf("\tTaille de cet en-tete:         \t\t\t%d(bytes)\n",swap_uint16(header.e_ehsize));
		printf("\tTaille de l'en-tete du programme:\t\t\t%d(bytes)\n",swap_uint16(header.e_phentsize));
		printf("\tNombre d'en-tete du programme:  \t\t\t%d\n",swap_uint16(header.e_phnum));
		printf("\tTaille des en-tetes de section:\t\t\t%d(bytes)\n",swap_uint16(header.e_shentsize));
		printf("\tNombre d'en-tetes de section:  \t\t\t%d\n",swap_uint16(header.e_shnum));
		printf("\tTable d'indexes des chaines d'en-tete de section:\t%d\n",swap_uint16(header.e_shstrndx));
}

void affiche_tableSection(Elf32_Ehdr header,FILE *file){
		int i;
		int sechnum=swap_uint16(header.e_shnum);
		int shoff=swap_uint32(header.e_shoff);
		int shstrndx=swap_uint16(header.e_shstrndx);
		int shentsize=swap_uint16(header.e_shentsize);
		int shflag;
		Elf32_Shdr *section = malloc(sizeof(Elf32_Shdr) * sechnum);;		
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
			fread(&section[i],sizeof(char),sizeof(Elf32_Shdr),file);
			shflag=swap_uint32(section[i].sh_flags);
			switch(swap_uint32(section[i].sh_type)){
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
			if(shflag!=0){
				if(shflag>=SHF_MASKPROC){
					shflag=shflag-SHF_MASKPROC;
					strcat(flag,"M");
				}
				if(shflag>=SHF_EXECINSTR){
					shflag=shflag-SHF_EXECINSTR;
					strcat(flag,"X");
				}
				if(shflag>=SHF_ALLOC){
					shflag=shflag-SHF_ALLOC;
					strcat(flag,"A");
				}
				if(shflag>=SHF_WRITE){
					shflag=shflag-SHF_WRITE;
					strcat(flag,"W");
				}	
			}	
			printf("[%2d] ",i);
			printf("%-20.20s", strtable+swap_uint32(section[i].sh_name));
        	printf("%s ",sec_type);        	
        	printf("%08x ",swap_uint32(section[i].sh_addr));
        	printf("%06x ",  swap_uint32(section[i].sh_offset));
        	printf("%06x ",  swap_uint32(section[i].sh_size));
		printf("%02x ",  swap_uint32(section[i].sh_entsize));
		printf("%3s ",flag);
        	printf("%2d ",  swap_uint32(section[i].sh_link));
        	printf("%2d ",  swap_uint32(section[i].sh_info));
        	printf("%2d \n",  swap_uint32(section[i].sh_addralign));
        	free(flag);
   		}

		free(strtable);

}





