#include "simulateur.h"
#include "interpreteur.h"

int main(int argc, char *argv[]){
    Machine *m;
    
    char *nom_fichier[];
    int i=0;

    while (argv[1][i]!='.') {
		nom_fichier[i]=argv[1][i];
		i++;
	  }
    nom_fichier[i]='\0';
    i++;
    if (argv[1][i]==h)        /* Si le fichier est en hexa */
    /*charger .hex */
    else if (argv[1][i]==s)
    /* traduire en hexa puis charger */                   
	else printf("Erreure chargement du fichier ");	
    
    return 0;
}
