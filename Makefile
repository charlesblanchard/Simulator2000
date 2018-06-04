# Exemple d'un fichier Makefile (sans l'utilisation des macros)
#

simulateur : simulateur.o
	gcc -ansi -Wall simulateur.o -o simulateur

simulateur.o : simulateur.c
	gcc -ansi -Wall -c simulateur.c -o simulateur.o


clean :
	rm *.o code
