ENTRY(kernel_entry)

MEMORY
{
	KERNEL (rwx) :
		ORIGIN = 0x700000,
		LENGTH = 16M
}

SECTIONS
{
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.vector : {
		*vector.o(.text)
	}
	.rodata : {
		*(.rodata)
	}
	.bss : {
		*(.bss)
	}
}