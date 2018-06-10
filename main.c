#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "simulateur.h"
#include "interpreteur.h"
#include "lecture.h"

int main(int argc, char *argv[]){
    Machine *m = init_machine();
        
    /* Charger programme en mémoire */
    flasher_prgm(m, argv[1]);
    
    /* Executer programme */
    interpreter(m);
    
    free(m);
    return 0;
}
