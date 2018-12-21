#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include "read_elf_func.h"

#define N 16



void setup_little_endian(Elf32_Ehdr *header){

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
	elf->header.e_shstrndx = swap_uint16(header->e_shstrndx);
	
}

void affiche_Magic(Elf32_Ehdr header){
    for(int i=0;i<16;i++){
	printf("%02x ",elf.header.e_ident[i]);
     }
}

void affiche_Classe(Elf32_Ehdr header){

		for(int i=EI_MAG1;i<=EI_MAG3;i++){
			printf("%c",elf.header.e_ident[i]);
		}
                switch(elf.header.e_ident[EI_CLASS]){
			case 0: printf("Invalid");
					break;
			case 1: printf("32");
					break;
			case 2: printf("64");
					break;
		}

}

void affiche_DATA(Elf32_Ehdr header){
                

		switch(elf.header.e_ident[EI_DATA]){
			case 0: printf("Invalid");
					break;
			case 1: printf("2's complement, little endian");
					break;
			case 2: printf("2's complement, big endian");
					break;
		}

}
void affiche_Version(Elf32_Ehdr header){
		if(elf.header.e_ident[EI_VERSION]==EV_CURRENT)
			printf("%d(current)\n",elf.header.e_ident[EI_VERSION]);
		printf("\tOS/ABI:\t\t\t");
		switch(elf.header.e_ident[EI_OSABI]){
			case 0:printf("UNIX System V ABI\n");
				break;
			case 3:printf("linux\n");
				break;
		}

}
void affiche_Type(Elf32_Ehdr header){
		switch(elf.header.e_type){
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

			switch(elf.header.e_machine){
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
			case EM_ARM :printf("ARM");
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

                printf("\n\tObjet file version:            \t\t\t%#02x\n",elf.header.e_version);
		printf("\tAdresse du point d'entree:     \t\t\t%#02x\n",elf.header.e_entry);
		printf("\tDebut des en-tetes de programme:\t\t\t%d(octets)\n",elf.header.e_phoff);
		printf("\tDebut des en-tetes de section: \t\t\t%d(octets)\n",elf.header.e_shoff);
		printf("\tFlags:                       \t\t\t%#02x\n",elf.header.e_flags);
		printf("\tTaille de cet en-tete:         \t\t\t%d(bytes)\n",elf.header.e_ehsize);
		printf("\tTaille de l'en-tete du programme:\t\t\t%d(bytes)\n",elf.header.e_phentsize);
		printf("\tNombre d'en-tete du programme:  \t\t\t%d\n",elf.header.e_phnum);
		printf("\tTaille des en-tetes de section:\t\t\t%d(bytes)\n",elf.header.e_shentsize);
		printf("\tNombre d'en-tetes de section:  \t\t\t%d\n",elf.header.e_shnum);
		printf("\tTable d'indexes des chaines d'en-tete de section:\t%d\n",elf.header.e_shstrndx);
}

void affiche_tableSection(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section){
		int i;
		int sechnum=elf.header.e_shnum;
		int shoff=elf.header.e_shoff;
		int shstrndx=elf.header.e_shstrndx;
		int shentsize=elf.header.e_shentsize;
		int shflag;
		//Elf32_Shdr *section = malloc(sizeof(Elf32_Shdr) * sechnum);		
		Elf32_Shdr strtab;
		char* sec_type="";
    	fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    	fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);//get the string table header
    	fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
    	unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    	fread(strtable, sizeof(char), swap_uint32(strtab.sh_size), file);	

		printf("Il y a %d en-tetes de sections, debutant a l'adresse de decalage %#x\n\n",elf.header.e_shnum, elf.header.e_shoff);
		printf("En-tetes de section:\n");
		printf("[Nr] Nom                 Type           Adr      Decal  Taille ES Fan LN Inf Al\n");	
		fseek(file,elf.header.e_shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){
			char* flag=malloc(sizeof(char)*4);
			shflag=elf->section[i].sh_flags;
			switch(elf->section[i].sh_type){
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
			printf("%-20.20s", strtable+elf->section[i].sh_name);
        	printf("%s ",sec_type);        	
        	printf("%08x ",elf->section[i].sh_addr);
        	printf("%06x ", elf->section[i].sh_offset);
        	printf("%06x ", elf->section[i].sh_size);
			printf("%02x ", elf->section[i].sh_entsize);
			printf("%3s ",flag);
        	printf("%2d ", elf->section[i].sh_link);
        	printf("%2d ", elf->section[i].sh_info);
        	printf("%2d \n", elf->section[i].sh_addralign);
        	free(flag);
   		}
		
		
		free(strtable);
}

void affiche_contentSection(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section){
		
		printf("%08x ",elf->section[1].sh_addr);
		printf("%06x ", elf->section[1].sh_offset);
}


void lire_Section_table(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section){
		int i;
		int sechnum=elf->header.e_shnum;
		fseek(file,elf->header.e_shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){			
			fread(&elf->section[i],sizeof(char),sizeof(Elf32_Shdr),file);
			
			if(elf->header.e_ident[EI_DATA]==2){	
				elf->section[i].sh_name = swap_uint32(elf->section[i].sh_name);
				elf->section[i].sh_type = swap_uint32(elf->section[i].sh_type);
				elf->section[i].sh_addr = swap_uint32(elf->section[i].sh_addr);
				elf->section[i].sh_offset = swap_uint32(elf->section[i].sh_offset);
				elf->section[i].sh_size = swap_uint32(elf->section[i].sh_size);
				elf->section[i].sh_entsize = swap_uint32(elf->section[i].sh_entsize);
				elf->section[i].sh_flags = swap_uint32(elf->section[i].sh_flags);
				elf->section[i].sh_link = swap_uint32(elf->section[i].sh_link);
				elf->section[i].sh_info = swap_uint32(elf->section[i].sh_info);
				elf->section[i].sh_addralign = swap_uint32(elf->section[i].sh_addralign);
			}
			
		}
}


void hexdump(FILE *file,int addr,int size){

  unsigned char buffer[N]; //Use unsigned char,prevent hex overflow.
  int count,i,j;
  j=addr;
    setvbuf(file,NULL,_IOFBF,size);//Set max buffer size to 1024 bytes.

  while(size!=0)//check the end of file.
  {
    count=fread(buffer,1,sizeof(buffer),file);
        printf(" 0x%08x  ",j);//number in hex.
        j+=16;
        for(i=0;i<sizeof(buffer);i++)
    {
            if(i<count)
            {
        printf("%02x",buffer[i]);
            }
            else
            {
              printf("   ");
            }
	    if((i+1)%4==0){
		printf(" ");
	    } 
    }
    printf("| ");
    for(i=0;i<sizeof(buffer);i++)
    {
            if(i<count)
            {
      printf("%c",isprint(buffer[i])?buffer[i]:'.');
            }
            else
            {
            printf(" ");
            }
    }
    printf("|");
    printf("\n");
    size-=16;
  }
}

void affiche_table_Symboles(Elf32_Shdr header,FILE *file){

	


}






