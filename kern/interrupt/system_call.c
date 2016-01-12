/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "system_call.h"

void puts(char *str) {

    asm volatile(
        "mov r0, %0;"
        "mov r1, %1;"
        "swi 0x10;"
        :: "r"(PUTS),"r"(str)
        : "r0","r1"
    );
}

void system_call(uint *params) {

	char *addr;

	switch (params[0]) {

	case PUTS:
		addr = (char*)params[1];
		puthex(*(u32*)params[1]);
		uart_spin_puts(addr);
		break;
	case 2:
		puthex(params[1]);
		break;
	default:
		break;
	}
}