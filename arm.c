#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "arm.h"


/* Chargement d'une constante 8 bits */
void mov(Machine *M, int8_t rd, int32_t op, bool s){ 
    M->REG[rd] = op;
    int64_t res = op;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
    }
}

/* Complément à 1 d'une constante 8 bits */
void mvn(Machine *M, int8_t rd, int32_t op, bool s){
    M->REG[rd] = ~op;
    int64_t res = ~op;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
    }
}

/* Chargement d'une constante 16 bits */
void movw(Machine *M, int8_t rd, int16_t x){
    M->REG[rd] = x;
}

/* Chargement (demi-mot de poids fort) */
void movt(Machine *M, int8_t rd, int16_t x){
    M->REG[rd] = (M->REG[rd] & 0xFFFF) + (x<<16);
}


/* ET bit à bit */
void and(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 & op2;
    int64_t res = op1 & op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}

/* ET NON bit à bit */
void bic(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 & (~op2);
    int64_t res = op1 & (~op2);
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}

/* OU bit à bit */
void orr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 | op2;
    int64_t res = op1 | op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}

/* OU NON bit à bit */
void orn(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 | (~op2);
    int64_t res = op1 | (~op2);
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}

/* OU EXCLU bit à bit */
void eor(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 ^ op2;
    int64_t res = op1 ^ op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        
        M->PSR[V] = 0;
    }
}


/* RD = (OP1 + OP2)%2^32 */
void add(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    /*M->REG[rd] = (op1 + op2)%((int32_t)pow(2,32));*/
    printf("op1= %i - op2 =%i\n",op1,op2);
    M->REG[rd] = (op1 + op2);
    int64_t res = op1 + op2;
    
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[V] = ((~op1+0x1) + (~op2+0x1)) > 0xFFFFFFFF;
        M->PSR[C] = res>0xFFFFFFFF;
    }
}

/* RD = (OP1 + OP2 + C)%2^32 */
void adc(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 + op2 + M->PSR[C])%((int32_t)pow(2,32));
    int64_t res = op1 + op2 + M->PSR[C];
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[V] = ((~op1+0x1) + (~op2+0x1)+ M->PSR[C]) > 0xFFFFFFFF;
        M->PSR[C] = res>0xFFFFFFFF;
    }
}

/* RD = (OP1 - OP2 - C)%2^32 */
void sbc(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 - op2 + M->PSR[C])%((int32_t)pow(2,32));
    int64_t res = op1 - op2 + M->PSR[C];
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[V] = ((~op1+0x1) - (~op2+0x1) + M->PSR[C]) > 0xFFFFFFFF;
        M->PSR[C] = res>0xFFFFFFFF;
       
    }
}

/* RD = (OP1 - OP2)%2^32 */
void sub(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op1 - op2)%((int32_t)pow(2,32));
    int64_t res = op1 - op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = res>0xFFFFFFFF;
        M->PSR[V] =((~op1+0x1) - (~op2+0x1)) > 0xFFFFFFFF;
    }
}

/* RD = (OP2 - OP1)%2^32 */
void rsb(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = (op2 - op1)%((int32_t)pow(2,32));
    int64_t res = op2 - op1;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = res>0xFFFFFFFF;
        M->PSR[V] =((~op2+0x1) - (~op1+0x1)) > 0xFFFFFFFF;
    }
}

/* RD = (OP1 * OP2)%2^32 */
void mul(Machine *M, int8_t rd, int32_t op1, int32_t op2){
    M->REG[rd] = (op1 * op2)%((int32_t)pow(2,32));
}



/* MAJ PSR TST op1 & op2 */ 
void tst(Machine *M, int32_t op1, int32_t op2){
    int64_t res = op1 & op2;
    M->PSR[Z] = res==0;
    M->PSR[N] = res<0;
    M->PSR[C] = 0;
    M->PSR[V] = 0;
}

/* MAJ PSR CMP op1 - op2 */ 
void cmp(Machine *M, int32_t op1, int32_t op2){
    int64_t res = op1 - op2;
    M->PSR[Z] = res==0;
    M->PSR[N] = res<0;
    M->PSR[C] = 0;
    M->PSR[V] = 0;
}



/* Logical left shift */
void lsl(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 * (1<<op2) ;

    int64_t res = op1 - op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}

/* Logical right shift */
void lsr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 / (1<<op2) ;
    int64_t res = op1 - op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}

/* Arithm right shift */
void asr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s){
    M->REG[rd] = op1 / (1<<op2) ;
    int64_t res = op1 - op2;
    if(s){
        M->PSR[Z] = res==0;
        M->PSR[N] = res<0;
        M->PSR[C] = 0;
        M->PSR[V] = 0;
    }
}



/* Store Register byte */
void strb(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    /* Stockage en little-endian */
    M->RAM[M->REG[rn]+offset] = M->REG[rd] && 0xFF;
}

/* Store Register halfword */
void strh(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    /* Stockage en little-endian */
    M->RAM[M->REG[rn]+offset] = M->REG[rd] & 0xFF;
    M->RAM[M->REG[rn]+offset+1] = (M->REG[rd] & 0xFF00)/pow(2,8);
}

/* Store Register word */
void str(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    /* Stockage en little-endian */
    M->RAM[M->REG[rn]+offset] = (M->REG[rd] & 0xFF000000)/pow(2,24);
    M->RAM[M->REG[rn]+offset+1] = (M->REG[rd] & 0xFF0000)/pow(2,16);
    M->RAM[M->REG[rn]+offset+2] = (M->REG[rd] & 0xFF00)/pow(2,8);
    M->RAM[M->REG[rn]+offset+3] = M->REG[rd] & 0xFF;
}

/* Load Register byte */
void ldrb(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    /* Chargement en little-endian */
    M->REG[rd] = M->RAM[M->REG[rn]+offset];
}

/* Load Register halfword */
void ldrh(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    /* Chargement en little-endian */
    M->REG[rd] = M->RAM[M->REG[rn]+offset]*pow(2,8);
    M->REG[rd] += M->RAM[M->REG[rn]+offset+1];
}

/* Load Register word */
void ldr(Machine *M, int8_t rd, int8_t rn, int32_t offset){
    /* Chargement en little-endian */
    M->REG[rd] = M->RAM[M->REG[rn]+offset]*pow(2,24);
    M->REG[rd] += M->RAM[M->REG[rn]+offset+1]*pow(2,16);
    M->REG[rd] += M->RAM[M->REG[rn]+offset+2]*pow(2,8);
    M->REG[rd] += M->RAM[M->REG[rn]+offset+3];
}



/* push {rt} */
void push(Machine *M, int8_t rt){
    if(M->REG[SP]!=0x00){
        M->REG[SP] = M->REG[SP]-4;
        ldr(M,rt,SP,0);
    }
}

/* pop {rt} */
void pop(Machine *M, int8_t rt){
    if(M->REG[SP]!=0xFF){
        str(M,rt,SP,0);
        M->REG[SP] = M->REG[SP]+4;
    }
}

/* b{}{cond} adresse */
void branch(Machine *M, int8_t cond, int8_t l, int8_t adresse){
    if(l==1){
        M->REG[LR] = M->REG[PC]+4;
    }
    adresse *= 4;
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
    



