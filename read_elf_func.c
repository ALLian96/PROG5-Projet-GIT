#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include "read_elf_func.h"

#define N 16
#define BUFF 8196

void initElf(Elf32_info *elf,FILE *file){
	fread(&elf->header,1,sizeof(elf->header),file);
	//if big endian then swap
	if(elf->header.e_ident[EI_DATA]==2){		
		setup_little_endian(elf);
	}
	elf->section = malloc(sizeof(Elf32_Shdr) * swap_uint16(elf->header.e_shnum));
	lire_Section_table(elf,file);
	init_strtab(elf,file);
	declaSymtab(elf);
	init_symtable(elf,file);
	lire_Symbol_table(elf,file);
	
}	

void declaSymtab(Elf32_info *elf){
	int i;
	int sechnum = elf->header.e_shnum;
	for(i=0;i<sechnum;i++){
		if(elf->section[i].sh_type==SHT_SYMTAB)
		elf->symtab = malloc(sizeof(Elf32_Sym) * (elf->section[i].sh_size/elf->section[i].sh_entsize));
	}
}
void declaReltab(Elf32_info *elf,int indice_rel){
	int relsize=elf->section[indice_rel].sh_size;
	int nbrel=relsize/elf->section[indice_rel].sh_entsize;
	elf->reltab = malloc(sizeof(Elf32_Rel) * nbrel);
}
void lire_Section_table(Elf32_info *elf,FILE *file){
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

void lire_Symbol_table(Elf32_info *elf,FILE *file){
		int i;
		int indice_sym=get_indice_sym(*elf);
		int symsize=elf->section[indice_sym].sh_size;
		int nbsym=symsize/elf->section[indice_sym].sh_entsize;

		fseek(file, elf->section[indice_sym].sh_offset, SEEK_SET);
		for(i=0;i<nbsym;i++){			
			fread(&elf->symtab[i],1,sizeof(Elf32_Sym),file);
			
				elf->symtab[i].st_name = swap_uint32(elf->symtab[i].st_name);
				elf->symtab[i].st_value = swap_uint32(elf->symtab[i].st_value);
				elf->symtab[i].st_size = swap_uint32(elf->symtab[i].st_size);
				elf->symtab[i].st_shndx = swap_uint16(elf->symtab[i].st_shndx);
			
		}
} 

void lire_Relo_table(Elf32_info *elf,FILE *file,int indice_rel){
		int i;
		int relsize=elf->section[indice_rel].sh_size;
		int nbrel=relsize/elf->section[indice_rel].sh_entsize;

		fseek(file, elf->section[indice_rel].sh_offset, SEEK_SET);
		for(i=0;i<nbrel;i++){			
			fread(&elf->reltab[i],1,sizeof(Elf32_Rel),file);
			elf->reltab[i].r_offset = swap_uint32(elf->reltab[i].r_offset);	
			elf->reltab[i].r_info = swap_uint32(elf->reltab[i].r_info);		
		}
} 
int get_indice_sym(Elf32_info elf){
	int i;
	int sechnum=elf.header.e_shnum;
	for(i=0;i<sechnum;i++){
		if(elf.section[i].sh_type==SHT_SYMTAB)
			return i;
	}
	return 0;
}

int get_indice_str(Elf32_info elf){
	int i;
	int sechnum=elf.header.e_shnum;
	int shstrndx=elf.header.e_shstrndx;
	for(i=0;i<sechnum;i++){
		if(elf.section[i].sh_type==SHT_STRTAB && i!=shstrndx)
			return i; 
		}
	return 0;
}
void init_strtab(Elf32_info *elf,FILE *file){

	int shoff=elf->header.e_shoff;
	int shstrndx=elf->header.e_shstrndx;
	int shentsize=elf->header.e_shentsize;

	Elf32_Shdr strtab;

	fseek(file, shoff + shstrndx*shentsize, SEEK_SET);
    	fread(&strtab, sizeof(char), sizeof(Elf32_Shdr), file);//get the string table header

    	fseek(file, swap_uint32(strtab.sh_offset), SEEK_SET);
		elf->strtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(strtab.sh_size));
    	fread(elf->strtable, sizeof(char), swap_uint32(strtab.sh_size), file);	
}


void init_symtable(Elf32_info *elf,FILE *file){
	int indice_str;
	int shoff=elf->header.e_shoff;
	int shentsize=elf->header.e_shentsize;
	Elf32_Shdr sym_strtab;
	indice_str=get_indice_str(*elf);

	fseek(file, shoff + indice_str*shentsize, SEEK_SET);
    fread(&sym_strtab, sizeof(char), sizeof(Elf32_Shdr), file);//symble string table
    fseek(file, swap_uint32(sym_strtab.sh_offset), SEEK_SET);
	elf->symtable = (unsigned char *)malloc(sizeof(unsigned char)*swap_uint32(sym_strtab.sh_size));
    fread(elf->symtable, sizeof(char), swap_uint32(sym_strtab.sh_size), file);	
}


