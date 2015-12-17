#include "page_table_defines.h"
#include "l1l2cache.h"
#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>

void fill_pte_common(TypePte *pte, u32 addr) {

	pte->type =	2;
	pte->B = 0;
	pte->C = 0;
	pte->XN = 0;
	pte->Domain = 0;
	pte->zero_L = 0;
	pte->AP_L = 2;
	pte->TEX = 0x0;
	pte->AP_H = 0x0;
	pte->S = 1;
	pte->ng = 0;
	pte->zero_H = 0;
	pte->NS = 0;
	pte->base = addr;

}

void kernel_high_addr_map() {

	uart_spin_puts("VM: Kernel Space High Address V2P Mapping.\r\n");
	
	u32 kernel_base = (u32)KERNEL_SPACE >> MEM_SECTION_SHIFT;
	
	for (u32 addr = 0; addr < PHY_MEM_SIZE; addr += MEM_SECTION_SIZE) {

		TypePte *pte = (TypePte*)(PAGE_TABLE_ADDRESS + kernel_base + addr);
		fill_pte_common(pte, addr);
		puthex(PAGE_TABLE_ADDRESS+addr);
		puthex(*(u32*)pte);
	}

}

void kernel_low_addr_map() {

	uart_spin_puts("VM: Kernel Space Low Address V2P Mapping.\r\n");
	
	for (u32 addr = 0; addr < PHY_MEM_SIZE; addr += MEM_SECTION_SIZE) {

		TypePte *pte = (TypePte*)(PAGE_TABLE_ADDRESS + addr);
		fill_pte_common(pte, addr);
		puthex(PAGE_TABLE_ADDRESS+addr);
		puthex(*(u32*)pte);
	}

}

void kernel_stack_map() {

	uart_spin_puts("VM: Kernel Stack V2P Mapping.\r\n");

	u32 kernel_stack_base = (u32)KERNEL_STACK_SPACE >> MEM_SECTION_SHIFT;
	
	for (u32 addr = 0; addr < PHY_MEM_SIZE; addr += MEM_SECTION_SIZE) {

		TypePte *pte = (TypePte*)(PAGE_TABLE_ADDRESS + kernel_stack_base + addr);
		fill_pte_common(pte, addr);
	}

}

void device_map() {

	uart_spin_puts("VM: Device V2P Mapping.\r\n");

	u32 device_base = (u32)DEVICE_SPACE >> MEM_SECTION_SHIFT;

	for (u32 addr = 0; addr < PHY_MEM_SIZE; addr += MEM_SECTION_SIZE) {

		TypePte *pte = (TypePte*)(PAGE_TABLE_ADDRESS + device_base + addr);
		fill_pte_common(pte, addr);
		pte->AP_L = 3;
	}
}

void init_level_1_page_table() {

	uart_spin_puts("VM: Set up Level 1 Page Table.\r\n");

	kernel_high_addr_map();
	kernel_low_addr_map();
	kernel_stack_map();
	device_map();

	uart_spin_puts("VM: Finish Level 1 Page Table.\r\n");

}

void setup_TTB() {

	uart_spin_puts("VM: Set up Translate Table Base.\r\n");

	asm volatile (
		"mov r0, %0;"
		"mcr p15, 0, r0, c2, c0, 0;"
		:
		: "r"(PAGE_TABLE_ADDRESS)
		: "r0"
	);
}

void setup_Domain_Access() {
	uart_spin_puts("VM: Setup Domain Access.\r\n");
	asm volatile (
		"ldr r0, =0x55555555;"
 		"mcr p15, 0, r0, c3, c0, 0;"
 		"isb;"
	);
}

void enable_MMU() {

	uart_spin_puts("VM: Enable Memory Management Unit.\r\n");

	asm volatile (
		"mrc p15, 0, r0, c1, c0, 0;"
		"orr R0, r0, #0x1;"
		"mcr p15, 0, r0, c1, c0, 0;"
		"isb;"
		"dsb;"
		:
		:
		: "r0"
	);
}



void modify_PC() {
	uart_spin_puts("VM: Change PC to Virtual Address.\r\n");
	asm volatile (
 		"ldr r0, =#0x80000000;"
		"add r14, r0;"
		"mov r15, r14;"
		"nop;"
		"nop;"
		"nop;"
		"nop;"
	);
}

void setup_vm() {

	uart_spin_puts("VM: Disable L1 Cache.\r\n");
	disable_l1cache();
	uart_spin_puts("VM: Disable L2 Cache.\r\n");
	disable_l2cache();

	init_level_1_page_table();
	setup_TTB();
	setup_Domain_Access();
	enable_MMU();

	uart_spin_puts("VM: Enable L2 Cache.\r\n");
	enable_l2cache();
	uart_spin_puts("VM: Enable L1 Cache.\r\n");
	enable_l1cache();

	//Setup_FP
	uart_spin_puts("VM: Set Up Stack.\r\n");
	asm volatile(
		"ldr r1, [fp, #0]\r\n"
		"ldr r0, =0x80000000\r\n"
		"add r0, r1, r0\r\n"
		"str r0, [fp, #0]\r\n"
	);

	modify_PC();
	
	u32 tmp_pc;
	uart_spin_puts("Now, PC should run on kernel address! PC = ");
	asm volatile("mov %0 ,pc" : "=r"(tmp_pc):: "r0");
	puthex(tmp_pc);

}