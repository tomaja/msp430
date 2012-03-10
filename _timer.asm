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
				pop.w r4
				pop.w r5
				pop.w r6
				pop.w r7
				pop.w r8
				pop.w r9
				pop.w r10
				pop.w r11
				pop.w r12
				pop.w r13
				pop.w r14
				pop.w r15
			.endm
; *************************************************************************
				.data
TimerCounter	.word 0
; *************************************************************************
			.text
			.global kermod
			.global systimer
			.global Reschedule
			.global TimerCounter

systimer:	inc.w	&TimerCounter	; Increment system clock
			cmp.w	#0, &kermod		; Check interrupted process mode (kernel or user)
			jne		exit			; Exit if kernel mode is active
			saveGPR
			mov.w	SP, r12
			call	#Reschedule
			mov.w	r12, SP
			restoreGPR
exit:		ret

			.sect   ".int09"                ; MSP430 Timer0 Vector
			.short  systimer

            .end
