/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>

#define SYS_STACK 0X1D000000
#define IRQ_STACK 0x1C000000
#define KERNEL_SPACE 0x80000000

void interrupt_init();
void C_SVC_handler(uint, uint*);
void C_prefetch_abort_handler();
void C_data_abort_handler();
void C_IRQ_handler();
void print_spsr();
void print_cpsr();

void system_call(uint*);

void enter_SVC_mode();
void enter_SYS_mode();
void enter_IRQ_mode();
void enter_USER_mode();
void enable_IRQ_mode();
void disable_IRQ_mode();

#endif