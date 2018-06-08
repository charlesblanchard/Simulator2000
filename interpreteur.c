#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "simulateur.h"
#include "interpreteur.h"
#include "arm.h"


#define T_MEM 0
#define PC_DER_LIGNE 4031713024 + T_MEM 



/* si la fonction retourn autre chose que 0, alors "Opération inconnue" et return = case mémoire du début de l'opération 32bit*/ 
int interpreter(Machine *M){
    
    int32_t i=0;
    int32_t instruction = 0;
    /*lorsqu'on aura mis en place un système d'erreurs*/
    int erreur = 0;
    
    /*[ATTENTION] les opérations de décalages sous la forme de "<<" ou "">>" s'appliquent uniquement à des nombres booleans. Pour faire un décalage d'un nombre entier il faut  effectuer les opperations suivantes :
     * << n correspond à * 2^(n)
     * >> n correspond à / 2^(n)
     Pour toute opération déjà notée en >> ou << il est possible de faire find replace par * pow(2,n) ou / pow(2,n), il faudra juste remplacer n par la valeur suivant << ou >> [ATTENTION]*/
    
    do{
        instruction = (instruction || M->RAM[i+1]) * pow(2,24);
        instruction = (instruction || M->RAM[i]) * pow(2,16);
        instruction = (instruction || M->RAM[i+3]) * pow(2,8);
        instruction = (instruction || M->RAM[i+2]);
        
    int code = (instruction & ~(0xFE1FFFFF)) / pow(2,20); /*code d'une opération*/
    int psr = (instruction & ~(0xFFEFFFFF)) / pow(2,20); /*bit informant de l'actualisation duPSR*/
    int rego_z = (instruction & ~(0xFFF0FFFF)) / pow(2,16); /*bits informants du numéro du registre lu pour fournir une valeur stockée / bits constituants la valeur paramètre de l'opération movt et movw*/
    int xy = (instruction & ~(0xFFFF8FFF)) / pow(2,12); /*bits constituants la valeur paramètre de movt, movw et les opérations de décalages*/
    int w = (instruction & ~(0xFBFFFFFF)) / pow(2,24); /*bit constituants la valeur paramètre de l'opération movt et movw*/
    int regd = (instruction & ~(0xFFFFF0FF)) / pow(2,8); /*bits informants du registre de déstination*/
    int val1 = (instruction & ~(0xFFFFFF0F)) / pow(2,4); /*bit informants de la valeur stockée dans la mémoire*/
    int val2_regv = (instruction & ~(0xFFFFFF0)); /*bit informants de la valeur stockée dans la mémoire / registre stockant la valeur paramètre*/
    int hex1 = (instruction & ~(0x0FFFFFFF)) / pow(2,28);
    int hex2 = (instruction & ~(0xF0FFFFFF)) / pow(2,24);
    int hex3 = (instruction & ~(0xFF0FFFFF)) / pow(2,20);
    int hex4 = (instruction & ~(0xFFF0FFFF)) / pow(2,16);
    int hex5 = (instruction & ~(0xFFFF0FFF)) / pow(2,12);
    int hex6 = (instruction & ~(0xFFFFF0FF)) / pow(2,8);
    int hex7 = (instruction & ~(0xFFFFFF0F)) / pow(2,4);
    int hex8 = (instruction & ~(0xFFFFFFF0));
    
    printf("code : %x\tpsr : %x\trego_z : %x\txy : %x\tw : %x\tregd : %x\tval1 : %x\tval2_regv : %x",code,psr,rego_z,xy,w,regd,val1,val2_regv);
        
        if (instruction / pow(2,28) == 0xE){
            /*mov(reg),mvm(reg),ops(reg),décalages(val)*/
           /*****************************************/
            /*mov(reg)*/
            
            
            
            
            /****************************************/
            
        } else if (instruction / pow(2,28) == 0xF){
            /*mov(val),movn(val),movt(val),movw(val),décalages(reg),mul(reg),ops(val)*/
            /**********************************/
            /*mov(val),movn(val)*/
            if (hex2 == 0x0 && hex4 == 0x0){
                if (hex3 / pow(2,1) == 2){
                    /*mov(val)*/
                    printf("mov(val)\n");
                    mov(M, regd, (val1 * pow(2,4)) || val2_regv, psr);
                } else if (hex3 / pow(2,1) == 0x3){
                    /*mvn(val)*/
                    printf("mvn(val)\n");
                    mvn(M, regd, (val1 * pow(2,4)) || val2_regv, psr);
                } else {
                    erreur = 1;
                    instruction = PC_DER_LIGNE;
                }
            }
            /**********************************/
            /*movt(val), movw(all)*/
            if (hex2 == 0x2 || hex2 == 0x6) {
                if (hex3 == 0xC){
                    /*movt(val)*/
                    printf("movt(val)\n");
                    movt(M, regd, (((((rego_z * pow(2,12)) || (w * pow(2,11) )) || (xy * pow(2,8))) || (val1 * pow(2,4))) || (val2_regv * pow(2,16)))); /*val = zzzzwyyy xxxxxxxx*/
                } else if (hex3 == 0x4){
                    /*movw(val)*/
                    printf("movw(val)\n");
                    movw(M, regd, ((((rego_z * pow(2,12)) || (w * pow(2,11))) || (xy * pow(2,8))) || (val1 * pow(2,4))) || val2_regv); /*val = zzzzwyyy xxxxxxxx*/
                } else {
                    erreur = 1;
                    instruction = PC_DER_LIGNE;
                }
            }
            
        } else {
            erreur = 1;
            instruction = PC_DER_LIGNE;
        }
        
    }while(instruction != PC_DER_LIGNE); 
    /*while pc ne revoit pas vers la derniere case*/
    
    if (erreur == 1){
        return i;
    }
    
}
