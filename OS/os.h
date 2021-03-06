#ifndef OS_H
#define OS_H

#include "os_types.h"

#define OS_TICK_HZ   1000

#define MAX_TASK_NUM 32							    /* Max number of tasks 						 		  */
#define MAX_PRIO     (MAX_TASK_NUM-1)		/* Max prio 											 		  */

#define IDLE_SIZE    256								/* Default stack size of idle task 		  */


enum{
	OS_SUSPEND = 0,
	OS_BLOCK,
	OS_READY
};

typedef void(*ptask)(void);	/* Rename pointer to function to ptask */


/* ---------------From os_asm.asm----------------- */	
extern OSTcb *OSTCBCurPtr;
extern OSTcb *OSTCBNextPtr;
extern uint32 CPU_CntLeadZeros(uint32 a);	
extern int OSLock(void);	
extern void OSUnlock(int key);
extern void OSStart(void);
extern void OSContextSwitch(void);
extern void OSPendSV_Handler(void);				


/* ------------Functions operate prio------------- */																				
void OSPrioInsert(uint32 prio);		
void OSPrioRemove(uint32 prio);
static uint32 OSPrioGetHighest(void); 		


/* ---------------Schedule tasks------------------ */	
void OSSched(void);								


/* -----------------Stack init-------------------- */	
static void OSTaskStkInit(OSTcb *tcb, ptask task, uint32 *stk);


/* -----------About idle and statistic task------- */	
static void OSTaskIdle(void);						
static void OSIdleInit(void);						


/* -----------Functions extern to user------------ */	
void OSTimeTick(void);										
void OSDelay(uint32 ticks);
void OSInit(void);
void OSTaskSuspend(OSTcb * ptcb);
void OSTaskResume(OSTcb * ptcb);

#endif

