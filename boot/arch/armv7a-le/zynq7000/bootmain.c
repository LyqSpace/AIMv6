/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
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
int (*sd_dma_spin_read)(u32, u16, u32) = (void*)(PRELOAD_VECTOR_BASE + 0x10);
void (*puthex)(u32) = (void *)(PRELOAD_VECTOR_BASE + 0x14);

void mbr_bootmain(void)
{
	uart_spin_puts("MBR: Here is MBR!\r\n");

	/* Read MBR to Memory */
	volatile u8 *mbr_memAddr = (void *)0x200002;
	sd_dma_spin_read((u32)mbr_memAddr, 1, 0);
	
	/* Read ELF in Partition 2 to Memory */
	volatile u32 partition2_start = *(u32*)(mbr_memAddr + 0x1D6); // 470

	/* Read ELF header to Memory */
	volatile u8 *elfHeader_memAddr = (void *)0x300000;
	sd_dma_spin_read((u32)elfHeader_memAddr, 3, partition2_start);
	
	/* Read kernel file to Memory */
	void (*kernel_entry)(void) = (void *)(*(u32*)(elfHeader_memAddr + 0x18)); 
	u16 program_header_size = *(u16*)(elfHeader_memAddr + 0x2A);
	u16 program_header_num = *(u16*)(elfHeader_memAddr + 0x2C);
	u32 program_header = *(u32*)(elfHeader_memAddr + 0x1C) + (u32)elfHeader_memAddr;

	for (int i = 0; i < program_header_num; i++) {

		u32 p_offset = *(u32*)(program_header + 4);
		u32 p_paddr = *(u32*)(program_header + 12);
		u32 p_filesz = *(u32*)(program_header + 16);
		u16 p_count = (u16) (((p_paddr + p_filesz) >> 9) - (p_paddr >> 9) + 1);
		p_offset = p_offset >> 9;
		p_paddr = p_paddr >> 9 << 9; 
		
		for (int j = 0; j < p_count; j++) {
			sd_dma_spin_read((u32)p_paddr, 1, partition2_start + p_offset);
			p_paddr += (1 << 9);
			p_offset++;
		}
		program_header = program_header + (program_header_size >> 2);
	}
	
	uart_spin_puts("MBR: Read ELF finished!\r\n");
	
	kernel_entry();

	while (1);
}
