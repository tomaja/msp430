/*
 * intrmanage.c
 *
 *  Created on: Jun 6, 2012
 *      Author: Administrator
 */

#include "intrmanage.h"
#include "procmanage.h"
#include "stddefs.h"


unsigned int InterrupHandlerPIDs[HWINTRCOUNT];
extern int TimerLow;
extern int TimerHigh;

/*
 * Initialize interrupt handler PIDs array
 */
void InitHWIntrHndls()
{
	int i = 0;
	for(;i < HWINTRCOUNT; ++i)
		InterrupHandlerPIDs[i] = 0xFF;
}

/*
 * Set association between hardware interrupt and handlers
 * @param InterruptNum	is a number of handling interrupt
 * @param PID	is a process identifier of interrupt handler
 */
int AssociateIntrHndlrToPID(unsigned int InterruptNum, unsigned int PID)
{
	if(InterruptNum >= HWINTRCOUNT)
		return ERROR_HWINTRASC_INVINTRNUM;
	if(InterrupHandlerPIDs[InterruptNum] == 0xFF)
		return ERROR_HWINTRASC_ALRDYSET;
	InterrupHandlerPIDs[InterruptNum] = PID;
	return 0;
}
extern struct SProcess	Process[MAXPROCESSCOUNT];
/*
 * Handle specified hardware interrupt
 * @param InterruptNum	is a number of handling interrupt
 */
int HWInterruptHandle(int InterrupNum, int KernelMode)
{
	if(InterrupHandlerPIDs[InterrupNum] == 0xFF)
		return 1;
	unsigned int PID = InterrupHandlerPIDs[InterrupNum];

	if(Process[PID].ProcState == readlock && Process[PID].WaitingProp.PID == InterrupNum + 100)
	{
		if(Process[PID].WaitingProp.DeadlineHigh < TimerHigh ||
		  (Process[PID].WaitingProp.DeadlineHigh == TimerHigh &&
		   Process[PID].WaitingProp.DeadlineLow < TimerLow))
			{
				Process[PID].ProcState = ready;
				Process[PID].RetVal = ERROR_TIMEOUT;
				return 1;
			}
//		Copy data to handler
//		Notice scheduler
//		Enable interrupt
	}
	return 0;

}
