
struct Machine {
  uint16_t M[256]; /* memory */
  int16_t REG[16];
  bool PSR[4];
};

void init_machine(Machine *m);

void afficher_machine(Machine m);
