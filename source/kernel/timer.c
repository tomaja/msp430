/*
 * timer.c
 *
 *  Created on: 21.02.2012
 *      Author: Vasily
 */

#include "timer.h"
#include "syscall.h"
#include  "msp430g2553.h"

/**
 *
 */
void InitSystemTimer()
{
	DCOCTL = DCO2;							// ������������� ������� ���������
	BCSCTL1 = XT2OFF + RSEL3 + RSEL2;		// ������������ ������� �� 1 ���
	extern int TimerCounter;
	TimerCounter = 0;
	TA0CCTL0 |= CCIE;						// Enable timer interrupt
	TACCR0 = 110; 							// Set upper border of timer counter
											// Timer interrupt frequency is about 10KHz
	TA0CTL |= MC_1 + TASSEL_2 + ID_1;		// Set up timer in up count mode
	__bis_SR_register(GIE);					// Enable global interrupts
}

/**
 *
 */
void StopSystemTimer()
{
	TA0CCTL0 &= ~CCIE;
}
