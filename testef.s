main:
    mov r0,#6
    mov r1,#10
    lsl r5, r0, #2
    add r2,r0,r1
    cmp r2, #16
    beq fin
test:
    lsl r2, r1, #4
    bal main
fin:
    mov r15, r14
