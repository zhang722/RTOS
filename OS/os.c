#include "os.h"
#include "stm32f1xx_hal.h"
#include "tasks.h"


uint8 flag = 0;

OStcb *OSTCBCurPtr;
OStcb *OSTCBNextPtr;

OSList rdyList[32];

//uint32 OSPrioTbl = (uint32)0;
uint32 OSPrioTbl = (uint32)0 | (uint32)1<<31 |(uint32)1<<30 | (uint32)1<<29;


/*
 * xtos_task_finished - 任务结束后的回调函数
 */
void xtos_distroy_task() {
    // to do...
    while(1){}
}
/*
 *  Init task stack when create a task
 *
 *  @tcb: 任务描述符
 *  @task: 任务入口函数
 *  @stk: 任务栈顶
 */
void OSTaskStkInit(OStcb * tcb, ptask task, uint32 * stk) {
    uint32  *pstk;
    pstk = stk;
    pstk = (uint32 *)((uint32)(pstk) & 0xFFFFFFF8uL);


    *(--pstk) = (uint32)0x01000000uL; // xPSR
    *(--pstk) = (uint32)task;         // Entry Point
    *(--pstk) = (uint32)xtos_distroy_task; // R14 (LR)
    *(--pstk) = (uint32)0x12121212uL; // R12
    *(--pstk) = (uint32)0x03030303uL; // R3
    *(--pstk) = (uint32)0x02020202uL; // R2
    *(--pstk) = (uint32)0x01010101uL; // R1
    *(--pstk) = (uint32)0x00000000u;  // R0

    *(--pstk) = (uint32)0x11111111uL; // R11
    *(--pstk) = (uint32)0x10101010uL; // R10
    *(--pstk) = (uint32)0x09090909uL; // R9
    *(--pstk) = (uint32)0x08080808uL; // R8
    *(--pstk) = (uint32)0x07070707uL; // R7
    *(--pstk) = (uint32)0x06060606uL; // R6
    *(--pstk) = (uint32)0x05050505uL; // R5
    *(--pstk) = (uint32)0x04040404uL; // R4

    tcb->stk = pstk;
}


/*
 *  Delay function causes a task switch 
 */
void OSDelay(uint32 ticks) {
	int a = OSLock();								/* Enter critical */
	
	OSTCBCurPtr->ticks = ticks;
	OSPrioTbl ^= OSTCBCurPtr->prio;	/* Remove from the ready list */
	OSUnlock(a);
	
	OSSched();											/* Exit critical */
}


void OSPrioInit(void)
{
	
}


/*
 *  Called in Systick_handler. Decrease task ticks by one each time.
 *  If a task is ready, this function changes OSPrioTbl to prepare 
 *  for switching.
 */
void OSTimeTick(void)
{
	int a = OSLock();
	
	for(int i = 0;i < 3;i++) {								/* Check all tasks */
		if(rdyList[i].tcb->ticks > 0) {
			rdyList[i].tcb->ticks --;
		}
		else if(rdyList[i].tcb->ticks == 0) {		/* If a task is ready */
			OSPrioTbl |= rdyList[i].tcb->prio ;
			flag = 1;															/* Tell IdleTask a task is ready */
		}	
	}
	OSUnlock(a);
}

void OSSched(void) {
	int primask = OSLock();
	
	uint32 maxPrio = CPU_CntLeadZeros(OSPrioTbl);	/* Get the highest priority */
	maxPrio = maxPrio > 31 ? 31 : maxPrio;		/* Ensure the priority is within range */
	OSTCBNextPtr = rdyList[maxPrio].tcb;		/* Find correct tcb */
	OSUnlock(primask);
	
	if(OSTCBNextPtr == OSTCBCurPtr) {
		return;
	}	
	OSContextSwitch();
}

void OSTaskIdle(void) {
	while(1) {
		if(flag == 1){	/* If a task is ready */
			flag = 0;
			OSSched(); 
		}
	}
}


void OSTaskListInit(void) {
	rdyList[0].tcb = &taskA;
	rdyList[1].tcb = &taskB;
	rdyList[2].tcb = &taskC;
	rdyList[31].tcb = &taskIdle;
}

