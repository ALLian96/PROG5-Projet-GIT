#include <stdio.h>
#include <stdlib.h>
#include "myelf.h"
#include "read_elf_func.h"


int main(int argc,char* argv[]){
	Elf32_Ehdr header;
	int n;
	FILE *file;
        int EIMAG0=127;
	char EIMAG1='E';
	char EIMAG2='L';
	char EIMAG3='F';
	file=fopen(argv[1],"r");
	if(file==NULL){
		printf("erreur.\n");
		exit(1);
	}
	else{
		fread(&header,1,sizeof(header),file);
               if (header.e_ident[EI_MAG0] == EIMAG0 &&
		   header.e_ident[EI_MAG1] == EIMAG1 &&
		   header.e_ident[EI_MAG2] == EIMAG2 &&
		   header.e_ident[EI_MAG3] == EIMAG3){
                     printf("\n-----------------------\n");  
                     printf("\n-        Menu         -\n"); 
                     printf("\n-----------------------\n");      

                     printf("\nAfficher l'entete <-------> 1\n");
			
                     scanf("%d",&n);
                     switch(n){
			case 1: affiche_header(header); break;
			case 2: affiche_tableSection(header,file);break;
                        default: printf("\nCe numero n'existe pas\n");
		     }                    
                        
                      
		}
                else {
                     printf("\nC'est pas un fichier ELF\n");
                }
                       	
	}
      fclose(file);
	return 0;
}
