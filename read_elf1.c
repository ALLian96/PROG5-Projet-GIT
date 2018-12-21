#include "myelf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "conversion.h"
#include <ctype.h>
#define N 16

void hexdump(FILE *file,int addr,int size){

  unsigned char buffer[N]; //Use unsigned char,prevent hex overflow.
  int count,i;

  while(size>sizeof(buffer)){
    	count=fread(buffer,1,sizeof(buffer),file);
        printf(" 0x%08x  ",addr);//number in hex.
        addr+=16;
        for(i=0;i<sizeof(buffer);i++){
            if(i<count){
                printf("%02x",buffer[i]);
            }
            else{
                printf("   ");
            }
	    if((i+1)%4==0){
		printf(" ");
	    } 
         }
    	printf("| ");
    	for(i=0;i<sizeof(buffer);i++){
            if(i<count){
      		printf("%c",isprint(buffer[i])?buffer[i]:'.');
            }
            else{
                printf(" ");
            }
    	}
   	printf("|");
    	printf("\n");
    	size-=16;
  }
    	fread(buffer,1,size,file);
        printf(" 0x%08x  ",addr);//number in hex.
        addr+=16;
        for(i=0;i<N;i++){
            if(i<size){
                printf("%02x",buffer[i]);
            }
            else{
                printf("  ");
            }
	    if((i+1)%4==0){
		printf(" ");
	    } 
         }
    	printf("| ");
    	for(i=0;i<N;i++){
            if(i<size){
      		printf("%c",isprint(buffer[i])?buffer[i]:'.');
            }
            else{
                printf(" ");
            }
	}
   	printf("|");
    	printf("\n");

}

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
		Elf32_Shdr *section= malloc(sizeof(Elf32_Shdr) * sechnum);		
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
		printf("%2d \n", swap_uint32(section[i].sh_addralign));
        		free(flag);
   		}
//etape 3
		printf("entrez un nombre de section pour afficher le contenu.\n");
		scanf("%d",&n);
		char *name=strtable+swap_uint32(section[n].sh_name);
		int addr=swap_uint32(section[n].sh_addr);
        	int offset=swap_uint32(section[n].sh_offset);
        	int size=swap_uint32(section[n].sh_size);
		printf("Vidange hexadécimale de la section « %s »:\n",name);
		printf("addr:%x offset:%x size:%x\n",addr,offset,size);
		fseek(file, offset, SEEK_SET);
		hexdump(file,addr,size);
		

//etape 4
		/*Elf32_Shdr symstrtab;
		printf("symbol table\n");
		int indice_sym=0,indice_str=0;
		char* bind="";
		char* type="";
		char* visi="";
		for(i=0;i<sechnum;i++){
			if(swap_uint32(section[i].sh_type)==SHT_SYMTAB)
				indice_sym=i;
			if(swap_uint32(section[i].sh_type)==SHT_STRTAB && i!=shstrndx)
				indice_str=i; 
		}
printf("sym:%d str:%d\n",indice_sym,indice_str);
		char* sym_name=strtable+swap_uint32(section[indice_sym].sh_name);
		int symsize=swap_uint32(section[indice_sym].sh_size);
		int symoff=swap_uint32(section[indice_sym].sh_offset);
		int nbsym=symsize/swap_uint32(section[indice_sym].sh_entsize);
		
		Elf32_Sym *symtab=malloc(sizeof(Elf32_Sym) * nbsym);

		printf("%s a %d entree\n", sym_name,nbsym);

		fseek(file, shoff + indice_str*shentsize, SEEK_SET);
    		fread(&symstrtab, sizeof(char), sizeof(Elf32_Shdr), file);//symble string table

    		fseek(file, swap_uint32(symstrtab.sh_offset), SEEK_SET);
    		unsigned char* strtab_sym = malloc(sizeof(unsigned char)*swap_uint32(symstrtab.sh_size));
    		fread(strtab_sym, sizeof(char), swap_uint32(symstrtab.sh_size), file);
		fseek(file, symoff, SEEK_SET);
		printf("[Nr] value    size  Type     Vis       Bind   ndex  Nom\n");
		for(i=0;i<nbsym;i++){

    			fread(&symtab[i],1, sizeof(Elf32_Sym),file);//get the symbol table

			switch(ELF32_ST_BIND(symtab[i].st_info)){
				case STB_LOCAL    :bind="LOCAL ";
					break;
				case STB_GLOBAL    :bind="GLOBAL";
					break;
				case STB_WEAK    :bind="WEAK  ";
					break;
			 }
			switch(ELF32_ST_TYPE(symtab[i].st_info)){
				case STT_NOTYPE    :type="NOTYPE  ";
					break;
				case STT_OBJECT    :type="OBJECT  ";
					break;
				case STT_FUNC    :type="FUNC    ";
					break;
				case STT_SECTION    :type="SECTION ";
					break;
				case STT_FILE    :type="FILE    ";
					break;
				case STT_COMMON    :type="COMMON  ";
					break;
				case STT_TLS    :type="CTLS    ";
					break;
			}
			switch(ELF32_ST_VISIBILITY(symtab[i].st_other)){
				case STV_DEFAULT    :visi="DEFAULT   ";
					break;
				case STV_INTERNAL    :visi="INTERNAL  ";
					break;
				case STV_HIDDEN    :visi="HIDDEN    ";
					break;
				case STV_PROTECTED    :visi="PROTECTED ";
					break;
			}

			printf("[%2d] ",i);
			printf("%08x ",swap_uint32(symtab[i].st_value));
        		printf("%x     ",swap_uint32(symtab[i].st_size));
			printf("%s ",type);        	
        		printf("%s ",bind);
			printf("%s ",visi);
        		printf("%x ",  swap_uint16(symtab[i].st_shndx));
        		printf("%-15s\n", strtab_sym+swap_uint32(symtab[i].st_name));
		}*/
	free(strtable);
	}
	fclose(file);
	return 0;
}



			
