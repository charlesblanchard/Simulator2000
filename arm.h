#ifndef _ARM_
#define _ARM_

#include "simulateur.h"

#include "math.h"



/* Calcul du PSR */
void calcul_PSR(Machine *M, int16_t res);
    


/* Chargement d'une constante 8 bits */
void mov(Machine *M, int8_t rd, int32_t op, bool s); 
    
/* Complément à 1 d'une constante 8 bits */
void mvn(Machine *M, int8_t rd, int32_t op, bool s);
    
/* Chargement d'une constante 16 bits */
void movw(Machine *M, int8_t rd, int16_t x);
    
/* Chargement (demi-mot de poids fort) */
void movt(Machine *M, int8_t rd, int16_t x);
    


/* ET bit à bit */
void and(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* ET NON bit à bit */
void bic(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* OU bit à bit */
void orr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* OU NON bit à bit */
void orn(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);

/* OU EXCLU bit à bit */
void eor(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* RD = (OP1 + OP2)%2^32 */
void add(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* RD = (OP1 + OP2 + C)%2^32 */
void adc(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* RD = (OP1 - OP2 - C)%2^32 */
void sbc(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* RD = (OP1 - OP2)%2^32 */
void sub(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* RD = (OP2 - OP1)%2^32 */
void rsb(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* RD = (OP1 * OP2)%2^32 */
void mul(Machine *M, int8_t rd, int32_t op1, int32_t op2);



/* MAJ PSR TST op1 & op2 */ 
void tst(Machine *M, int32_t op1, int32_t op2);

/* MAJ PSR CMP op1 - op2 */ 
void cmp(Machine *M, int32_t op1, int32_t op2);



/* Logical left shift */
void lsl(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);
    
/* Logical right shift */
void lsr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);

/* Arithm right shift */
void asr(Machine *M, int8_t rd, int32_t op1, int32_t op2, bool s);


#endif





    
    
    
    
    
