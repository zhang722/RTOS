#ifndef OS_H
#define OS_H

#include "types.h"

// xtos任务入口
typedef void(*ptask)(void);


/*
 * Task control block(TCB)
 */
struct tcb {
  uint32 * stk;                /* Pointer to top of stack */
  uint32   stkSize;            /* Size of stack */
  uint8    state;              /* State of Task */
  //uint32   pid;               /* ID of task */
  uint32    prio;              /* Prio of task */
  uint32   ticks;              /* Total ticks to delay */
};
typedef struct tcb OStcb;

struct taskList 
{
	OStcb * tcb;
};
typedef struct taskList OSList;
	


extern OSList rdyList[32];
extern uint32 OSPrioTbl;

extern OStcb *OSTCBCurPtr;
extern OStcb *OSTCBNextPtr;

extern uint32 CPU_CntLeadZeros(uint32 a);	/*From os_asm.asm*/
extern int OSLock(void);	
extern void OSUnlock(int key);
extern void OSStart(void);
extern void OSContextSwitch(void);
extern void OSPendSV_Handler(void);

void OSTimeTick(void);
void OSSched(void);	
void OSTaskStkInit(OStcb *tcb, ptask task, uint32 *stk);
void OSDelay(uint32 ticks);
void OSHistroyTask(void);
void OSTaskIdle(void);

#endif

