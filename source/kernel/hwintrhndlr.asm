saveGPR		.macro
				push.w r4
				push.w r5
				push.w r6
				push.w r7
				push.w r8
				push.w r9
				push.w r10
				push.w r11
				push.w r12
				push.w r13
				push.w r14
				push.w r15
			.endm

restoreGPR	.macro
				pop.w r15
				pop.w r14
				pop.w r13
				pop.w r12
				pop.w r11
				pop.w r10
				pop.w r9
				pop.w r8
				pop.w r7
				pop.w r6
				pop.w r5
				pop.w r4
			.endm
; *************************************************************************
			.data
			.global kermod
; *************************************************************************
			.text
			.global irq1
			.global HWInterruptHandle

irq1:
			saveGPR
			mov.w	#0x1, r12
			mov.w	#0x0, r13
			cmp.w	#0x0, &kermod
			jeq		norm
			mov.w	#0x1, r13
norm:		call	#HWInterruptHandle
			restoreGPR
			reti

			.sect   ".int01"
			.short  irq1

            .end