//Step1
void setup_little_endian(Elf32_info *elf){

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
	elf->header.e_shstrndx = swap_uint16(elf->header.e_shstrndx);
	
}

void affiche_Magic(Elf32_info elf){
    for(int i=0;i<16;i++){
	printf("%02x ",elf.header.e_ident[i]);
     }
}

void affiche_Classe(Elf32_info elf){

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

void affiche_DATA(Elf32_info elf){
                

		switch(elf.header.e_ident[EI_DATA]){
			case 0: printf("Invalid");
					break;
			case 1: printf("2's complement, little endian");
					break;
			case 2: printf("2's complement, big endian");
					break;
		}

}
void affiche_Version(Elf32_info elf){
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
void affiche_Type(Elf32_info elf){
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

void affiche_Machine(Elf32_info elf){

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

void affiche_header(Elf32_info elf){
                printf("ELF Header:\n");
 		/*e_ident[]*/
		printf("\tMagic number:\t\t");
                affiche_Magic(elf);
                printf("\n");
		printf("\tClasse:\t\t\t");
                affiche_Classe(elf);
                printf("\n");
		printf("\tDonnee:\t\t\t");
                affiche_DATA(elf);
                printf("\n");
		printf("\tVersion:\t\t\t");
                affiche_Version(elf);
		/*e_type*/
		printf("\tType:\t\t\t");
                affiche_Type(elf);
                printf("\n");
                 /* e_machine */
		printf("\tmachine:\t\t\t");
                affiche_Machine(elf);

		int W=0; // Debug purposes
		int ait=W; // Debug purposes
		ait+=ait;

                printf("\n\tObjet file version:            \t\t\t%#02x\n",elf.header.e_version);
		printf("\tAdresse du point d'entree:     \t\t\t%#02x\n",elf.header.e_entry);
		printf("\tDebut des en-tetes de programme:\t\t\t%d(octets)\n",elf.header.e_phoff);
		printf("\tDebut des en-tetes de section: \t\t\t%d(octets)\n",elf.header.e_shoff);
		printf("\tFlags:                       \t\t\t%#02x\n",elf.header.e_flags);
		printf("\tTaille de cet en-tete:         \t\t\t%d(octets)\n",elf.header.e_ehsize);
		printf("\tTaille de l'en-tete du programme:\t\t\t%d(octets)\n",elf.header.e_phentsize);
		printf("\tNombre d'en-tete du programme:  \t\t\t%d\n",elf.header.e_phnum);
		printf("\tTaille des en-tetes de section:\t\t\t%d(octets)\n",elf.header.e_shentsize);
		printf("\tNombre d'en-tetes de section:  \t\t\t%d\n",elf.header.e_shnum);
		printf("\tTable d'indexes des chaines d'en-tete de section:\t%d\n",elf.header.e_shstrndx);
}

//Step2


void affiche_tableSection(Elf32_info elf,FILE *file){
		int i;
		int shflag;
		char* sec_type="";

		printf("Il y a %d en-tetes de sections, debutant a l'adresse de decalage %#x\n\n",elf.header.e_shnum, elf.header.e_shoff);
		printf("En-tetes de section:\n");
		printf("[Nr] Nom                 Type           Adr      Decal  Taille ES Fan LN Inf Al\n");	
		fseek(file,elf.header.e_shoff,SEEK_SET);
		for(i=0;i<elf.header.e_shnum;i++){
			char* flag=malloc(sizeof(char)*4);
			shflag=elf.section[i].sh_flags;
			switch(elf.section[i].sh_type){
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
				if(shflag>=SHF_INFO_LINK){
					shflag=SHF_INFO_LINK;
					strcpy(flag, "I");

				} else {	
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
			}	
			printf("[%2d] ",i);
			printf("%-20.20s", elf.strtable+elf.section[i].sh_name);
        	printf("%s ",sec_type);        	
        	printf("%08x ",elf.section[i].sh_addr);
        	printf("%06x ", elf.section[i].sh_offset);
        	printf("%06x ", elf.section[i].sh_size);
			printf("%02x ", elf.section[i].sh_entsize);
			printf("%3s ",flag);
        	printf("%2d ", elf.section[i].sh_link);
        	printf("%2d ", elf.section[i].sh_info);
        	printf("%2d \n", elf.section[i].sh_addralign);
        	free(flag);
   		}
}

//step 3
void myhexdump(FILE *file,int addr,int size){

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

int get_section_number(Elf32_info elf, char * nom){
	int i;
	if(nom[0]==46){
		
		for(i=0;i<elf.header.e_shnum;i++){
			if(!strcmp(nom,(char *)elf.strtable+(elf.section[i].sh_name))){
				return i;
			}
		}	
	} else if(nom[0]>='0' && nom[0]<='9'){
		return nom[0]-'0';
	}
	return -1;
		
}
void affiche_contentSection(Elf32_info elf,FILE *file){
		char nom[32];
		int n;
		printf("Entrez un nombre ou un nom de section pour afficher le contenu.\n");		
		scanf("%s",nom);
		
		n = get_section_number(elf, nom);
		
		if(n>=0){
			unsigned char *name= elf.strtable+(elf.section[n].sh_name);
			int addr=elf.section[n].sh_addr;
		    	int offset=elf.section[n].sh_offset;
		    	int size=elf.section[n].sh_size;
			printf("Vidange hexadécimale de la section « %s (%d)»:\n",name, n);
			fseek(file, offset, SEEK_SET);
			myhexdump(file,addr,size);

		} else {
			printf("Erreur, nom de table inexistante\n");
		
		}
		
}


//step 4

void affiche_table_Symbol(Elf32_info elf,FILE *file){
	int i;
	char* bind="";
	char* type="";
	char* visi="";
	int indice_sym=get_indice_sym(elf);
	int symsize=elf.section[indice_sym].sh_size;
	int symoff=elf.section[indice_sym].sh_offset;
	int nbsym=symsize/elf.section[indice_sym].sh_entsize;
	printf("Table de symboles « %s » contient %d entrées:\n", elf.strtable+elf.section[indice_sym].sh_name,nbsym);
	printf("[Nr] value    size  Type     Vis       Bind   Index  Nom\n");

	fseek(file, symoff, SEEK_SET);
	int ici = ftell(file);
	printf("ICI : %d\n", ici);
	for(i=0;i<nbsym;i++){

			switch(ELF32_ST_BIND(elf.symtab[i].st_info)){
				case STB_LOCAL    :bind="LOCAL ";
					break;
				case STB_GLOBAL    :bind="GLOBAL";
					break;
				case STB_WEAK    :bind="WEAK  ";
					break;
			 }
			switch(ELF32_ST_TYPE(elf.symtab[i].st_info)){
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
			switch(ELF32_ST_VISIBILITY(elf.symtab[i].st_other)){
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
			printf("%08x ",elf.symtab[i].st_value);
        	printf("%x     ",elf.symtab[i].st_size);
			printf("%s ",type);        	
        	printf("%s ",bind);
			printf("%s ",visi);
    		printf("%x ",  elf.symtab[i].st_shndx);
    		printf("%-15s\n", elf.symtable+elf.symtab[i].st_name);
		}
}

void affiche_Relocation(Elf32_info *elf,FILE *file){
	int relsize;
	int reloff;
	int nbrel;
	int i,j,indice_sym;
	int indice_sym_name;
	char* type="";
	unsigned char *sym_name;
	if(elf->header.e_type!=ET_REL){
		printf("There are no relocations in this file.\n");
	}else{
		for(i=0;i<elf->header.e_shnum;i++){
			if(elf->section[i].sh_type==SHT_REL){
				reloff=elf->section[i].sh_offset;
				relsize=elf->section[i].sh_size;
				nbrel=relsize/elf->section[i].sh_entsize;
				printf("Section de relocalisation '%s' à l'adresse de décalage 0x%3x contenant %d entrées:\n",elf->strtable+elf->section[i].sh_name,reloff,nbrel);
				printf("Offset   Info      Type       Sym.Value    Sym.Name\n");
				declaReltab(elf,i);
				lire_Relo_table(elf,file,i);
				for(j=0;j<nbrel;j++){
					indice_sym=ELF32_R_SYM(elf->reltab[j].r_info);
					switch(ELF32_ST_TYPE(elf->symtab[indice_sym].st_info)){
						case STT_NOTYPE    :indice_sym_name=indice_sym;
											sym_name=elf->symtable+elf->symtab[indice_sym_name].st_name;
						break;
						case STT_SECTION    :indice_sym_name=elf->symtab[indice_sym].st_shndx;
											sym_name=elf->strtable+elf->section[indice_sym_name].sh_name;
						break;
			
					}
					switch(ELF32_R_TYPE(elf->reltab[j].r_info)){
						case R_ARM_NONE    :type="R_ARM_NONE  ";
						break;
						case R_ARM_PC24    :type="R_ARM_PC24  ";
						break;
						case R_ARM_ABS32   :type="R_ARM_ABS32 ";
						break;
						case R_ARM_ABS16    :type="R_ARM_ABS16";
						break;
						case R_ARM_ABS8    :type="R_ARM_ABS8  ";
						break;
			 			case R_ARM_REL32   :type="R_ARM_REL32 ";
						break;
						case R_ARM_CALL    :type="R_ARM_CALL  ";
						break;
						case R_ARM_JUMP24  :type="R_ARM_JUMP24";
						break;
					
					}
					printf("%08x ",elf->reltab[j].r_offset);
					printf("%08x ",elf->reltab[j].r_info);
					printf("%s ",type);
					printf("%08x   ",elf->symtab[indice_sym].st_value);
					printf("%s  \n",sym_name);			
					
					
				}
			}
		}
	}
	
}


void mod_sec(Elf32_info elf, FILE *in, FILE *out){
	unsigned char buffer[BUFF];
	fseek(in, 0, SEEK_END);
	int length = ftell(in);
	int i,j,cpt_nrel=0; //compteur de non-rel

	int end;
	int s_end;
	fseek(in, 0, SEEK_SET);
	fseek(out, 0, SEEK_SET);
	fread(&buffer, 1, elf.header.e_shoff, in);
	for(i=0;i<elf.header.e_shoff;i++){
		//printf("%8x",buffer[i]);
		fprintf(out,"%c",buffer[i]);	
	}
	for(i=0;i<elf.header.e_shnum;i++){			
		fread(&buffer, sizeof(char),sizeof(Elf32_Shdr), in);
		if(elf.section[i].sh_type!=SHT_REL){
			for(j=0;j<sizeof(char)*sizeof(Elf32_Shdr);j++){
				//printf("%8x", buffer[j]);
				
				fprintf(out,"%c",buffer[j]);		
			}
		}	
		else if(elf.section[i].sh_type==SHT_REL){
			cpt_nrel++;
		}
	}
	s_end = elf.header.e_shoff + i*(sizeof(char)*sizeof(Elf32_Shdr));
	end = length - elf.header.e_shoff - i;
	fread(&buffer, 1, end, in);
	
	
	for(i=s_end;i<end;i++){
		printf("%8x", buffer[i]);
		fprintf(out,"%c", buffer[i]);		
	}
	//modifier e_shnum
	int offs_shnum = sizeof(Elf32_Ehdr) - sizeof(Elf32_Half)*2; //
	fseek(out,offs_shnum,SEEK_SET);  
	uint16_t shnum=elf.header.e_shnum - cpt_nrel;
	shnum=swap_uint16(shnum);
	fwrite(&shnum,1,sizeof(uint16_t),out);
	
	//modifier e_shstrndx
	int offs_shstrndx = sizeof(Elf32_Ehdr) - sizeof(Elf32_Half); // 
	fseek(out,offs_shstrndx,SEEK_SET);
	uint16_t shstrndx=elf.header.e_shstrndx - cpt_nrel;
	shstrndx=swap_uint16(shstrndx);
	fwrite(&shstrndx,1,sizeof(uint16_t),out);
	fseek(out, 0, SEEK_SET); 
	
	//lecture du fichier de sortie
	Elf32_info elf_1;
	initElf(&elf_1,out);	
	affiche_tableSection(elf_1,out);
	
	
	// Mofification value
	int indice_sym=get_indice_sym(elf);
	int symsize=elf.section[indice_sym].sh_size;
	int symoff=elf.section[indice_sym].sh_offset;
	int nbsym=symsize/elf.section[indice_sym].sh_entsize;
	for(i=0;i<shnum;i++){
		for(j=i;j<nbsym;j++){
			if(elf_1.section[i].sh_name==elf.section[elf.symtab[j].st_shndx].sh_name){
				elf_1.symtab[j].st_value=elf.section[elf.symtab[j].st_shndx].sh_addr + 
											elf.symtab[j].st_value;
				elf_1.symtab[j].st_shndx=i;
			}
		}
	}
	
	for(i=0;i<nbsym;i++){
	
		fseek(out,symoff+sizeof(Elf32_Sym)*(i+1) - sizeof(Elf32_Sym) +  sizeof(Elf32_Word),SEEK_SET);
		elf_1.symtab[i].st_value=swap_uint32(elf_1.symtab[i].st_value);
		fwrite(&elf_1.symtab[i].st_value,1,sizeof(uint32_t),out);	
	
		fseek(out,symoff+sizeof(Elf32_Sym)*(i+1)-sizeof(Elf32_Half),SEEK_SET);
		elf_1.symtab[i].st_shndx=swap_uint16(elf_1.symtab[i].st_shndx);
		fwrite(&elf_1.symtab[i].st_shndx,1,sizeof(uint16_t),out);
	}
	fseek(out, 0, SEEK_SET);
	initElf(&elf_1,out);
	affiche_table_Symbol(elf_1,out);
}

