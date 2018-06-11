#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>


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
    uint32_t mot;
    int i = 0;
    
    while ((fscanf(f,"%" SCNx32, &mot) == 1)){
        mot_haut = (mot >> 16);
        mot_bas = mot & (0x0000ffff);
        mot_haut = (mot_haut >> 8) | ((mot_haut &~ 0xff00) << 8);
        mot_bas = (mot_bas >> 8) | ((mot_bas &~ 0xff00) << 8);
        mot = (mot_haut << 16) | mot_bas;
        
        m->FLASH[i] = mot_final;
        i++;
    }
    
    /* m->M[i] = ??? */
    i++;
    
    fclose(f);
}
    
