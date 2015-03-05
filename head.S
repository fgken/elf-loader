#include "console.S"

#define BOOT_HEAP_SIZE		0x400000
#define BOOT_STACK_SIZE		1024*16
.text

.global __start;
__start:

PUTC('>')
PUTC('\r')
PUTC('\n')
move	$a0, $sp
dump_reg
PUTC('D')

//restart_bootloader



/* Init heap/sp */
//dla $a0, (.heap)
dla 	$sp, (.stack + BOOT_STACK_SIZE)

/* goto main */
j	main

.comm .heap,BOOT_HEAP_SIZE,4
.comm .stack,BOOT_STACK_SIZE,4

