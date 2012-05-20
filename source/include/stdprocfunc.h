/*
 * stdprocfunc.h
 *
 *  Created on: 24.02.2012
 *      Author: Vasily
 */

#ifndef STDPROCFUNC_H_
#define STDPROCFUNC_H_

#define MAXPID				100


typedef void (*ptrFunction)(void);

///*
// * Structure describe sending/receiving data size and location
// */
//struct SIPCData
//{
//	unsigned int Size;
//	void *pData;
//};


void Prolog();
void Epilog();
int CreateProcess(ptrFunction ptrFunc);
int DestroyProcess(unsigned int PID, int Flags);
int Read(unsigned int SrcPID, unsigned int Timeout, unsigned int Size, void *pData);
int Send(unsigned int DstPID, unsigned int Timeout, unsigned int Size, void *pData);



#endif /* STDPROCFUNC_H_ */
