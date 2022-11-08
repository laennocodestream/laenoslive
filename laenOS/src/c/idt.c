#include "../../head/c/idt.h"
#include "../../head/c/serial.h"
extern Serial com1;
void load_idt(IdtPointer* idtPointer);
#define LOAD_INTERRUPT(x, entries) int __nasm_interrupt_##x();\
	entries[x].present = 1; \
	entries[x].gate_type = INTERRUPT_GATE_TYPE;\
	entries[x].caller_protection = 3;\
	entries[x].zero = 0;\
	entries[x].reserved = 0;\
	entries[x].segment_selector = 8;\
	entries[x].offset_low = (uint)(&__nasm_interrupt_##x) & 0xFFFF;\
	entries[x].offset_high = (uint)(&__nasm_interrupt_##x) >> 16 ;
#define LOAD_TRAP(x, entries) int __nasm_interrupt_##x();\
	entries[x].present = 1; \
	entries[x].gate_type = TRAP_GATE_TYPE;\
	entries[x].caller_protection = 3;\
	entries[x].zero = 0;\
	entries[x].reserved = 0;\
	entries[x].segment_selector = 8;\
	entries[x].offset_low = (uint)(&__nasm_interrupt_##x) & 0xFFFF;\
	entries[x].offset_high = (uint)(&__nasm_interrupt_##x) >> 16 ;

void __c_interrupt_vector(uint interruptCode,uint edi, uint esi, uint ebp, uint esp, uint ebx, uint edx, uint ecx, uint eax, uint error_code)
{
	Serial_writeStr(&com1, "[debug] interruption called \0");
	Serial_writeHex32(&com1, interruptCode);
	Serial_writeStr(&com1, "\n\0");
}

void make_and_load_idt(IdtGateDescriptor* entries, IdtPointer* idtPointer){
	LOAD_INTERRUPT(0, entries)
	LOAD_INTERRUPT(1, entries)
	LOAD_INTERRUPT(2, entries)
	LOAD_INTERRUPT(3, entries)
	LOAD_INTERRUPT(4, entries)
	LOAD_INTERRUPT(5, entries)
	LOAD_INTERRUPT(6, entries)
	LOAD_INTERRUPT(7, entries)
	LOAD_TRAP(8, entries)
	LOAD_INTERRUPT(9, entries)
	LOAD_TRAP(10, entries)
	LOAD_TRAP(11, entries)
	LOAD_TRAP(12, entries)
	LOAD_TRAP(13, entries)
	LOAD_TRAP(14, entries)
	LOAD_INTERRUPT(15, entries)
	LOAD_INTERRUPT(16, entries)
	LOAD_TRAP(17, entries)
	LOAD_INTERRUPT(18, entries)
	LOAD_INTERRUPT(19, entries)
	LOAD_INTERRUPT(20, entries)
	LOAD_TRAP(21, entries)
	LOAD_INTERRUPT(22, entries)
	LOAD_INTERRUPT(23, entries)
	LOAD_INTERRUPT(24, entries)
	LOAD_INTERRUPT(25, entries)
	LOAD_INTERRUPT(26, entries)
	LOAD_INTERRUPT(27, entries)
	LOAD_INTERRUPT(28, entries)
	LOAD_TRAP(29, entries)
	LOAD_TRAP(30, entries)
	LOAD_INTERRUPT(31, entries)
	LOAD_INTERRUPT(32, entries)
	LOAD_INTERRUPT(33, entries)
	LOAD_INTERRUPT(34, entries)
	LOAD_INTERRUPT(35, entries)
	LOAD_INTERRUPT(36, entries)
	LOAD_INTERRUPT(37, entries)
	LOAD_INTERRUPT(38, entries)
	LOAD_INTERRUPT(39, entries)
	LOAD_INTERRUPT(40, entries)
	LOAD_INTERRUPT(41, entries)
	LOAD_INTERRUPT(42, entries)
	LOAD_INTERRUPT(43, entries)
	LOAD_INTERRUPT(44, entries)
	LOAD_INTERRUPT(45, entries)
	LOAD_INTERRUPT(46, entries)
	LOAD_INTERRUPT(47, entries)
	LOAD_INTERRUPT(48, entries)
	LOAD_INTERRUPT(49, entries)
	LOAD_INTERRUPT(50, entries)
	LOAD_INTERRUPT(51, entries)
	LOAD_INTERRUPT(52, entries)
	LOAD_INTERRUPT(53, entries)
	LOAD_INTERRUPT(54, entries)
	LOAD_INTERRUPT(55, entries)
	LOAD_INTERRUPT(56, entries)
	LOAD_INTERRUPT(57, entries)
	LOAD_INTERRUPT(58, entries)
	LOAD_INTERRUPT(59, entries)
	LOAD_INTERRUPT(60, entries)
	LOAD_INTERRUPT(61, entries)
	LOAD_INTERRUPT(62, entries)
	LOAD_INTERRUPT(63, entries)
	LOAD_INTERRUPT(64, entries)
	LOAD_INTERRUPT(65, entries)
	LOAD_INTERRUPT(66, entries)
	LOAD_INTERRUPT(67, entries)
	LOAD_INTERRUPT(68, entries)
	LOAD_INTERRUPT(69, entries)
	LOAD_INTERRUPT(70, entries)
	LOAD_INTERRUPT(71, entries)
	LOAD_INTERRUPT(72, entries)
	LOAD_INTERRUPT(73, entries)
	LOAD_INTERRUPT(74, entries)
	LOAD_INTERRUPT(75, entries)
	LOAD_INTERRUPT(76, entries)
	LOAD_INTERRUPT(77, entries)
	LOAD_INTERRUPT(78, entries)
	LOAD_INTERRUPT(79, entries)
	LOAD_INTERRUPT(80, entries)
	LOAD_INTERRUPT(81, entries)
	LOAD_INTERRUPT(82, entries)
	LOAD_INTERRUPT(83, entries)
	LOAD_INTERRUPT(84, entries)
	LOAD_INTERRUPT(85, entries)
	LOAD_INTERRUPT(86, entries)
	LOAD_INTERRUPT(87, entries)
	LOAD_INTERRUPT(88, entries)
	LOAD_INTERRUPT(89, entries)
	LOAD_INTERRUPT(90, entries)
	LOAD_INTERRUPT(91, entries)
	LOAD_INTERRUPT(92, entries)
	LOAD_INTERRUPT(93, entries)
	LOAD_INTERRUPT(94, entries)
	LOAD_INTERRUPT(95, entries)
	LOAD_INTERRUPT(96, entries)
	LOAD_INTERRUPT(97, entries)
	LOAD_INTERRUPT(98, entries)
	LOAD_INTERRUPT(99, entries)
	LOAD_INTERRUPT(100, entries)
	LOAD_INTERRUPT(101, entries)
	LOAD_INTERRUPT(102, entries)
	LOAD_INTERRUPT(103, entries)
	LOAD_INTERRUPT(104, entries)
	LOAD_INTERRUPT(105, entries)
	LOAD_INTERRUPT(106, entries)
	LOAD_INTERRUPT(107, entries)
	LOAD_INTERRUPT(108, entries)
	LOAD_INTERRUPT(109, entries)
	LOAD_INTERRUPT(110, entries)
	LOAD_INTERRUPT(111, entries)
	LOAD_INTERRUPT(112, entries)
	LOAD_INTERRUPT(113, entries)
	LOAD_INTERRUPT(114, entries)
	LOAD_INTERRUPT(115, entries)
	LOAD_INTERRUPT(116, entries)
	LOAD_INTERRUPT(117, entries)
	LOAD_INTERRUPT(118, entries)
	LOAD_INTERRUPT(119, entries)
	LOAD_INTERRUPT(120, entries)
	LOAD_INTERRUPT(121, entries)
	LOAD_INTERRUPT(122, entries)
	LOAD_INTERRUPT(123, entries)
	LOAD_INTERRUPT(124, entries)
	LOAD_INTERRUPT(125, entries)
	LOAD_INTERRUPT(126, entries)
	LOAD_INTERRUPT(127, entries)
	LOAD_INTERRUPT(128, entries)
	LOAD_INTERRUPT(129, entries)
	LOAD_INTERRUPT(130, entries)
	LOAD_INTERRUPT(131, entries)
	LOAD_INTERRUPT(132, entries)
	LOAD_INTERRUPT(133, entries)
	LOAD_INTERRUPT(134, entries)
	LOAD_INTERRUPT(135, entries)
	LOAD_INTERRUPT(136, entries)
	LOAD_INTERRUPT(137, entries)
	LOAD_INTERRUPT(138, entries)
	LOAD_INTERRUPT(139, entries)
	LOAD_INTERRUPT(140, entries)
	LOAD_INTERRUPT(141, entries)
	LOAD_INTERRUPT(142, entries)
	LOAD_INTERRUPT(143, entries)
	LOAD_INTERRUPT(144, entries)
	LOAD_INTERRUPT(145, entries)
	LOAD_INTERRUPT(146, entries)
	LOAD_INTERRUPT(147, entries)
	LOAD_INTERRUPT(148, entries)
	LOAD_INTERRUPT(149, entries)
	LOAD_INTERRUPT(150, entries)
	LOAD_INTERRUPT(151, entries)
	LOAD_INTERRUPT(152, entries)
	LOAD_INTERRUPT(153, entries)
	LOAD_INTERRUPT(154, entries)
	LOAD_INTERRUPT(155, entries)
	LOAD_INTERRUPT(156, entries)
	LOAD_INTERRUPT(157, entries)
	LOAD_INTERRUPT(158, entries)
	LOAD_INTERRUPT(159, entries)
	LOAD_INTERRUPT(160, entries)
	LOAD_INTERRUPT(161, entries)
	LOAD_INTERRUPT(162, entries)
	LOAD_INTERRUPT(163, entries)
	LOAD_INTERRUPT(164, entries)
	LOAD_INTERRUPT(165, entries)
	LOAD_INTERRUPT(166, entries)
	LOAD_INTERRUPT(167, entries)
	LOAD_INTERRUPT(168, entries)
	LOAD_INTERRUPT(169, entries)
	LOAD_INTERRUPT(170, entries)
	LOAD_INTERRUPT(171, entries)
	LOAD_INTERRUPT(172, entries)
	LOAD_INTERRUPT(173, entries)
	LOAD_INTERRUPT(174, entries)
	LOAD_INTERRUPT(175, entries)
	LOAD_INTERRUPT(176, entries)
	LOAD_INTERRUPT(177, entries)
	LOAD_INTERRUPT(178, entries)
	LOAD_INTERRUPT(179, entries)
	LOAD_INTERRUPT(180, entries)
	LOAD_INTERRUPT(181, entries)
	LOAD_INTERRUPT(182, entries)
	LOAD_INTERRUPT(183, entries)
	LOAD_INTERRUPT(184, entries)
	LOAD_INTERRUPT(185, entries)
	LOAD_INTERRUPT(186, entries)
	LOAD_INTERRUPT(187, entries)
	LOAD_INTERRUPT(188, entries)
	LOAD_INTERRUPT(189, entries)
	LOAD_INTERRUPT(190, entries)
	LOAD_INTERRUPT(191, entries)
	LOAD_INTERRUPT(192, entries)
	LOAD_INTERRUPT(193, entries)
	LOAD_INTERRUPT(194, entries)
	LOAD_INTERRUPT(195, entries)
	LOAD_INTERRUPT(196, entries)
	LOAD_INTERRUPT(197, entries)
	LOAD_INTERRUPT(198, entries)
	LOAD_INTERRUPT(199, entries)
	LOAD_INTERRUPT(200, entries)
	LOAD_INTERRUPT(201, entries)
	LOAD_INTERRUPT(202, entries)
	LOAD_INTERRUPT(203, entries)
	LOAD_INTERRUPT(204, entries)
	LOAD_INTERRUPT(205, entries)
	LOAD_INTERRUPT(206, entries)
	LOAD_INTERRUPT(207, entries)
	LOAD_INTERRUPT(208, entries)
	LOAD_INTERRUPT(209, entries)
	LOAD_INTERRUPT(210, entries)
	LOAD_INTERRUPT(211, entries)
	LOAD_INTERRUPT(212, entries)
	LOAD_INTERRUPT(213, entries)
	LOAD_INTERRUPT(214, entries)
	LOAD_INTERRUPT(215, entries)
	LOAD_INTERRUPT(216, entries)
	LOAD_INTERRUPT(217, entries)
	LOAD_INTERRUPT(218, entries)
	LOAD_INTERRUPT(219, entries)
	LOAD_INTERRUPT(220, entries)
	LOAD_INTERRUPT(221, entries)
	LOAD_INTERRUPT(222, entries)
	LOAD_INTERRUPT(223, entries)
	LOAD_INTERRUPT(224, entries)
	LOAD_INTERRUPT(225, entries)
	LOAD_INTERRUPT(226, entries)
	LOAD_INTERRUPT(227, entries)
	LOAD_INTERRUPT(228, entries)
	LOAD_INTERRUPT(229, entries)
	LOAD_INTERRUPT(230, entries)
	LOAD_INTERRUPT(231, entries)
	LOAD_INTERRUPT(232, entries)
	LOAD_INTERRUPT(233, entries)
	LOAD_INTERRUPT(234, entries)
	LOAD_INTERRUPT(235, entries)
	LOAD_INTERRUPT(236, entries)
	LOAD_INTERRUPT(237, entries)
	LOAD_INTERRUPT(238, entries)
	LOAD_INTERRUPT(239, entries)
	LOAD_INTERRUPT(240, entries)
	LOAD_INTERRUPT(241, entries)
	LOAD_INTERRUPT(242, entries)
	LOAD_INTERRUPT(243, entries)
	LOAD_INTERRUPT(244, entries)
	LOAD_INTERRUPT(245, entries)
	LOAD_INTERRUPT(246, entries)
	LOAD_INTERRUPT(247, entries)
	LOAD_INTERRUPT(248, entries)
	LOAD_INTERRUPT(249, entries)
	LOAD_INTERRUPT(250, entries)
	LOAD_INTERRUPT(251, entries)
	LOAD_INTERRUPT(252, entries)
	LOAD_INTERRUPT(253, entries)
	LOAD_INTERRUPT(254, entries)
	LOAD_INTERRUPT(255, entries)
	idtPointer -> entries_size = 0x7ff;
	idtPointer -> entry_pos = entries;
	Serial_writeHex32(&com1,entries);
	Serial_writeStr(&com1, " end entries\n\0");
	load_idt(idtPointer);
}