ENTRY(_start)

SECTIONS
{
    . = 1M;
    kernel_text_start = .;
    .text : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    kernel_text_end = .;

    .rodata : ALIGN(4K){
        *(.rodata)
    }
    .data : ALIGN(4K){
        *(.data)
    }

    .bss : ALIGN(4K){
        *(.kstack)
        *(.bss)
        . = ALIGN(4K);
        *(.page)
    }
    kernel_memory_end = .;
}