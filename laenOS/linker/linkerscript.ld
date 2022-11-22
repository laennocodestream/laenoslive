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
    . = ALIGN(4K);
    CR3_HOLDER = .;
    . = . + 4096;
    .program : ALIGN(4K){
        *(.program)
    }
    kernel_memory_end = .;
}