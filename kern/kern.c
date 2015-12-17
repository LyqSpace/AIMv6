#include <drivers/serial/uart.h>
#include "vm/page_table_defines.h"

void kernel_entry(void) {

	uart_spin_puts("KERN: Here is Kernel!\r\n");

	setup_vm();

	while (1);
}