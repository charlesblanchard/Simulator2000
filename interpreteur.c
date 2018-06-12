#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "simulateur.h"
#include "arm.h"

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
        
        instruction = instruction + (M->FLASH[M->REG[PC]] + 1) * pow(2,24);
        instruction = instruction + (M->FLASH[M->REG[PC]]) * pow(2,16);
        instruction = instruction + (M->FLASH[M->REG[PC]] + 3) * pow(2,8);
        instruction = instruction + (M->FLASH[M->REG[PC]] + 2);

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
    int adress = instruction & ~(0xFF000000);
    int l = hex2 & ~(0xE);
    
    
    printf("\nCode interpreté\n");
    switch (hex1){
        /*mov(reg),mvm(reg),ops(reg),décalages(val),tests(reg)*/
        case 0xE:
            /*test(reg)*/
           	if (regd == 0xf && rego_z != 0xF){
            
            /* décalages (val),mov(reg),mvn(reg)*/
                switch (code){
                    case(0x0):
                        printf("tst r%d,r%d\n",rego_z,val2_regv);
                        tst(M, M->FLASH[rego_z], M->FLASH[val2_regv]);
                        break;
                    
                    case(0xD):
                        printf("cmp r%d,r%d\n",rego_z,val2_regv);
                        cmp(M, M->FLASH[rego_z],  M->FLASH[val2_regv]);
                        break;
                }
            }
            else if (rego_z == 0xF){
                /*décalages (val)*/
                if(hex5 + val1 != 0xF) {
                    switch (pp) {
                        case(0x0):
                            if (psr)
                                printf("lsls r%d, r%d, #0x%08x\n",regd, val2_regv,xxxyy);
                            else
                                printf("lsl r%d, r%d, #0x%08x\n",regd, val2_regv,xxxyy);
                            
                            lsl(M, regd,M->FLASH[val2_regv],xxxyy, psr);
                            break;
                        
                        case(0x1):
                            if (psr)
                                printf("lsrs r%d, r%d, #0x%08x\n",regd, val2_regv,xxxyy);
                            else
                                printf("lsr r%d, r%d, #0x%08x\n",regd, val2_regv,xxxyy);
                            
                            lsr(M, regd,M->FLASH[val2_regv],xxxyy, psr);
                            break;
                        
                        case(0x2):
                            if (psr)
                                printf("asrs r%d, r%d, #0x%08x\n",regd, val2_regv,xxxyy);
                            else
                                printf("asr r%d, r%d, #0x%08x\n",regd, val2_regv,xxxyy);
                            
                            asr(M, regd,M->FLASH[val2_regv],xxxyy, psr);
                            break;
                    }
                } 
                /*mov(reg)*/
                else if (hex3 == 0x4 || hex3 == 0x5){
                    if (psr)
                        printf("movs r%d, r%d\n",regd, val2_regv);
                    else
                        printf("mov r%d, r%d\n",regd, val2_regv);
                    mov(M, regd, M->FLASH[val2_regv], psr);
                }
                /*mvn(reg)*/
                else if (hex3 == 0x6 || hex3 == 0x7){
                    if (psr)
                        printf("mvns r%d, r%d\n",regd, val2_regv);
                    else
                        printf("mvn r%d, r%d\n",regd, val2_regv);
                    mvn(M, regd, M->FLASH[val2_regv], psr);
                }
            }
            /*ops (reg)*/
            else {
                switch (code) {
                    case(0x0):
                        if (psr)
                            printf("ands r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("and r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        
                        and(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x1):
                        if (psr)
                            printf("bics r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("bic r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        
                        bic(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x2):
                        if (psr)
                            printf("orrs r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("orr r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        
                        orr(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x3):
                        if (psr)
                            printf("orns r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("orn r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        orn(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x4):
                        if (psr)
                            printf("eors r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("eor r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        
                        eor(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0x8):
                        if (psr)
                            printf("adds r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("add r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        add(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0xA):
                        if (psr)
                            printf("adc r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("adc r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        
                    adc(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0xB):
                        if (psr)
                            printf("sbcs r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("sbc r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        sbc(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                
                    case(0xD):
                        if (psr)
                            printf("subs r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("sub r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        sub(M, M->FLASH[regd], M->FLASH[rego_z], M->FLASH[val2_regv], psr);
                        break;
                    
                    case(0xE):
                        if (psr)
                            printf("rsbs r%d, r%d, r%d\n",regd, rego_z,val2_regv);
                        else
                            printf("rsbs r%d, r%d, r%d\n",regd, rego_z,val2_regv);
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
                        printf("movs r%d, #0x%08x\n",regd, val);
                    else
                        printf("mov r%d, #0x%08x\n",regd, val);
                    
                    mov(M, regd, val, psr);
                } else if (hex3 == 0x7 || hex3 == 0x6){
                    /*mvn(val)*/
                    if (psr)
                        printf("mvns r%d, #0x%08x\n",regd, val);
                    else
                        printf("mvn r%d, #0x%08x\n",regd, val);
                    
                    mvn(M, regd, val, psr);
                } else {
                    erreur = 1;
                    instruction = PC_DER_LIGNE;
                }
            }
            
            /*movt(val), movw(all)*/
            else if (hex2 == 0x2 || hex2 == 0x6) {
                if (hex3 == 0xC){
                    /*movt(val)*/
                    printf("movt r%d, #0x%08x\n",regd, zzzzwyyyxxxxxxxx);
                    movt(M, regd, zzzzwyyyxxxxxxxx);
                } else if (hex3 == 0x4){
                    /*movw(val)*/
                    printf("movw r%d, #0x%08x\n",regd, zzzzwyyyxxxxxxxx);
                    movw(M, regd, zzzzwyyyxxxxxxxx); /*val = zzzzwyyy xxxxxxxx*/
                } else {
                    printf("erreur à l'instruction 0x%08x\n",M->REG[PC]);
                    erreur = 1;
                    M->REG[PC] = M->REG[LR];
                }
            }
            
            /*décalages(reg)*/
            else if (hex2 == 0xA) {
                switch(pp_reg){
                    case 0x0:
                        if (s)
                            printf("lsls r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                        else
                            printf("lsl r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                        
                        lsl(M, regd, M->FLASH[rego_z],M->FLASH[val2_regv],s);
                        break;
                    case 0x1:
                        if (s)
                            printf("lsrs r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                        else
                            printf("lsr r%d, r%d, r%d\n",regd, rego_z, val2_regv);                        
                        
                        lsr(M, regd, M->FLASH[rego_z],M->FLASH[val2_regv],s);
                        break;
                    case 0x2:
                        if (s)
                            printf("asrs r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                        else
                            printf("asr r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                        
                        printf("asr r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                        asr(M, regd, M->FLASH[rego_z],M->FLASH[val2_regv],s);
                        break;
                }
            }
            
            /*mul(reg)*/
            else if (hex2 == 0xB) {
                printf("mul r%d, r%d, r%d\n",regd, rego_z, val2_regv);
                mul(M, M->FLASH[regd],M->FLASH[rego_z], M->FLASH[val2_regv]);
            }
            
            /*tests(val)*/
            else if (regd == 0xF){
                switch (code){
                    case(0x0):
                        printf("tst r%d, #0x%08x\n", rego_z, val);
                        tst(M, M->FLASH[rego_z], val);
                        break;
                    case(0xD):
                        printf("cmp r%d, #0x%08x\n", rego_z, val);
                        cmp(M, M->FLASH[rego_z], val);
                        break;
                }
            }
            
            /*ops(val)*/
            else if (hex4!= 0xF){
                switch (code) {
                    case(0x0):
                        if (psr)
                            printf("ands r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("and r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        and(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x1):
                        if (psr)
                            printf("bics r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("bic r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        bic(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x2):
                        if (psr)
                            printf("orrs r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("orr r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        orr(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x3):
                        if (psr)
                            printf("orns r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("orn r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        orn(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x4):
                        if (psr)
                            printf("eors r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("eor r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        eor(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0x8):
                        if (psr)
                            printf("adds r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("add r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        add(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0xA):
                        if (psr)
                            printf("adcs r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("adc r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        adc(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0xB):
                        if (psr)
                            printf("sbcs r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("sbc r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        sbc(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                
                    case(0xD):
                        if (psr)
                            printf("subs r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("sub r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        sub(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                    
                    case(0xE):
                        if (psr)
                            printf("rsbs r%d, r%d, r%d\n",regd, rego_z, val);
                        else
                            printf("rsb r%d, r%d, r%d\n",regd, rego_z, val);
                        
                        rsb(M, M->FLASH[regd], M->FLASH[rego_z], val, psr);
                        break;
                }
            }
            
            break;
            
        case 0xD:
        /*ldr*/
            if (XYZ == 0)
                printf("ldr r%d, [r%d]\n",hex7, hex2);
            else
                printf("ldr r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
        
            ldr (M, hex7, hex2, XYZ);
            break;
            
        case 0x9:
        /*ldrb*/
            if (XYZ == 0)
                printf("ldrb r%d, [r%d]\n",hex7, hex2);
            else
                printf("ldrb r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
            
            ldrb (M, hex7, hex2, XYZ);
            break;
        
        case 0xB:
        /*ldrh*/
            if (XYZ == 0)
                printf("ldrh r%d, [r%d]\n",hex7, hex2);
            else
                printf("ldrh r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
            
            ldrh (M, hex7, hex2, XYZ);
            break;
            
        case 0xC:
        /*str*/
            if (XYZ == 0)
                printf("str r%d, [r%d]\n",hex7, hex2);
            else
                printf("str r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
        
            str (M, hex7, hex2, XYZ);
            break;
            
        case 0xA:
        /*strh*/
            if (XYZ == 0)
                printf("strh r%d, [r%d]\n",hex7, hex2);
            else
                printf("strh r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
            
            strh (M, hex7, hex2, XYZ);
            break;
            
        case 0x8:
        /*strb*/
            if (XYZ == 0)
                printf("strb r%d, [r%d]\n",hex7, hex2);
            else
                printf("strb r%d, [r%d], #0x%08x\n",hex7, hex2, XYZ);
            strb (M, hex7, hex2, XYZ);
            break;
        
        case 0x5:
        /*pop*/
            printf("pop{r%d}\n",regd);
            pop(M,regd);
            break;
        
        case 0x4:
        /*push*/
            printf("psuh{r%d}\n",regd);
            push(M,regd);
            break;
            
        case 0x0:
        /*beq*/
            if (l){
                printf("bleq 0x%08x\n",adress);
                
            } else {
                printf("beq 0x%08x\n",adress);
            }
            branch(M, EQ, l, adress);        
            break;
        
        case 0x1:
        /*bne*/
            if (l){
                printf("blne 0x%08x\n",adress);
                
            } else {
                printf("bne 0x%08x\n",adress);
            }
            branch(M, NE, l, adress);   
            
            break;
        
        case 0x2:
        /*bhs*/
            if (l){
                printf("blhs 0x%08x\n",adress);
                
            } else {
                printf("bhs 0x%08x\n",adress);
            }
            branch(M, HS, l, adress);   
        
            break;
            
        case 0x3:
        /*blo*/
            if (l){
                printf("bllo 0x%08x\n",adress);
                
            } else {
                printf("blo 0x%08x\n",adress);
            }
            branch(M, LO, l, adress);   
        
            break;
            
            
        case 0x6:
        /*bvs*/
            if (l){
                printf("blvs 0x%08x\n",adress);
                
            } else {
                printf("bvs 0x%08x\n",adress);
            }
            branch(M, VS, l, adress);   
        
            break;
            
            
        case 0x7:
        /*bvc*/
            if (l){
                printf("blvc 0x%08x\n",adress);
                
            } else {
                printf("bvc 0x%08x\n",adress);
            }
            branch(M, VC, l, adress);   
        
            break;
            
            
            
        default:
            printf("erreur à l'instruction 0x%08x\n",M->REG[PC]);
            erreur = 1;
            M->REG[PC] = M->REG[LR];
            break;
        
    }
    
    
    M->REG[PC] = M->REG[PC] + 0x4;
    
    }while(M->REG[PC] != M->REG[LR]); 
    /*while pc ne revoit pas vers la derniere case*/
    

    if (erreur == 1){
        return i;
    }
    
    return 0;
    

}
