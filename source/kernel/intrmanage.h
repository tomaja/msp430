/*
 * intrmanage.h
 *
 *  Created on: Jun 6, 2012
 *      Author: Administrator
 */

#ifndef INTRMANAGE_H_
#define INTRMANAGE_H_

#define HWINTRCOUNT	3
#define ERROR_HWINTRASC_ALRDYSET	-20;
#define ERROR_HWINTRASC_INVINTRNUM	-21;


int AssociateIntrHndlrToPID(unsigned int InterruptNum, unsigned int PID);
void InitHWIntrHndls();


#endif /* INTRMANAGE_H_ */
