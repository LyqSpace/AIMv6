#ifndef L1_L2_CACHE
#define L1_L2_CACHE

#include <config.h>

#define L2CC_BASE_ADDR (void*)0xF8F02000

void disable_l1cache();
void enable_l1cache();

void disable_l2cache();
void enable_l2cache();

#endif