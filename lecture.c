#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
<<<<<<< HEAD

=======
>>>>>>> 26808e6af5f1f7cbfafb49f908c736a8cf60c26b

#include "simulateur.h"

#include "lecture.h"

void flasher_prgm(Machine *m, char fic_hexa[]){
    
    FILE *f;
    f = fopen(fic_hexa, "r");
    
    if (f == NULL){
        printf("Impossible d'ouvrir le fichier");
        return;
    }
    
        /* Ré-arrangement des mots de 32 bits (avec little endian) */
         
    uint16_t mot_haut, mot_bas;
    uint32_t mot_final;
    int i = 0;
    
    while ((fscanf(f,"%" SCNx16, &mot_haut) == 1) & (fscanf(f,"%" SCNx16, &mot_bas) == 1)){
        
        mot_haut = (mot_haut >> 8) | ((mot_haut &~ 0xff00) << 8);
        mot_bas = (mot_bas >> 8) | ((mot_bas &~ 0xff00) << 8);
        mot_final = (mot_haut << 16) | mot_bas;
        
        m->FLASH[i] = mot_final;
        i++;
    }
    
    /* m->M[i] = ??? */
    i++;
    
    while (i<255){
        m->FLASH[i] = 0;
        i++;
    }
    
    fclose(f);
}
    
