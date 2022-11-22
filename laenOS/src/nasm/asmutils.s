global inb
global outb
global load_gdt
global load_idt
global load_page_directory
section .text
inb:
    push ebp
    mov ebp, esp
    xor eax, eax
    mov edx, [ebp+8]
    in al, dx
    pop ebp
    ret
outb:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    mov eax, [ebp + 12]
    out dx, ax
    pop ebp
    ret

load_page_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    pop ebp
    ret

load_gdt:
    push ebp
    mov ebp, esp
    mov word [size], 6*8-1
    mov eax, [ebp + 8]
    mov [offset], eax
    mov eax, GDT_POINTER
    lgdt [GDT_POINTER]
    jmp 0x08:gdt_loaded
gdt_loaded:
    mov ax, 0x10 
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    pop ebp
    ret

load_idt:
    push ebp
    mov ebp, esp
    push eax
    mov eax, [ebp + 8]
    lidt [eax]
    pop eax
    pop ebp
    ret
section .bss
    GDT_POINTER:
        size: resw 1
        offset: resd 1
        