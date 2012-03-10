/*
 * main.c
 */
#include "msp430.h"
#include "stdprocfunc.h"
#include "timer.h"

void SecondProcess(void)
{
	Prolog();

	P1OUT |= BIT6;

	Epilog();
}

void FirstProcess(void)
{
	Prolog();

	P1OUT = BIT0;
	CreateProcess(SecondProcess);

	Epilog();
}



void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1OUT = 0;
	P1DIR |= BIT0 + BIT6;



//	CreateProcess(FirstProcess);

	InitSystemTimer();
	__bis_SR_register(LPM0);


}



