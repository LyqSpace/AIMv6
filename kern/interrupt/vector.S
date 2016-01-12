.global interrupt_vector

.text
.arm

interrupt_vector:
	nop
	nop
	b SVC_handler
	b Prefetch_handler
	b Data_handler
	nop
	b IRQ_handler
	nop

SVC_handler:
	stmfd sp!, {r0-r12, lr}
	ldr r0, [lr, #-4]
	bic r0, r0, #0xFF000000
	mov r1, sp
	bl C_SVC_handler
	ldmfd sp!, {r0-r12, pc}^

Prefetch_handler:
	stmfd sp!, {r0-r12, lr}
	mov r0, sp
    bl C_Prefetch_handler
    ldmfd sp!, {r0-r12, pc}^

Data_handler:
	stmfd sp!, {r0-r12, lr}
	mov r0, sp
    bl C_Data_handler
    ldmfd sp!, {r0-r12, pc}^

IRQ_handler:
	stmfd sp!, {r0-r12, lr}
	bl C_IRQ_handler
	ldmfd sp!, {r0-r12, pc}^