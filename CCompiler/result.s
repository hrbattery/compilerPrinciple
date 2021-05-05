.extern scanf
    .rdata
data_section_$$0: 
    .asciiz "%d"
    .text
    .globl __c_read
    .ent __c_read
__c_read:
    .set noreorder
    .frame $s8, 48, $ra
    .cpload $t9
    .set reorder 
    addi $sp, $sp, -48
    .cprestore 32
    sw $ra, 28($sp)
    sw $s8, 36($sp)
    move $s8, $sp
__c_read_$$branch_0:
    li $t0, 40
    add $t1, $t0, $s8
    la $t0, data_section_$$0

    sw $t0, 0($s8)
    sw $t1, 4($s8)
    lw $a0, 0($s8)
    lw $a1, 4($s8)
    jal scanf
     
    lw $t1, 40($s8)
    move $v0, $t1
__c_read_$$epilogue:
    move $sp, $s8
    lw $s8, 36($sp)
    lw $ra, 28($sp)
    addi $sp, $sp, 48
    jr $ra
    .end __c_read
.extern printf
    .rdata
data_section_$$1: 
    .asciiz "%d\n"
# gcc headers for __c_write
    .text
    .globl __c_write
    .ent __c_write
__c_write:
    .set noreorder
    .frame $s8, 40, $ra
    .cpload $t9
    .set reorder 
    addi $sp, $sp, -40
    .cprestore 32
    sw $ra, 28($sp)
    sw $s8, 36($sp)
    move $s8, $sp
__c_write_$$branch_0:
    la $t0, data_section_$$1
    sw $t0, 0($s8)
    sw $a0, 4($s8)
    lw $a0, 0($s8)
    lw $a1, 4($s8)
    jal printf
__c_write_$$epilogue:
    move $sp, $s8
    lw $s8, 36($sp)
    lw $ra, 28($sp)
    addi $sp, $sp, 40
    jr $ra
    .end __c_write
    .text
    .globl main
    .ent main
main:
    .set noreorder
    .frame $s8, 1024, $ra
    .cpload $t9
    .set reorder 
    addi $sp, $sp, -1024
    .cprestore 24
    sw $ra, 20($sp)
    sw $s8, 28($sp)
    move $s8, $sp
main_$$branch_0:
	addi $t0, $zero, 100
	sw $t0, 32($s8)
	sw $zero, 36($s8)
	sw $zero, 40($s8)
	addi $t0, $zero, 0
	sw $t0, 80($s8)
	addi $t0, $zero, 1
	sw $t0, 84($s8)
	addi $t0, $zero, 3
	sw $t0, 88($s8)
	addi $t0, $zero, 5
	sw $t0, 92($s8)
	lw $t0, 80($s8)
	lw $t1, 84($s8)
	div $t2, $t0, $t1
	sw $t2, 96($s8)
	lw $t0, 96($s8)
	lw $t1, 88($s8)
	div $t2, $t0, $t1
	sw $t2, 100($s8)
	lw $t0, 100($s8)
	lw $t1, 92($s8)
	mul $t2, $t1, $t0
	sw $t2, 104($s8)
	lw $t0, 104($s8)
	sw $t0, 32($s8)
	jal __c_read
	sw $v0, 36($s8)
	addi $t0, $zero, 233
	sw $t0, 96($s8)
	lw $t0, 32($s8)
	lw $t1, 96($s8)
	slt $t2, $t0, $t1
	sw $t2, 100($s8)
	addi $t0, $zero, 1
	sw $t0, 104($s8)
	lw $t0, 104($s8)
	addi $t1, $t0, 40
	sll $t1, $t1, 2
	add $t2, $s8, $t1
	lw $t0, 0($t2)
	sw $t0, 108($s8)
	addi $t0, $zero, 233
	sw $t0, 112($s8)
	lw $t0, 108($s8)
	lw $t1, 112($s8)
	sge $t2, $t0, $t1
	sw $t2, 116($s8)
	lw $t0, 100($s8)
	lw $t1, 116($s8)
	or $t2, $t0, $t1
	sw $t2, 120($s8)
	lw $t7, 120($s8)
	beqz $t7, Lelse0
	nop
	lw $a0, 32($s8)
	jal __c_write
	j Lexit0
Lelse0:
wDo0:
	addi $t0, $zero, 233
	sw $t0, 96($s8)
	lw $t0, 32($s8)
	lw $t1, 96($s8)
	sne $t2, $t0, $t1
	sw $t2, 100($s8)
	lw $t7, 100($s8)
	beqz $t7, wDo0
	nop
Lexit0:
main_$$epilogue:
    move $sp, $s8
    lw $s8, 28($sp)
    lw $ra, 20($sp)
    addi $sp, $sp, 1024
    jr $ra
    .end main

