#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "simulateur.h"
#include "interpreteur.h"
#include "lecture.h"

int main(int argc, char *argv[]){
    (void)argc; /* Supprimer warning argc innutilisé*/
    int i;
    
    int pas_a_pas = 1; /* 0 pour auto, 1 pour pas a pas */
    int deja_completer = 0;
    
    Machine *m = init_machine();
    char Programme[64][30];
    
    for(i=0;i<64;i++){
        Programme[i][0]='\0';
    }
        
    /* Charger programme en mémoire */
    flasher_prgm(m, argv[1]);
    
    
    /* Executer programme */
    if(!pas_a_pas){
        interpreter(m,Programme,0);
        affichage(*m,Programme);
        getchar();
    } else {
        if(deja_completer){
            interpreter(m,Programme,0);
            m=init_machine();
            flasher_prgm(m, argv[1]);
            interpreter(m,Programme,1);
        } else {
            interpreter(m,Programme,1);
        }
    }
    free(m);
    return 0;
}
