#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"

uint32 taskA_Stk[TASKA_STK_SIZE];
uint32 taskB_Stk[TASKB_STK_SIZE];
uint32 taskIdle_Stk[TASKB_STK_SIZE];


OStcb taskA = {.prio = (uint32)1<<31};
OStcb taskB = {.prio = (uint32)1<<30};
OStcb taskIdle = {.prio = (uint32)1};


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
		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
		OSDelay(1000);
		GPIOA->BSRR = GPIO_PIN_8;
		OSDelay(1000);
	}
}


