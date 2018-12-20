#include <stdio.h>
#include <stdlib.h>
#include "myelf.h"
#include "read_elf_func.h"


int main(int argc,char* argv[]){
	Elf32_Ehdr header;
	char c;
	int out = 0;
	FILE *file;
    int EIMAG0=127;
	char EIMAG1='E';
	char EIMAG2='L';
	char EIMAG3='F';
	file=fopen(argv[1],"r");
	if(file==NULL){
		printf("erreur.\n");
		exit(1);
	}else{
		fread(&header,1,sizeof(header),file);
		if (header.e_ident[EI_MAG0] == EIMAG0 &&
			header.e_ident[EI_MAG1] == EIMAG1 &&
			header.e_ident[EI_MAG2] == EIMAG2 &&
			header.e_ident[EI_MAG3] == EIMAG3){
			
					
			printf("-----------------------\n");  
			printf("-        Menu         -\n"); 
			printf("-----------------------\n");      
			printf("Quitter : q\n");
			printf("Afficher l'entete : h\n");
			printf("Afficher l'entete des sectons : S\n");
			Elf32_Shdr *section = malloc(sizeof(Elf32_Shdr) * swap_uint16(header.e_shnum));
			//récupérer le contenu de section table
			lire_Section_table(header,file,section);
			
			while(!out){

				c=getc(stdin);
			
				switch(c){
					case 'h': 
						affiche_header(header); 
						break;

					case 'S':
						affiche_tableSection(header, file,section);
						break;
					case 'x':
						affiche_contentSection(header,file,section);
					case 'Q': // Q
					case 'q': // q
						out = 1;
						break;
				
					default: 
						printf("\nCette option n'existe pas\n");
						break;
				}

			}

		} else {
			printf("\nC'est pas un fichier ELF\n");
		}

	}


      fclose(file);
	return 0;
}
