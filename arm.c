#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "arm.h"



/* Calcul du PSR */
void calcul_PSR(Machine *M, int16_t res){
    M->PSR[Z] = res==0;
    M->PSR[N] = res<0;
    M->PSR[C] = 0;
    M->PSR[V] = 0;
}




/* Chargement d'une constante 8 bits */
void mov(Machine *M, int8_t rd, int32_t op, bool s){ 
    M->REG[rd] = op;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* Complément à 1 d'une constante 8 bits */
void mvn(Machine *M, int8_t rd, int32_t op, bool s){
    M->REG[rd] = ~op;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* Chargement d'une constante 16 bits */
void movw(Machine *M, int8_t rd, int16_t x){
    M->REG[rd] = x;
    M->REG[PC] = M->REG[PC]+1;
}

/* Chargement (demi-mot de poids fort) */
void movt(Machine *M, int8_t rd, int16_t x){
    M->REG[rd] = (M->REG[rd] & 0xFFFF) + (x<<16);
    M->REG[PC] = M->REG[PC]+1;
}


/* ET bit à bit */
void and(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 & op2;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* ET NON bit à bit */
void bic(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 & (~op2);
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* OU bit à bit */
void orr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 | op2;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* OU NON bit à bit */
void orn(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 | (~op2);
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* OU EXCLU bit à bit */
void eor(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 ^ op2;
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}


/* RD = (OP1 + OP2)%2^32 */
void add(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 + op2)%((int32_t)pow(2,32));
    if(s)
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* RD = (OP1 + OP2 + C)%2^32 */
void adc(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 + op2 + M->PSR[C])%((int32_t)pow(2,32));
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* RD = (OP1 - OP2 - C)%2^32 */
void sbc(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 - op2 + M->PSR[C])%((int32_t)pow(2,32));
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* RD = (OP1 - OP2)%2^32 */
void sub(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 - op2)%((int32_t)pow(2,32));
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* RD = (OP2 - OP1)%2^32 */
void rsb(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op2 - op1)%((int32_t)pow(2,32));
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* RD = (OP1 * OP2)%2^32 */
void mul(Machine *M, int8_t rd, int32_t op1, int32_t op2){
    M->REG[rd] = (op1 * op2)%((int32_t)pow(2,32));
    M->REG[PC] = M->REG[PC]+1;
}



/* MAJ PSR TST op1 & op2 */ 
void tst(Machine *M, int32_t op1, int32_t op2){
    calcul_PSR( M , op1 & op2 );
}

/* MAJ PSR CMP op1 - op2 */ 
void cmp(Machine *M, int32_t op1, int32_t op2){
    calcul_PSR( M , (op1 - op2)%((int32_t)pow(2,32)) );
}



/* Logical left shift */
void lsl(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 * (1<<op2) ;
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* Logical right shift */
void lsr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 / (1<<op2) ;
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}

/* Arithm right shift */
void asr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 / (1<<op2) ;
    if(s) 
        calcul_PSR( M , M->REG[rd] );
    M->REG[PC] = M->REG[PC]+1;
}



/* Store Register byte */
void strb(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    M->RAM[M->REG[rn]+offset] = M->REG[rd];
}

/* Store Register halfword */
void strh(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    M->RAM[M->REG[rn]+offset] = M->REG[rd];
    M->RAM[M->REG[rn]+offset+1] = M->REG[rd];
    
    M->RAM[rd] = rn & 0x00ff;
    M->RAM[rd+1] = rn >> 8;
}

/* Store Register word */
void str(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    M->RAM[M->REG[rn]+offset] = M->REG[rd];
    M->RAM[M->REG[rn]+offset+1] = M->REG[rd];
    M->RAM[M->REG[rn]+offset+2] = M->REG[rd];
    M->RAM[M->REG[rn]+offset+3] = M->REG[rd];
    
    
    M->RAM[rd] = (rn & 0x00ff0000) >> 16;
    M->RAM[rd+1] = (rn >> 24);
    M->RAM[rd+2] = (rn & 0x000000ff);
    M->RAM[rd+3] = (rn & 0x0000ff00) >> 8;
}

/* Load Register byte */
void ldrb(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    rd = M->RAM[rn];
}

/* Load Register halfword */
void ldrh(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    rd = M->RAM[rn] | (M->RAM[rn+1] << 8);
}

/* Load Register word */
void ldr(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    rd = (M->RAM[rn] << 16) | (M->RAM[rn+1] << 24) | (M->RAM[rn+2]) | (M->RAM[rn+3] << 8);
}



/* push {rt} */
void push(Machine *M, int8_t rt){
    M->RAM[M->REG[SP]] = rt;
    M->REG[SP] = M->REG[SP]-4;
}

/* pop {rt} */
void pop(Machine *M, int8_t rt){
    M->REG[rt] = M->RAM[M->REG[SP]];
    M->REG[SP] = M->REG[SP]+4;
}

/* b{}{cond} adresse */
void branch(Machine *M, int8_t cond, int8_t l, int8_t adresse){
    if(l==1){
        M->REG[LR] = M->REG[PC]+4;
    }
    switch(cond){
        case EQ:
            if(M->PSR[Z]==1)
                M->REG[PC] = adresse;
            break;
        case NE:
            if(M->PSR[Z]==0)
                M->REG[PC] = adresse;
            break;
        case HS:
            if(M->PSR[C]==1)
                M->REG[PC] = adresse;
            break;
        case LO:
            if(M->PSR[C]==0)
                M->REG[PC] = adresse;
            break;
        case MI:
            if(M->PSR[N]==1)
                M->REG[PC] = adresse;
            break;
        case PL:
            if(M->PSR[N]==0)
                M->REG[PC] = adresse;
            break;
        case VS:
            if(M->PSR[V]==1)
                M->REG[PC] = adresse;
            break;
        case VC:
            if(M->PSR[V]==0)
                M->REG[PC] = adresse;
            break;
        case HI:
            if(M->PSR[C]==1 && M->PSR[Z]==0)
                M->REG[PC] = adresse;
            break;
        case LS:
            if(M->PSR[C]==0 && M->PSR[Z]==1)
                M->REG[PC] = adresse;
            break;
        case GE:
            if(M->PSR[N]==M->PSR[V])
                M->REG[PC] = adresse;
            break;
        case LT:
            if(M->PSR[Z]!=M->PSR[V])
                M->REG[PC] = adresse;
            break;
        case GT:
            if( M->PSR[Z]==0 && M->PSR[N]==M->PSR[V] )
                M->REG[PC] = adresse;
            break;
        case LE:
            if( M->PSR[Z]==1 && M->PSR[N]!=M->PSR[V] )
                M->REG[PC] = adresse;
            break;
        case AL:
            M->REG[PC] = adresse;
            break;
    }
}
    



