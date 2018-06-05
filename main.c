#include "simulateur.h"
#include "interpreteur.h"
#include "lecture.h"

int main(int argc, char *argv[]){
    int i=0;
    
    Machine *m;
    init_machine(*m);
    
    
    /*if( argv[1][strlen(argv[1]-1] == 's'){
        assembleur_to_hex(argv[1]);
        
    }*/
    
    /* Charger en hexa */
    flasher_prgm(m, argv[1] );
        
        
    return 0;
}
