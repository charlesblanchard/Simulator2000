void lecture_fichier_hexa(char txt, Machine *m){
    
    FILE *f;
    f = fopen(txt, "r");
    
    if (f == NULL){
        printf("Impossible d'ouvrir le fichier");
        return 0;
    }
    
    //     Ré-arrangement des mots de 32 bits (avec little endian)  //

    uint32_t mot_haut, mot_bas, mot_haut_dupli, mot_bas_dupli;
    int i = 0;
    
    while (feof(flux) == 0){
        scanf("%" SCNx16, &mot_haut);
        scanf("%" SCNx16, &mot_bas);
        
        mot_haut_dupli = mot_haut;
        mot_bas_dupli = mot_bas;
        
        mot_haut = (mot_haut >> 8);
        mot_haut_dupli = ((mot_haut_dupli &~ 0xff00) << 8);
        mot_haut = mot_haut & mot_haut_dupli;
        
        mot_bas = (mot_bas >> 8);
        mot_bas_dupli = ((mot_bas_dupli &~ 0xff00) << 8);
        mot_bas = mot_bas & mot_bas_dupli;
        
        uint32_t mot;
        mot_haut = (mot_haut << 16);
        mot = mot_haut & mot_bas;
        
        m->M[i] = mot;
        i ++;
    }
}
