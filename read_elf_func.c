#include <stdio.h>
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



































