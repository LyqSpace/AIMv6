#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include <config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <drivers/serial/uart.h>
#include <drivers/serial/puthex.h>


#define PUTS 0x1

void puts(char*);
void system_call(uint*);

#endif