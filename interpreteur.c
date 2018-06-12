#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "machine.h"
#include "arm.h"

#define T_MEM 0 



/* si la fonction retourn autre chose que 0, alors "Opération inconnue" et return = case mémoire du début de l'opération 32bit*/ 
int interpreter(Machine *M, char Programme[64][30], int8_t pas_a_pas){
    
    
    uint32_t instruction = 0x0;
    /*lorsqu'on aura mis en place un système d'erreurs*/
    int erreur = 0;
    /*[ATTENTION] les opérations de décalages sous la forme de "<<" ou "">>" s'appliquent uniquement à des nombres booleans. Pour faire un décalage d'un nombre entier il faut  effectuer les opperations suivantes :
     * << n correspond à * 2^(n)
     * >> n correspond à / 2^(n)
     Pour toute opération déjà notée en >> ou << il est possible de faire find replace par * pow(2,n) ou / pow(2,n), il faudra juste remplacer n par la valeur suivant << ou >> [ATTENTION]*/
        
    do{ 
        instruction = 0;
        
        instruction = instruction + (M->FLASH[M->REG[PC]]) ;
        instruction = instruction + (M->FLASH[M->REG[PC] + 1]) * pow(2,8);
        instruction = instruction + (M->FLASH[M->REG[PC] + 2]) * pow(2,16);
        instruction = instruction + (M->FLASH[M->REG[PC] + 3]) * pow(2,24);

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
    int pp = (hex7 & 0x3);
    int xxxyy = ((hex5*pow(2,2)) + (hex7 & 0xC));
    int val = (val1 * pow(2,4)) + val2_regv;
    int XYZ = hex8 * pow(2,8) + hex5 * pow(2,4) + hex6;
    int adress = instruction & ~(0xFFFFF000);
    int l = hex2 & ~(0xE);
   
   /*printf("pc : %x\n",M->REG[PC]);
   printf("hex3 : %x\n",hex3);*/
   
    
    switch (hex1){
        /*mov(reg),mvm(reg),ops(reg),décalages(val),tests(reg)*/
        case 0xE:
            /*test(reg)*/
           	if (regd == 0xf && rego_z != 0xF){
            
            /* décalages (val),mov(reg),mvn(reg)*/
                switch (code){
                    case(0x0):
                        sprintf(Programme[ M->REG[PC]/4 ],"tst r%d,r%d",rego_z,val2_regv);
                        tst(M, M->FLASH[rego_z], M->FLASH[val2_regv]);
                        break;
                    
                    case(0xD):
                        sprintf(Programme[ M->REG[PC]/4 ],"cmp r%d,r%d",rego_z,val2_regv);
                        cmp(M, M->FLASH[rego_z],  M->FLASH[val2_regv]);
                        break;
                }
            }
            else if (rego_z == 0xF){
                /*décalages (val)*/
                if(hex5 + val1 != 0x0) {
                    switch (pp) {
                        case(0x0):
                            if (psr)
                                sprintf(Programme[ M->REG[PC]/4 ],"lsls r%d, r%d, #0x%x",regd, val2_regv,xxxyy);
                            else
                                sprintf(Programme[ M->REG[PC]/4 ],"lsl r%d, r%d, #0x%x",regd, val2_regv,xxxyy);
                            
                            lsl(M, regd,M->FLASH[val2_regv],xxxyy, psr);
                            break;
                        
                        case(0x1):
                            if (psr)
                                sprintf(Programme[ M->REG[PC]/4 ],"lsrs r%d, r%d, #0x%x",regd, val2_regv,xxxyy);
                            else
                                sprintf(Programme[ M->REG[PC]/4 ],"lsr r%d, r%d, #0x%x",regd, val2_regv,xxxyy);
                            
                            lsr(M, regd,M->FLASH[val2_regv],xxxyy, psr);
                            break;
                        
                        case(0x2):
                            if (psr)
                                sprintf(Programme[ M->REG[PC]/4 ],"asrs r%d, r%d, #0x%x",regd, val2_regv,xxxyy);
                            else
                                sprintf(Programme[ M->REG[PC]/4 ],"asr r%d, r%d, #0x%x",regd, val2_regv,xxxyy);
                            
                            asr(M, regd,M->FLASH[val2_regv],xxxyy, psr);
                            break;
                    }
                } 
                /*mov(reg)*/
                else if (hex3 == 0x4 || hex3 == 0x5){
                    if (psr)
                        sprintf(Programme[ M->REG[PC]/4 ],"movs r%d, r%d",regd, val2_regv);
                    else
                        sprintf(Programme[ M->REG[PC]/4 ],"mov r%d, r%d",regd, val2_regv);
                    mov(M, regd, M->FLASH[val2_regv], psr);
                }
                
                /*mvn(reg)*/
                else if (hex3 == 0x6 || hex3 == 0x7){
                    if (psr)
                        sprintf(Programme[ M->REG[PC]/4 ],"mvns r%d, r%d",regd, val2_regv);
                    else
                        sprintf(Programme[ M->REG[PC]/4 ],"mvn r%d, r%d",regd, val2_regv);
                    mvn(M, regd, M->FLASH[val2_regv], psr);
                }
            }
            else if (hex3 + hex4 == 0 && hex5){
                    /*bal*/
                if (l){
                    sprintf(Programme[ M->REG[PC]/4 ],"blal 0x%08x",adress);
                    
                } else {
                    sprintf(Programme[ M->REG[PC]/4 ],"bal 0x%08x",adress);
                }
                branch(M, AL, l, adress);   
            }
            
            /*ops (reg)*/
            else {
                switch (code) {
                    case(0x0):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"ands r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"and r%d, r%d, r%d",regd, rego_z,val2_regv);
                        
                        and(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x1):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"bics r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"bic r%d, r%d, r%d",regd, rego_z,val2_regv);
                        
                        bic(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x2):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"orrs r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"orr r%d, r%d, r%d",regd, rego_z,val2_regv);
                    
                        orr(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x3):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"orns r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"orn r%d, r%d, r%d",regd, rego_z,val2_regv);
                        orn(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x4):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"eors r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"eor r%d, r%d, r%d",regd, rego_z,val2_regv);
                        
                        eor(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x8):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"adds r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"add r%d, r%d, r%d",regd, rego_z,val2_regv);
                        add(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0xA):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"adc r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"adc r%d, r%d, r%d",regd, rego_z,val2_regv);
                        
                    adc(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0xB):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"sbcs r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"sbc r%d, r%d, r%d",regd, rego_z,val2_regv);
                        sbc(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                
                    case(0xD):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"subs r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"sub r%d, r%d, r%d",regd, rego_z,val2_regv);
                        sub(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0xE):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"rsbs r%d, r%d, r%d",regd, rego_z,val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"rsbs r%d, r%d, r%d",regd, rego_z,val2_regv);
                        rsb(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                }
            }
    
            break;
        /*mov(val),movn(val),movt(val),movw(val),décalages(reg),mul(reg),ops(val)*/
        case 0xF:
            /*mov(val),movn(val)*/
            if (hex2 == 0x0 && hex4 == 0xF){
                if (hex3 == 0x5 || hex3 == 0x4){
                    /*mov(val)*/
                    if (psr)
                        sprintf(Programme[ M->REG[PC]/4 ],"movs r%d, #0x%08x",regd, val);
                    else
                        sprintf(Programme[ M->REG[PC]/4 ],"mov r%d, #0x%08x",regd, val);
                    
                    mov(M, regd, val, psr);
                } else if (hex3 == 0x7 || hex3 == 0x6){
                    /*mvn(val)*/
                    if (psr)
                        sprintf(Programme[ M->REG[PC]/4 ],"mvns r%d, #0x%08x",regd, val);
                    else
                        sprintf(Programme[ M->REG[PC]/4 ],"mvn r%d, #0x%08x",regd, val);
                    
                    mvn(M, regd, val, psr);
                } else {
                    sprintf(Programme[ M->REG[PC]/4 ],"erreur à l'instruction 0x%08x\n",M->REG[PC]);
                    erreur = 1;
                    M->REG[PC] = 0xFF;
                }
            }
            
            /*movt(val), movw(all)*/
            else if (hex2 == 0x2 || hex2 == 0x6) {
                if (hex3 == 0xC){
                    /*movt(val)*/
                    sprintf(Programme[ M->REG[PC]/4 ],"movt r%d, #0x%08x",regd, zzzzwyyyxxxxxxxx);
                    movt(M, regd, zzzzwyyyxxxxxxxx);
                } else if (hex3 == 0x4){
                    /*movw(val)*/
                    sprintf(Programme[ M->REG[PC]/4 ],"movw r%d, #0x%08x",regd, zzzzwyyyxxxxxxxx);
                    movw(M, regd, zzzzwyyyxxxxxxxx); /*val = zzzzwyyy xxxxxxxx*/
                } else {
                    sprintf(Programme[ M->REG[PC]/4 ],"erreur à l'instruction 0x%08x",M->REG[PC]);
                    erreur = 1;
                    M->REG[PC] = 0xFF;
                }
            }
            
            /*décalages(reg)*/
            else if (hex2 == 0xA) {
                switch(pp_reg){
                    case 0x0:
                        if (s)
                            sprintf(Programme[ M->REG[PC]/4 ],"lsls r%d, r%d, r%d",regd, rego_z, val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"lsl r%d, r%d, r%d",regd, rego_z, val2_regv);
                        
                        lsl(M, regd, M->FLASH[rego_z],M->FLASH[val2_regv],s);
                        break;
                    case 0x1:
                        if (s)
                            sprintf(Programme[ M->REG[PC]/4 ],"lsrs r%d, r%d, r%d",regd, rego_z, val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"lsr r%d, r%d, r%d",regd, rego_z, val2_regv);                        
                        
                        lsr(M, regd, M->FLASH[rego_z],M->FLASH[val2_regv],s);
                        break;
                    case 0x2:
                        if (s)
                            sprintf(Programme[ M->REG[PC]/4 ],"asrs r%d, r%d, r%d",regd, rego_z, val2_regv);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"asr r%d, r%d, r%d",regd, rego_z, val2_regv);
                        
                        sprintf(Programme[ M->REG[PC]/4 ],"asr r%d, r%d, r%d",regd, rego_z, val2_regv);
                        asr(M, regd, M->FLASH[rego_z],M->FLASH[val2_regv],s);
                        break;
                }
            }
            
            /*mul(reg)*/
            else if (hex2 == 0xB) {
                sprintf(Programme[ M->REG[PC]/4 ],"mul r%d, r%d, r%d",regd, rego_z, val2_regv);
                mul(M, M->FLASH[regd],M->FLASH[rego_z], M->FLASH[val2_regv]);
            }
            
            /*tests(val)*/
            else if (regd == 0xF){
                switch (code){
                    case(0x0):
                        sprintf(Programme[ M->REG[PC]/4 ],"tst r%d, #0x%08x", rego_z, val);
                        tst(M, M->FLASH[rego_z], val);
                        break;
                    case(0xD):
                        sprintf(Programme[ M->REG[PC]/4 ],"cmp r%d, #0x%08x", rego_z, val);
                        cmp(M, M->FLASH[rego_z], val);
                        break;
                }
            }
            
            /*ops(val)*/
            else if (hex4!= 0xF){
                switch (code) {
                    case(0x0):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"ands r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"and r%d, r%d, r%d",regd, rego_z, val);
                        
                        and(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x1):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"bics r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"bic r%d, r%d, r%d",regd, rego_z, val);
                        
                        bic(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x2):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"orrs r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"orr r%d, r%d, r%d",regd, rego_z, val);
                        
                        orr(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x3):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"orns r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"orn r%d, r%d, r%d",regd, rego_z, val);
                        
                        orn(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x4):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"eors r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"eor r%d, r%d, r%d",regd, rego_z, val);
                        
                        eor(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x8):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"adds r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"add r%d, r%d, r%d",regd, rego_z, val);
                        
                        add(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0xA):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"adcs r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"adc r%d, r%d, r%d",regd, rego_z, val);
                        
                        adc(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0xB):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"sbcs r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"sbc r%d, r%d, r%d",regd, rego_z, val);
                        
                        sbc(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                
                    case(0xD):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"subs r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"sub r%d, r%d, r%d",regd, rego_z, val);
                        
                        sub(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0xE):
                        if (psr)
                            sprintf(Programme[ M->REG[PC]/4 ],"rsbs r%d, r%d, r%d",regd, rego_z, val);
                        else
                            sprintf(Programme[ M->REG[PC]/4 ],"rsb r%d, r%d, r%d",regd, rego_z, val);
                        
                        rsb(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                }
            }
            
            break;
            
        case 0xD:
        /*ldr*/
            if (XYZ == 0)
                sprintf(Programme[ M->REG[PC]/4 ],"ldr r%d, [r%d]",hex7, hex2);
            else
                sprintf(Programme[ M->REG[PC]/4 ],"ldr r%d, [r%d], #0x%08x",hex7, hex2, XYZ);
        
            ldr (M, hex7, hex2, XYZ);
            break;
            
        case 0x9:
        /*ldrb*/
            if (XYZ == 0)
                sprintf(Programme[ M->REG[PC]/4 ],"ldrb r%d, [r%d]",hex7, hex2);
            else
                sprintf(Programme[ M->REG[PC]/4 ],"ldrb r%d, [r%d], #0x%08x",hex7, hex2, XYZ);
            
            ldrb (M, hex7, hex2, XYZ);
            break;
        
        case 0xB:
        /*ldrh*/
            if (XYZ == 0)
                sprintf(Programme[ M->REG[PC]/4 ],"ldrh r%d, [r%d]",hex7, hex2);
            else
                sprintf(Programme[ M->REG[PC]/4 ],"ldrh r%d, [r%d], #0x%08x",hex7, hex2, XYZ);
            
            ldrh (M, hex7, hex2, XYZ);
            break;
            
        case 0xC:
        /*str*/
            if (XYZ == 0)
                sprintf(Programme[ M->REG[PC]/4 ],"str r%d, [r%d]",hex7, hex2);
            else
                sprintf(Programme[ M->REG[PC]/4 ],"str r%d, [r%d], #0x%08x",hex7, hex2, XYZ);
        
            str (M, hex7, hex2, XYZ);
            break;
            
        case 0xA:
        /*strh*/
            if (XYZ == 0)
                sprintf(Programme[ M->REG[PC]/4 ],"strh r%d, [r%d]",hex7, hex2);
            else
                sprintf(Programme[ M->REG[PC]/4 ],"strh r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
            
            strh (M, hex7, hex2, XYZ);
            break;
            
        case 0x8:
        /*strb*/
            if (XYZ == 0)
                sprintf(Programme[ M->REG[PC]/4 ],"strb r%d, [r%d]",hex7, hex2);
            else
                sprintf(Programme[ M->REG[PC]/4 ],"strb r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
            strb (M, hex7, hex2, XYZ);
            break;
        
        case 0x5:
        /*pop*/
            if (hex3 == 0xF){
                sprintf(Programme[ M->REG[PC]/4 ],"pop{r%d}",regd);
                pop(M,regd);
            }
            break;
        
        case 0x4:
        /*push*/
            if (hex3 == 0xF){
                sprintf(Programme[ M->REG[PC]/4 ],"psuh{r%d}",regd);
                push(M,regd);
            }
            break;
            
        default:
            sprintf(Programme[ M->REG[PC]/4 ],"erreur à l'instruction 0x%08x",M->REG[PC]);
            erreur = 1;
            M->REG[PC] = 0xFF;
            break;
            
        }
        
        
        /****************************************/
        /*branchements*/
        if(hex3 + hex4 == 0){
            switch(hex1){
                case 0x0:
        /*beq*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"bleq 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"beq 0x%08x",adress);
            }
            branch(M, EQ, l, adress);        
            break;
        
        case 0x1:
        /*bne*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blne 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bne 0x%08x",adress);
            }
            branch(M, NE, l, adress);   
            
            break;
        
        case 0x2:
        /*bhs*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blhs 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bhs 0x%08x",adress);
            }
            branch(M, HS, l, adress);   
        
            break;
            
        case 0x3:
        /*blo*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"bllo 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"blo 0x%08x",adress);
            }
            branch(M, LO, l, adress);   
        
            break;
            
        case 0x4:
        /*bmi*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blmi 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bmi 0x%08x",adress);
            }
            branch(M, MI, l, adress);   
        
            break;
            
        case 0x5:
        /*bpl*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blpl 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bpl 0x%08x",adress);
            }
            branch(M, PL, l, adress);   
        
            break;
            
            
        case 0x6:
        /*bvs*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blvs 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bvs 0x%08x",adress);
            }
            branch(M, VS, l, adress);   
        
            break;
            
            
        case 0x7:
        /*bvc*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blvc 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bvc 0x%08x",adress);
            }
            branch(M, VC, l, adress);   
        
            break;
            
        case 0x8:
        /*bhi*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blhi 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bhi 0x%08x",adress);
            }
            branch(M, HI, l, adress);   
        
            break;
        
        case 0x9:
        /*bls*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blls 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bls 0x%08x",adress);
            }
            branch(M, LS, l, adress);   
        
            break;
            
        case 0xA:
        /*bge*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blge 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bge 0x%08x",adress);
            }
            branch(M, GE, l, adress);   
        
            break;
            
        case 0xB:
        /*blt*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"bllt 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"blt 0x%08x",adress);
            }
            branch(M, LT, l, adress);   
        
            break;
            
        case 0xC:
        /*bgt*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blgt 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"bgt 0x%08x",adress);
            }
            branch(M, GT, l, adress);   
        
            break;
            
        case 0xD:
        /*ble*/
            if (l){
                sprintf(Programme[ M->REG[PC]/4 ],"blle 0x%08x",adress);
                
            } else {
                sprintf(Programme[ M->REG[PC]/4 ],"ble 0x%08x",adress);
            }
            branch(M, LE, l, adress);   
        
            break;
            
            
        default:
            sprintf(Programme[ M->REG[PC]/4 ],"erreur à l'instruction 0x%08x",M->REG[PC]);
            erreur = 1;
            M->REG[PC] = 0xFF;
            break;
                
                
            }
        }
    
    if(!pas_a_pas){
        M->REG[PC] = M->REG[PC] + 4 ;   
    } else {
        affichage(*M,Programme);
        getchar();
        M->REG[PC] = M->REG[PC] + 4 ;   
    }
    
    } while(M->REG[PC] - 4  < 0xFF ); 
    /*while pc ne revoit pas vers la derniere case*/
        
    return 0;
    

}
