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

int DestroyProcess(unsigned int PID, int Flags)
{
	unsigned int Data[2];
	Data[0] = PID;
	Data[1] = Flags;
	return syscall(destroyproc, (void*)&Data);
}

int Read(unsigned int SrcPID, unsigned int Timeout, unsigned int Size, void *pData)
{
	unsigned int Data[4];
	Data[0] = SrcPID;
	Data[1] = Timeout;
	Data[2] = Size;
	Data[3] = (unsigned int)pData;

	return syscall(destroyproc, (void*)&Data);
}
int Send(unsigned int DstPID, unsigned int Timeout, unsigned int Size, void *pData)
{
	unsigned int Data[4];
	Data[0] = DstPID;
	Data[1] = Timeout;
	Data[2] = Size;
	Data[3] = (unsigned int)pData;
	return syscall(destroyproc, (void*)&Data);
}
