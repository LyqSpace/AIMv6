/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "page_table_defines.h"
#include "run.h"

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

	TypePte *kernel_base = (TypePte*)PAGE_TABLE_ADDRESS + (KERNEL_SPACE >> MEM_SECTION_SHIFT);
	
	for (u32 addr = 0; addr < KERNEL_MEM_SIZE; addr++) {

		TypePte *pte = kernel_base + addr;
		fill_pte_common(pte, addr);
		//puthex((u32)(&(*pte)));
		//puthex(addr);
	}

}

void kernel_origin_addr_map() {

	TypePte *origin_base = (TypePte*)PAGE_TABLE_ADDRESS;

	for (u32 addr = 0; addr < KERNEL_MEM_SIZE; addr++) {

		TypePte *pte = origin_base + addr;
		fill_pte_common(pte, addr);
		//puthex((u32)(&(*pte)));
		//puthex(addr);
	}

}

void kernel_stack_map() {

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

u32 kernel_TTB[4096];

void init_level_1_page_table() {

	uart_spin_puts("VM: Set up Level 1 Page Table.\r\n");

	kernel_high_addr_map();
	kernel_origin_addr_map();
	kernel_stack_map();
	device_map();

	for (int i = 0; i < 4096; i++) {

		uint *pte = (uint*)(PAGE_TABLE_ADDRESS + i * 4);
		kernel_TTB[i] = *pte;
	}

}

void invalidate_TLBs() {

	asm volatile (
		"mov r0, #0x0;"
		//"mcr p15, 0, r0, c8, c6, 0;"
		//"mcr p15, 0, r0, c8, c5, 0;"
		"mcr p15, 0, r0, c8, c7, 0;"
		"dsb;"
		"isb;"
	);
}

void invalidate_ICache() {

	asm volatile (
		"mov r0, #0x0;"
		//"mcr p15, 0, r0, c7, c5, 6;"
		"mcr p15, 0, r0, c7, c5, 0;"
		//"mcr p15, 0, r0, c7, c14, 2;"
	);

}

void invalidate_BranchPredictor() {

	asm volatile (
		"mov r0, #0x0;"
		"mcr p15, 0, r0, c7, c5, 6;"
	);
}

void invalidate_DCache() {

	asm volatile (
		"mcr p15, 0, r11, c7, c14, 2;"
	);
}

void invalidate_TLBs_Caches() {

	uart_spin_puts("VM: Invalidate TLBs & caches.\r\n");

	invalidate_TLBs();
	invalidate_ICache();
	invalidate_BranchPredictor();
	invalidate_DCache();

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
		// "orr r0, r0, #0x2;"				// align
		// "orr r0, r0, #0x4;"				// data & unified cache
  		// "orr r0, r0, #0x20;"			// DMB $ DSB $ ISB
  		// "orr r0, r0, #0x800;"			// flow prediction
  		// "orr r0, r0, #0x1000;"			// instruction cache
 		"mcr p15, 0, r0, c1, c0, 0;"
		"dsb;"
		"isb;"
	);
}

void vm_init() {

	init_level_1_page_table();

	invalidate_TLBs_Caches();
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

char* new_TTB() {

	char *ttb_addr = kalloc(1);
	for (int i = 0; i < 4096; i++) {
		u32 *ttb_ele = (u32*)(ttb_addr + i * 4);
		*ttb_ele = kernel_TTB[i];
	}
	return ttb_addr;

}