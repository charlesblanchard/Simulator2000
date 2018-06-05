#define SP 13 // Stack Pointer
#define LR 14 // Link Register
#define PC 15 // Program Counter

#define Z 0
#define N 1
#define C 2
#define V 3

typedef registre int32_t;

struct Machine {
  uint16_t M[256]; /* memory */
  registre REG[16];
  bool PSR[4];
};

void init_machine(Machine *m);

void afficher_machine(Machine m);
