#include "os.h"
#include "os_flag.h"

extern int OSLock(void);	
extern void OSUnlock(int key);

extern OSTcb *OSTCBCurPtr;
extern OSTcb *OSTCBNextPtr;


uint32 OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt) 
{
	uint32 flagRdy;
	uint32 taskFlag;
	
	int a = OSLock();
	
	pflag->flag |= flag;
	for(int i = 0;i < TASK_NUM;i++){
		if(tasks[i].pflag == pflag) {
			taskFlag = tasks[i].pflag->flag;
			flagRdy = (uint32)(pflag->flag & taskFlag);
			if(opt == OS_FLAG_SET_ALL) {	/* If need to set all bits */
				if(flagRdy == taskFlag) {
					OSPrioInsert(tasks[i].prio);	/* Ready to run */
					tasks[i].state = OS_READY;
				}
			}
			else {	/* It is enough if any bit set*/
				if(flagRdy != (uint32)0) {
					OSPrioInsert(tasks[i].prio);	/* Ready to run */
					tasks[i].state = OS_READY;
				}
			}			
		}

	}
	
	OSUnlock(a);
	
	return pflag->flag;
}


void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt) 
{
	int a = OSLock();
	OSTCBCurPtr->pflag = pflag;	/* Save */
	
	if((pflag->flag & flag) == flag) {	/* Match flag */
		if(opt == OS_FLAG_CONSUME) {
			pflag->flag &= ~(OSTCBCurPtr->pflag->flag);
		} 	
		
		OSUnlock(a);
		return;
	} else {	/* Not match */
		OSPrioRemove(OSTCBCurPtr->prio);	/* Block task */
		OSTCBCurPtr->state = OS_BLOCK;
		OSSched();
	}

	OSUnlock(a);
}

