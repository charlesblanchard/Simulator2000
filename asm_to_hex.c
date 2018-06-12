#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "math.h"
#include "asm_to_hex.h"


int main(int argc, char *argv[]){
    FILE* f_s = NULL;
    FILE* f_hex = NULL;
    
    if(argc != 2 || argv[1][strlen(argv[1])-1] != 's' || argv[1][strlen(argv[1])-2] != '.'){
        printf("Mauvaise synthaxe: ./asm_to_hex nom_de_ficher.s\n");
        return ERR_SYNTHAXE;
    }

    /* Ouverture des fichiers */
    ouverture_fichier(argv[1],&f_s,&f_hex);
    
    /* Lecture du fichier */
    lecture_fichier(f_s,f_hex);
    /* Fermeture des fichiers */
    fermeture_fichier(f_s,f_hex);
    return EXIT_SUCCESS;
}



int lecture_fichier(FILE* f_s, FILE* f_hex){
    char instruction[T_INSTRUCTION]; /* exemple : add r1, r2, #4 */
    char mnemonique[T_MNEMONIQUE];   /* add */
    char operande1[T_OPERANDE];      /* r1 */
    char operande2[T_OPERANDE];      /* r2 */
    char operande3[T_OPERANDE];      /* #4 */
    
    char trash;     /* Si op = r1 --> trash = r; si op = #4 --> trash = # */
    
    int ope1, ope2, ope3, i, address;
    int x,y,z,w;
    
    int type, s;

    Label e[20];
    int taille_label=0;
    
    int pc=0;
    
    while( fgets(instruction,TAILLE_LIGNE,f_s)){
        if( instruction[strlen(instruction)-2] == ':'){     /*S'il s'agit d'une étiquette ("boucle :" par exemple)*/
            /* Nouvelle étiquette */
            instruction[strlen(instruction)-2] = '\0';
            
            strcpy( (e[taille_label]).nom_label, instruction);
            (e[taille_label]).valeur_label = pc;
            taille_label = taille_label+1;
            
            /*printf("pc %s = %i\n",instruction,pc);*/
        } else {
            /* Instruction habituelle */
            nettoyage_instruction(instruction);
                
            sscanf(instruction,"\t%s %s %s %s",mnemonique,operande1,operande2,operande3);   /*attribution des valeurs à partir de l'instruction*/
            
            /* RECUPERATION TYPE */
            type = recuperation_type(operande2,operande3);
            sscanf(operande1,"%c%i",&trash,&ope1);
            sscanf(operande2,"%c%i",&trash,&ope2);
            if( type==TROIS_REGISTRES || type==DEUX_REGISTRES_UNE_VALEUR)
                sscanf(operande3,"%c%i",&trash,&ope3);
                   
            /* RECUPERATION S */
            s = recuperation_s(mnemonique);
            
            /*printf("%s %s %s %s - type=%i - s=%i - pc=%i\n",mnemonique,operande1, operande2, operande3, type, s, pc);*/
            
            /* CONSTRUCTION */
            switch(hash(mnemonique)){
                case LDR:
                    switch(type){
                        case DEUX_REGISTRES:
                            /* ldr rd, [rm] */
                            /* 1101mmmm 11111000 00000000 dddd0000 */
                            fprintf(f_hex,"d%xf800%x0\n",ope2,ope1);
                            break;   
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* ldr rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                            /* 1101mmmm 11111000 yyyyzzzz ddddxxxx */
                            x= (ope3 & 0xF00) / pow(2,8);
                            y= (ope3 & 0xF0) / pow(2,4);
                            z= (ope3 & 0xF);
                            fprintf(f_hex,"d%xf8%x%x%x%x\n",ope2,y,z,ope1,x);
                            break;
                    }
                    break;
                case LDRH:
                    switch(type){
                        case DEUX_REGISTRES:
                            /* ldrh rd, [rm] */
                            /* 1011mmmm 11111000 00000000 dddd0000 */
                            fprintf(f_hex,"b%xf800%x0\n",ope2,ope1);
                            break;   
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* ldrh rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                            /* 1011mmmm 11111000 yyyyzzzz ddddxxxx */
                            x= (ope3 & 0xF00) / pow(2,8);
                            y= (ope3 & 0xF0) / pow(2,4);
                            z= (ope3 & 0xF);
                            fprintf(f_hex,"b%xf8%x%x%x%x\n",ope2,y,z,ope1,x);
                            break;
                    }
                    break;
                case LDRB:
                    switch(type){
                        case DEUX_REGISTRES:
                            /* ldrb rd, [rm] */
                            /* 1001mmmm 11111000 00000000 dddd0000 */
                            fprintf(f_hex,"9%xf800%x0\n",ope2,ope1);
                            break;   
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* ldrb rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                            /* 1001mmmm 11111000 yyyyzzzz ddddxxxx */
                            x= (ope3 & 0xF00) / pow(2,8);
                            y= (ope3 & 0xF0) / pow(2,4);
                            z= (ope3 & 0xF);
                            fprintf(f_hex,"9%xf8%x%x%x%x\n",ope2,y,z,ope1,x);
                            break;
                    }
                    break;
                case STR:
                    switch(type){
                        case DEUX_REGISTRES:
                            /* str rd, [rm] */
                            /* 1100mmmm 11111000 00000000 dddd0000 */
                            fprintf(f_hex,"c%xf800%x0\n",ope2,ope1);
                            break;   
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* str rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                            /* 1100mmmm 11111000 yyyyzzzz ddddxxxx */
                            x= (ope3 & 0xF00) / pow(2,8);
                            y= (ope3 & 0xF0) / pow(2,4);
                            z= (ope3 & 0xF);
                            fprintf(f_hex,"c%xf8%x%x%x%x\n",ope2,y,z,ope1,x);
                            break;
                    }                
                    break;
                case STRH:
                    switch(type){
                        case DEUX_REGISTRES:
                            /* strh rd, [rm] */
                            /* 1010mmmm 11111000 00000000 dddd0000 */
                            fprintf(f_hex,"a%xf800%x0\n",ope2,ope1);
                            break;   
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* strh rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                            /* 1010mmmm 11111000 yyyyzzzz ddddxxxx */
                            x= (ope3 & 0xF00) / pow(2,8);
                            y= (ope3 & 0xF0) / pow(2,4);
                            z= (ope3 & 0xF);
                            fprintf(f_hex,"a%xf8%x%x%x%x\n",ope2,y,z,ope1,x);
                            break;
                    }                
                    break;
                case STRB:
                    switch(type){
                        case DEUX_REGISTRES:
                            /* strb rd, [rm] */
                            /* 1000mmmm 11111000 00000000 dddd0000 */
                            fprintf(f_hex,"8%xf800%x0\n",ope2,ope1);
                            break;   
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* strb rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                            /* 1000mmmm 11111000 yyyyzzzz ddddxxxx */
                            x= (ope3 & 0xF00) / pow(2,8);
                            y= (ope3 & 0xF0) / pow(2,4);
                            z= (ope3 & 0xF);
                            fprintf(f_hex,"8%xf8%x%x%x%x\n",ope2,y,z,ope1,x);
                            break;
                    }                
                    break;
                case MOV:
                    switch(type){
                        case UN_REGISTRE_UNE_VALEUR:
                            /* mov(s) rd,#ope2 */
                            /* 11110000 010s1111 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f0%xf0%x%0*x\n",4+s,ope1,2,ope2);
                            break;
                        case DEUX_REGISTRES:
                            /* mov(s) rd,rm */
                            /* 11101010 010s1111 0000dddd 0000mmmm */
                            fprintf(f_hex,"ea%xf0%x0%x\n",4+s,ope1,ope2);
                            break;                        
                    }
                    break;
                case MVN:
                    switch(type){
                        case UN_REGISTRE_UNE_VALEUR:
                            /* mvn(s) rd,#ope2 */
                            /* 11110000 011s1111 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f0%xf0%x%0*x\n",6+s,ope1,2,ope2);
                            break;
                        case DEUX_REGISTRES:
                            /* mvn(s) rd,rm */
                            /* 11101010 011s1111 0000dddd 0000mmmm */
                            fprintf(f_hex,"fA%xf0%x0%x\n",6+s,ope1,ope2);
                            break;                        
                    }
                    break;
                case MOVW:
                    /* movw rd,#ope2       avec ope2 = zzzzwyyy xxxxxxxx*/
                    /* 11110w10 0100zzzz 0yyydddd xxxxxxxx */
                    w = (ope2 & 0x0800)/pow(2,11);
                    z = (ope2 & 0xF000)/pow(2,12);
                    y = (ope2 & 0x0700)/pow(2,8);
                    x = (ope2 & 0x00FF);
                            
                    fprintf(f_hex,"f%x4%x%x%x%0*x\n",2+w,z,y,ope1,2,x);
                    break;
                case MOVT:
                    /* movt rd,#ope2       avec ope2 = zzzzwyyy xxxxxxxx*/
                    /* 11110w10 1100zzzz 0yyydddd xxxxxxxx */
                    w = (ope2 & 0x0800)/pow(2,11);
                    z = (ope2 & 0xF000)/pow(2,12);
                    y = (ope2 & 0x0700)/pow(2,8);
                    x = (ope2 & 0x00FF);
                    
                    fprintf(f_hex,"f%xc%x%x%x%0*x\n",2+w,z,y,ope1,2,x);
                    break;
                    
                        
                        
                case AND:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 000snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"ea%x%x0%x0%x\n",s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 000snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f0%x%x0%x%0*x\n",s,ope2,ope1,2,ope3);
                            break;                        
                        }
                    break;
                case BIC:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 001snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"ea%x%x0%x0%x\n",2+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 001snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f0%x%x0%x%0*x\n",2+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ORR:
                    switch(type){
                        case TROIS_REGISTRES:
                                /* op(s) rd,rn,rm */
                                /* 11101010 010snnnn 0000dddd 0000mmmm */
                                fprintf(f_hex,"ea%x%x0%x0%x\n",4+s,ope2,ope1,ope3);
                                break;
                            case DEUX_REGISTRES_UNE_VALEUR:
                                /* op(s) rd,rn,#x */
                                /* 11110000 010snnnn 0000dddd xxxxxxxx */
                                fprintf(f_hex,"f0%x%x0%x%0*x\n",4+s,ope2,ope1,2,ope3);
                                break;                        
                        }
                        break;
                case ORN:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 011snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"ea%x%x0%x0%x\n",6+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 011snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f0%x%x0%x%0*x\n",6+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case EOR:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101010 100snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"ea%x%x0%x0%x\n",8+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110000 100snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f0%x%x0%x%0*x\n",8+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ADD:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 000snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"eb%x%x0%x0%x\n",s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 1111000o ooosnnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f1%x%x0%x%0*x\n",s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case ADC:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 010snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"eb%x%x0%x0%x\n",4+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 010snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f1%x%x0%x%0*x\n",4+s,ope2,ope1,2,ope3);
                            break;                        
                        }
                    break;
                case SBC:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 011snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"eb%x%x0%x0%x\n",6+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 011snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f1%x%x0%x%0*x\n",6+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case SUB:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 101snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"eb%x%x0%x0%x\n",10+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 101snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f1%x%x0%x%0*x\n",10+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                case RSB:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* op(s) rd,rn,rm */
                            /* 11101011 110snnnn 0000dddd 0000mmmm */
                            fprintf(f_hex,"eb%x%x0%x0%x\n",12+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* op(s) rd,rn,#x */
                            /* 11110001 110snnnn 0000dddd xxxxxxxx */
                            fprintf(f_hex,"f1%x%x0%x%0*x\n",12+s,ope2,ope1,2,ope3);
                            break;                        
                    }
                    break;
                    
                    
                    
                case MUL:
                    /* mul rd,rn,rm */
                    /* 11111011 0000nnnn 1111dddd 0000mmmm */
                    fprintf(f_hex,"fb0%xf%x0%x\n",ope2,ope1,ope3);
                    break;
                    
                    
                    
                case TST:
                    switch(type){
                        case UN_REGISTRE_UNE_VALEUR:
                            /* tst rn,#x */
                            /* 11110000 0001nnnn 00001111 xxxxxxxx */
                            fprintf(f_hex,"f01%x0f%0*x\n",ope1,2,ope2);
                            break;
                        case DEUX_REGISTRES:
                            /* tst rn,rm */
                            /* 11101010 0001nnnn 00001111 0000mmmm */
                            fprintf(f_hex,"ea1%x0f0%x\n",ope1,ope2);
                            break;                        
                    }
                    break;
                case CMP:
                    switch(type){
                        case UN_REGISTRE_UNE_VALEUR:
                            /* cmp rn,#x */
                            /* 11110001 1011nnnn 00001111 xxxxxxxx */
                            fprintf(f_hex,"f1b%x0f%0*x\n",ope1,2,ope2);
                            break;
                        case DEUX_REGISTRES:
                            /* cmp rn,rm */
                            /* 11101011 1011nnnn 00001111 0000mmmm */
                            fprintf(f_hex,"ebb%x0f0%x\n",ope1,ope2);
                            break;                        
                    }
                    break;
                    
                    
                    
                case LSL:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* lsl rd,rn,rm */
                            /* 11111010 000snnnn 1111dddd 0000mmmm */
                            fprintf(f_hex,"fa%x%xf%x0%x\n",s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* lsl rd,rm,#k */
                            /* 11101010 010s1111 0xxxdddd yy00mmmm */
                            x= (ope3 && 0x1C)/pow(2,2);
                            y= ope3 && 0x3;
                            fprintf(f_hex,"ea%xf%x%x%x%x\n",6+s,x ,ope1, y<<2 ,ope2);
                            break;                      
                    }
                    break;
                case LSR:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* lsr rd,rn,rm */
                            /* 11111010 001snnnn 1111dddd 0000mmmm */
                            fprintf(f_hex,"fa%x%xf%x0%x\n",2+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* lsr rd,rm,#k */
                            /* 11101010 010s1111 0xxxdddd yy01mmmm */
                            x= (ope3 && 0x1C)/pow(2,2);
                            y= ope3 && 0x3;
                            fprintf(f_hex,"ea%xf%x%x%x%x\n",6+s,x ,ope1, (y<<2)+1 ,ope2);
                            break;                      
                    }
                    break;
                case ASR:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* asr rd,rn,rm */
                            /* 11111010 010snnnn 1111dddd 0000mmmm */
                            fprintf(f_hex,"fa%x%xf%x0%x\n",4+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* asr rd,rm,#k */
                            /* 11101010 010s1111 0xxxdddd yy10mmmm */
                            x= (ope3 && 0x1C)/pow(2,2);
                            y= ope3 && 0x3;
                            fprintf(f_hex,"ea%xf%x%x%x%x\n",6+s,x ,ope1, (y<<2)+2 ,ope2);
                            break;                      
                    }
                    break;
                case ROR:
                    switch(type){
                        case TROIS_REGISTRES:
                            /* ror rd,rn,rm */
                            /* 11111010 011snnnn 1111dddd 0000mmmm */
                            fprintf(f_hex,"fa%x%xf%x0%x\n",6+s,ope2,ope1,ope3);
                            break;
                        case DEUX_REGISTRES_UNE_VALEUR:
                            /* ror rd,rm,#k */
                            /* 11101010 010s1111 0xxxdddd yy11mmmm */
                            x= (ope3 && 0x1C)/pow(2,2);
                            y= ope3 && 0x3;
                            fprintf(f_hex,"ea%xf%x%x%x%x\n",6+s,x ,ope1, (y<<2)+3 ,ope2);
                            break;                      
                    }
                    break;
                    
                    
                    
                case POP:   
                    /* pop {rd} */
                    /* 01011101 11111000 00000100 dddd1011 */
                    fprintf(f_hex,"fa%x%xf%x0%x\n",6+s,ope2,ope1,ope3);
                    break;   
                case PUSH:
                    /* push {rd} */
                    /* 01001101 11111000 00000100 dddd1101 */
                    fprintf(f_hex,"fa%x%xf%x0%x\n",6+s,ope2,ope1,ope3);
                    break;
                    

                    
                case BEQ:
                    /* beq label */
                    /* 0000 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"0a%06x\n",address);
                    break;
                case BNE:
                    /* bne label */
                    /* 0001 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"1a%06x\n",address);
                    break;
                case BHS:
                    /* bhs label */
                    /* 0010 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"2a%06x\n",address);
                    break;
                case BLO:
                    /* blo label */
                    /* 0011 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"3a%06x\n",address);
                    break;
                case BMI:
                    /* bmi label */
                    /* 0100 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"4a%06x\n",address);
                    break;
                case BPL:
                    /* bpl label */
                    /* 0101 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"5a%06x\n",address);
                    break;
                case BVS:
                    /* bvs label */
                    /* 0110 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"6a%06x\n",address);
                    break;
                case BVC:
                    /* bvc label */
                    /* 0111 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"7a%06x\n",address);
                    break;
                case BHI:
                    /* bhi label */
                    /* 1000 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"8a%06x\n",address);
                    break;
                case BLS:
                    /* bls label */
                    /* 1001 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"9a%06x\n",address);
                    break;
                case BGE:
                    /* bge label */
                    /* 1010 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"aa%06x\n",address);
                    break;
                case BLT:
                    /* blt label */
                    /* 1011 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"ba%06x\n",address);
                    break;
                case BGT:
                    /* bgt label */
                    /* 1100 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"ca%06x\n",address);
                    break;
                case BLE:
                    /* ble label */
                    /* 1101 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"da%06x\n",address);
                    break;
                case BAL:
                    /* bal label */
                    /* 1110 1010 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"fa%06x\n",address);
                    break;
                    
                    
                    
                case BLEQ:
                    /* bleq label */
                    /* 0000 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"0b%06x\n",address);
                    break;
                case BLNE:
                    /* blne label */
                    /* 0001 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"1b%06x\n",address);
                    break;
                case BLHS:
                    /* blhs label */
                    /* 0010 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"2b%06x\n",address);
                    break;
                case BLLO:
                    /* bllo label */
                    /* 0011 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"3b%06x\n",address);
                    break;
                case BLMI:
                    /* blmi label */
                    /* 0100 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"4b%06x\n",address);
                    break;
                case BLPL:
                    /* blpl label */
                    /* 0101 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"5b%06x\n",address);
                    break;
                case BLVS:
                    /* blvs label */
                    /* 0110 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"6b%06x\n",address);
                    break;
                case BLVC:
                    /* blvc label */
                    /* 0111 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"7b%06x\n",address);
                    break;
                case BLHI:
                    /* blhi label */
                    /* 1000 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"8b%06x\n",address);
                    break;
                case BLLS:
                    /* blls label */
                    /* 1001 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"9b%06x\n",address);
                    break;
                case BLGE:
                    /* blge label */
                    /* 1010 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"ab%06x\n",address);
                    break;
                case BLLT:
                    /* bllt label */
                    /* 1011 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"bb%06x\n",address);
                    break;
                case BLGT:
                    /* blgt label */
                    /* 1100 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"cb%06x\n",address);
                    break;
                case BLLE:
                    /* blle label */
                    /* 1101 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"db%06x\n",address);
                    break;
                case BLAL:
                    /* blal label */
                    /* 1110 1011 'adresse label sur 3 octet' */
                    for(i=0; i<taille_label; i++){
                        if(strcmp(operande1,e[i].nom_label)==0)
                            address = e[i].valeur_label;
                    }
                    fprintf(f_hex,"eb%06x\n",address);
                    break;
            }
            pc++;
        }
    }
    fprintf(f_hex,"fa0000FF\n");
    return EXIT_SUCCESS;
}

