    mov r0,#7
    mov r1,#1
    mov r2,#1
boucle:
    mul r2,r2,r1
    cmp r1,r0
    beq fin
    add r1,r1,#1
    bal boucle
fin:
    mov r0,r2
    mov r15,#255
