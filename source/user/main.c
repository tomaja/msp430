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
//	for(; i < 1000;++i)
	while(1)
	{
	//	P1OUT ^= BIT6;
		SuspendProcess(300);
	}

	Epilog();
}

void FirstProcess(void)
{
	Prolog();

//	CreateProcess(SecondProcess);

	int i = 0;
//	for(;i < 10000; ++i)
	while(1)
	{
	//	P1OUT ^= BIT0;
	;//	SuspendProcess(100);
	}


	Epilog();
}



void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	DCOCTL = 0x0;
	BCSCTL2 |= DIVM_3;
	P1OUT = 0;
	P1DIR |= BIT0 + BIT6;

	InitSystemTimer();
//	CreateProcess(FirstProcess);

//	while(1)
//	{
//		;
//		P1OUT ^= BIT6 + BIT0;
//	}
//	P1OUT = BIT0 + BIT6;
	P1OUT = BIT0;
	__bis_SR_register(LPM0);
}