int ouverture_fichier(char *nom, FILE** f_s, FILE** f_hex){
    *f_s = fopen(nom,"r");
    
    nom[strlen(nom)-1] = '\0';
    strcat(nom,"hex");
    
    *f_hex = fopen(nom,"w+");
    
    if(*f_s == NULL){
        printf("Erreur ouverture fichier f_s.\n");
        return ERR_F_S;
    }
    if(*f_hex == NULL){
        printf("Erreur ouverture fichier f_s.\n");
        return ERR_F_HEX;
    }
    return EXIT_SUCCESS;
}

int fermeture_fichier(FILE* f_s, FILE* f_hex){
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
        if(instruction[i]=='[')
            instruction[i] = ' ';
        if(instruction[i]==']')
            instruction[i] = ' ';
        if(instruction[i]=='{')
            instruction[i] = ' ';
        if(instruction[i]=='}')
            instruction[i] = ' ';
    }
}


unsigned long hash(char *src){
    unsigned long res = 5381;
    int c;
    while( (c = *src++) ){
        res = ((res << 5) + res) + c; 
    }
    return res;
}

int recuperation_type(char *operande2, char *operande3){
    if(operande2[0]=='r' && operande3[0]=='r')
        return TROIS_REGISTRES;
    if(operande2[0]=='r' && operande3[0]=='#')
        return DEUX_REGISTRES_UNE_VALEUR;
    if(operande2[0]=='r' && operande3[0]=='\0')
        return DEUX_REGISTRES;
    if(operande2[0]=='#' && operande3[0]=='\0')
        return UN_REGISTRE_UNE_VALEUR;
    if(operande2[0]=='\0' && operande3[0]=='\0')
        return UN_REGISTRE;
    return -1;
}

int recuperation_s(char *mnemonique){
    if(mnemonique[3]=='s'){
        mnemonique[3]='\0';
        return 1;
    } else {
        return 0;
    }
}
