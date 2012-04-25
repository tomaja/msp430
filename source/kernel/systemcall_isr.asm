saveGPR		.macro
				push.w r4	; instead saving SR
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
				pop.w r4	; instead loading SR
			.endm
; *************************************************************************
			.data
kermod		.word 0
; *************************************************************************
			.text
			.global kermod
			.global Action
			.global Reschedule
			.global syscall

syscall:
			dint
									; stack includes return address
									; r12 includes syscal type
									; r13 includes syscall data
			mov.w	#0x1, &kermod 	; set kernel mode to active
			saveGPR
			call 	#Action
			mov.w	SP, r12
			call	#Reschedule
			mov.w	r12, SP
			restoreGPR
			mov.w	#0x0, &kermod	; set kernel mode to inactive
			eint
			ret ;???????
		.end
