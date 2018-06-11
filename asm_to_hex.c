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
    char instruction[T_INSTRUCTION];
    char mnemonique[T_MNEMONIQUE];
    
    char prefix1, prefix2, prefix3;
    int ope1, ope2, ope3;
    int x,y,z,w;
    
    int type, s;
    
    Ens_Label e;
    e.taille=0;
    
    int pc=0;
    
    while( fgets(instruction,TAILLE_LIGNE,f_s)){
        if( instruction[strlen(instruction)-1]==':'){
            instruction[strlen(instruction)-1]='\0';
            strcpy( (e.tab[e.taille]).nom_label, instruction);
            (e.tab[e.taille]).valeur_label = pc;
            e.taille = e.taille+1;            
        } else {

        nettoyage_instruction(instruction);
                
        sscanf(instruction,"%s %c%i %c%i %c%i",mnemonique,&prefix1,&ope1,&prefix2,&ope2,&prefix3,&ope3);
                
        /* RECUPERATION TYPE */
        type = recuperation_type(prefix2,prefix3);
    
        /* RECUPERATION S */
        s = recuperation_s(mnemonique);
                  
        /* CONSTRUCTION */
        
        switch(hash(mnemonique)){
            case LDR:
                switch(type){
                    case DEUX_REGISTRES:
                        /* ldr rd, [rm] */
                        /* 1101mmmm 11111000 00000000 dddd0000 */
                        fprintf(f_hex,"D%x F8 00 %x0\n",ope2,ope1);
                        break;   
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* ldr rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                        /* 1101mmmm 11111000 yyyyzzzz ddddxxxx */
                        x= (ope3 & 0xF00) / pow(2,8);
                        y= (ope3 & 0xF0) / pow(2,4);
                        z= (ope3 & 0xF);
                        fprintf(f_hex,"D%x F8 %x%x %x%x\n",ope2,y,z,ope1,x);
                        break;
                }
                break;
            case LDRH:
                switch(type){
                    case DEUX_REGISTRES:
                        /* ldrh rd, [rm] */
                        /* 1011mmmm 11111000 00000000 dddd0000 */
                        fprintf(f_hex,"B%x F8 00 %x0\n",ope2,ope1);
                        break;   
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* ldrh rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                        /* 1011mmmm 11111000 yyyyzzzz ddddxxxx */
                        x= (ope3 & 0xF00) / pow(2,8);
                        y= (ope3 & 0xF0) / pow(2,4);
                        z= (ope3 & 0xF);
                        fprintf(f_hex,"B%x F8 %x%x %x%x\n",ope2,y,z,ope1,x);
                        break;
                }
                break;
            case LDRB:
                switch(type){
                    case DEUX_REGISTRES:
                        /* ldrb rd, [rm] */
                        /* 1001mmmm 11111000 00000000 dddd0000 */
                        fprintf(f_hex,"9%x F8 00 %x0\n",ope2,ope1);
                        break;   
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* ldrb rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                        /* 1001mmmm 11111000 yyyyzzzz ddddxxxx */
                        x= (ope3 & 0xF00) / pow(2,8);
                        y= (ope3 & 0xF0) / pow(2,4);
                        z= (ope3 & 0xF);
                        fprintf(f_hex,"9%x F8 %x%x %x%x\n",ope2,y,z,ope1,x);
                        break;
                }
                break;
            case STR:
                switch(type){
                    case DEUX_REGISTRES:
                        /* str rd, [rm] */
                        /* 1100mmmm 11111000 00000000 dddd0000 */
                        fprintf(f_hex,"C%x F8 00 %x0\n",ope2,ope1);
                        break;   
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* str rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                        /* 1100mmmm 11111000 yyyyzzzz ddddxxxx */
                        x= (ope3 & 0xF00) / pow(2,8);
                        y= (ope3 & 0xF0) / pow(2,4);
                        z= (ope3 & 0xF);
                        fprintf(f_hex,"C%x F8 %x%x %x%x\n",ope2,y,z,ope1,x);
                        break;
                }                
                break;
            case STRH:
                switch(type){
                    case DEUX_REGISTRES:
                        /* strh rd, [rm] */
                        /* 1010mmmm 11111000 00000000 dddd0000 */
                        fprintf(f_hex,"A%x F8 00 %x0\n",ope2,ope1);
                        break;   
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* strh rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                        /* 1010mmmm 11111000 yyyyzzzz ddddxxxx */
                        x= (ope3 & 0xF00) / pow(2,8);
                        y= (ope3 & 0xF0) / pow(2,4);
                        z= (ope3 & 0xF);
                        fprintf(f_hex,"A%x F8 %x%x %x%x\n",ope2,y,z,ope1,x);
                        break;
                }                
                break;
            case STRB:
                switch(type){
                    case DEUX_REGISTRES:
                        /* strb rd, [rm] */
                        /* 1000mmmm 11111000 00000000 dddd0000 */
                        fprintf(f_hex,"8%x F8 00 %x0\n",ope2,ope1);
                        break;   
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* strb rd, [rm,#x] avec ope3 = xxxxyyyyzzzz*/
                        /* 1000mmmm 11111000 yyyyzzzz ddddxxxx */
                        x= (ope3 & 0xF00) / pow(2,8);
                        y= (ope3 & 0xF0) / pow(2,4);
                        z= (ope3 & 0xF);
                        fprintf(f_hex,"8%x F8 %x%x %x%x\n",ope2,y,z,ope1,x);
                        break;
                }                
                break;
            case MOV:
                switch(type){
                    case UN_REGISTRE_UNE_VALEUR:
                        /* mov(s) rd,#ope2 */
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
                        /* mvn(s) rd,#ope2 */
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
                /* movw rd,#ope2       avec ope2 = zzzzwyyy xxxxxxxx*/
                /* 11110w10 0100zzzz 0yyydddd xxxxxxxx */
                w = (ope2 & 0x0800)/pow(2,11);
                z = (ope2 & 0xF000)/pow(2,12);
                y = (ope2 & 0x0700)/pow(2,8);
                x = (ope2 & 0x00FF);
                        
                fprintf(f_hex,"F%x 4%x %x%x %0*x\n",2+w,z,y,ope1,2,x);
                break;
            case MOVT:
                /* movt rd,#ope2       avec ope2 = zzzzwyyy xxxxxxxx*/
                /* 11110w10 1100zzzz 0yyydddd xxxxxxxx */
                w = (ope2 & 0x0800)/pow(2,11);
                z = (ope2 & 0xF000)/pow(2,12);
                y = (ope2 & 0x0700)/pow(2,8);
                x = (ope2 & 0x00FF);
                
                fprintf(f_hex,"F%x C%x %x%x %0*x\n",2+w,z,y,ope1,2,x);
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
            case TST:
                switch(type){
                    case UN_REGISTRE_UNE_VALEUR:
                        /* tst rn,#x */
                        /* 11110000 0001nnnn 00001111 xxxxxxxx */
                        fprintf(f_hex,"F0 1%x 0F %0*x\n",ope1,2,ope2);
                        break;
                    case DEUX_REGISTRES:
                        /* tst rn,rm */
                        /* 11101010 0001nnnn 00001111 0000mmmm */
                        fprintf(f_hex,"EA 1%x 0F 0%x\n",ope1,ope2);
                        break;                        
                }
                break;
            case CMP:
                switch(type){
                    case UN_REGISTRE_UNE_VALEUR:
                        /* cmp rn,#x */
                        /* 11110001 1011nnnn 00001111 xxxxxxxx */
                        fprintf(f_hex,"F1 B%x 0F %0*x\n",ope1,2,ope2);
                        break;
                    case DEUX_REGISTRES:
                        /* cmp rn,rm */
                        /* 11101011 1011nnnn 00001111 0000mmmm */
                        fprintf(f_hex,"EB B%x 0F 0%x\n",ope1,ope2);
                        break;                        
                }
                break;
            case LSL:
                switch(type){
                    case TROIS_REGISTRES:
                        /* lsl rd,rn,rm */
                        /* 11111010 000snnnn 1111dddd 0000mmmm */
                        fprintf(f_hex,"FA %x%x F%x 0%x\n",s,ope2,ope1,ope3);
                        break;
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* lsl rd,rm,#k */
                        /* 11101010 010s1111 0xxxdddd yy00mmmm */
                        x= (ope3 && 0x1C)/pow(2,2);
                        y= ope3 && 0x3;
                        fprintf(f_hex,"EA %xF %x%x %x%x\n",6+s,x ,ope1, y<<2 ,ope2);
                        break;                      
                }
                break;
            case LSR:
                switch(type){
                    case TROIS_REGISTRES:
                        /* lsr rd,rn,rm */
                        /* 11111010 001snnnn 1111dddd 0000mmmm */
                        fprintf(f_hex,"FA %x%x F%x 0%x\n",2+s,ope2,ope1,ope3);
                        break;
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* lsr rd,rm,#k */
                        /* 11101010 010s1111 0xxxdddd yy01mmmm */
                        x= (ope3 && 0x1C)/pow(2,2);
                        y= ope3 && 0x3;
                        fprintf(f_hex,"EA %xF %x%x %x%x\n",6+s,x ,ope1, (y<<2)+1 ,ope2);
                        break;                      
                }
                break;
            case ASR:
                switch(type){
                    case TROIS_REGISTRES:
                        /* asr rd,rn,rm */
                        /* 11111010 010snnnn 1111dddd 0000mmmm */
                        fprintf(f_hex,"FA %x%x F%x 0%x\n",4+s,ope2,ope1,ope3);
                        break;
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* asr rd,rm,#k */
                        /* 11101010 010s1111 0xxxdddd yy10mmmm */
                        x= (ope3 && 0x1C)/pow(2,2);
                        y= ope3 && 0x3;
                        fprintf(f_hex,"EA %xF %x%x %x%x\n",6+s,x ,ope1, (y<<2)+2 ,ope2);
                        break;                      
                }
                break;
            case ROR:
                switch(type){
                    case TROIS_REGISTRES:
                        /* ror rd,rn,rm */
                        /* 11111010 011snnnn 1111dddd 0000mmmm */
                        fprintf(f_hex,"FA %x%x F%x 0%x\n",6+s,ope2,ope1,ope3);
                        break;
                    case DEUX_REGISTRES_UNE_VALEUR:
                        /* ror rd,rm,#k */
                        /* 11101010 010s1111 0xxxdddd yy11mmmm */
                        x= (ope3 && 0x1C)/pow(2,2);
                        y= ope3 && 0x3;
                        fprintf(f_hex,"EA %xF %x%x %x%x\n",6+s,x ,ope1, (y<<2)+3 ,ope2);
                        break;                      
                }
                break;
        }
        pc++;
        }
    }
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
            instruction[i] = '\0';
        if(instruction[i]==']')
            instruction[i] = '\0';
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

int recuperation_type(char prefix2, char prefix3){
    if(prefix2=='r' && prefix3=='r')
        return TROIS_REGISTRES;
    if(prefix2=='r' && prefix3=='#')
        return DEUX_REGISTRES_UNE_VALEUR;
    if(prefix2=='r' && prefix3=='\0')
        return DEUX_REGISTRES;
    if(prefix2=='#' && prefix3=='\0')
        return UN_REGISTRE_UNE_VALEUR;
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
