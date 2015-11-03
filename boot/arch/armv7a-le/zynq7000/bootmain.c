/*
 * Copyright (C) 2015 David Gao <davidgao1001@gmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <config.h>

#define PRELOAD_VECTOR_BASE	0x1ff00000

void (*uart_init)(void) = (void *)(PRELOAD_VECTOR_BASE + 0x4);
void (*uart_enable)(void) = (void *)(PRELOAD_VECTOR_BASE + 0x8);
void (*uart_spin_puts)(const char *) = (void *)(PRELOAD_VECTOR_BASE + 0xC);
void (*sd_dma_spin_write)(u32, u16, u32) = (void*)(PRELOAD_VECTOR_BASE + 0x10);

void mbr_bootmain(void)
{
	int ret;
	volatile u8 *my_kernel = (void *)0x200000;

	uart_spin_puts("Hello, AIMv6!\r\n");
	ret = sd_dma_spin_read((u32)my_kernel, 1, 0);
	if (ret == 0) {
		uart_spin_puts("FW: MY_KERNEL read OK.\r\n");
	} else {
		uart_spin_puts("FW: MY_KERNEL read failed.\r\n");
		while(1);
	}
	while(1);
}

