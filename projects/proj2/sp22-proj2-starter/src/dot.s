.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
	li t0, 1
	blt a2, t0, error_length
	blt a3, t0, error_stride
	blt a4, t0, error_stride

	slli t1, a3, 2
	slli t2, a4, 2

	li t0, 0
loop_start:
	beq a2, zero, loop_end
	lw t3, 0(a0)
	lw t4, 0(a1)
	mul t5, t3, t4
	add t0, t0, t5

	add a0, a0, t1
	add a1, a1, t2
	addi a2, a2, -1
	j loop_start
loop_end:
	mv a0, t0
	ret

	# Epilogue
error_length:
	li a0, 36
	j exit
error_stride:
	li a0, 37
	j exit
