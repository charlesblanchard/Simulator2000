
void mov_c(registre rd, int8_t x, bool s); //Chargement d'une constante 8 bits
void mvn_c(registre rd, int8_t x, bool s); //Complément à 1 d'une constante 8 bits
void movw_c(registre rd, int16_t x); //Chargement d'une constante 16 bits
void movt_c(registre rd, int16_t x); //Chargement (demi-mot de poids fort)

void mov_r(registre rd, registre rm, bool s); //Transfert (copie) de registre
void mvn_r(registre rd, registre rm, bool s); //Complément à 1 de registre

