# Définition du compilateur à utiliser
CC 	?= gcc

# Définition des paramètres de compilation
FLAGS	?= -Wall -ansi

# Définition des écutables
EXEC	?= Simulateur Assembleur


all: $(EXEC)

# PROGRAMME PRINCIPAL

Simulateur : arm.o machine.o interpreteur.o lecture.o main.o
	$(CC) $(FLAGS) machine.o arm.o interpreteur.o lecture.o main.o -o Simulateur


main.o : main.c machine.h interpreteur.h lecture.h arm.h
	$(CC) $(FLAGS) -c main.c -o main.o

arm.o : arm.c
	$(CC) $(FLAGS) -c arm.c -o arm.o


machine.o : machine.c
	$(CC) $(FLAGS) -c machine.c -o machine.o


lecture.o : lecture.c machine.h
	$(CC) $(FLAGS) -c lecture.c -o lecture.o


interpreteur.o : interpreteur.c machine.h arm.h arm.c
	$(CC) $(FLAGS) -c interpreteur.c -o interpreteur.o



# PROGRAMME ASM_to_HEX

Assembleur : assemblage.o
	$(CC) $(FLAGS) assemblage.o -o Assembleur

assemblage.o : assemblage.c
	$(CC) $(FLAGS) -c assemblage.c -o assemblage.o



# COMMANDES BONUS

clean :
	rm *.o $(EXEC)

clear : 
	rm *.o
