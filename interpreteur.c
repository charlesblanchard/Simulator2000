#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "simulateur.h"
#include "arm.h"
#include "interpreteur.h"



#define T_MEM 0
#define PC_DER_LIGNE 4031713024 + T_MEM 



/* si la fonction retourn autre chose que 0, alors "Opération inconnue" et return = case mémoire du début de l'opération 32bit*/ 
int interpreter(Machine *M){
    
    int32_t i=0;
    uint32_t instruction = 0x00000000;
    /*lorsqu'on aura mis en place un système d'erreurs*/
    int erreur = 0;
    
    /*[ATTENTION] les opérations de décalages sous la forme de "<<" ou "">>" s'appliquent uniquement à des nombres booleans. Pour faire un décalage d'un nombre entier il faut  effectuer les opperations suivantes :
     * << n correspond à * 2^(n)
     * >> n correspond à / 2^(n)
     Pour toute opération déjà notée en >> ou << il est possible de faire find replace par * pow(2,n) ou / pow(2,n), il faudra juste remplacer n par la valeur suivant << ou >> [ATTENTION]*/
    
    printf("\n");
    
    M->FLASH[1] = 0xF6;
    M->FLASH[0] = 0x44;
    M->FLASH[3] = 0x35;
    M->FLASH[2] = 0xF5;
    
    do{    
        instruction = 0;
        
        instruction = instruction + M->FLASH[i+1] * pow(2,24);
        instruction = instruction + M->FLASH[i] * pow(2,16);
        instruction = instruction + M->FLASH[i+3] * pow(2,8);
        instruction = instruction + M->FLASH[i+2];
        
    printf("instruction: %x\n",instruction);
    

    int code = (instruction & ~(0xFE1FFFFF)) / pow(2,20); /*code d'une opération*/
    int psr = (instruction & ~(0xFFEFFFFF)) / pow(2,20); /*bit informant de l'actualisation duPSR*/
    int rego_z = (instruction & ~(0xFFF0FFFF)) / pow(2,16); /*bits informants du numéro du registre lu pour fournir une valeur stockée / bits constituants la valeur paramètre de l'opération movt et movw*/
    int xy = (instruction & ~(0xFFFF8FFF)) / pow(2,12); /*bits constituants la valeur paramètre de movt, movw et les opérations de décalages*/
    int w = (instruction & ~(0xFBFFFFFF)) / pow(2,24); /*bit constituants la valeur paramètre de l'opération movt et movw*/
    int regd = (instruction & ~(0xFFFFF0FF)) / pow(2,8); /*bits informants du registre de déstination*/
    int val1 = (instruction & ~(0xFFFFFF0F)) / pow(2,4); /*bit informants de la valeur stockée dans la mémoire*/
    int val2_regv = (instruction & ~(0xFFFFFFF0)); /*bit informants de la valeur stockée dans la mémoire / registre stockant la valeur paramètre*/
    int hex1 = (instruction & ~(0x0FFFFFFF)) / pow(2,28);
    int hex2 = (instruction & ~(0xF0FFFFFF)) / pow(2,24);
    int hex3 = (instruction & ~(0xFF0FFFFF)) / pow(2,20);
    int hex4 = (instruction & ~(0xFFF0FFFF)) / pow(2,16);
    int hex5 = (instruction & ~(0xFFFF0FFF)) / pow(2,12);
    int hex6 = (instruction & ~(0xFFFFF0FF)) / pow(2,8);
    int hex7 = (instruction & ~(0xFFFFFF0F)) / pow(2,4);
    int hex8 = (instruction & ~(0xFFFFFFF0));
    int zzzzwyyyxxxxxxxx = rego_z * pow(2,12) + w * pow(2,11) + xy * pow(2,8) + val1 * pow(2,4) + val2_regv;
    int pp_reg = (instruction & ~(0xFF6FFFFF)) / pow(2,21);
    int s = (instruction & ~(0xFFEFFFFF)) / pow(2,20);
    
    
        if (hex1 == 0xE){
            /*mov(reg),mvm(reg),ops(reg),décalages(val)*/
           /*****************************************/
            /*mov(reg)*/
            
            
            
            
            /****************************************/
            
        } else if (hex1 == 0xF){
            /*mov(val),movn(val),movt(val),movw(val),décalages(reg),mul(reg),ops(val)*/
            /**********************************/
            /*mov(val),movn(val)*/
            if (hex2 == 0x0 && hex4 == 0xF){
                if (hex3 == 0x5 || hex3 == 0x4){
                    /*mov(val)*/
                    printf("mov(val)\n");
                    mov(M, regd, (val1 * pow(2,4)) + val2_regv, psr);
                } else if (hex3 == 0x7 || hex3 == 0x6){
                    /*mvn(val)*/
                    printf("mvn(val)\n");
                    mvn(M, regd, (val1 * pow(2,4)) + val2_regv, psr);
                } else {
                    erreur = 1;
                    instruction = PC_DER_LIGNE;
                }
            }
            /**********************************/
            /*movt(val), movw(all)*/
            else if (hex2 == 0x2 || hex2 == 0x6) {
                if (hex3 == 0xC){
                    /*movt(val)*/
                    printf("movt(val)\n");
                    movt(M, regd, zzzzwyyyxxxxxxxx);
                } else if (hex3 == 0x4){
                    /*movw(val)*/
                    printf("movw(val)\n");
                    movw(M, regd, zzzzwyyyxxxxxxxx); /*val = zzzzwyyy xxxxxxxx*/
                } else {
                    erreur = 1;
                    instruction = PC_DER_LIGNE;
                }
            }
            /**********************************/
            /*décalages(reg)*/
            else if (hex2 == 0xA) {
                switch(pp_reg){
                    case 0x00:
                        lsl(M, regd, M->REG[rego_z],M->REG[val2_regv],s);
                        break;
                    case 0x01:
                        lsr(M, regd, M->REG[rego_z],M->REG[val2_regv],s);
                        break;
                    case 0x10:
                        asr(M, regd, M->REG[rego_z],M->REG[val2_regv],s);
                        break;
                }
            }
            /**********************************/
            /*mul(reg)*/
            else if (hex2 == 0xB) {
                mul(M, M->REG[regd],M->REG[rego_z], val2_regv);
            }
            /**********************************/
            /*
            
            /**********************************/
            /*opération innexistante*/
            else {
                erreur = 1;
            }
            
            
        } else {
            erreur = 1;
            instruction = PC_DER_LIGNE;
        }
        
        
    }while(i != 0); 
    /*while pc ne revoit pas vers la derniere case*/
    

    if (erreur == 1){
        return i;
    }
    

}
