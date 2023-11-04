.set IRQ_BASE, 0x20

.section .text

.extern _ZN6cernos12hardwarecomm16InterruptManager15handleInterruptEhj

.global _ZN6cernos12hardwarecomm16InterruptManager22IgnoreInterruptRequestEv

.macro handleException num
.global _ZN6cernos12hardwarecomm16InterruptManager16handleException\num\()Ev
_ZN6cernos12hardwarecomm16InterruptManager16handleException\num\()Ev:
	movb $\num, (interruptnum)
	jmp int_bottom
.endm

.macro handleInterruptRequest num
.global _ZN6cernos12hardwarecomm16InterruptManager26handleInterruptRequest\num\()Ev
_ZN6cernos12hardwarecomm16InterruptManager26handleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interruptnum)
	pushl $0
	jmp int_bottom
.endm

handleInterruptRequest 0x00
handleInterruptRequest 0x01
handleInterruptRequest 0x0C

int_bottom:
	
	#pusha
	#pushl %ds
	#pushl %es
	#pushl %fs
	#pushl %gs

	pushl %ebp
	pushl %edi
	pushl %esi

	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

	pushl %esp
	push (interruptnum)
	call _ZN6cernos12hardwarecomm16InterruptManager15handleInterruptEhj
	#addl $5, %esp
	movl %eax, %esp

	popl %eax
	popl %ebx
	popl %ecx
	popl %edx

	popl %esi
	popl %edi
	popl %ebp
	
	#popl %gs
	#popl %fs
	#popl %es
	#popl %ds
	#popa

	add $4, %esp

_ZN6cernos12hardwarecomm16InterruptManager22IgnoreInterruptRequestEv:

	iret

.data
	interruptnum: .byte 0