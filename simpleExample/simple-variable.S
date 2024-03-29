# Module : main
	.extern scanf
	.extern printf
# data sections of function __micro_read
	.rdata
data_section_$$0: 
	.asciiz "%d"
# gcc headers for __micro_read
	.text
	.globl __micro_read
	.ent __micro_read
__micro_read:
	# prologue area
	.set noreorder
	.frame $s8, 48, $ra
	.cpload $t9
	.set reorder 
	addi $sp, $sp, -48
	.cprestore 32
	sw $ra, 28($sp)
	sw $s8, 36($sp)
	move $s8, $sp
__micro_read_$$branch_0:
	li $t0, 40
	add $t1, $t0, $s8
	la $t0, data_section_$$0

	# start calling scanf
	sw $t0, 0($s8)
	sw $t1, 4($s8)
	lw $a0, 0($s8)
	lw $a1, 4($s8)
	jal scanf
	 
	# end calling scanf
	lw $t1, 40($s8)
	move $v0, $t1
__micro_read_$$epilogue:
	# epilogue area
	move $sp, $s8
	lw $s8, 36($sp)
	lw $ra, 28($sp)
	addi $sp, $sp, 48
	jr $ra
	.end __micro_read
# data sections of function __micro_write
	.rdata
data_section_$$1: 
	.asciiz "%d\n"
# gcc headers for __micro_write
	.text
	.globl __micro_write
	.ent __micro_write
__micro_write:
	# prologue area
	.set noreorder
	.frame $s8, 40, $ra
	.cpload $t9
	.set reorder 
	addi $sp, $sp, -40
	.cprestore 32
	sw $ra, 28($sp)
	sw $s8, 36($sp)
	move $s8, $sp
__micro_write_$$branch_0:
	la $t0, data_section_$$1
	# start calling printf
	sw $t0, 0($s8)
	sw $a0, 4($s8)
	lw $a0, 0($s8)
	lw $a1, 4($s8)
	jal printf
	# end calling printf
__micro_write_$$epilogue:
	# epilogue area
	move $sp, $s8
	lw $s8, 36($sp)
	lw $ra, 28($sp)
	addi $sp, $sp, 40
	jr $ra
	.end __micro_write
# data sections of function main
# gcc headers for main
	.text
	.globl main
	.ent main
main:
	# prologue area
	.set noreorder
	.frame $s8, 40, $ra
	.cpload $t9
	.set reorder 
	addi $sp, $sp, -40
	.cprestore 24
	sw $ra, 20($sp)
	sw $s8, 28($sp)
	move $s8, $sp
main_$$branch_0:
	# start calling __micro_read
	jal __micro_read
	move $t2, $v0
	# end calling __micro_read

	li $t0, 1
	add $t1, $t2, $t0
	li $t2, 1
	add $t0, $t1, $t2

	# start calling __micro_write
	sw $t1, 32($s8)
	sw $t0, 36($s8)
	sw $t1, 0($s8)
	lw $a0, 0($s8)
	jal __micro_write

	lw $t1, 32($s8)
	lw $t0, 36($s8)
	# end calling __micro_write

	# start calling __micro_write
	sw $t1, 32($s8)
	sw $t0, 36($s8)
	sw $t0, 0($s8)
	lw $a0, 0($s8)
	jal __micro_write
	lw $t1, 32($s8)
	lw $t0, 36($s8)

	# end calling __micro_write

	add $t2, $t1, $t0

	# start calling __micro_write
	sw $t2, 0($s8)
	lw $a0, 0($s8)
	jal __micro_write
	# end calling __micro_write

	addi $v0, $zero, 0

main_$$epilogue:
	# epilogue area
	move $sp, $s8
	lw $s8, 28($sp)
	lw $ra, 20($sp)
	addi $sp, $sp, 40
	jr $ra
	.end main
