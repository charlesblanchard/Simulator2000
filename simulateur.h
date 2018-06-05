#include <stdint.h>
#include <stdbool.h>


#define SP 13 /* Stack Pointer */
#define LR 14 /* Link Register */
#define PC 15 /* Program Counter */

#define Z 0
#define N 1
#define C 2
#define V 3

typedef int32_t registre;

typedef struct label{
    uint16_t ADR;
    int8_t NAME[20];    
} Label;

typedef struct machine{
  uint16_t M[256]; /* memory */
  registre REG[16];
  bool PSR[4];
  Label LBL[100];
} Machine;

void init_machine(Machine *m);

void afficher_machine(Machine m);
void afficher_registres(Machine m);
void afficher_PSR(Machine m);
