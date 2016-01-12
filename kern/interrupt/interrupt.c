/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

 #include "interrupt.h"

void SYS_init() {

	enter_SYS_mode();
	asm volatile (
		"mov sp, %0;"
		:
		:"r"(KERNEL_SPACE + SYS_STACK)
	);
}

void IRQ_init() {

	enter_IRQ_mode();
	asm volatile (
		"mov sp, %0;"
		:
		:"r"(KERNEL_SPACE + IRQ_STACK)
	);
}

void interrupt_init() {
	
	uart_spin_puts("INTERRUPT: Initialize interrupts!\r\n");

	SYS_init();
	IRQ_init();
	enter_SVC_mode();

	asm volatile(
		"ldr r0, =interrupt_vector;"
		"ldr r1, =0x80000000;"
		"add r0, r1, r0;"
		"mcr p15, 0, r0, c12, c0, 0;"
		"isb;"
		"dsb;"
	);
}


void C_SVC_handler(uint number, uint *params) {

	uart_spin_puts("INTERRUPT: SVC handler.\r\n");
	switch (number) {
		case 0x10:
			system_call(params);
			break;
		case 1:
			uart_spin_puts("1.\r\n");		
			break;
		default:
			uart_spin_puts("nothing.\r\n");		
	}
}

void C_IRQ_handler() {
	uart_spin_puts("IRQ handler.\r\n");

}

void C_Prefetch_handler() {
	
}

void C_Data_handler() {

}

void system_call(uint *params) {

	puthex(params[0]);
	puthex(params[1]);

}

void print_spsr() {
	int spsr;
	asm volatile(
		"mrs r0, spsr;"
		"mov %0, r0;" 
		: "=r"(spsr)
	);
	uart_spin_puts("DEBUG: SPSR=");
	puthex(spsr);
}

void print_cpsr() {
	int cpsr;
	asm volatile(
		"mrs r0, cpsr;"
		"mov %0, r0;" 
		: "=r"(cpsr)
	);
	uart_spin_puts("DEBUG: CPSR=");
	puthex(cpsr);
}