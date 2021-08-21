#ifndef OS_H
#define OS_H

#include "types.h"

#define MAX_TASK_NUM 32		/* Max number of tasks */
#define MAX_PRIO (MAX_TASK_NUM-1)
#define IDLE_SIZE 256

typedef void(*ptask)(void);


extern OStcb *OSTCBCurPtr;
extern OStcb *OSTCBNextPtr;

extern uint32 CPU_CntLeadZeros(uint32 a);	/*From os_asm.asm*/
extern int OSLock(void);	
extern void OSUnlock(int key);
extern void OSStart(void);
extern void OSContextSwitch(void);
extern void OSPendSV_Handler(void);

void OSTimeTick(void);
static void OSSched(void);	
void OSTaskStkInit(OStcb *tcb, ptask task, uint32 *stk);
void OSDelay(uint32 ticks);
void OSTaskIdle(void);
void OSInit(void);
static void OSIdleInit(void);

#endif

