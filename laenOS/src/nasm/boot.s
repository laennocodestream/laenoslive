global _start
global page_directory
global page_entries
multiboot_magic:    equ 0x1BADB002
multiboot_flags:    equ 0x4
multiboot_checksum: equ -(multiboot_flags + multiboot_magic)
width:  equ 1920
heigth: equ 1080
depth:  equ 24
mode:   equ 0


section .multiboot
mMagic:     dd multiboot_magic
mFlags:     dd multiboot_flags
mCheck:     dd multiboot_checksum
mMap:       dd 0, 0, 0, 0, 0
mMode_type: dd mode
mWidth:     dd width
mHeight:    dd heigth
mDepth:     dd depth

section .text
extern kmain
_start:
    mov esp, stack_start
    mov ebp, stack_start
    push ebx
    call kmain 
section .kstack nobits
stack_end:
    resq 1000000
stack_start:
section .page nobits
page_directory: resd 1024
page_entries: resd 1024*1024

