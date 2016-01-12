/*
 * Copyright (C) 2015 Yongqing Liang <root@lyq.me>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "run.h"

static TypeRun *freeMM_head = NULL;

void mm_init() {

	freeMM_head = (TypeRun*)MM_ADDR_START;
	freeMM_head->size = MM_ADDR_END - MM_ADDR_START;
	freeMM_head->next = NULL;
}

char* kalloc(uint count) {

	if (count == 0) return NULL;

	uint size = count * base_size;
	char *addr = NULL;
	TypeRun *cur_run = freeMM_head;
	TypeRun *pre_run;

	while (cur_run != NULL) {

		if (cur_run->size < size) {
			
			pre_run = cur_run;	
			cur_run = cur_run->next;

		} else if (cur_run->size == size) {
			
			addr = (char*)cur_run;
			if (cur_run == freeMM_head) {
				freeMM_head = cur_run->next;
			} else {
				pre_run->next = cur_run->next;
			}
			break;

		} else {

			addr = (char*)cur_run;
			uint rest_size = cur_run->size - size;
			uint rest_addr = (uint)cur_run + size;
			TypeRun *rest_run = (TypeRun*)rest_addr;
			rest_run->size = rest_size;
			rest_run->next = cur_run->next;
			if (cur_run == freeMM_head) {
				freeMM_head = rest_run;
			} else {
				pre_run->next = rest_run;
			}
			break;

		}
	}

	return addr;

}

int kfree(char *addr, uint count) {

	if (count == 0) return -1;
	
	uint size = count * base_size;
	TypeRun *cur_run = freeMM_head;
	TypeRun *free_run = (TypeRun*)addr;
	free_run->size = size;

	if (free_run < freeMM_head) {
		
		free_run->next = freeMM_head;
		if (free_run + free_run->size >= free_run->next) {
			free_run->size = freeMM_head->size + (uint)freeMM_head - (uint)free_run;
			free_run->next = freeMM_head->next;
		}
		freeMM_head = free_run;
	} else {

		TypeRun *pre_run = NULL;
		while (cur_run < free_run && cur_run != NULL) {
			pre_run = cur_run;
			cur_run = cur_run->next;
		}
		free_run->next = cur_run;
		if (cur_run != NULL && free_run + free_run->size >= cur_run) {
			free_run->size = cur_run->size + (uint)cur_run - (uint)free_run;
			free_run->next = cur_run->next;
		}

		if (pre_run + pre_run->size == free_run) {
			pre_run->size += free_run->size;
			pre_run->next = free_run->next;
		} else {
			pre_run->next = free_run;
		}
	}

	return 0;
	
}