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
}

void afficher_registres(Machine m){
	
	/* registres */
	printf("Registres: ");
	for (int i=0; i<15;i++){
	printf("r%d = %d \n",i, m->REG[i]); }
	printf("PC= %d \n", m->REG[PC]);
	}
	
void afficher_PSR(Machine m){
    printf("Z=[%d] \n N=[%d] \n C=[%d] \n V=[%d]\n" , m->PSR[0],m->PSR[1], m->PSR[2],m->PSR[3]); }
}	
	
	
	

