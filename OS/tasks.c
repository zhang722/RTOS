#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"
#include "os_flag.h"


/***********************************************
 *  Do not change the following code
***********************************************/
DECLARE_TASKS();																	/* Declare and define stack of tasks */
OSTcb tasks[] = {DECLARE_TCBS()};									/* Define tcbs 											 */
const int TASK_NUM = sizeof(tasks)/sizeof(OSTcb); /* Compute total number of tasks 		 */
/**********************************************/
OSFlag test1_FLAG = {.flag = 0x00};
OSFlag test2_FLAG = {.flag = 0x00};


/***********************************************
 *  Change the following code if needed
***********************************************/
void taska(void) {
	while(1) {
		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
		OLED_ShowString(0,0,"1");
		
		OSDelay(1000);
		GPIOD->BSRR = GPIO_PIN_2;
		OLED_ShowString(0,0,".");
		
		OSDelay(1000);
		OSFlagPost(&test1_FLAG,0x01,OS_FLAG_SET_ALL);
	}
}


void taskb(void) {
	while(1) {
		OSFlagPend(&test1_FLAG,0x01,OS_FLAG_CONSUME);
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

void taskd(void) {
	while(1) {
		OLED_ShowString(30,0,"4");
		OSDelay(1000);
		OLED_ShowString(30,0,".");
		OSDelay(1000);
	}
}


