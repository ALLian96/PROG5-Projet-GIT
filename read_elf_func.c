#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "all_elf_func.h"


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

//etap 8
	int relsize;
	int nbrel;
	uint32_t S,A,T=0,ref;
	char *str_text=".text";
	char *str_data=".data";
	for(i=0;i<elf.header.e_shnum;i++){
			if(elf.section[i].sh_type==SHT_REL){
				relsize=elf.section[i].sh_size;
				nbrel=relsize/elf.section[i].sh_entsize;
				declaReltab(&elf,i);
				lire_Relo_table(&elf,in,i);
				for(j=0;j<nbrel;j++){
					indice_sym=ELF32_R_SYM(elf.reltab[j].r_info);
					unsigned char *name=elf.strtable+elf.section[elf.symtab[indice_sym].st_shndx].sh_name;
					if(!strcmp((char *)name,str_text)||!strcmp((char *)name,str_data)){
						S=elf.symtab[indice_sym].st_value;
						A=elf.reltab[j].r_offset+elf.section[elf.symtab[indice_sym].st_shndx].sh_offset;
						switch(ELF32_R_TYPE(elf.reltab[j].r_info)){
							case R_ARM_ABS32   :if(ELF32_ST_TYPE(elf.symtab[indice_sym].st_info)==STT_FUNC){
									T=1;}
								    ref=ABS32(S,A,T);
								    fseek(out, A, SEEK_SET);
								    fwrite(&ref,1,sizeof(uint32_t),out);
							break;
							case R_ARM_ABS16    :fseek(out, A, SEEK_SET);
								    ref=ABS16(S,A);
								    fwrite(&ref,1,sizeof(uint32_t),out);
							break;
							case R_ARM_ABS8    :fseek(out, A, SEEK_SET);
								    ref=ABS8(S,A);
								    fwrite(&ref,1,sizeof(uint32_t),out);
							break;
							case R_ARM_CALL   :if(ELF32_ST_TYPE(elf.symtab[indice_sym].st_info)==STT_FUNC){
									T=1;}
									fseek(out, A, SEEK_SET);
									ref=CALL(S,A,T, (uintptr_t)&A);
									fwrite(&ref,1,sizeof(uint32_t),out);
							break;
							case R_ARM_JUMP24   :if(ELF32_ST_TYPE(elf.symtab[indice_sym].st_info)==STT_FUNC){
									T=1;}
									fseek(out, A, SEEK_SET);
									ref=JUMP24(S,A,T,(uintptr_t)&A);
									fwrite(&ref,1,sizeof(uint32_t),out);
							break;
					
						}
					}
				}
			}
	}
	fseek(out, 0, SEEK_SET);
	initElf(&elf_1,out);
	affiche_tableSection(elf_1,out);
	affiche_table_Symbol(elf_1,out);
}

