		.text

		.long	0x0003ffff, __startup,  0x00000000, 0x00000000

		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 10 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 20 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 30 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 40 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 50 */

		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 60 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 70 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 80 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* 90 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* A0 */
		.long	0x00000000, 0x00000000, 0x00000000, 0x00000000	/* B0 */

		.globl	__startup
__startup:
		jsr		entry
halt:
		bra		halt
