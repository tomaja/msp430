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
int KernelDestroyProcess();
void Reschedule();


#endif /* KPROCMANAGE_H_ */
