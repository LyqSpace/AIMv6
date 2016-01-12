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

void enter_user_mode() {
	asm volatile(
		"mrs r0, cpsr\r\n"
		"bic r0, r0, #0xF\r\n"
		"msr cpsr, r0\r\n"
		"isb\r\n"
		"movs pc, lr\r\n");
}

void enter_sys_mode() {
	asm volatile(
		"mrs r0, cpsr\r\n"
		"orr r0, r0, #0xF\r\n"
		"msr cpsr, r0\r\n"
		"isb\r\n"
		"movs pc, lr\r\n"
		"isb\r\n");
}