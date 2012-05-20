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
	read,
	send
};

extern int syscall(enum SSysCallType Type, void *ptrData);

#endif /* SYSCALL_H_ */
