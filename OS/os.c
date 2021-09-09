#include "os.h"
#include "stm32f1xx_hal.h"
#include "tasks.h"


uint8 taskReady = 0;	/* Flag shows if a task is ready */

OSTcb *OSTCBCurPtr;	/* Pointer to current tcb */
OSTcb *OSTCBNextPtr; /* Pointer to highest prio ready tcb */

OSList OSRdyList[MAX_TASK_NUM]; 
uint32 OSPrioTbl = (uint32)0; /* Initialize to 0 */


uint32 taskIdle_STK[IDLE_SIZE]; /* Defination of idle task */
OSTcb taskIdle = {.prio = (uint32)1}; /* Lowest prio */



/*
 * Callback to distory task
 */
void OSDistroyTask(void) {
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
static void OSTaskStkInit(OSTcb * tcb, ptask task, uint32 * stk) {
    uint32  *pstk;
    pstk = stk;
    pstk = (uint32 *)((uint32)(pstk) & 0xFFFFFFF8uL);


    *(--pstk) = (uint32)0x01000000uL; // xPSR
    *(--pstk) = (uint32)task;         // Entry Point
    *(--pstk) = (uint32)OSDistroyTask; // R14 (LR)
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
	int a = OSLock();								/* Enter critical	 						*/
	
	OSTCBCurPtr->ticks = ticks;
	OSPrioRemove(OSTCBCurPtr->prio);
	OSUnlock(a);
	
	OSSched();											/* Exit critical    					*/
}



/*
 *  Called in Systick_handler. Decrease task ticks by one each time.
 *  If a task is ready, this function changes OSPrioTbl to prepare 
 *  for switching.
 */
void OSTimeTick(void)
{
	int a = OSLock();
	
	for(int i = 0;i < TASK_NUM;i++) {								/* Check all tasks 						 	 */
		if(OSRdyList[i].tcb->ticks > 0) {
			OSRdyList[i].tcb->ticks --;
		}
		else if(OSRdyList[i].tcb->ticks == 0 &&
				    OSRdyList[i].tcb->state == OS_READY) {				/* If a task is ready 					 */
			OSPrioTbl |= OSRdyList[i].tcb->prio ;
		}	
	}
	
	OSUnlock(a);
}

/*
 *  Convert prio to BigEnd
 */
inline static uint32 OSPrioToBigEnd(uint32 prio) {
	return (uint32)1<<(MAX_PRIO-prio); /* Convert prio to BigEnd */
}


/*
 *  Insert a prio to prio table
 */
inline void OSPrioInsert(uint32 prio) {
	OSPrioTbl |= prio;
}


/*
 *  Note: Prio is BigEnd already 
 */
inline void OSPrioRemove(uint32 prio) {
	OSPrioTbl &= ~prio;								 /* Remove from the ready list */
}


static uint32 OSPrioGetHighest(void) {
	uint32 maxPrio = CPU_CntLeadZeros(OSPrioTbl);											/* Get the highest priority					 	 */
	maxPrio = maxPrio > MAX_TASK_NUM-1 ? MAX_TASK_NUM-1 : maxPrio;		/* Ensure the priority is within range */
	return maxPrio;
}


/*
 *  Core function to schedual tasks
 */
uint32 maxPrio;
void OSSched(void) {
	int primask = OSLock();
	
	maxPrio = OSPrioGetHighest();
	OSTCBNextPtr = OSRdyList[maxPrio].tcb;														/* Find correct tcb 									 */
	
	OSUnlock(primask);
	
	if(OSTCBNextPtr == OSTCBCurPtr) {
		return;
	}	
	OSContextSwitch();
}


/*
 *  Idle task defined by OS instead of user
 */
static void OSTaskIdle(void) {
	while(1) {
		if(OSPrioTbl != 0) {	/* If a task is ready */
			OSSched();
		}
	}
}


/*
 *  Idle task init
 */
inline static void OSIdleInit(void) {
	OSRdyList[MAX_PRIO].tcb = &taskIdle;
	OSTaskStkInit(&taskIdle, OSTaskIdle, &taskIdle_STK[IDLE_SIZE - 1]);
}


/*
 *  Function to suspend a task
 */
void OSTaskSuspend(OSTcb * ptcb) {
	int primask = OSLock();
	
	OSPrioRemove(ptcb->prio);
	ptcb->state = OS_SUSPEND;
	
	OSUnlock(primask);
}


/*
 *  Function to resume a suspended task
 */
void OSTaskResume(OSTcb * ptcb) {
	int primask = OSLock();
	
	ptcb->state = OS_READY;
	
	OSUnlock(primask);
}


/*
 *  Init all tasks, including idle task. Init systick.
 */
void OSInit(void) {
	for(int i = 0;i < TASK_NUM;i++) {
		tasks[i].prio = OSPrioToBigEnd(tasks[i].prio);
		OSPrioInsert(tasks[i].prio);
		OSRdyList[i].tcb = &tasks[i];
		OSTaskStkInit(&tasks[i],tasks[i].task,tasks[i].stk);
		tasks[i].state = OS_READY;
	}
	OSIdleInit();
	OSTCBCurPtr = OSRdyList[0].tcb;
  OSTCBNextPtr = OSRdyList[0].tcb;
	
	/*systick circle 1ms*/
	SysTick_Config(72000000 / OS_TICK_HZ);	/*默认时钟源AHB,产生异常请求,立即使能*/
}

