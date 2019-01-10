#include <stdio.h>
#include <stdlib.h>
#include "myelf.h"
#include "all_elf_func.h"

int main(int argc,char* argv[]){
	Elf32_info elf;
	
	
	
	char c;
//	int rep=0;
	FILE *file,*fout;
    int EIMAG0=127; // Ox7F
	char EIMAG1='E';
	char EIMAG2='L';
	char EIMAG3='F';

    printf("**************************************************************\n");
    printf("*    		  BIENVENUE DANS NOTRE PROGRAMME             *\n");
    printf("*      PROG 5 - Projet: Realisation d'un editeur de liens    *\n");
    printf("*                                                            *\n");
    printf("*                                                            *\n");
    printf("* REALISE PAR :                     ENCADRE PAR :            *\n");
    printf("*  HAFID MOHAMED                       Guillaume Huard       *\n");
    printf("*  TANG LIAN                           Vincent Danjean       *\n");
    printf("*  WANG BIYUN                          Philippe Waille       *\n");
    printf("*  CHENICHENE ABDALLAH                 Christopher Ferreira  *\n");
    printf("*  PRIORE TOM                          Renaud Lachaize       *\n");
    printf("*  WANG XINWEI                         Anne Rasse            *\n");
    printf("**************************************************************\n\n");

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

			do{


				            printf("-------------------------------------------------------------------------\n");  
							printf("-                                   Menu                                -\n"); 
							printf("-------------------------------------------------------------------------\n"); 
							printf("\n");     
							printf("Afficher l'entete                    =================================> h\n");
							printf("Afficher l'entete des sectons        =================================> S\n");
							printf("Afficher le contenu d'une section    =================================> x\n");
							printf("Afficher le table symbole            =================================> s\n");
							printf("Afficher réimplantation              =================================> r\n");
							printf("Supprimer les relocations des tables =================================> d\n");
							printf("Quitter                              =================================> q\n");
							
							//récupère le contenu de la table des sections
							
							
						        scanf(" %c",&c);

								switch(c){
									case 'h': affiche_header(elf); 
										//system("readelf Examples_loader/example1 -h"); 
										break;

									case 'S': affiche_tableSection(elf,file); break;

									case 'x': affiche_contentSection(elf,file); break;
										
									case 's': affiche_table_Symbol(elf,file); break;

									case 'r': affiche_Relocation(&elf,file); break;
										
									case 'd':  
										if(argc == 3){
											mod_sec(elf,file,fout); 
										}else{
											printf("Erreur, entrer un deuxieme fichier elf en argument.\n");
											exit(1);
										}
										break;  // Supp rel.

                                   
                                   case '\n': 
                                   break;
                                   
                                    default:  printf("\nCette option n'existe pas\n");
                                    break;
							
										
							}							

            }
   		    while (c!='q');	
		} 
		else {
			printf("\nCe n'est pas un fichier ELF\n");
		}

	}


      fclose(file);
	return 0;
}
