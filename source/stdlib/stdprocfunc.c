/*
 * stdprocfunc.c
 *
 *  Created on: 24.02.2012
 *      Author: Vasily
 */
#include "stdprocfunc.h"
#include "syscall.h"

/*
 *
 */
void Prolog()
{
//	Initialize process specific data
}
/*
 *
 */
void Epilog()
{
	//SystemCall(destroyproc, 0);
	syscall(destroyproc, 0);
}

/*
 *
 */
int CreateProcess(ptrFunction ptrFunc)
{
	//SystemCall(createproc, (void*)ptrFunction);
	syscall(createproc, ptrFunc);
	return 0;
}

int SuspendProcess(unsigned int SleepTime)
{
	syscall(suspendproc, &SleepTime);
	return 0;
}
