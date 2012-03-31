/*
 * syscall.h
 *
 *  Created on: 14.02.2012
 *      Author: Vasily
 */

#ifndef SYSCALL_H_
#define SYSCALL_H_

/*
 * Enumeration of possible sytemcall types
 */
enum SSysCallType
{
	createproc,
	destroyproc,
	suspendproc
};

extern void syscall(enum SSysCallType Type, void *ptrData);
/*
 *
 */
inline void SystemCall(enum SSysCallType Type, void *ptrData)
{
	syscall(Type, ptrData);
}

#endif /* SYSCALL_H_ */
