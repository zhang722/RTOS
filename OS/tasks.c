#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"

uint32 taskA_Stk[TASKA_STK_SIZE];
uint32 taskB_Stk[TASKB_STK_SIZE];

struct xtos_task_struct taskA;
struct xtos_task_struct taskB;

void delay(int c) {
    for (int i = 0; i < c; i++){};
}


void taskSwitch(void) {
	int primask = OSLock();
	
	if (gp_xtos_cur_task == &taskA)
			gp_xtos_next_task = &taskB;
	else if(gp_xtos_cur_task == &taskB)
			gp_xtos_next_task = &taskA;
	OSContextSwitch();
	
	OSUnlock(primask);
}

void taska(void) {
	while(1) {
		OLED_ShowString(0,0,"1");
		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
		GPIOD->BSRR = GPIO_PIN_2;
		OSDelay(1000);
	}
	
}

void taskb(void) {
	while(1) {
		OLED_ShowString(0,0,"2");
		GPIOA->BSRR = GPIO_PIN_8;
		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
		OSDelay(1000);
	}
	
}

