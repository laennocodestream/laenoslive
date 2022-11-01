VERSION_STRING = "\"v0.1\""
NASMFLAG32BIT = -f elf32
CFLAGS = -m32 -g
run: os.iso
	qemu-system-i386 -cdrom os.iso -m 256M -serial stdio
os.iso: iso/boot/kernel/kernel
	grub-mkrescue -o os.iso iso
iso/boot/kernel/kernel: bin/kernel/kernel
	cp bin/kernel/kernel iso/boot/kernel/kernel
bin/kernel/kernel: bin/nasm/boot.o bin/c/kstdlib.o bin/c/kmain.o linker/linkerscript.ld bin/nasm/asmutils.o bin/c/serial.o bin/c/gdt.o bin/c/paging.o
	ld bin/c/paging.o bin/c/gdt.o bin/nasm/boot.o bin/c/kmain.o bin/nasm/asmutils.o bin/c/serial.o bin/c/kstdlib.o -T linker/linkerscript.ld -melf_i386 -o bin/kernel/kernel
bin/c/kmain.o: src/c/kmain.c head/c/kmain.h head/c/serial.h head/c/definitions.h head/c/kstdlib.h head/c/gdt.h head/c/paging.h
	gcc src/c/kmain.c -c ${CFLAGS} -o bin/c/kmain.o
bin/c/kstdlib.o: src/c/kstdlib.c head/c/definitions.h head/c/kstdlib.h
	gcc src/c/kstdlib.c -c ${CFLAGS} -o bin/c/kstdlib.o
bin/c/serial.o: src/c/serial.c head/c/serial.h
	gcc src/c/serial.c -c ${CFLAGS} -o bin/c/serial.o
bin/c/gdt.o: src/c/gdt.c head/c/gdt.h
	gcc src/c/gdt.c -c ${CFLAGS} -o bin/c/gdt.o
bin/c/paging.o: src/c/paging.c head/c/paging.h
	gcc src/c/paging.c -c ${CFLAGS} -o bin/c/paging.o
bin/nasm/boot.o: src/nasm/boot.s
	nasm ${NASMFLAG32BIT} src/nasm/boot.s -o bin/nasm/boot.o
bin/nasm/asmutils.o: src/nasm/asmutils.s
	nasm ${NASMFLAG32BIT} src/nasm/asmutils.s -o bin/nasm/asmutils.o
debug: os.iso
	qemu-system-i386 -cdrom os.iso -m 256M -serial stdio -s -S -d int