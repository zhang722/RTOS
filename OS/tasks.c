#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"

uint32 taskA_Stk[TASKA_STK_SIZE];
uint32 taskB_Stk[TASKB_STK_SIZE];
uint32 taskC_Stk[TASKB_STK_SIZE];
uint32 taskIdle_Stk[TASKB_STK_SIZE];

OStcb taskA = {.prio = (uint32)1<<31};
OStcb taskB = {.prio = (uint32)1<<30};
OStcb taskC = {.prio = (uint32)1<<29};
OStcb taskIdle = {.prio = (uint32)1};

void taska(void) {
	while(1) {
		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
		OLED_ShowString(0,0,"1");
		OSDelay(1000);		
		GPIOD->BSRR = GPIO_PIN_2;
		OLED_ShowString(0,0,".");
		OSDelay(1000);
	}
}


void taskb(void) {
	while(1) {
		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
		OLED_ShowString(10,0,"2");
		OSDelay(1000);
		GPIOA->BSRR = GPIO_PIN_8;
		OLED_ShowString(10,0,".");
		OSDelay(1000);
	}
}


void taskc(void) {
	while(1) {
		OLED_ShowString(20,0,"3");
		OSDelay(1000);
		OLED_ShowString(20,0,".");
		OSDelay(1000);
	}
}


