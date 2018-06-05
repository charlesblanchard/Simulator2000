#include "stdio.h"
#include "interpreteur.h"

#define PC_DER_LIGNE 4031713024 + T_MEM

int lecture_memoire(Machine *m){
    
    int i=0;
    int instruction = 0;
    
    
    do{
        instruction = instruction || m->M[i+1] << 6;
        instruction = instruction || m->M[i] << 4;
        instruction = instruction || m->M[i+3] << 2;
        instruction = instruction || m->M[i+2];
        
    }while(instruction != PC_DER_LIGNE)/*while pc ne revoit pas vers la derniere case*/
    
}
