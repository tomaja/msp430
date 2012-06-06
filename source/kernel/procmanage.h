/*
 * procmanage.h
 *
 *  Created on: Jun 6, 2012
 *      Author: Administrator
 */

#ifndef PROCMANAGE_H_
#define PROCMANAGE_H_

/*
 * Enumeration of possible process state
 */
enum EProcessState
{
	ready,
	run,
	readlock,
	sendlock,
	zombie
};

/*
 * Set of data which describe process which is waited
 */
struct SWaitingProp
{
	unsigned int	PID;
	void			*pData;
	unsigned int	Size;
	unsigned int	DeadlineLow;
	unsigned int	DeadlineHigh;
};
/*
 * Set of data which describes process entity
 */
struct SProcess
{
	void					(*ptrFunction)(void);
	enum					EProcessState ProcState;
	int 					*ptrStack;
	struct SWaitingProp		WaitingProp;
	int						RetVal;
};



//**********************************************//

#define MAXPROCESSCOUNT		3
#define FRAMESIZE			28
#define	FRAMEWORDS			14
#define STARTSTACK			0x3F8



#endif /* PROCMANAGE_H_ */
