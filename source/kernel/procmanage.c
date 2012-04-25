/*
 * procmanage.c
 *
 *  Created on: 14.02.2012
 *      Author: Vasily
 */

#include "msp430g2553.h"
#include "syscall.h"



/*
 * Enumeration of possible process state
 */
enum EProcessState
{
	ready,
	run,
	lock,
	zombie
};
/*
 * Set of data which describes process entity
 */
struct SProcess
{
	void	(*ptrFunction)(void);
	enum	EProcessState ProcState;
	int 	*ptrStack;
};



//**********************************************//
#define MAXPROCESSCOUNT		3
#define FRAMESIZE			28
#define	FRAMEWORDS			14
#define STARTSTACK			0x3F8

struct SProcess				Process[MAXPROCESSCOUNT];
static int ProcCount 		= 0;	// Количество процессов в системе
static int ProcCurrent 		= 0;	// Идентификатор текущего процесса
static char NeedReschedule	= 0;	// Флаг необходимости перепланирования процессов
extern int TimerCounter;

/*
 *
 */

void InitializeProcessArray()
{
	int Cur = 1;
	for(; Cur < MAXPROCESSCOUNT; ++Cur)
	{
			Process[Cur].ProcState = zombie;
			Process[Cur].ptrFunction = 0;
			Process[Cur].ptrStack = 0;
	}
}
/*
 *
 */
int KernelCreateProcess(void (*ptrFunction)(void))
{
	if(ProcCount == MAXPROCESSCOUNT - 1)
		return -1;
	/*************************************************************************************/
	if(ProcCount == 0)
	{
		InitializeProcessArray();
		Process[0].ptrStack = (int*)(STARTSTACK - FRAMESIZE); // Initial stack address minus 26 words for frame and minus 2 words for PC to return back to syscall
		Process[0].ptrFunction = ptrFunction;
		Process[0].ProcState = ready;
		*(Process[0].ptrStack + FRAMEWORDS - 1) = (int)ptrFunction;//(int)ProcessFunction; // !!! Address of new process entry point, but not direct address of start function
	}
	else
	{
		int Current = 1;
		for(;Current < MAXPROCESSCOUNT && Process[Current].ProcState != zombie; ++Current);
		//	if(Process[Current].ProcState == zombie)
		//		break;

		Process[Current].ptrStack = Process[Current - 1].ptrStack - 0x30 - FRAMESIZE; // New stack's address minus 26 words for frame and minus 2 words for PC to return back to syscall
		Process[Current].ptrFunction = ptrFunction;
		Process[Current].ProcState = ready;
		*(Process[Current].ptrStack + FRAMEWORDS - 1) = (int)ptrFunction;//(int)ProcessFunction; // !!! Address of new process entry point, but not direct address of start function
		// Fill stack by zero
	}
	NeedReschedule = 1;
	++ProcCount;
	return 0;
}

/*
 *
 */
int KernelDestroyProcess()
{
	Process[ProcCurrent].ProcState = zombie;
	--ProcCount;
	NeedReschedule = 1;


	if(ProcCurrent == 0)
	{
		TA0CCTL0 &= ~CCIE;
//		Blinking by led1 and led2
		P1OUT |= BIT0 + BIT6;
		__bis_SR_register(LPM4);
	}
 	return 0;
}

/*
 *
 */
int KernelSuspendProcess(unsigned int *pSleepTime)
{
	int i = 0;
	for(; i < *pSleepTime * 50; ++i)
	{;}
	// Get current time value
	// Calculate wake up time related on pSleepTime
	// Change process state to suspended
	// Set Reschedule flag to enabled
	return 0;
}

/*
 *
 */
int* Reschedule(int *ptrStack)
{
//	if(ProcCount == 1 && ProcCurrent == 0)
//		return Process[ProcCurrent].ptrStack;

//	int Current = 0;
//	for(; Current < MAXPROCESSCOUNT - 1; ++Current)
//	{
//
//		Check all current process blocking data and set process state ready if it is possible
//	}

	/***************************************************************/
	//if( (*(int*)TimerCounter % 5) == 0)// !!!!!! ONLY FOR TESTING
	if(TimerCounter % 10000 == 0)
	{
		P1OUT ^= BIT0;	// !!!!!! ONLY FOR TESTING
		return ptrStack;
	}
	else
	{
		P1OUT ^= BIT6;	// !!!!!! ONLY FOR TESTING
		return ptrStack;
	}
	/***************************************************************/

//
//	if(TimerCounter % 20 == 0 || NeedReschedule == 1)
//	{
//		Process[ProcCurrent].ptrStack = ptrStack;
//		++ProcCurrent;
//		ProcCurrent %= 3;
//		for(;Process[ProcCurrent].ProcState != ready; ++ProcCurrent, ProcCurrent %= 3);
//		NeedReschedule = 0;
//		return Process[ProcCurrent].ptrStack;
//	}
//	return ptrStack;
}

