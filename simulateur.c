void init_machine(Machine *m){
    int i;
    
    for( i=0 ; i<256 ; i++){
        m->M[i] = 0;
    }
    
    for( i=0 ; i<16 ; i++){
        m->REG[i] = 0;
    }
    
    for( i=0 ; i<4 ; i++){
        m->PSR[i] = 0;
    }
    
    eeeee
    
}

void afficher_machine(Machine m){
    
}

void afficher_memoire(Machine m){
	
	prinf("ADR ");
	for (int colonne =0; colonne<16; colonne++)
		printf("    %x",colonne);
	printf("\n   +");
	for (int colonne=0; colonne<16; colonne++)	
		printf("−−−−−");
	printf("\n");
	
	
	/*16 lignes de 16 nombres de  chiffres hexa */
	
	for (int ligne =0;ligne<16;ligne++) {
		int adresse = 16*ligne;
		printf("%02x | ",adresse);
		for (int colonne=0; colonne<16; colonne++) {
			printf("%04x ",m -> M[adresse + colonne]);
		}
		printf("\n");
	}
	
	
	/* registres */
	
	printf("Registres:  ACC [hex=%04 dec=%6d]  PC [%04x]  HALT [%x]\n" , m->ACC, m->ACC, m->PC, m->HALT); }
	
	
	
	
	
}
