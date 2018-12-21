#include "myelf.h"
#include "conversion.h"

typedef struct{
	Elf32_Ehdr header;
	Elf32_Shdr *section;
	unsigned char *strtable;
	Elf32_Sym  *symtab;	
}Elf32_info;

void initElf(Elf32_info *elf,FILE *file);
void lire_Section_table(Elf32_info *elf,FILE *file);
// Step1
void affiche_Magic(Elf32_info elf);
void affiche_Classe(Elf32_info elf);
void affiche_DATA(Elf32_info elf);
void affiche_Version(Elf32_info elf);
void affiche_Type(Elf32_info elf);
void affiche_Machine(Elf32_info elf);

void setup_little_endian(Elf32_info *elf);

void affiche_header(Elf32_info elf);

//Step2
void init_strtab(Elf32_info *elf,FILE *file);
void affiche_tableSection(Elf32_info elf,FILE *file);


//Step3
void myhexdump(FILE *file,int addr,int size);
void affiche_contentSection(Elf32_info elf,FILE *file);
int get_section(Elf32_info elf, char * nom);


//Stpep4
void affiche_table_Symboles(Elf32_Shdr header,FILE *file);
