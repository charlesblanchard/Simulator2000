#define EXIT_SUCCESS 0
#define ERR_SYNTHAXE 1
#define ERR_F_S 2
#define ERR_F_HEX 3

#define T_INSTRUCTION 50
#define T_MNEMONIQUE 4
#define T_OPERANDE 20

#define TAILLE_LIGNE 50
 
#define TROIS_REGISTRES 0
#define DEUX_REGISTRES_UNE_VALEUR 1
#define DEUX_REGISTRES 2
#define UN_REGISTRE_UNE_VALEUR 3
#define UN_REGISTRE 4

int ouverture_fichier(char *nom, FILE** f_s, FILE** f_hex);
int fermeture_fichier(FILE* f_s, FILE* f_hex);
int lecture_fichier(FILE* f_s, FILE* f_hex);
void nettoyage_instruction(char instruction[]);
unsigned long hash(char *src);
int recuperation_type(char *operande2, char *operande3);
int recuperation_s(char *mnemonique);

typedef struct label{
    char nom_label[20];
    int valeur_label;
} Label;


#define MOV 193499479
#define MVN 193499702
#define MOVW 6385482926
#define MOVT 6385482923


#define AND 193486360
#define BIC 193487283
#define ORR 193501752
#define ORN 193501748
#define EOR 193454827
#define ADD 193486030
#define ADC 193486029
#define SBC 193505565
#define SUB 193506191
#define RSB 193505036


#define MUL 193499667


#define TST 193507232
#define CMP 193488517


#define LSL 193498512
#define LSR 193498518
#define ASR 193486539
#define ROR 193504920


#define LDR 193498023
#define LDRH 6385434863
#define LDRB 6385434857
#define STR 193506174
#define STRH 6385703846
#define STRB 6385703840


#define PUSH 6385597157
#define POP 193502740


#define B 193487028
#define BEQ 193487165
#define BNE 193487450
#define BHS 193487266
#define BLO 193487394
#define BMI 193487421
#define BPL 193487523
#define BVS 193487728
#define BVC 193487712
#define BHI 193487256
#define BLS 193487398
#define BGE 193487219
#define BLT 193487399
#define BLT 193487399
#define BGT 193487234
#define BLE 193487384
#define BAL 193487028

#define BL 6385083648
#define BLEQ 6385083785
#define BLNE 6385084070                                                                       
#define BLHS 6385083886                                                                       
#define BLLO 6385084014                                                                       
#define BLMI 6385084041                                                                       
#define BLPL 6385084143                                                                       
#define BLVS 6385084348                                                                       
#define BLVC 6385084332                                                                       
#define BLHI 6385083876                                                                       
#define BLLS 6385084018
#define BLGE 6385083839
#define BLLT 6385084019
#define BLLT 6385084019
#define BLGT 6385083854
#define BLLE 6385084004
#define BLAL 6385083648

/* printf("#define PUSH %lu\n",hash("push")); */




















