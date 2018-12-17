#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf_header.h"


int main(int argc, char *argv[]){
         FILE * fp;
         int i;
         Elf64_Ehdr elfheader;
         fp= fopen(argv[1],"r");
         fread(&elfheader,sizeof(Elf64_Ehdr),1,fp);
         for(i=0;i<16; i++)
         printf("%x",elfheader.e_ident[i]);
	
         printf("\n");

         printf("%hx\n", elfheader.e_type);

         printf("%hx\n", elfheader.e_machine);

         printf("%x\n",  elfheader.e_version);

         printf("%llx\n", elfheader.e_entry);

         printf("%llx\n", elfheader.e_phoff);

         printf("%llx\n", elfheader.e_shoff);

         printf("%x\n",  elfheader.e_flags);

         printf("%hx\n", elfheader.e_ehsize);

         printf("%hx\n", elfheader.e_phentsize);

         printf("%hx\n", elfheader.e_phnum);

         printf("%hx\n", elfheader.e_shentsize);

         printf("%hx\n", elfheader.e_shnum);

         printf("%hx\n", elfheader.e_shstrndx);

         return 0;

} 

