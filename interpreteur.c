#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "simulateur.h"

#include "interpreteur.h"


#define T_MEM 0
#define PC_DER_LIGNE 4031713024 + T_MEM 
#define OP_INCONNU 404



int interpreter(Machine *m){
    
    int32_t i=0;
    int32_t instruction = 0;
    /*lorsqu'on aura mis en place un système d'erreurs*/
    int erreur = 0;
    
    
    do{
        instruction = instruction || m->RAM[i+1] << 24;
        instruction = instruction || m->RAM[i] << 16;
        instruction = instruction || m->RAM[i+3] << 8;
        instruction = instruction || m->RAM[i+2];
        
        if (instruction >> 28 == 0xE){
            
        } else if (instruction >> 28 == 0xF){
        
        } else {
            erreur = 1;
            instruction = PC_DER_LIGNE;
        }
        
    }while(instruction != PC_DER_LIGNE); 
    /*while pc ne revoit pas vers la derniere case*/
    
    if (erreur = 1){
        printf("Opération inconnue, interpretation %d intérrompue\n",i); 
        return OP_INCONNU;
    }
}
