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
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>
#include <drivers/sd/sd-zynq7000.h>
#include "kern/mm/run.h"
#include "kern/mm/page_table_defines.h"
#include "kern/sched/sched.h"
#include "kern/interrupt/mode_switch.h"
#include "kern/interrupt/interrupt.h"
#include "kern/interrupt/system_call.h"

void load_shell();