	.set	noat
	.equ	RAM_START, 0x24C0
	.text
	.global _start
	.org 	0x0000
_start:
		stw r0, 0(r0)
		ldw r1, 0(r0)
		stw r1, RAM_START(r0)
		ldw r1, RAM_START(r0)
		br _start
		