/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

 #include "sched.h"

TypePCB PCBs[MAX_PROC];
int list_waiting[MAX_PROC];
int list_running[MAX_PROC];
int list_ready[MAX_PROC];

uint pid_count;
uint list_waiting_front;
uint list_waiting_rear;
uint list_running_front;
uint list_running_rear;
uint list_ready_front;
uint list_ready_rear;

void scheduler_init() {

 	for (int i = 0; i < MAX_PROC; i++) {
 		PCBs[i].state = STATE_UNUSED;
 		PCBs[i].pcb_id = i;
 		for (int j = 0; j < 13; j++) {
 			PCBs[i].context.r[j] = 0;
 		}
 		PCBs[i].context.sp = 0x1B000000;
 		PCBs[i].context.lr = 0;
 		PCBs[i].context.spsr = 0;
 		PCBs[i].context.pc = 0x0;
 	}
 	pid_count = 0;

 	list_waiting_front = 0;
	list_waiting_rear = 0;
	list_running_front = 0;
	list_running_rear = 0;
	list_ready_front = 0;
	list_ready_rear = 0;
}

uint get_emptyPCB() {
	
	for (uint i = 0; i < MAX_PROC; i++) {
		if (PCBs[i].state == STATE_UNUSED) {
			PCBs[i].state = STATE_WAITING;
			PCBs[i].pid = pid_count++;
			return i;
		}
	}
	PCBs[0].state = STATE_WAITING;
	PCBs[0].pid = pid_count++;
	return 0;
}

uint create_process(char *ttb_addr) {

	uint PCB_id = get_emptyPCB();
	PCBs[PCB_id].TTB_addr = (uint)ttb_addr;
	PCBs[PCB_id].state = STATE_READY;
	list_ready[list_ready_rear++] = PCB_id;
	return PCB_id;

}

void enter_scheduler() {

	uart_spin_puts("SCHED: Schedule first process.\r\n");

	uint PCB_id = list_ready[list_ready_front++];
	PCBs[PCB_id].state = STATE_RUNNING;

	asm volatile (
		"mov lr, %0;"
		"mov sp, %1;"
		:
		:"r"(PCBs[PCB_id].context.lr), "r"(PCBs[PCB_id].context.sp)
		:"lr","sp"
	);

	invalidate_TLBs_Caches();

	asm volatile (
		"mov r0, %0;"
        "mcr p15,0,r0,c2,c0,0;"
        "isb;"
        :
        :"r"(PCBs[PCB_id].TTB_addr)
        :"r0"
	);

	enter_USER_mode();

	asm volatile (
		"mov pc, %0;"
		:
		:"r"(PCBs[PCB_id].context.pc)
		:
	);

}