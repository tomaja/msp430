/*
 * kprocmanage.h
 *
 *  Created on: 14.02.2012
 *      Author: Vasily
 */

#ifndef KPROCMANAGE_H_
#define KPROCMANAGE_H_

typedef void (*ptrFunction)(void);
int KernelCreateProcess(ptrFunction ptrFunc);
int KernelDestroyProcess(void *ptrData);
int KernelRead(void *ptrData);
int KerneSend(void *ptrData);
void Reschedule();


#endif /* KPROCMANAGE_H_ */
