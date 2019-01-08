#include <stdio.h>
#include <stdlib.h>
#include "myelf.h"
#include "read_elf_func.h"

int main(int argc,char* argv[]){
	Elf32_info elf;
	
	
	
	char c;
	int out = 0;
	FILE *file,*fout;
    int EIMAG0=127; // Ox7F
	char EIMAG1='E';
	char EIMAG2='L';
	char EIMAG3='F';
	file=fopen(argv[1],"r");
	if(argc==3){
		fout=fopen(argv[2],"rb+");
		if(fout==NULL){
			printf("Erreur, entrer deuxieme fichier elf en argument.\n");
			exit(1);
		}
	}
	if(file==NULL){
		printf("Erreur, entrer un fichier elf en argument.\n");
		exit(1);
	}else{
		initElf(&elf,file);
		
		if (elf.header.e_ident[EI_MAG0] == EIMAG0 &&
			elf.header.e_ident[EI_MAG1] == EIMAG1 &&
			elf.header.e_ident[EI_MAG2] == EIMAG2 &&
			elf.header.e_ident[EI_MAG3] == EIMAG3){

			
			
				
			printf("-----------------------\n");  
			printf("-        Menu         -\n"); 
			printf("-----------------------\n");      
			printf("Quitter : q\n");
			printf("Afficher l'entete : h\n");
			printf("Afficher l'entete des sectons : S\n");
			printf("Afficher le contenu d'une section : x\n");
			printf("Afficher le table symbol : s\n");
			printf("Afficher réimplantation : r\n");
			printf("Supprimer les relocations des tables : d\n");
			
			
			//récupère le contenu de la table des sections
			
			
			c=getc(stdin);
			
			while(!out){

				
			
				switch(c){
					case 'h': 
						affiche_header(elf); 
						break;

					case 'S':
						affiche_tableSection(elf,file);	
						break;

					case 'x':
						affiche_contentSection(elf,file);					
						break;
						
					case 's':
						affiche_table_Symbol(elf,file);
						break;

					case 'r':
						affiche_Relocation(&elf,file);
						break;
						
					case 'd': // Supp rel.
						mod_sec(elf,file,fout);
						break;
						
					case 'Q': 
					case 'q': 
						out = 1;
						break;
						
					default: 
						printf("\nCette option n'existe pas\n");
						break;
				}
				c=getc(stdin);

			}

		} else {
			printf("\nCe n'est pas un fichier ELF\n");
		}

	}


      fclose(file);
	return 0;
}
