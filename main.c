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
    
    Machine *m = init_machine();
    char Programme[64][30];
    
    for(i=0;i<64;i++){
        Programme[i][0]='\0';
    }
        
    /* Charger programme en mémoire */
    flasher_prgm(m, argv[1]);
    
    printf("\n\n");
    /* Executer programme */
    interpreter(m,Programme,1);
    
    free(m);
    return 0;
}
