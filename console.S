
#define PUTC(CHAR)			\
	li	$t1, (CHAR);			\
	dla $t0, 0x8001180000000800;\
	sd	$t1, 0x40($t0)

.macro putc_reg
	dla $v0, 0x8001180000000800
	sd	$a0, 0x40($v0)
.endm

/* Dump a0 */
.macro dump_reg
	move	$t0, $a0
	li		$t9, 16
loop:
	move	$t1, $zero
	andi	$t1, $t0, 0x000f
	addiu	$t1, $t1, '0'

	/* if $t1 < '9'*/
	slti	$t8, $t1, ':'
	bnez	$t8, putreg
	addiu	$t1, $t1, 7

putreg:
	move	$a0, $t1
	putc_reg

	dsrl	$t0, $t0, 4

	addi	$t9, $t9, -1
	bnez	$t9, loop

	PUTC('\r')
	PUTC('\n')
.endm

.macro restart_bootloader
	lui		$t9, 0xbf00
	jr		$t9
.endm
