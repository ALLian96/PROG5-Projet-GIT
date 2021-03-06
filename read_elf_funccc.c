#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include "read_elf_func.h"

#define N 16



void setup_little_endian(Elf32_Ehdr *header){

	header->e_type = swap_uint16(header->e_type);
	header->e_machine = swap_uint16(header->e_machine);
	header->e_version = swap_uint32(header->e_version);
	header->e_entry = swap_uint32(header->e_entry);
	header->e_phoff = swap_uint32(header->e_phoff);
	header->e_shoff = swap_uint32(header->e_shoff);
	header->e_flags = swap_int32(header->e_flags);
	header->e_ehsize = swap_uint16(header->e_ehsize);
	header->e_phentsize = swap_uint16(header->e_phentsize);
	header->e_phnum = swap_uint16(header->e_phnum);
	header->e_shentsize = swap_uint16(header->e_shentsize);
	header->e_shnum = swap_uint16(header->e_shnum);
	header->e_shstrndx = swap_uint16(header->e_shstrndx);
	
}

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
		switch(header.e_type){
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

			switch(header.e_machine){
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

                printf("\n\tObjet file version:            \t\t\t%#02x\n",header.e_version);
		printf("\tAdresse du point d'entree:     \t\t\t%#02x\n",header.e_entry);
		printf("\tDebut des en-tetes de programme:\t\t\t%d(octets)\n",header.e_phoff);
		printf("\tDebut des en-tetes de section: \t\t\t%d(octets)\n",header.e_shoff);
		printf("\tFlags:                       \t\t\t%#02x\n",header.e_flags);
		printf("\tTaille de cet en-tete:         \t\t\t%d(bytes)\n",header.e_ehsize);
		printf("\tTaille de l'en-tete du programme:\t\t\t%d(bytes)\n",header.e_phentsize);
		printf("\tNombre d'en-tete du programme:  \t\t\t%d\n",header.e_phnum);
		printf("\tTaille des en-tetes de section:\t\t\t%d(bytes)\n",header.e_shentsize);
		printf("\tNombre d'en-tetes de section:  \t\t\t%d\n",header.e_shnum);
		printf("\tTable d'indexes des chaines d'en-tete de section:\t%d\n",header.e_shstrndx);
}

void affiche_tableSection(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section){
		int i;
		int sechnum=header.e_shnum;
		int shoff=header.e_shoff;
		int shstrndx=header.e_shstrndx;
		int shentsize=header.e_shentsize;
		int shflag;
		//Elf32_Shdr *section = malloc(sizeof(Elf32_Shdr) * sechnum);		
		Elf32_Shdr strtab;
		char* sec_type="";
    	fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    	fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);//get the string table header
    	fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
    	unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    	fread(strtable, sizeof(char), swap_uint32(strtab.sh_size), file);	

		printf("Il y a %d en-tetes de sections, debutant a l'adresse de decalage %#x\n\n",header.e_shnum, header.e_shoff);
		printf("En-tetes de section:\n");
		printf("[Nr] Nom                 Type           Adr      Decal  Taille ES Fan LN Inf Al\n");	
		fseek(file,header.e_shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){
			char* flag=malloc(sizeof(char)*4);
			shflag=section[i].sh_flags;
			switch(section[i].sh_type){
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
			printf("%-20.20s", strtable+section[i].sh_name);
        	printf("%s ",sec_type);        	
        	printf("%08x ",section[i].sh_addr);
        	printf("%06x ", section[i].sh_offset);
        	printf("%06x ", section[i].sh_size);
			printf("%02x ", section[i].sh_entsize);
			printf("%3s ",flag);
        	printf("%2d ", section[i].sh_link);
        	printf("%2d ", section[i].sh_info);
        	printf("%2d \n", section[i].sh_addralign);
        	free(flag);
   		}
		
		
		free(strtable);
}

void affiche_contentSection(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section){
		
		printf("%08x ",section[1].sh_addr);
		printf("%06x ", section[1].sh_offset);
}


void lire_Section_table(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section){
		int i;
		int sechnum=header.e_shnum;
		fseek(file,header.e_shoff,SEEK_SET);
		for(i=0;i<sechnum;i++){			
			fread(&section[i],sizeof(char),sizeof(Elf32_Shdr),file);
			
			if(header.e_ident[EI_DATA]==2){	
				section[i].sh_name = swap_uint32(section[i].sh_name);
				section[i].sh_type = swap_uint32(section[i].sh_type);
				section[i].sh_addr = swap_uint32(section[i].sh_addr);
				section[i].sh_offset = swap_uint32(section[i].sh_offset);
				section[i].sh_size = swap_uint32(section[i].sh_size);
				section[i].sh_entsize = swap_uint32(section[i].sh_entsize);
				section[i].sh_flags = swap_uint32(section[i].sh_flags);
				section[i].sh_link = swap_uint32(section[i].sh_link);
				section[i].sh_info = swap_uint32(section[i].sh_info);
				section[i].sh_addralign = swap_uint32(section[i].sh_addralign);
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

void affiche_table_Symboles(Elf32_Ehdr header,FILE *file){
		
	int sechnum  =header.e_shnum;
	int shstrndx  =header.e_shstrndx;
	int i=0,read;
	
	Elf32_Shdr* tab_sec=malloc(sechnum*sizeof(Elf32_Shdr)); lire_Section_table(header,file,tab_sec);
	Elf32_Shdr symtab;//section symbole
	Elf32_Shdr strtab;//table du nom symbole string
	Elf32_Sym symtable;//chaque symbole
	char* symbole_type="";
	char* symbole_bind="";
	for(i=0;i<sechnum;i++){
		if(tab_sec[i].sh_type==SHT_SYMTAB){
			symtab=tab_sec[i];//get section:.symtab
		}
		if(tab_sec[i].sh_type==SHT_STRTAB && i!=shstrndx){
			strtab=tab_sec[i];//get table du nom symbole string
		}
	}
	fseek(file,strtab.sh_offset, SEEK_SET);
	unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*strtab.sh_size);
	fread(strtable, sizeof(char), strtab.sh_size, file);
    	fseek(file, symtab.sh_offset, SEEK_SET);
	read=symtab.sh_size/symtab.sh_entsize;
	printf("num:\tnom\t\t\tvaleur\ttype\t\tlien\tindice\n");
	i=0;
	while(i<read){
		fread(&symtable, sizeof(Elf32_Sym), 1, file); //get symtable
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
		printf("%d:",i);
		printf("\t%-20.20s", strtable+swap_uint32(symtable.st_name));
		printf("%08x",swap_uint32(symtable.st_value));
		printf("\t%s ",symbole_type);
		printf("\t%s ",symbole_bind);
		int indice = swap_uint16(symtable.st_shndx);
		if(indice>=0 && indice<=sechnum){
			printf(" %d",indice);
		}
		else{
			printf(" ABS");
		}
		printf("\n");	
		i++;	

	}
	free(tab_sec);
	free(strtable);
}

/*
typedef struct elf32_rel {
  Elf32_Addr	r_offset;
  Elf32_Word	r_info;
} Elf32_Rel;
typedef struct elf32_rela{
  Elf32_Addr	r_offset;
  Elf32_Word	r_info;
  Elf32_Sword	r_addend;
} Elf32_Rela;
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)
*/
void affiche_table_relocation(Elf32_Ehdr header,FILE *file){
	int i;
	int sechnum  =header.e_shnum;
	int shstrndx=header.e_shstrndx;
	//int shentsize=header.e_shentsize;

	Elf32_Shdr* tab_sec=malloc(sechnum*sizeof(Elf32_Shdr)); lire_Section_table(header,file,tab_sec);
	Elf32_Shdr section_rel;
	Elf32_Shdr shstrtab;
	Elf32_Rel rel;

	if(header.e_type!=ET_REL){
		printf("Il n'y a pas de relocalisation dans ce fichier");
	}
	else{	
		shstrtab = tab_sec[shstrndx];//get the section shstrtab
		fseek(file, shstrtab.sh_offset, SEEK_SET);///////////
		unsigned char* shstrtable = (unsigned char *)malloc(sizeof(unsigned char)*shstrtab.sh_size);
    		fread(shstrtable, sizeof(char), shstrtab.sh_size, file);
									//get shstrtab str
		for(i=0;i<sechnum;i++){
			if(tab_sec[i].sh_type==SHT_REL){
				section_rel=tab_sec[i];			//get section rel
				fseek(file,section_rel.sh_offset, SEEK_SET);
				int nbr=section_rel.sh_size/section_rel.sh_entsize;
				int j=0;
				while(j<nbr){
				fread(&rel, sizeof(Elf32_Rel), 1, file); //get rel
				
					
				}


				printf("Section de relocalisation'");
				printf("%-20.20s", shstrtable + section_rel.sh_name);
				printf("' a l'adresse de decalage 0x%03x contient %d en entree:\n",section_rel.sh_offset,nbr);
			}
	
		}	
	}


}






















