#include <stdio.h>

#include "simulateur.h"

void init_machine(Machine *m){
    int i;
    
    for( i=0 ; i<256 ; i++){
        m->M[i] = 0;
    }
    for( i=0 ; i<16 ; i++){
        m->REG[i] = 0;
    }
    for( i=0 ; i<4 ; i++){
        m->PSR[i] = 0;
    }    
}


void afficher_memoire(Machine m){
    int colonne, ligne, adresse;
    
    printf("ADR ");
    for(colonne =0; colonne<16; colonne++)
        printf("    %x",colonne);
    printf("\n   +");
    for (colonne=0; colonne<16; colonne++)	
        printf("−−−−−");
    printf("\n");


    /*16 lignes de 16 nombres de chiffres hexa */

    for (ligne =0;ligne<16;ligne++) {
        adresse = 16*ligne;
        printf("%02x | ",adresse);
        for (colonne=0; colonne<16; colonne++) {
            printf("%04x ", m.M[adresse + colonne]);
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




