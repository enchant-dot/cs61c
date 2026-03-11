.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
# ==============================================================================
read_matrix:

    addi sp, sp, -28
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)

  
    mv s0, a1        
    mv s1, a2        


    li a1, 0        
    jal ra, fopen
    

    blt a0, zero, error_fopen
    mv s2, a0       

 
    mv a0, s2
    mv a1, s0
    addi a2, zero, 4 
    jal ra, fread
    

    addi t0, zero, 4
    bne a0, t0, error_fread


    mv a0, s2
    mv a1, s1
    addi a2, zero, 4 
    jal ra, fread
    addi t0, zero, 4
    bne a0, t0, error_fread


    lw t1, 0(s0)
    lw t2, 0(s1)
    mul t3, t1, t2    
    slli s4, t3, 2    
    
    mv a0, s4
    jal ra, malloc
    beq a0, zero, error_malloc
    mv s3, a0         


    mv a0, s2
    mv a1, s3
    mv a2, s4
    jal ra, fread
    bne a0, s4, error_fread


    mv a0, s2
    jal ra, fclose
    bnez a0, error_fclose


    mv a0, s3


    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    addi sp, sp, 28

    ret


error_malloc:
    li a0, 26
    j exit
error_fopen:
    li a0, 27
    j exit
error_fclose:
    li a0, 28
    j exit
error_fread:
    li a0, 29
    j exit