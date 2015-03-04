.text

.global __start;
__start:

/* Init sp */
dla $sp, main

/* goto main */
j	main

