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

void interrupt_init() {
	uart_spin_puts("INTERRUPT: Initialized interrupts.\r\n");
	asm volatile(
		"ldr r0, =interrupt_vector;"
		"ldr r1, =0x80000000;"
		"add r0, r1, r0;"
		"mcr p15, 0, r0, c12, c0, 0;"
	);
}


void C_SVC_handler(uint number, uint *param) {
	uart_spin_puts("SVC handler.\r\n");
	print_spsr();
	print_cpsr();
	switch (number) {
		case 0:
			system_call(param);
			break;
		case 1:
			break;
	}
	print_spsr();
}

void C_IRQ_handler() {
	uart_spin_puts("IRQ handler.\r\n");

}

void C_Prefetch_handler() {
	
}

void C_Data_handler() {

}

void system_call(uint *param) {

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