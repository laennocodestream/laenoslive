extern page_directory
extern CR3_HOLDER

%macro trap 1
global __nasm_interrupt_%1
__nasm_interrupt_%1:
	extern __c_interrupt_vector
	push dword %1
	jmp __nasm_end_interrupt
%endmacro

%macro interrupt 1
global __nasm_interrupt_%1
__nasm_interrupt_%1:
	extern __c_interrupt_vector
	push dword 0
	push dword %1
	jmp __nasm_end_interrupt
%endmacro


__nasm_end_interrupt:
	push eax
	mov eax, [esp + 16]
	cmp eax, 0x8
	je __nasm_end_interrupt_no_cr3
	mov eax, cr3
	mov [CR3_HOLDER], eax
	mov eax, page_directory
	mov cr3, eax
	pop eax
	pushad
	call __c_interrupt_vector
	mov eax, [CR3_HOLDER]
	mov cr3, eax
	popad
	add esp, 8
	iret

__nasm_end_interrupt_no_cr3:
	pop eax
	pushad
	call __c_interrupt_vector
	popad
	add esp, 8
	iret

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