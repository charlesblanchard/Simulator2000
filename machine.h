#ifndef _SIMU_
#define _SIMU_

#define SP 13 /* Stack Pointer */
#define LR 14 /* Link Register */
#define PC 15 /* Program Counter */

#define Z 0
#define N 1
#define C 2
#define V 3


typedef struct machine{
    uint8_t FLASH[256]; /* Mémoire FLASH */
    uint8_t RAM[256]; /* Mémoire RAM */
  
    int32_t REG[16];
    
    bool PSR[4];
    
} Machine;


Machine *init_machine();

void afficher_machine(Machine m);
void afficher_memoire(Machine m);
void afficher_flash(Machine m);
void afficher_registres(Machine m);
void afficher_PSR(Machine m);

void affichage(Machine m, char Programme[64][30]);

#endif

