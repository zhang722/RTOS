#ifndef OS_H
#define OS_H

#include "types.h"

// xtos任务入口
typedef void(*xtos_task)(void);


/*
 * xtos_task_struct - 任务描述符
 */
struct xtos_task_struct {
  uint32 * stk;                /* Pointer to top of stack */
  uint32   stkSize;            /* Size of stack */
//  uint8    state;              /* State of Task */
//  //uint32   pid;               /* ID of task */
  uint32    prio;              /* Prio of task */
//  uint32   tickRemain;         /* Number of ticks remaining */
  uint32   ticks;             /* Total ticks to delay */
};
typedef struct xtos_task_struct OStcb;

struct taskList 
{
	OStcb * tcb;
};
typedef struct taskList OSList;
	


extern OSList rdyList[3];
extern uint32 OSPrioTbl;

extern OStcb *OSTCBCurPtr;
extern OStcb *OSTCBNextPtr;

extern uint32 CPU_CntLeadZeros(uint32 a);
extern int OSLock(void);	/*From os_asm.asm*/
extern void OSUnlock(int key);
extern void OSStart(void);
extern void OSContextSwitch(void);
extern void OSPendSV_Handler(void);

void OSCreateTask(OStcb *tcb, xtos_task task, uint32 *stk);
void OSDelay(uint32 ticks);
void OSHistroyTask(void);

#endif

