#ifndef _SIMU_
#define _SIMU_

#define SP 13 /* Stack Pointer */
#define LR 14 /* Link Register */
#define PC 15 /* Program Counter */

#define Z 0
#define N 1
#define C 2
#define V 3

typedef int32_t registre;

typedef struct machine{
    
    uint16_t FLASH[256]; /* Mémoire FLASH */
    uint16_t RAM[256]; /* Mémoire RAM */
  
    registre REG[16];
    
    bool PSR[4];
    
} Machine;


Machine *init_machine();

void afficher_machine(Machine m);
void afficher_registres(Machine m);
void afficher_PSR(Machine m);

#endif
