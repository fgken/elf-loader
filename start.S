#define MIPS_COP_0_CAUSE	_(13)

.text
.blobal _start
.type _start, @function
_start:

// Unsafe to use a0..a3, need to preserve the args form boot loader
  mtc0 zero, MIPS_COP_0_CAUSE
