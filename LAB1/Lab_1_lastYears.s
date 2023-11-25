# Queen University
# Also really good reference sheet for NIOS II for those who want to do computer architecture
# http://www-ug.eecg.toronto.edu/msl/manuals/n2cpu_nii5v1.pdf enjoy
# Also really good reference sheet for ascii character to hex
# https://www.eso.org/~ndelmott/ascii.html enjoy as well

.global _start
.equ	JTAG_UART_BASE, 0x10001000 # address of first JTAG UART register
.equ	DATA_OFFSET, 0 # offset of JTAG UART data register
.equ	STATUS_OFFSET, 4 # offset of JTAG UART status register
.equ	WSPACE_MASK, 0xFFFF # used in AND operation to check status
.equ 	SWITCHES, 0x10000040 # switch offset

_start:
	movi sp, 0x7FFC # setup our stack pointer to be somewhere far away, 0x7FFC is the desired offset by the ways.
	
	call SwitchSetting
	addi r2, r2, '0' # we add this to offset the result to be a character
	# aka, '0' + 0x0 = '0', '0' + 0x1 = '1', this will show in JTAG UART console
	call PrintChar
	mov r3, r2 # we want to use r3 as out switch check
	# aka if we switch the... "switch", we can use r3 to verify if it did switch, to not spam the console
	
_loop:
	call SwitchSetting
	beq r2, r3, _loop # if our r3 value is the same as what we read, then we didn't touch the switch, return to loop instead.
	mov r4, r2 # store r2 for now in r4, we will need this later

	movia r2, SLASH_B # move the cursor back by 1x space, this will overwrite what's next
	ldw r2, 0(r2) 
	call PrintChar

	mov r2, r4 # restore r2 to what we read from the switch
	addi r2, r2, '0' # we add this to offset the result to be a character
	# aka, '0' + 0x0 = '0', '0' + 0x1 = '1', this will show in JTAG UART console
	call PrintChar

	mov r3, r4 # set our switch tracker to the new switch state
	br _loop

_end:
	br _end

#-----------------------------
/* Pseudo code of SwitchSetting
SwitchSetting::
	val = read switch port data register
	val = val AND 0x1
	return val
*/

SwitchSetting:
	movia r2, SWITCHES # move in the pointer address to the switch
	ldwio r2, 0(r2) # read from I/O port the switch's status
	andi r2, r2, 0x1 # mask off all other switches (since it's an array of 10x switch) and only account for the first switch
	ret

#-----------------------------
/* Pseudo code of PrintChar
PrintChar(ch)::
	do
		st = read JTAG UART status register
		st = st AND 0xFFFF0000
	while (st is zero)
		write ch to JTAG UART data register
*/

PrintChar:
	subi sp, sp, 8 # adjust stack pointer down to reserve space
	stw r3, 4(sp) # save register is r3 so it came be a temp
	stw r4, 0(sp) # save register is r4 so it came be a temp
	movia r3, JTAG_UART_BASE # point to first memory mapped I/O register

pc_loop:
	ldwio r4, STATUS_OFFSET(r3) # read bits from status register
	andhi r4, r4, WSPACE_MASK # mask off lower bits to isolate upper bits
	beq r4, r0, pc_loop # if there is a status present, it's likely not ready to recieve a command, so loop back till ready
	stwio r2, DATA_OFFSET(r3) # otherwise, write character to data register
	ldw r3, 4(sp) # restore value of r3 from stack
	ldw r4, 0(sp) # restore value of r4 from stack
	addi sp, sp, 8 # readjust stack pointer up to deallocate space
	ret # return to calling routine
	
.org 0x2000
SLASH_B: .asciz "\b"