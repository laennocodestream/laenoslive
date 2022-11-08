%macro trap 1
global __nasm_interrupt_%1
__nasm_interrupt_%1:
	extern __c_interrupt_vector
	pushad
	push dword %1
	call __c_interrupt_vector
	pop eax
	popad
	add esp, 4
	iret
%endmacro

%macro interrupt 1
global __nasm_interrupt_%1
__nasm_interrupt_%1:
	extern __c_interrupt_vector
	push dword 0
	pushad
	push dword %1
	call __c_interrupt_vector
	pop eax
	popad
	add esp, 4
	iret
%endmacro

interrupt 0
interrupt 1
interrupt 2
interrupt 3
interrupt 4
interrupt 5
interrupt 6
interrupt 7
trap 8
interrupt 9
trap 10
trap 11
trap 12
trap 13
trap 14
interrupt 15
interrupt 16
trap 17
interrupt 18
interrupt 19
interrupt 20
trap 21
%assign i 22
%rep 7
interrupt i
%assign i i+1
%endrep
trap 29
trap 30
%assign i 31
%rep 225
interrupt i
%assign i i+1
%endrep