#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    
    m->REG[SP] = 0xFF;
    m->REG[LR] = 0xFF;
    m->REG[PC] = 0x00;
    
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

void affichage(Machine m, char Programme[64][30]){
    int i;
    int ligne, colonne,adresse;
    char indicateur_pc[3];
    int fin = 0;
    printf("\n\n");
    printf("#####################################################################################################\n");
    printf("#                     # r0 = %4i | r4 = %4i | r8  = %4i | r12 = %4i | Z = %i #                   #\n",m.REG[0],m.REG[4],m.REG[8],m.REG[12],m.PSR[Z]);
    printf("#                     # r1 = %4i | r5 = %4i | r9  = %4i |  SP = %4i | N = %i #                   #\n",m.REG[1],m.REG[5],m.REG[9],m.REG[13],m.PSR[N]);
    printf("#        FLASH        # r2 = %4i | r6 = %4i | r10 = %4i |  LR = %4i | C = %i #        RAM        #\n",m.REG[2],m.REG[6],m.REG[10],m.REG[14],m.PSR[C]);
    printf("#                     # r3 = %4i | r7 = %4i | r11 = %4i |  PC = %4i | V = %i #                   #\n",m.REG[3],m.REG[7],m.REG[11],m.REG[15],m.PSR[V]);
    printf("#####################################################################################################\n");
    printf("#   ADR |  0  1  2  3 #                                                         # ADR |  0  1  2  3 #\n");
    printf("#-------+-------------#                                                         #-----+-------------#\n");

    
    for (ligne =0;ligne<32;ligne++) {
        adresse = 4*ligne;
        printf("# ");
        if(ligne==m.REG[PC]){
            green();
            strcpy(indicateur_pc,"->");
        } else {
            strcpy(indicateur_pc,"  ");
        }
        printf("%s %02x | ",indicateur_pc,adresse);
        for (colonne=0; colonne<4; colonne++) {
            printf("%02x ", m.FLASH[adresse + colonne]);
        }
        reset();
        printf("# %s ",indicateur_pc);
        fin=0;
        for(i=0;i<30;i++){
            if(Programme[m.REG[PC]][i]=='\0')
                fin=1;
            if(!fin)
                printf("%c",Programme[m.REG[PC]][i]);
            if(fin)
                printf(" ");
        }
        adresse = 4*ligne;
        printf("                       #  %02x | ",adresse);
        for (colonne=0; colonne<4; colonne++) {
            printf("%02x ", m.RAM[adresse + colonne]);
        }
        printf("#\n");
    }
    printf("#####################################################################################################\n");
}

void bold_red() {
  printf("\033[1;31m");
}
void red() {
  printf("\033[0;31m");
}


void green(){
  printf("\033[0;32m");
}
void bold_green(){
  printf("\033[1;32m");
}


void reset(){
  printf("\033[0m");
}

