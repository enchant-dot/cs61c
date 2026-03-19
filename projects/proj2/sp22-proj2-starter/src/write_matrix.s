write_matrix:

    addi sp, sp, -24
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)


    mv s1, a1       
    mv s2, a2       
    mv s3, a3       
    mul s4, s2, s3


    li a1, 1
    jal ra, fopen


    blt a0, zero, error_fopen
    mv s0, a0        


    addi sp, sp, -8      
    sw s2, 0(sp)         
    sw s3, 4(sp)       

    mv a0, s0            
    mv a1, sp           
    li a2, 2             
    li a3, 4            
    jal ra, fwrite

  
    li t0, 2
    bne a0, t0, error_fwrite


    addi sp, sp, 8
    

    mv a0, s0           
    mv a1, s1          
    mv a2, s4            
    li a3, 4            
    jal ra, fwrite


    bne a0, s4, error_fwrite


    mv a0, s0           
    jal ra, fclose


    bnez a0, error_fclose


    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    addi sp, sp, 24

    ret


error_fopen:
    li a0, 27
    j exit
error_fwrite:
    li a0, 30
    j exit
error_fclose:
    li a0, 28
    j exit