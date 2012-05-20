/*
 * syscall.c
 *
 *  Created on: 14.02.2012
 *      Author: Vasily
 */

#include "syscall.h"
#include "kprocmanage.h"


/*
 *
 */
int Action(enum SSysCallType Type, void *ptrData)
{
	switch(Type)
	{
	case createproc:
		return KernelCreateProcess((ptrFunction)ptrData);
	case destroyproc:
		return KernelDestroyProcess(ptrData);
	case read:
		return KernelRead(ptrData);
	case send:
		return KernelSend(ptrData);
	default:
		return -1;
	}
}

