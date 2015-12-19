#include "page_table_defines.h"
#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>

void fill_pte_common(TypePte *pte, u32 addr) {

	pte->type =	2;		// 0-1
	pte->B = 0;			// 2
	pte->C = 0;			// 3
	pte->XN = 0;		// 4
	pte->Domain = 0x0;	// 5-8
	pte->zero_L = 0;	// 9
	pte->AP_L = 0x2;	// 10-11
	pte->TEX = 0x0;		// 12-14
	pte->AP_H = 0;		// 15
	pte->S = 1;			// 16
	pte->ng = 0;		// 17
	pte->zero_H = 0;	// 18
	pte->NS = 0;		// 19
	pte->base = addr;	// 20-31

}

void kernel_high_addr_map() {

	uart_spin_puts("VM: Map high kernel space virtual address to physical address.\r\n");
	
	TypePte *kernel_base = (TypePte*)PAGE_TABLE_ADDRESS + (KERNEL_SPACE >> MEM_SECTION_SHIFT);
	
	for (u32 addr = 0; addr < KERNEL_MEM_SIZE; addr++) {

		TypePte *pte = kernel_base + addr;
		fill_pte_common(pte, addr);
		//puthex((u32)(&(*pte)));
		//puthex(addr);
	}

}

void kernel_origin_addr_map() {

	uart_spin_puts("VM: Map origin kernel space virtual address to physical address.\r\n");
	
	TypePte *origin_base = (TypePte*)PAGE_TABLE_ADDRESS;

	for (u32 addr = 0; addr < KERNEL_MEM_SIZE; addr++) {

		TypePte *pte = origin_base + addr;
		fill_pte_common(pte, addr);
		//puthex((u32)(&(*pte)));
		//puthex(addr);
	}

}

void kernel_stack_map() {

	uart_spin_puts("VM: Map kernel stack virtual address to physical address.\r\n");

	TypePte *kernel_stack_base = (TypePte*)PAGE_TABLE_ADDRESS + (KERNEL_STACK_SPACE >> MEM_SECTION_SHIFT);
	u32 kernel_stack_phy_base = KERNEL_STACK_PHY >> MEM_SECTION_SHIFT;

	for (u32 addr = 0; addr < KERNEL_STACK_MEM_SIZE; addr++) {

		TypePte *pte = kernel_stack_base - addr;
		fill_pte_common(pte, kernel_stack_phy_base - addr);

		//puthex((u32)(&(*pte)));
		//puthex(kernel_stack_phy_base + addr);
	}

}

void device_map() {

	uart_spin_puts("VM: Map device virtual address to physical address.\r\n");

	TypePte *device_base = (TypePte*)PAGE_TABLE_ADDRESS + (DEVICE_SPACE >> MEM_SECTION_SHIFT);
	u32 device_phy = DEVICE_SPACE >> MEM_SECTION_SHIFT;
	
	for (u32 addr = 0; addr < DEVICE_MEM_SIZE; addr++) {

		TypePte *pte = device_base + addr;
		fill_pte_common(pte, device_phy + addr);
		pte->AP_L = 0x3;

		//puthex((u32)(&(*pte)));
		//puthex(device_phy + addr);
	}
}

void init_level_1_page_table() {

	uart_spin_puts("VM: Set up Level 1 Page Table.\r\n");

	kernel_high_addr_map();
	kernel_origin_addr_map();
	kernel_stack_map();
	device_map();

}

void invalidate_Cache() {

	uart_spin_puts("VM: Invalidate branch predictor cache & data cache & instruction cache.\r\n");

	asm volatile (
		"mov r0, #0x0;"
		"mcr p15, 0, r0, c7, c5, 6;"
		"mcr p15, 0, r0, c7, c5, 0;"
		"mcr p15, 0, r0, c7, c14, 2;"
		"dsb;"
		"isb;"
	);

}

void invalidate_TLB() {

	uart_spin_puts("VM: Invalidate data TLB & instruction TLB & unified TLB.\r\n");

	asm volatile (
		"mov r0, #0x0;"
		"mcr p15, 0, r0, c8, c6, 0;"
		"mcr p15, 0, r0, c8, c5, 0;"
		"mcr p15, 0, r0, c8, c7, 0;"
		"dsb;"
		"isb;"
	);
}

void setup_TTB() {

	uart_spin_puts("VM: Set up Translate Table Base.\r\n");

	asm volatile (
		"mov r0, %0;"
		"mcr p15, 0, r0, c2, c0, 0;"
		"dsb;"
		"isb;"
		:
		: "r"(PAGE_TABLE_ADDRESS)
		: "r0"
	);
}

void setup_Domain_Access() {
	uart_spin_puts("VM: Set up domain access.\r\n");
	asm volatile (
		"ldr r0, =0x00000001;" // 01 client mode; 11 no check mode;
 		"mcr p15, 0, r0, c3, c0, 0;"
 		"dsb;"
 		"isb;"
	);
}

void enable_MMU() {

	uart_spin_puts("VM: Enable Memory Management Unit & caches.\r\n");

	asm volatile (
		"mrc p15, 0, r0, c1, c0, 0;"
		"orr r0, r0, #0x1;"				// MMU
		"orr r0, r0, #0x2;"				// align
    	"orr r0, r0, #0x4;"				// data & unified cache
    	"orr r0, r0, #0x20;"			// DMB $ DSB $ ISB
    	"orr r0, r0, #0x800;"			// flow prediction
    	"orr r0, r0, #0x1000;"			// instruction cache
 		"mcr p15, 0, r0, c1, c0, 0;"
		"isb;"
		"dsb;"
	);
}

void setup_vm() {

	init_level_1_page_table();

	invalidate_Cache();
	invalidate_TLB();
	setup_TTB();
	setup_Domain_Access();
	enable_MMU();

	// Modify FP SP
	uart_spin_puts("VM: Modify kernel stack frame pointers.\r\n");
	asm volatile (
		"pop {r0, r1};"
		"ldr r0, =0x80000000;"
		"add r0, r1, r0;"
		"mov fp, %0;"
		"mov sp, fp;"
		"mov pc, r0;"
		:
		: "r"(KERNEL_STACK_SPACE)
		: 
	);

	uart_spin_puts("VM: This sentence should not be seen.\r\n");

}