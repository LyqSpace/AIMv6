/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef MM_H
#define MM_H

#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>
#include "page_table_defines.h"

#define base_size 0x100000
#define MM_ADDR_START (0x00900000+KERNEL_SPACE)
#define MM_ADDR_END (0x1F000000+KERNEL_SPACE)

typedef struct TypeRun {
	uint size;
	struct TypeRun *next;
} TypeRun;

void mm_init();
char* kalloc(uint);
int kfree(char*, uint);

#endif