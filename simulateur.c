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
