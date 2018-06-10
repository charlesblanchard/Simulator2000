# Définition du compilateur à utiliser
CC 	?= gcc

# Définition des paramètres de compilation
FLAGS	?= -Wall -ansi

# Définition des écutables
EXEC	?= simulateur conversion_assembleur_hexa


all: $(EXEC)

# PROGRAMME PRINCIPAL

simulateur : arm.o simulateur.o interpreteur.o lecture.o main.o
	$(CC) $(FLAGS) simulateur.o arm.o interpreteur.o lecture.o main.o -o simulateur


main.o : main.c simulateur.h interpreteur.h lecture.h arm.h
	$(CC) $(FLAGS) -c main.c -o main.o

arm.o : arm.c
	$(CC) $(FLAGS) -c arm.c -o arm.o


simulateur.o : simulateur.c
	$(CC) $(FLAGS) -c simulateur.c -o simulateur.o


lecture.o : lecture.c simulateur.h
	$(CC) $(FLAGS) -c lecture.c -o lecture.o


interpreteur.o : interpreteur.c simulateur.h arm.h arm.c
	$(CC) $(FLAGS) -c interpreteur.c -o interpreteur.o



# PROGRAMME ASM_to_HEX

conversion_assembleur_hexa : asm_to_hex.o
	$(CC) $(FLAGS) asm_to_hex.o -o conversion_assembleur_hexa

asm_to_hex.o : asm_to_hex.c
	$(CC) $(FLAGS) -c asm_to_hex.c -o asm_to_hex.o



# COMMANDES BONUS

clean :
	rm *.o $(EXEC)
