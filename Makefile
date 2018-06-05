# Définition du compilateur à utiliser
CC 	?= gcc

# Définition des paramètres de compilation
FLAGS	?= -W -Wall -ansi

# Exécutables
all: exec


exec : simulateur.o interpreteur.o lecture.o main.o
	$(CC) $(FLAGS) simulateur.o interpreteur.o lecture.o main.o -o exec


main.o : main.c simulateur.h interpreteur.h lecture.h
	$(CC) $(FLAGS) -c main.c -o main.o


simulateur.o : simulateur.c
	$(CC) $(FLAGS) -c simulateur.c -o simulateur.o


lecture.o : lecture.c simulateur.h
	$(CC) $(FLAGS) -c lecture.c -o lecture.o


interpreteur.o : interpreteur.c simulateur.h
	$(CC) $(FLAGS) -c interpreteur.c -o interpreteur.o


arm.o : arm.c
	$(CC) $(FLAGS) -c arm.c -o arm.o


clean :
	rm *.o exec
