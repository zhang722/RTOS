#include "os.h"
#include "stm32f1xx_hal.h"
#include "tasks.h"


OSTcb *OSTCBCurPtr;										/* Pointer to current tcb 					 */
OSTcb *OSTCBNextPtr;  								/* Pointer to highest prio ready tcb */

OSList OSRdyList[MAX_TASK_NUM]; 
uint32 OSPrioTbl = (uint32)0; 				/* Initialize to 0 									 */


uint32 taskIdle_STK[IDLE_SIZE]; 			/* Defination of idle task 					 */
OSTcb  taskIdle = {.prio = (uint32)1}; /* Lowest prio 											 */


/*
 * Callback to distory task
 */
void OSDistroyTask(void) {
    while(1){}	/* to do... */
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

#if (__FPU_PRESENT==1)&&(__FPU_USED==1)     	/* Registers stacked as if auto-saved on exception        */
	*(--pstk) = (uint32)0x00000000u; //No Name Register  
	*(--pstk) = (uint32)0x00001000u; //FPSCR
	*(--pstk) = (uint32)0x00000015u; //s15
	*(--pstk) = (uint32)0x00000014u; //s14
	*(--pstk) = (uint32)0x00000013u; //s13
	*(--pstk) = (uint32)0x00000012u; //s12
	*(--pstk) = (uint32)0x00000011u; //s11
	*(--pstk) = (uint32)0x00000010u; //s10
	*(--pstk) = (uint32)0x00000009u; //s9
	*(--pstk) = (uint32)0x00000008u; //s8
	*(--pstk) = (uint32)0x00000007u; //s7
	*(--pstk) = (uint32)0x00000006u; //s6
	*(--pstk) = (uint32)0x00000005u; //s5
	*(--pstk) = (uint32)0x00000004u; //s4
	*(--pstk) = (uint32)0x00000003u; //s3
	*(--pstk) = (uint32)0x00000002u; //s2
	*(--pstk) = (uint32)0x00000001u; //s1
	*(--pstk) = (uint32)0x00000000u; //s0
#endif

    *(--pstk) = (uint32)0x01000000uL; // xPSR
    *(--pstk) = (uint32)task;         // Entry Point
    *(--pstk) = (uint32)OSDistroyTask; // R14 (LR)
    *(--pstk) = (uint32)0x12121212uL; // R12
    *(--pstk) = (uint32)0x03030303uL; // R3
    *(--pstk) = (uint32)0x02020202uL; // R2
    *(--pstk) = (uint32)0x01010101uL; // R1
    *(--pstk) = (uint32)0x00000000u;  // R0

#if (__FPU_PRESENT==1)&&(__FPU_USED==1)
	*(--pstk) = (uint32)0x00000031u; //s31
	*(--pstk) = (uint32)0x00000030u; //s30
	*(--pstk) = (uint32)0x00000029u; //s29
	*(--pstk) = (uint32)0x00000028u; //s28
	*(--pstk) = (uint32)0x00000027u; //s27
	*(--pstk) = (uint32)0x00000026u; //s26	
	*(--pstk) = (uint32)0x00000025u; //s25
	*(--pstk) = (uint32)0x00000024u; //s24
	*(--pstk) = (uint32)0x00000023u; //s23
	*(--pstk) = (uint32)0x00000022u; //s22
	*(--pstk) = (uint32)0x00000021u; //s21
	*(--pstk) = (uint32)0x00000020u; //s20
	*(--pstk) = (uint32)0x00000019u; //s19
	*(--pstk) = (uint32)0x00000018u; //s18
	*(--pstk) = (uint32)0x00000017u; //s17
	*(--pstk) = (uint32)0x00000016u; //s16
#endif

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
	
	OSUnlock(a);										/* Exit critical    					*/
	
	OSSched();											
}



/*
 *  Called in Systick_handler. Decrease task ticks by one each time.
 *  If a task is ready, this function changes OSPrioTbl to prepare 
 *  for switching.
 */
void OSTimeTick(void)
{
	int a = OSLock();
	
	for(int i = 0;i < TASK_NUM;i++) {									/* Check all tasks 		*/
		if(OSRdyList[tasks[i].prioLE].tcb->ticks > 0) {
			OSRdyList[tasks[i].prioLE].tcb->ticks --;
		}
		if(OSRdyList[tasks[i].prioLE].tcb->ticks == 0 &&
			 OSRdyList[tasks[i].prioLE].tcb->state == OS_READY) {	/* If a task is ready */
			OSPrioTbl |= OSRdyList[tasks[i].prioLE].tcb->prio;
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
	int a = OSLock();
	
	maxPrio 		 = OSPrioGetHighest();
	OSTCBNextPtr = OSRdyList[maxPrio].tcb;	/* Find correct tcb 		 */
	
	OSUnlock(a);
	
	if(OSTCBNextPtr == OSTCBCurPtr) {
		return;																/* Do not need to switch */
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
	OSRdyList[MAX_PRIO].tcb = &taskIdle;									 /* Link OSRdyList with idle task tcb */
	OSTaskStkInit(&taskIdle, OSTaskIdle, &taskIdle_STK[IDLE_SIZE - 1]);	/* Init idle task stack */
}


/*
 *  Function to suspend a task
 */
void OSTaskSuspend(OSTcb * ptcb) {
	int a = OSLock();
	
	OSPrioRemove(ptcb->prio);
	ptcb->state = OS_SUSPEND;
	
	OSUnlock(a);
}


/*
 *  Function to resume a suspended task
 */
void OSTaskResume(OSTcb * ptcb) {
	int a = OSLock();
	
	ptcb->state = OS_READY;
	
	OSUnlock(a);
}


/*
 *  Init all tasks, including idle task. Init systick.
 */
void OSInit(void) {
	for(int i = 0;i < TASK_NUM;i++) {
		tasks[i].prioLE = tasks[i].prio;
		tasks[i].prio   = OSPrioToBigEnd(tasks[i].prio);			/* Convert to BigEnd 						*/
		OSPrioInsert(tasks[i].prio);
		OSRdyList[tasks[i].prioLE].tcb = &tasks[i];						/* Link OSRdyList with task tcb */
		OSTaskStkInit(&tasks[i],tasks[i].task,tasks[i].stk);	/* Init task stack 							*/
		tasks[i].state = OS_READY;														/* Make tasks ready 						*/
	}
	OSIdleInit();
	OSTCBCurPtr  = OSRdyList[MAX_PRIO].tcb;
  OSTCBNextPtr = OSRdyList[MAX_PRIO].tcb;
	
	/*systick circle 1ms*/
	SysTick_Config(72000000 / OS_TICK_HZ);	/*默认时钟源AHB,产生异常请求,立即使能*/

}

