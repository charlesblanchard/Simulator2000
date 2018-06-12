#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "simulateur.h"

Machine *init_machine(){
    int i;
    Machine *m = (Machine *) malloc(sizeof(Machine));
    
    for( i=0 ; i<256 ; i++){
        m->RAM[i] = 0;
    }
    
    for( i=0 ; i<256 ; i++){
        m->FLASH[i] = 0;
    }
    
    for( i=0 ; i<16 ; i++){
        m->REG[i] = 0;
    }
    m->REG[SP] = 256;
    
    for( i=0 ; i<4 ; i++){
        m->PSR[i] = 0;
    }
    return m;
}


void afficher_memoire(Machine m){
    int colonne, ligne, adresse;
    
    printf("ADR ");
    for(colonne =0; colonne<4; colonne++)
        printf("  %x",colonne);
    printf("\n   +-");
    for (colonne=0; colonne<4; colonne++)	
        printf("−−-");
    printf("\n");


    /*16 lignes de 16 nombres de chiffres hexa */

    for (ligne =0;ligne<64;ligne++) {
        adresse = 4*ligne;
        printf("%02x | ",adresse);
        for (colonne=0; colonne<4; colonne++) {
            printf("%02x ", m.RAM[adresse + colonne]);
        }
        printf("\n");
    }
}


void afficher_flash(Machine m){
    int colonne, ligne, adresse;
    
    printf("ADR ");
    for(colonne =0; colonne<4; colonne++)
        printf("  %x",colonne);
    printf("\n   +-");
    for (colonne=0; colonne<4; colonne++)	
        printf("−−-");
    printf("\n");


    /*16 lignes de 16 nombres de chiffres hexa */

    for (ligne =0;ligne<64;ligne++) {
        adresse = 4*ligne;
        printf("%02x | ",adresse);
        for (colonne=0; colonne<4; colonne++) {
            printf("%02x ", m.FLASH[adresse + colonne]);
        }
        printf("\n");
    }
}


void afficher_registres(Machine m){
    int i;
    
    printf("Registres: ");
    for (i=0; i<15;i++){
    printf("r%d = %d \n",i, m.REG[i]); }
    printf("PC= %d \n", m.REG[PC]);
}
    
void afficher_PSR(Machine m){
    printf("Z=[%d] \n N=[%d] \n C=[%d] \n V=[%d]\n" , m.PSR[0],m.PSR[1], m.PSR[2],m.PSR[3]); 

}

void afficher_machine(Machine m){
    afficher_registres(m);
    afficher_PSR(m);
    afficher_memoire(m);
}




