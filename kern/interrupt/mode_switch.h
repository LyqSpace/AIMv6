#ifndef MODE_SWITCH_H
#define MODE_SWITCH_H

#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>
#include "system_call.h"

void enter_SVC_mode();
void enter_SYS_mode();
void enter_IRQ_mode();
void enter_USER_mode();
void enable_IRQ_mode();
void disable_IRQ_mode();

void get_cur_mode();

#endif