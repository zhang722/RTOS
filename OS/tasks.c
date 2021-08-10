#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"

uint32 taskA_Stk[TASKA_STK_SIZE];
uint32 taskB_Stk[TASKB_STK_SIZE];
uint32 taskIdle_Stk[TASKB_STK_SIZE];

OStcb taskA = {.prio = (uint32)1<<31};
OStcb taskB = {.prio = (uint32)1<<30};
OStcb taskIdle = {.prio = (uint32)1<<29};

uint8 flag = 0;

/*
 *  Called in Systick_handler. Decrease task ticks by one each time.
 *  If a task is ready, change OSPrioTbl to prepare for switching.
 */
void OSTimeTick(void)
{
	int a = OSLock();
	
	for(int i = 0;i < 2;i++) {								/* Check all tasks */
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

uint32 maxPrio;

void OSSched(void) {
	int primask = OSLock();
	
	maxPrio = CPU_CntLeadZeros(OSPrioTbl);	/* Get the highest priority */
	maxPrio = maxPrio > 2 ? 2 : maxPrio;		/* Ensure the priority is within range */
	OSTCBNextPtr = rdyList[maxPrio].tcb;		/* Find correct tcb */
	OSUnlock(primask);
	
	if(OSTCBNextPtr == OSTCBCurPtr) {
		return;
	}	
	OSContextSwitch();
}

void taska(void) {
	while(1) {
		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
		OSDelay(1000);		
		GPIOD->BSRR = GPIO_PIN_2;
		OSDelay(1000);
	}
}

void taskb(void) {
	while(1) {
		GPIOA->BSRR = GPIO_PIN_8;
		OSDelay(1000);
		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
		OSDelay(1000);
	}
}

void taskidle(void) {
	while(1) {
		if(flag == 1){	/* If a task is ready */
			flag = 0;
			OSSched(); 
		}
	}
}
