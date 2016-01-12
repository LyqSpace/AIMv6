/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef SCHED_H
#define SCHED_H

#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>
#include "../mm/page_table_defines.h"
#include "../interrupt/mode_switch.h"

typedef struct TypeContext {
    uint r[13];
    uint sp,lr,spsr,pc;
} TypeContext;

#define STATE_UNUSED 		0
#define STATE_RUNNING 		1
#define STATE_WAITING		2
#define STATE_READY 		3

typedef struct TypePCB {
	uint state;
	uint TTB_addr;
	uint pid;
	uint pcb_id;
	char name[20];
	TypeContext context;

} TypePCB;

#define MAX_PROC 100
extern TypePCB PCBs[MAX_PROC];
extern int list_waiting[MAX_PROC];
extern int list_running[MAX_PROC];
extern int list_ready[MAX_PROC];
extern uint pid_count;
extern uint list_waiting_front;
extern uint list_waiting_rear;
extern uint list_running_front;
extern uint list_running_rear;
extern uint list_ready_front;
extern uint list_ready_rear;

void scheduler_init();
void enter_scheduler();
uint create_process(char*);
uint get_emptyPCB();

#endif