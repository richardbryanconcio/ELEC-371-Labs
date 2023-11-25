.text
.global _start
.org 0x0000
.equ JTAG_UART_BASE, 0x10001000  # address of first JTAG UART register
.equ DATA_OFFSET, 0              # offset of JTAG UART data register
.equ STATUS_OFFSET, 4            # offset of JTAG UART status register
.equ WSPACE_MASK, 0xFFFF         # used in AND operation to check status
.equ SWITCHES,	0x10000040

_start:
    movia   sp, 0x7FFFFC         # initialize the stack pointer
    addi    r2, r2, '.'          # character to print
    call    PrintChar            # call subroutine
	movi	r2, 0
	call	PrintSwitchStatus
main_loop:
	ldwio   r3, SWITCHES(r0)
	srli	r3, r3, 9
	andi	r3, r3, 0x1
br
end_if

_end:
    break

# Subroutine PrintChar
PrintChar:
    subi    sp, sp, 8            # adjust stack pointer down to reserve space
    stw     r3, 4(sp)            # save r3
    stw     r4, 0(sp)            # save r4
    movia   r3, JTAG_UART_BASE   # set base address of JTAG UART
pc_loop:
    ldwio   r4, STATUS_OFFSET(r3)# read status register
    andhi   r4, r4, WSPACE_MASK  # isolate write space
    beq     r4, r0, pc_loop      # loop if zero
    stwio   r2, DATA_OFFSET(r3)  # write char to data register
    ldw     r3, 4(sp)            # restore r3
    ldw     r4, 0(sp)            # restore r4
    addi    sp, sp, 8            # readjust stack pointer
    ret                          # return

# Subroutine PrintSwitchStatus
PrintSwitchStatus:
	subi