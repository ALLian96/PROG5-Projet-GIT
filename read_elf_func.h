#include "myelf.h"
#include "conversion.h"


// Step1
void affiche_Magic(Elf32_Ehdr header);
void affiche_Classe(Elf32_Ehdr header);
void affiche_DATA(Elf32_Ehdr header);
void affiche_Version(Elf32_Ehdr header);
void affiche_Type(Elf32_Ehdr header);
void affiche_Machine(Elf32_Ehdr header);

void affiche_header(Elf32_Ehdr header);

//Step2
void affiche_tableSection(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section);
void lire_Section_table(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section);

//Step3
void myhexdump(FILE *file,int addr,int size);
void affiche_contentSection(Elf32_Ehdr header,FILE *file,Elf32_Shdr *section);

