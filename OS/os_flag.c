#include "os_flag.h"

extern int OSLock(void);	
extern void OSUnlock(int key);

extern OSTcb *OSTCBCurPtr;
extern OSTcb *OSTCBNextPtr;

void OSFlagCreate(OSFlag * pflag, uint32 flag) 
{
	int a = OSLock();
	
	pflag->flag = flag;
	
	OSUnlock(a);
}



uint32 OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt) 
{
	int a = OSLock();
	uint32 flagRdy;
	uint32 taskFlag;
	pflag->flag |= flag;
	for(int i = 0;i < TASK_NUM;i++) {
		taskFlag = tasks[i].pflag->flag;
		flagRdy = (uint32)(pflag->flag & taskFlag);
		if(opt == OS_FLAG_SET_ALL) {	/* If need to set all bits */
			if(flagRdy == taskFlag) {
				/*这里让任务就绪或恢复*/
			}
		}
		else {												/* It is enough if any bit set*/
			if(flagRdy != (uint32)0) {
				/*这里让任务就绪或恢复*/
			}
		}
	}
	
	OSUnlock(a);
	
	return pflag->flag;
}


void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt) {
	int a = OSLock();
	
	OSTCBCurPtr->pflag->flag = flag;
	
	if(opt == OS_FLAG_CONSUME_ALL) {
		pflag->flag &= ~(OSTCBCurPtr->pflag->flag);
	} 
	else if(opt == OS_FLAG_CONSUME_ANY) {
		
	} 
	else {
		
	}
	
	OSUnlock(a);
}

