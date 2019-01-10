#include "myelf.h"
#include "conversion.h"
#include <stdio.h>

#define N 16
#define BUFF 8196
#define ABS32(S, A, T)	(S + A)| T
#define ABS16(S, A)	(S + A)
#define ABS8(S, A)	(S + A)
#define CALL(S, A, T, P)	((S + A)| T) - P
#define JUMP24(S, A, T, P)	((S + A)| T) - P

typedef struct{
	Elf32_Ehdr header;
	Elf32_Shdr *section;
	unsigned char *strtable;
	Elf32_Sym  *symtab;
	unsigned char *symtable;
	Elf32_Rel  *reltab;	
}Elf32_info;

void setup_little_endian(Elf32_info *elf);
void initElf(Elf32_info *elf,FILE *file);
void declaSymtab(Elf32_info *elf);
void declaReltab(Elf32_info *elf,int indice_rel);
void lire_Section_table(Elf32_info *elf,FILE *file);
void lire_Symbol_table(Elf32_info *elf,FILE *file);
int get_indice_sym(Elf32_info elf);
int get_indice_str(Elf32_info elf);
// Step1
void affiche_Magic(Elf32_info elf);
void affiche_Classe(Elf32_info elf);
void affiche_DATA(Elf32_info elf);
void affiche_Version(Elf32_info elf);
void affiche_Type(Elf32_info elf);
void affiche_Machine(Elf32_info elf);

void affiche_header(Elf32_info elf);

int get_section_number(Elf32_info elf, char * nom);

//Step2
void init_strtab(Elf32_info *elf,FILE *file);
void affiche_tableSection(Elf32_info elf,FILE *file);


//Step3
void myhexdump(FILE *file,int addr,int size);
void affiche_contentSection(Elf32_info elf,FILE *file);
int get_section(Elf32_info elf, char * nom);


//Stpep4
void init_symtable(Elf32_info *elf,FILE *file);
void affiche_table_Symbol(Elf32_info elf,FILE *file);

//step5
void lire_Relo_table(Elf32_info *elf,FILE *file,int indice_rel);
void affiche_Relocation(Elf32_info *elf,FILE *file);

// PHASE 2
//step 6
void mod_sec(Elf32_info elf, FILE *in, FILE *out);
