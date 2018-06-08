#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "asm_to_hex.h"

#define EXIT_SUCCESS 0
#define ERR_SYNTHAXE 1
#define ERR_F_S 2
#define ERR_F_HEX 3

#define TAILLE_LIGNE 50
 
#define TROIS_REGISTRES 0
#define DEUX_REGISTRES_UNE_VALEUR 1
#define DEUX_REGISTRES 2
#define UN_REGISTRE_UNE_VALEUR 3
 
void nettoyage_instruction(char instruction[]);
unsigned long hash(char *src);

int main(int argc, char *argv[]){
    char instruction[50];
    char mnemonique[5];
    char operande1[20];
    char operande2[20];
    char operande3[20];
    
    int ope1=0;
    int ope2=0;
    int ope3=0;
    
    int s=0;
    int type;
    
    char trash;
    
    if(argc != 2 || argv[1][strlen(argv[1])-1] != 's' || argv[1][strlen(argv[1])-2] != '.'    ){
        printf("Mauvaise synthaxe: veuillez utiliser ./asm_to_hex nom_de_ficher.s\n");
        return ERR_SYNTHAXE;
    }
    
    /* Ouverture des fichiers */
    
    FILE* f_s = fopen(argv[1],"r");
    
    argv[1][strlen(argv[1])-1] = '\0';
    strcat(argv[1],"hex");
    
    FILE* f_hex = fopen(argv[1],"w+");
    
    if(f_s == NULL){
        printf("Erreur ouverture fichier f_s.\n");
        return ERR_F_S;
    }
    if(f_s == NULL){
        printf("Erreur ouverture fichier f_s.\n");
        return ERR_F_HEX;
    }
    
    /* Lecture du fichier */
    
    while( fgets(instruction,TAILLE_LIGNE,f_s)){
        nettoyage_instruction(instruction);
        
        sscanf(instruction,"%s",mnemonique);
        
        if( strcmp(mnemonique,"ldr") == 0 || strcmp(mnemonique,"str") == 0 ){
            sscanf(instruction,"%s %s %s %s",mnemonique,operande1,operande2,operande3);
            
            
        } else {
            sscanf(instruction,"%s %s %s %s",mnemonique,operande1,operande2,operande3);
        
        
            /* RECUPERATION TYPE */
        
            if(operande2[0]=='r' && operande3[0]=='r')
                type = TROIS_REGISTRES;
            if(operande2[0]=='r' && operande3[0]=='#')
                type = DEUX_REGISTRES_UNE_VALEUR;
            if(operande2[0]=='r' && operande3[0]=='\0')
                type = DEUX_REGISTRES;
            if(operande2[0]=='#' && operande3[0]=='\0')
                type = UN_REGISTRE_UNE_VALEUR;
        
        
            /* RECUPERATION S */
        
            if(mnemonique[3]=='s'){
                s=1;
                mnemonique[3]='\0';
            } else {
                s=0;
            }
            
    
            
            /* RECUPERATION OPERANDE */
            sscanf(operande1,"%c%i",&trash,&ope1);
            sscanf(operande2,"%c%i",&trash,&ope2);
            if(type == TROIS_REGISTRES || type == DEUX_REGISTRES_UNE_VALEUR)
                sscanf(operande3,"%c%i",&trash,&ope3);
        
            
            /* CONSTRUCTION */
        
            switch(hash(mnemonique)){
                case MOV:
                    switch(type){
                        case UN_REGISTRE_UNE_VALEUR:
                            /* mov(s) rd,#x */
                            /* 11110000 010s1111 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %xF 0%x %0*x\n",4+s,ope1,2,ope2);
                            break;
                        case DEUX_REGISTRES:
                            /* mov(s) rd,rm */
                            /* 11101010 010s1111 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %xF 0%x 0%x\n",4+s,ope1,ope2);
                            break;                        
                    }
                    break;
                case MVN:
                    switch(type){
                        case UN_REGISTRE_UNE_VALEUR:
                            /* mvn(s) rd,#x */
                            /* 11110000 011s1111 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %xF 0%x %0*x\n",6+s,ope1,2,ope2);
                            break;
                        case DEUX_REGISTRES:
                            /* mvn(s) rd,rm */
                            /* 11101010 011s1111 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %xF 0%x 0%x\n",6+s,ope1,ope2);
                            break;                        
                    }
                    break;
                case MOVW:
                    /* movw rd,#x       avec x = zzzzwyyy xxxxxxxx*/
                    /* 11110w10 0100zzzz 0yyydddd xxxxxxxx */
                    printf("F");
                    break;
                case MOVT:
                    /* movt rd,#x       avec x = zzzzwyyy xxxxxxxx*/
                    /* 11110w10 1100zzzz 0yyydddd xxxxxxxx */
                    printf("F");
                    break;
                
                    
                    
                    
                case AND:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 000snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %x%x 0%x 0%x\n",s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 000snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %x%x 0%x %0*x\n",s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case BIC:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 001snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %x%x 0%x 0%x\n",2+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 001snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %x%x 0%x %0*x\n",2+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ORR:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 010snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %x%x 0%x 0%x\n",4+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 010snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %x%x 0%x %0*x\n",4+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ORN:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 011snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %x%x 0%x 0%x\n",6+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 011snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %x%x 0%x %0*x\n",6+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case EOR:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 100snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EA %x%x 0%x 0%x\n",8+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 100snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F0 %x%x 0%x %0*x\n",8+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ADD:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 000snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EB %x%x 0%x 0%x\n",s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 1111000o ooosnnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F1 %x%x 0%x %0*x\n",s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ADC:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 010snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EB %x%x 0%x 0%x\n",4+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 010snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F1 %x%x 0%x %0*x\n",4+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case SBC:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 011snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EB %x%x 0%x 0%x\n",6+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 011snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F1 %x%x 0%x %0*x\n",6+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case SUB:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 101snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EB %x%x 0%x 0%x\n",10+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 101snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F1 %x%x 0%x %0*x\n",10+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case RSB:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 110snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"EB %x%x 0%x 0%x\n",12+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 110snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"F1 %x%x 0%x %0*x\n",12+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                    
                case MUL:
                    /* mul rd,rn,rm */
                    /* 11111011 0000nnnn 1111dddd 0000mmmm */
                    fprintf(f_hex,"FB 0%x F%x 0%x\n",ope2,ope1,ope3);
                    break;
                    
            }        

            s=0;
            ope1=0;
            ope2=0;
            ope3=0;
        }
    }

    
    /* Fermeture des fichiers */
    fclose(f_s);
    fclose(f_hex);
    
    return EXIT_SUCCESS;
}

void nettoyage_instruction(char instruction[]){
    uint16_t i;
    for(i=0 ; i<strlen(instruction) ; i++){
        if(instruction[i]==',')
            instruction[i] = ' ';
        if(instruction[i]=='%')
            instruction[i] = '\0';
    }
}

unsigned long hash(char *src){
    unsigned long res = 5381;
    int c;
    while(c = *src++){
        res = ((res << 5) + res) + c; /* hash * 33 + c */
    }
    return res;
}
