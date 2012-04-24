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
		KernelCreateProcess((ptrFunction)ptrData);
		break;
	case destroyproc:
		KernelDestroyProcess();
		break;
	case suspendproc:
		KernelSuspendProcess((unsigned int)ptrData);
		break;
	default:
		return -1;
	}

	return 0;
}

