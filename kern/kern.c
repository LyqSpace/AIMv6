/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <drivers/serial/uart.h>
#include "mm/page_table_defines.h"
#include "mm/run.h"
#include "interrupt/interrupt.h"

void kernel_entry(void) {

	uart_spin_puts("KERN: Here is Kernel!\r\n");

	vm_init();
	uart_spin_puts("KERN: Enable MMU successfully.\r\n");

	mm_init();
	print_cpsr();
	interrupt_init();
	print_cpsr();

	while (1);
}