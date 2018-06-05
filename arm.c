#include "simulateur.h"

//Chargement d'une constante 8 bits
void mov_c(Machine *M, registre rd, int8_t x, bool s){ 
    M->REG[rd] = x;
    calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

//Complément à 1 d'une constante 8 bits
void mvn_c(registre rd, int8_t x, bool s){
    M->REG[rd] = ~x;
    calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

//Chargement d'une constante 16 bits
void movw_c(registre rd, int16_t x){
    M->REG[rd] = x;
    M->REG[PC] = M->REG[PC]+1;
}

//Chargement (demi-mot de poids fort)
void movt_c(registre rd, int16_t x){
    M->REG[rd] = 
    M->REG[PC] = M->REG[PC]+1;
}



//Transfert (copie) de registre
void mov_r(registre rd, registre rm, bool s){
    
    
    
}

//Complément à 1 de registre
void mvn_r(registre rd, registre rm, bool s); 



void calcul_PSR(Machine *M, int16_t res){
    M->PSR(Z) = res==0;
    M->PSR(N) = res<0;
    M->PSR(C) = 0;
    M->PSR(V) = 0;
}
