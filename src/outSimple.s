.globl main

.data
m1:     .word 6
m2:     .word 5

.text
main:
    la     $t3, m1
    lw     $t2, 0($t3)
    la     $t3, m2
    lw     $t3, 0($t3)
    add    $t3, $t2, $t3
