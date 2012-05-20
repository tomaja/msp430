/*
 * main.c
 */
#include "msp430.h"
#include "stdprocfunc.h"
#include "timer.h"

void SecondProcess(void)
{
	Prolog();

	int i = 0;
	for(; i < 1000;++i)
	{
		P1OUT ^= BIT6;
		SuspendProcess(300);
	}

	Epilog();
}

void FirstProcess(void)
{
	Prolog();

	CreateProcess(SecondProcess);

	int i = 0;
	for(;i < 10000; ++i)
	{
		P1OUT ^= BIT0;
		SuspendProcess(100);
	}


	Epilog();
}

extern int kermod;


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1OUT = 0;
	P1DIR |= BIT0 + BIT6;
	kermod = 0;
	InitSystemTimer();
//	CreateProcess(FirstProcess);

	__bis_SR_register(LPM0);
}



