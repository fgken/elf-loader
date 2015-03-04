OUTPUT_FORMAT("elf64-tradbigmips", "elf64-tradbigmips",
	      "elf64-tradlittlemips")
OUTPUT_ARCH(mips)
ENTRY(__start)
SECTIONS
{
  /* Read-only sections, merged into text segment: */
  PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x81000000));
  . = SEGMENT_START("text-segment", 0x81000000);
  .text           :
  {
    *(.text)
  }
  .rodata         : { *(.rodata) }

  /* Adjust the address for the data segment.
     We want to adjust up to the same address
	 within the page on the next page up.  */
  . = ALIGN (CONSTANT (MAXPAGESIZE)) - ((CONSTANT (MAXPAGESIZE) - .) & (CONSTANT (MAXPAGESIZE) - 1));
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  .data           :
  {
    *(.data)
  }
  
  HIDDEN (_gp = ALIGN (16)/* + 0x7ff0*/);
  .got            : { *(.got) }
  
  .bss            :
  {
   *(.bss)
   . = ALIGN(. != 0 ? 64 / 8 : 1);
  }

  . = DATA_SEGMENT_END (.);
}
