/*
 * main.c
 */
#include "msp430.h"
#include "stdprocfunc.h"
#include "timer.h"

void SecondProcess(void)
{
	Prolog();

	int i = 0;;
	for(; i < 1000;++i)
	{
		P1OUT ^= BIT6;
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
		P1OUT = BIT0;
	}


	Epilog();
}



void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1OUT = 0;
	P1DIR |= BIT0 + BIT6;


	CreateProcess(FirstProcess);

	InitSystemTimer();

	P1OUT = BIT0 + BIT6;

	__bis_SR_register(LPM0);
}



