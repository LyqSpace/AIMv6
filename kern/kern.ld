ENTRY(kernel_entry)

MEMORY
{
	KERNEL (rwx) :
		ORIGIN = 0x700000,
		LENGTH = 10M
}

SECTIONS
{
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