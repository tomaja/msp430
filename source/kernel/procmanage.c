/*
 * procmanage.c
 *
 *  Created on: 14.02.2012
 *      Author: Vasily
 */

#include "msp430g2553.h"
#include "syscall.h"
#include "stddefs.h"



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
int BigTimerCounter;

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
	unsigned int CurrentPID;
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
		CurrentPID = 0;
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
		CurrentPID = Current;
	}
	NeedReschedule = 1;

	++ProcCount;
	return CurrentPID;
}

/*
 *
 */
int KernelDestroyProcess(void *ptrData)
{
	unsigned int PID = *(unsigned int*)ptrData;
	if(Process[PID].ProcState == zombie)
		return -1;
	if(PID >= MAXPROCESSCOUNT)
		return -2;
	if(PID == CURRENTPID)
		Process[ProcCurrent].ProcState = zombie;
	else
		Process[PID].ProcState = zombie;
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
int KernelRead(void *ptrData)
{
	unsigned int *pData = (unsigned int*)ptrData;
	unsigned int SrcPID = pData[0];
	unsigned int Timeout = pData[1];
	unsigned int Size = pData[2];
	void *pIPCData = (void*)pData[3];



	return 0;
}
/*
 *
 */
int KernelSend(void *ptrData)
{
	unsigned int *pData = (unsigned int*)ptrData;
	unsigned int DstPID = pData[0];
	unsigned int Timeout = pData[1];
	unsigned int Size = pData[2];
	void *pIPCData = (void*)pData[3];


	return 0;
}
/*
 *
 */
int* Reschedule(int *ptrStack)
{
	if(ProcCount == 1 && ProcCurrent == 0)
		return Process[ProcCurrent].ptrStack;


//	int Current = 0;
//	for(; Current < MAXPROCESSCOUNT - 1; ++Current)
//	{
//
//		Check all current process blocking data and set process state ready if it is possible
//	}

	/***************************************************************/
//	if(TimerCounter % 1000 == 0)// !!!!!! ONLY FOR TESTING
//	{
//		P1OUT ^= BIT0;
//		TimerCounter = 0;
//		return ptrStack;
//	}
//	else
//	{
//		P1OUT ^= BIT6;
//		return ptrStack;
//	}
	/***************************************************************/

	if(TimerCounter % 5000 == 0 || NeedReschedule == 1)
	{
		Process[ProcCurrent].ptrStack = ptrStack;
		++ProcCurrent;
		ProcCurrent %= 3;
		for(;Process[ProcCurrent].ProcState != ready; ++ProcCurrent, ProcCurrent %= MAXPROCESSCOUNT);
		NeedReschedule = 0;
		return Process[ProcCurrent].ptrStack;
	}
	return ptrStack;
}

