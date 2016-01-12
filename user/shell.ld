ENTRY(shell_main)

SECTIONS
{
	. = 0x900000;
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.rodata : {
		*(.rodata)
	}
	.bss : {
		*(.bss)
	}
}