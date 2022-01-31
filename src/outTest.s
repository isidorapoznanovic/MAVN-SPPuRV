.globl main

.data
m1:     .word 6
m2:     .word 5
m3:     .word 0
m4:     .word 1

.text
main:
    la     $t2, m1
    lw     $t2, 0($t2)
    la     $t2, m2
    lw     $t2, 0($t2)
lab:
    la     $t2, m3
    addi   $t2, $t2, 1
    bltz   $t3, lab
