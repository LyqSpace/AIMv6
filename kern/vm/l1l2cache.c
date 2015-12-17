#include "l1l2cache.h"

void disable_l1cache() {
	asm volatile (
		"mov r0, #0;"
		"MCR P15, 0, R0, C8, C7, 0;" // invalidate TLBs
		"MCR P15, 0, R0, C7, C5, 0;" // invalidate I-cache
		"MCR P15, 0, R0, C7, C5, 6;" // invalidate branch predictor array
		"MCR P15, 0, R11, C7, C14, 2;" // invalidate D-cache
		"DSB" // ensure completion of the l1 inval
		:
		:
		:
	);
}

void enable_l1cache() {
	asm volatile (
		"MCR P15, 0, R0, C1, C0, 0;" // enable I-cache, D-cache
		"DSB;" // allow MMU to start
		"ISB;" // flush pre-fetch buffer
		:
		:
		:
	);
}

void disable_l2cache() {
	// -1.
	u32 *L2CC_ENABLE = (u32*)(L2CC_BASE_ADDR + 0x100);
	*L2CC_ENABLE = 0;

	// 0.
	u32 *unknown_r = (u32*)0xF8000A1C; // unknown system level control register, UG585 1585
	*unknown_r = 0x020202; // UG585 102

	// 1.
	// set latencies of RAM accesses
	u32 *L2CC_TAG_RAM = L2CC_BASE_ADDR + 0x108;
	u32 *L2CC_DATA_RAM = L2CC_BASE_ADDR + 0x10C;
	*L2CC_TAG_RAM |= 0b000100010001; // set the latencies for the 3 tag RAM attributes to 2
	*L2CC_DATA_RAM |= 0b000100100001; // set the write access and setup latencies of the data RAM to 2
									  // and set the read access latency of the data RAM to 3
	// 2.
	u32 *L2CC_INV_WAY = (u32*)(L2CC_BASE_ADDR + 0x77C);
	*(L2CC_INV_WAY) = 0xFFFF; // invalidate all entries l2 cache

	u32 *L2CC_CACHE_SYNC = (u32*)(L2CC_BASE_ADDR + 0x730);
	while (*L2CC_CACHE_SYNC); // ensure completion of the L2 inval
}

void enable_l2cache() {
	u32 *L2CC_ENABLE = (u32*)(L2CC_BASE_ADDR + 0x100);
	*L2CC_ENABLE = 1;
}