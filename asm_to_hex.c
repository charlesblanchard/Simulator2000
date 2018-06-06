#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]){
    
    if(argc != 2 || argv[1][strlen(argv[1])-1] != 's' || argv[1][strlen(argv[1])-2] != '.'    ){
        printf("Mauvaise synthaxe: veuillez utiliser ./asm_to_hex nom_de_ficher.s\n");
        return 1;
    }
    
    FILE* f_s = fopen(argv[1],"r");
    
    argv[1][strlen(argv[1])-1] = '\0';
    strcat(argv[1],"hex");
    
    FILE* f_hex = fopen(argv[1],"a");
    
    return 0;
}
