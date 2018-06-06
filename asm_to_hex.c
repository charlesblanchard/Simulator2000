#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "asm_to_hex.h"

#define EXIT_SUCCESS 0
#define ERR_SYNTHAXE 1
#define ERR_F_S 2
#define ERR_F_HEX 3

#define TAILLE_LIGNE 50
 
void nettoyage_instruction(char instruction[]);
unsigned long hash(char *src);

int main(int argc, char *argv[]){
    char instruction[50];
    char mnemonique[5];
    char dest[20];
    char operande1[20];
    char operande2[20];
    int s;
    
    
    
    if(argc != 2 || argv[1][strlen(argv[1])-1] != 's' || argv[1][strlen(argv[1])-2] != '.'    ){
        printf("Mauvaise synthaxe: veuillez utiliser ./asm_to_hex nom_de_ficher.s\n");
        return ERR_SYNTHAXE;
    }
    
    /* Ouverture des fichiers */
    
    FILE* f_s = fopen(argv[1],"r");
    
    argv[1][strlen(argv[1])-1] = '\0';
    strcat(argv[1],"hex");
    
    FILE* f_hex = fopen(argv[1],"w+");
    
    if(f_s == NULL){
        printf("Erreur ouverture fichier f_s.\n");
        return ERR_F_S;
    }
    if(f_s == NULL){
        printf("Erreur ouverture fichier f_s.\n");
        return ERR_F_HEX;
    }
    
    /* Lecture du fichier */
    
    while( fgets(instruction,TAILLE_LIGNE,f_s)){
        nettoyage_instruction(instruction);
        sscanf(instruction,"%s %s %s %s",mnemonique,dest,operande1,operande2);
        
        /* RECUPERATION S */
        
        if(mnemonique[3]=='S'){
            s=1;
            mnemonique[3]='\0';
        }
        
        /* RECUPERATION MNEMONIQUE */
        
        switch(hash(mnemonique)){
            case MOV:
                
                break;
            case SUB:
            case ADD:
                
                break;
        }
        
        /* RECUPERATION OPERANDE */
        
        s=0;
    }
    
    
    /* Fermeture des fichiers */
    fclose(f_s);
    fclose(f_hex);
    
    return EXIT_SUCCESS;
}

void nettoyage_instruction(char instruction[]){
    uint16_t i;
    for(i=0 ; i<strlen(instruction) ; i++){
        if(instruction[i]==',')
            instruction[i] = ' ';
        if(instruction[i]=='#')
            instruction[i] = '\0';
    }
}

unsigned long hash(char *src){
    unsigned long res = 5381;
    int c;
    while(c = *src++)
        res = ((res << 5) + res) + c; /* hash * 33 + c */
    return res;
}
