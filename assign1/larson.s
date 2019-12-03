.equ DELAY, 0x1F8000
// configure GPIO 20 for output
ldr w0, FSEL2

mov w2,#0
mov w3,#3

//Set the bits pattern to be put in FSEL2 address
init:
    mov w1,#1
    mul w4, w2, w3
    lsl w1, w1, w4
    orr w5, w1, w5
    add w2, w2, #1
    cmp w2, w3
    ble init

str w5,[x0] // put the bit pattern in FSEL2

counter_init:
mov w2,#20

loop://goes forward direction
    mov w1,#1
    lsl w1, w1, w2
    ldr w0, SET0
    str w1, [x0]
    add w2, w2, #1

    mov w3, #DELAY
    wait2:
        subs w3,w3, #1
        bne wait2

    ldr w0, CLR0
    str w1, [x0]
    cmp w2, #24

    blt loop

    inner_loop://goes backward direction
        lsr w1, w1, #1
        ldr w0, SET0
        str w1, [x0]
        mov w3, #DELAY
        wait3:
            subs w3,w3, #1
            bne wait3
        ldr w0, CLR0
        str w1, [x0]
        sub w2, w2, #1
        cmp w2, #21
        bgt inner_loop
    
    b loop

FSEL0: .word 0x3F200000
FSEL1: .word 0x3F200004
FSEL2: .word 0x3F200008
SET0:  .word 0x3F20001C
SET1:  .word 0x3F200020
CLR0:  .word 0x3F200028
CLR1:  .word 0x3F20002C
