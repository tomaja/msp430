/*
 * procmanage.c
 *
 *  Created on: 14.02.2012
 *      Author: Vasily
 */

#include "msp430g2553.h"
#include "syscall.h"
#include "stddefs.h"
#include "procmanage.h"
#include "intrmanage.h"


struct SProcess				Process[MAXPROCESSCOUNT];
static int ProcCount 		= 0;	// Количество процессов в системе
static int ProcCurrent 		= 0;	// Идентификатор текущего процесса
static char NeedReschedule	= 0;	// Флаг необходимости перепланирования процессов
extern int TimerLow;
extern int TimerHigh;
extern int retval;

/*
 * Initialize by default process' describing structures
 */
void InitializeProcessArray()
{
	int Cur = 1;
	for(; Cur < MAXPROCESSCOUNT; ++Cur)
	{
			Process[Cur].ProcState = zombie;
			Process[Cur].ptrFunction = 0;
			Process[Cur].ptrStack = 0;
			Process[Cur].RetVal = 0;
			Process[Cur].WaitingProp.DeadlineLow = 0;
			Process[Cur].WaitingProp.DeadlineHigh = 0;
//			Process[Cur].WaitingProp.Error = 0;
			Process[Cur].WaitingProp.PID = 0;
			Process[Cur].WaitingProp.Size = 0;
			Process[Cur].WaitingProp.pData = 0;
	}
}
/*
 * Create new user process
 * @param ptrFunction is a pointer to process function
 * @return operation result
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
 * Destroy user process
 * @param ptrData is a packed PID of destroying process
 * @return operation result
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

	if(ProcCurrent == SrcPID)
	{
		Process[ProcCurrent].RetVal = READ_ERROR_CUR_EQ_SRC;
		return 0;
	}

	// If current process waits data from any process
	// ONLY FOR RECEIVING !!!!
	if(SrcPID == ANYPID)
	{
		unsigned int i = 0;

		// Find first process which wants to send data to current process
		for(; i < MAXPROCESSCOUNT; ++i)
			if(Process[i].ProcState == sendlock && Process[i].WaitingProp.PID == ProcCurrent)
			{
				SrcPID = i;
				break;
			}
	}

//	if(!IsLocalPID(SrcPID))
//		DIPCMgr();
	if(SrcPID - 100 >= 0 && SrcPID - 100 < HWINTRCOUNT)
	{
//		Check interrupt state
//		if interrupt was initiated (interrupt flag is active and interrupt disabled now)
//			Enable interrupt
//			Process[ProcCurrent].RetVal = 0;
//			return 0;
		;
	}
	// If sending process is already waiting for receiving process
	else if(SrcPID != NILLPID && Process[SrcPID].ProcState == sendlock)
	{
		// If sending process can wait yet
		if(Process[SrcPID].WaitingProp.DeadlineHigh <= TimerHigh &&
		   Process[SrcPID].WaitingProp.DeadlineLow <= TimerLow) // Src side time isn't over
		{
			// If sending process is waiting for current process
			if(Process[SrcPID].WaitingProp.PID == ProcCurrent
					/*|| Process[SrcPID].WaitingProp.PID == ANYPID*/ // Only for receiving but not sending
					)
			{
				Process[SrcPID].ProcState = ready;
				int i = 0;
				void *pSrcData = Process[SrcPID].WaitingProp.pData;
				for(; i < Size; ++i)
					((char*)pIPCData)[i] = ((char*)pSrcData)[i];
				Process[ProcCurrent].RetVal = 0;
				return 0;
			}
		}
		else // Src side time is over
		{
			Process[SrcPID].RetVal = ERROR_TIMEOUT;
		}
	}

	if(Timeout == 0)
	{
		Process[ProcCurrent].RetVal = ERROR_TIMEOUT;
		return 0;
	}

	Process[ProcCurrent].ProcState = readlock;
	Process[ProcCurrent].WaitingProp.DeadlineHigh = (TimerLow + Timeout) / 65000;
	Process[ProcCurrent].WaitingProp.DeadlineLow =  (TimerLow + Timeout) % 65000;
	Process[ProcCurrent].WaitingProp.PID = SrcPID;
	Process[ProcCurrent].WaitingProp.Size = Size;
	Process[ProcCurrent].WaitingProp.pData = pIPCData;
	Process[ProcCurrent].RetVal = 0;
	NeedReschedule = 1;

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

	if(ProcCurrent == DstPID)
	{
		Process[ProcCurrent].RetVal = SEND_ERROR_CUR_EQ_DST;
		return 0;
	}

