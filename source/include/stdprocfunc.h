/*
 * stdprocfunc.h
 *
 *  Created on: 24.02.2012
 *      Author: Vasily
 */

#ifndef STDPROCFUNC_H_
#define STDPROCFUNC_H_

typedef void (*ptrFunction)(void);

void Prolog();
void Epilog();
int CreateProcess(ptrFunction ptrFunc);
int DestroyProcess();
int SuspendProcess(unsigned int SleepTime);



#endif /* STDPROCFUNC_H_ */
