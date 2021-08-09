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

void OSTimeTick(void)
{
	int a = OSLock();
	
	for(int i = 0;i < 2;i++) {
		if(rdyList[i].tcb->ticks > 0) {
			rdyList[i].tcb->ticks --;
		}
		else if(rdyList[i].tcb->ticks == 0) {
			OSPrioTbl |= rdyList[i].tcb->prio ;
			flag = 1;
		}
		
	}
	
	OSUnlock(a);
}

uint32 maxPrio;

void OSSched(void) {
	int primask = OSLock();
	
	maxPrio = CPU_CntLeadZeros(OSPrioTbl);
	maxPrio = maxPrio > 2 ? 2 : maxPrio;
	OSTCBNextPtr = rdyList[maxPrio].tcb;
	OSUnlock(primask);
	
	if(OSTCBNextPtr == OSTCBCurPtr) {
		return;
	}
	
	OSContextSwitch();
}

void taska(void) {
	while(1) {
		//OLED_ShowString(0,0,"1");
		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
		GPIOD->BSRR = GPIO_PIN_2;
		OSDelay(3);
	}
	
}

void taskb(void) {
	while(1) {
		//OLED_ShowString(0,0,"2");
		GPIOA->BSRR = GPIO_PIN_8;
		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
		OSDelay(3);
	}
	
}


void taskidle(void) {
	while(1) {
		//OLED_ShowString(0,0,"D");
//		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
//		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
		if(flag == 1){
			OSSched();
			flag = 0;
		}
		
	}
	
}
