.global kernel_entry

kernel_entry:
	mrs r0, CPSR
	orr r0, #0xF
	msr CPSR, r0 
	ldr fp, =0xF00000
	mov sp, fp
	bl kernel_main