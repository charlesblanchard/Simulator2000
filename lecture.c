#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>

#include "simulateur.h"

#include "lecture.h"

void flasher_prgm(Machine *m, char *fic_hexa){
    char instruction[10];
    int i = 0;
    unsigned int a,b,c,d;
    
    
    /* Ouverture du ficher .hex */
    FILE *f;
    f = fopen(fic_hexa, "r");
    
    if (f == NULL){
        printf("Impossible d'ouvrir le fichier");
        return;
    }
    
    /* Ré-arrangement des mots de 32 bits (avec little endian) */
<<<<<<< HEAD
    while( fgets(instruction,TAILLE_LIGNE,f)){
        sscanf(instruction,"%02x%02x%02x%02x",&a,&b,&c,&d);
        m->FLASH[i] = b;
        m->FLASH[i+1] = a;
        m->FLASH[i+2] = d;
        m->FLASH[i+3] = c;
        i=i+4;
=======
         
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
>>>>>>> cd16972baa1662663e4dea901408cb642ce0f414
    }
    
    fclose(f);
}
    