//	if(!IsLocalPID(SrcPID))
//		DIPCMgr();

	// If receiving process is already waiting for sending process
	if(DstPID != NILLPID && Process[DstPID].ProcState == readlock)
	{
		// If receiving process can wait yet
		if(Process[DstPID].WaitingProp.DeadlineHigh <= TimerHigh &&
		   Process[DstPID].WaitingProp.DeadlineLow <= TimerLow) // Dst side time isn't over
		{
			// If sending process is waiting for current process
			if(Process[DstPID].WaitingProp.PID == ProcCurrent || Process[DstPID].WaitingProp.PID == ANYPID)
			{
				Process[DstPID].ProcState = ready;
				int i = 0;
				void *pDtsData = Process[DstPID].WaitingProp.pData;
				for(; i < Size; ++i)
					 ((char*)pDtsData)[i] = ((char*)pIPCData)[i];
				Process[ProcCurrent].RetVal = 0;
				return 0;
			}
		}
		else // Dst side time is over
		{
			Process[DstPID].RetVal = ERROR_TIMEOUT;
		}
	}

	if(Timeout == 0)
	{
		Process[ProcCurrent].RetVal = ERROR_TIMEOUT;
		return 0;
	}

	Process[ProcCurrent].ProcState = sendlock;
	Process[ProcCurrent].WaitingProp.DeadlineHigh = (TimerLow + Timeout) / 65000;
	Process[ProcCurrent].WaitingProp.DeadlineLow =  (TimerLow + Timeout) % 65000;
	Process[ProcCurrent].WaitingProp.PID = DstPID;
	Process[ProcCurrent].WaitingProp.Size = Size;
	Process[ProcCurrent].WaitingProp.pData = pIPCData;
	Process[ProcCurrent].RetVal = 0;

	NeedReschedule = 1;

	return 0;
}
/*
 *
 */
int* Reschedule(int *ptrStack)
{
	// Если процессов в системе нет еще, то вернуться из функции
	if(ProcCount == 1 && ProcCurrent == 0)
		return Process[ProcCurrent].ptrStack;

	// Проверить состояния всех процессов в системе и изменить если требуется
	int Current = 0;
	for(; Current < MAXPROCESSCOUNT - 1; ++Current)
	{
		if(Process[Current].ProcState == readlock || Process[Current].ProcState == sendlock)
			if(Process[Current].WaitingProp.DeadlineHigh < TimerHigh ||
				(Process[Current].WaitingProp.DeadlineHigh == TimerHigh &&
				 Process[Current].WaitingProp.DeadlineLow < TimerLow))
			{
				Process[Current].ProcState = ready;
				Process[Current].RetVal = ERROR_TIMEOUT;
				NeedReschedule = 1;
			}

	}

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

	// Переключить процесс если пришло время или установлен флаг необходимости перепланирования
	if(TimerLow % 5000 == 0 || NeedReschedule == 1)
	{
		if(TimerLow == 65000)
			TimerLow = 0;
		int OldProc = ProcCurrent;					// Номер текущего процесса
		Process[ProcCurrent].ptrStack = ptrStack;	// Сохраняем стек текущего процесса
		++ProcCurrent;								// Делаем текущим процессом следующий
		ProcCurrent %= 3;							// Список процессов круговой
		// Выбираем следующий процесс из готовых пока не пройдем весь список
		for(;Process[ProcCurrent].ProcState != ready && ProcCurrent != OldProc; ++ProcCurrent, ProcCurrent %= MAXPROCESSCOUNT);
		NeedReschedule = 0;
		return Process[ProcCurrent].ptrStack;
	}
	retval = Process[ProcCurrent].RetVal;
	return ptrStack;
}

