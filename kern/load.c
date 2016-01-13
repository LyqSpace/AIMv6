/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "load.h"

void load_shell() {

	uart_spin_puts("LOAD: Loading Shell process.\r\n");

	char *shell_addr = kalloc(1);
	sd_dma_spin_read((uint)shell_addr - KERNEL_SPACE, 1, 0);
	char * addr = (char*)(shell_addr+0x1D6);
	uint part2_offset = addr[0] + (addr[1]<<8) + (addr[2]<<16) + (addr[3]<<24);
	for (int i = 0; i < 10; i++) {
    	sd_dma_spin_read((uint)shell_addr-KERNEL_SPACE+(i<<9), 1, part2_offset+10000+i);
    }

    char *ttb_addr = new_TTB();
    TypePte *ttb_first = (void*)ttb_addr;
    ttb_first->base = ((u32)shell_addr - KERNEL_SPACE) >> 20;
	
    create_process(ttb_addr);

    return;

}