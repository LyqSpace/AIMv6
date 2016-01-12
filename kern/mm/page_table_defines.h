/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef PAGE_TABLE_DEFINES_H
#define PAGE_TABLE_DEFINES_H

#define PAGE_TABLE_ADDRESS		0x1F000000
#define KERNEL_SPACE			0x80000000
#define DEVICE_SPACE			0xE0000000
#define KERNEL_STACK_SPACE		0xDE000000
#define KERNEL_STACK_PHY		0x1E000000
#define MEM_SECTION_SIZE		0x4 // 4 Bytes
#define MEM_SECTION_SHIFT		20
#define KERNEL_MEM_SIZE			0x200
#define DEVICE_MEM_SIZE			0x100
#define KERNEL_STACK_MEM_SIZE	0x80

typedef struct TypePte {
	unsigned type : 2;
	unsigned B : 1;
	unsigned C : 1;
	unsigned XN : 1;
	unsigned Domain : 4;
	unsigned zero_L : 1;
	unsigned AP_L : 2;
	unsigned TEX : 3;
	unsigned AP_H : 1;
	unsigned S : 1;
	unsigned ng : 1;
	unsigned zero_H : 1;
	unsigned NS : 1;
	unsigned base : 12;	
} TypePte;

void invalidate_TLBs_Caches();
void setup_TTB();
void setup_Domain_Access();
void enable_MMU();

void vm_init();


#endif