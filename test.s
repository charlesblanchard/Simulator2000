    mov r0,r1
    mov r2,r3
    orn r3, r2, r1
test:
    lsl r2, r1, #4
    bal test
