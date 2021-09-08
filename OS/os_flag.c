#include "os.h"
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
	for(int i = 0;i < TASK_NUM;i++){
		taskFlag = tasks[i].pflag->flag;
		flagRdy = (uint32)(pflag->flag & taskFlag);
		if(opt == OS_FLAG_SET_ALL) {	/* If need to set all bits */
			if(flagRdy == taskFlag) {
				OSTCBCurPtr->state = OS_READY;	/* Ready to run */
			}
		}
		else {	/* It is enough if any bit set*/
			if(flagRdy != (uint32)0) {
				OSTCBCurPtr->state = OS_READY;	/* Ready to run */
			}
		}
	}
	
	OSUnlock(a);
	
	return pflag->flag;
}


void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt) 
{
	int a = OSLock();
	uint32 f;
	OSTCBCurPtr->pflag->flag = flag;	/* Save */
	
	f = pflag->flag & flag;
	if(f == flag) {	/* Match flag */
		if(opt == OS_FLAG_CONSUME) {
			pflag->flag &= ~(OSTCBCurPtr->pflag->flag);
		} 	
		
		OSUnlock(a);
		return;
	} else {	/* Not match */
		OSTCBCurPtr->state = OS_BLOCK;	/* Block task */
	}

	OSUnlock(a);
}

