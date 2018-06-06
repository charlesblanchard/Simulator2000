#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "simulateur.h"

#include "interpreteur.h"


#define T_MEM 0
#define PC_DER_LIGNE 4031713024 + T_MEM 



int interpreter(Machine *m){
    
    int32_t i=0;
    int32_t instruction = 0;
    
    
    do{
        instruction = instruction || m->RAM[i+1] << 6;
        instruction = instruction || m->RAM[i] << 4;
        instruction = instruction || m->RAM[i+3] << 2;
        instruction = instruction || m->RAM[i+2];
        
    }while(instruction != PC_DER_LIGNE); 
    /*while pc ne revoit pas vers la derniere case*/
    
}
