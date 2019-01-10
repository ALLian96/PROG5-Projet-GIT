#!/bin/sh

choix="r"
rep="Examples_loader"
cd "$rep"
drp=0

		while [ $choix = "r" ]
		do
			for file in *.o
					do  

						   echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
						   echo \*\*         $file          \*\* 
						   echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
						   echo ""
							
					done

				echo Donnez svp un fichier test ELF :
				read -r fic 

				echo Donnez svp un fichier de sortie :
				read -r out 

                touch out 

					for file in *
					do
							if [ $file = $fic ]
							then
								drp=1
									
							fi
					done

					if [ $drp -eq 1 ]
					then
					  cd ..
					  make
					  chmod +x read_elf
					  ./read_elf "Examples_loader/$fic" "Examples_loader/$out"
					else 
						echo Le fichier \< $fic \> n\'existe pas parmi les fichiers de test
					fi

				  echo Vous voulez recommancer avec un autre example  \<====\>  r
				  echo Quitter                                        \<====\>  q
				  read -r rp
		          
		          if [ $rp = "r" ]
		          then
		          	clear
		          	cd "$rep"
		          fi 

				  choix=$rp
		done

			
