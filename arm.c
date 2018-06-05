#include "simulateur.h"

//Chargement d'une constante 8 bits
void mov_c(Machine *M, int8_t rd, int8_t x, bool s){ 
    M->REG[rd] = x;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

//Complément à 1 d'une constante 8 bits
void mvn_c(int8_t rd, int8_t x, bool s){
    M->REG[rd] = ~x;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

//Chargement d'une constante 16 bits
void movw_c(int8_t rd, int16_t x){
    M->REG[rd] = x;
    M->REG[PC] = M->REG[PC]+1;
}

//Chargement (demi-mot de poids fort)
void movt_c(int8_t rd, int16_t x){
    M->REG[rd] = (M->REG[rd] & 0xFFFF) + x<<16;
    M->REG[PC] = M->REG[PC]+1;
}



//Transfert (copie) de registre
void mov_r(int8_t rd, int8_t rm, bool s){
    M->REG[rd] = M->REG[rm];
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

//Complément à 1 de registre
void mvn_r(int8_t rd, int8_t rm, bool s){
    M->REG[rd] = ~M->REG[rm];
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}



void and(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 & op2;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void bic(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 & (~op2);
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void orr(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 | op2;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void orn(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 | (~op2);
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void eor(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 ^ op2;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void add(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 + op2)%(1<<32);
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void adc(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 + op2 + M->PSR[C])%(1<<32);
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void sbc(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 - op2 + M->PSR[C])%(1<<32);
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void sub(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 - op2)%(1<<32);
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void rsb(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op2 - op1)%(1<<32);
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


void mul(int8_t rd, int32_t op1, int32_t op2){
    M->REG[rd] = (op1 * op2)%(1<<32);
    M->REG[PC] = M->REG[PC]+1;
}


void tst(int32_t op1, int32_t op2){
    calcul_PSR( M , op1 & op2 );
}


void cmp(int32_t op1, int32_t op2){
    calcul_PSR( M , (op1 - op2)%(1<<32) );
}


void lsl(int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op2 - op1)%(1<<32);
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}











//Calcul du PSR
void calcul_PSR(Machine *M, int16_t res){
    M->PSR(Z) = res==0;
    M->PSR(N) = res<0;
    M->PSR(C) = 0;
    M->PSR(V) = 0;
}
