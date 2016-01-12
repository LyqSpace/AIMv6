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

void enter_SVC_mode() {
	asm volatile (
		"isb;"
		"mov r1, lr;"
		"mrs r0, cpsr;"
		"bic r0, r0, #0xf;"
		"orr r0, r0, #0x3;"
		"msr cpsr, r0;"
		"mov pc, r1;"
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
		"bic r0, r0, #0xf;"
		"orr r0, r0, #0x0;"
		"msr cpsr, r0;"
		"mov pc, r1;"
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
