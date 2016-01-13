/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "mode_switch.h"

void enter_SVC_mode() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
		"bic r0, r0, #0xf;"
		"orr r0, r0, #0x3;"
		"msr cpsr, r0;"
		"mov pc, r1;"
		:
		:
		:"r0","r1"
	);
}

void enter_IRQ_mode() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
		"bic r0, r0, #0xf;"
		"orr r0, r0, #0x2;"
		"msr cpsr, r0;"
		"mov pc, r1;"
	);
}

void enter_SYS_mode() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
		"bic r0, r0, #0xf;"
		"orr r0, r0, #0xf;"
		"msr cpsr, r0;"
		"mov pc, r1;"
	);
	
}

void enter_USER_mode() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
		"ldr r2, =#0xfffffff0;"
		"and r0, r0, r2;"
		"msr cpsr, r0;"
		"mov pc, r1;"
		:
		:
		:"r0","r1","r2"
	);
	
}

void enable_IRQ() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
    	"bic r0, r0, #0x80;"
    	"msr cpsr, r0;"
    	"mov pc, r1;"
	);
}

void disable_IRQ() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
    	"orr r0, r0, #0x80;"
    	"msr cpsr, r0;"
    	"mov pc, r1;"
	);
}

void get_cur_mode() {

	int cpsr;
	asm volatile(
		"mrs r0, cpsr;"
		"mov %0, r0;" 
		: "=r"(cpsr)
	);

	switch (cpsr) {
	case 0x600001D0:
		puts("MODE: Now in USER mode. CPSR=0x600001D0");
		break;
	case 0x600001DF:
		uart_spin_puts("MODE: Now in SYS mode. CPSR=");	
		puthex(cpsr);
		break;
	case 0x600001D3:
		uart_spin_puts("MODE: Now in SVC mode. CPSR=");	
		puthex(cpsr);
		break;
	case 0x600001D2:
		uart_spin_puts("MODE: Now in IRQ mode. CPSR=");	
		puthex(cpsr);
		break;
	default:
		uart_spin_puts("MODE: Now in UNKNOWN mode. CPSR=");	
		puthex(cpsr);
		break;
	}
}