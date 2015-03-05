.text
li	$t1,'O' 
dla	$t0, 0x8001180000000800
sd	$t1, 0x40($t0)

.macro restart_bootloader
	lui		$t9, 0xbf00
	jr		$t9
.endm

restart_bootloader
