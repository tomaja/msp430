/*
 * main.c
 */
#include "msp430.h"
#include "stdprocfunc.h"
#include "timer.h"
/*
 *
 */
void SecondProcess(void)
{
//	Prolog();

	while(1)
	{
		P1OUT ^= BIT6;
//		Read(NILLPID, 100, 0, 0);
	}

//	Epilog();
}
/*
 *
 */
void FirstProcess(void)
{
//	Prolog();

	CreateProcess(SecondProcess);
	while(1)
	{

		P1OUT ^= BIT0;
//		Read(NILLPID, 300, 0, 0);
	}


//	Epilog();
}

extern int kermod;


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1OUT = 0;
	P1DIR |= BIT0 + BIT6;
	kermod = 0;
	InitSystemTimer();
	CreateProcess(FirstProcess);
	while(1);
//	__bis_SR_register(LPM0);
}



