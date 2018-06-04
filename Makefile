# Exemple d'un fichier Makefile (sans l'utilisation des macros)
#

main : main.o simulateur.o interpreteur.o arm.o
	gcc -ansi -Wall main.o simulateur.o interpreteur.o arm.o -o main

simulateur.o : simulateur.c
	gcc -ansi -Wall -c simulateur.c -o simulateur.o

interpreteur.o : interpreteur.o
	gcc -ansi -Wall -c interpreteur.c -o interpreteur.o

arm.o : arm.o
	gcc -ansi -Wall -c arm.c -o arm.o

clean :
	rm *.o main
