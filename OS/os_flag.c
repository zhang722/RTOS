#include "os.h"
#include "os_flag.h"

extern int OSLock(void);	
extern void OSUnlock(int key);

extern OSTcb *OSTCBCurPtr;
extern OSTcb *OSTCBNextPtr;


void OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt) 
{
	uint32 flagRdy;
	uint32 taskFlag;
	
	int a = OSLock();
	
	pflag->flag |= flag;
	for(int i = 0;i < TASK_NUM;i++){			/* Check all tasks 												*/
		if(tasks[i].pflag == pflag) {				/* Check out tasks that waiting this flag */
			taskFlag = tasks[i].flag;
			flagRdy = (uint32)(pflag->flag & taskFlag);
			if(opt == OS_FLAG_SET_ALL) {			/* If need to set all bits 								*/
				if(flagRdy == taskFlag) {				/* Match 																	*/
					OSPrioInsert(tasks[i].prio);	/* Ready to run 													*/
					tasks[i].state = OS_READY;
				}
			}
			else {														/* It is enough if any bit set						*/
				if(flagRdy != (uint32)0) {			/* Match 																	*/
					OSPrioInsert(tasks[i].prio);	/* Ready to run 													*/
					tasks[i].state = OS_READY;
				}
			}			
		}

	}
	
	OSUnlock(a);
}


void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt) 
{
	int a = OSLock();
	OSTCBCurPtr->pflag = pflag;											/* Save 								*/
	OSTCBCurPtr->flag = flag;
	
	if((pflag->flag & flag) == flag) {							/* Match flag 					*/
		if(opt == OS_FLAG_GET_CLR) {			
			pflag->flag &= ~(OSTCBCurPtr->pflag->flag);	/* Clear bits after get */
		} 	
		
		OSUnlock(a);
		return;
	} else {																				/* Not match 						*/
		OSPrioRemove(OSTCBCurPtr->prio);							/* Block task 					*/
		OSTCBCurPtr->state = OS_BLOCK;
		OSSched();																		/* Switch task 					*/
	}

	OSUnlock(a);
}

