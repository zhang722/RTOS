#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"
#include "os_flag.h"


/***************************************************************************************
 *  Do not change the following code
****************************************************************************************/
DECLARE_TASKS();																	/* Declare and define stack of tasks */
OSTcb tasks[] = {DECLARE_TCBS()};									/* Define tcbs 											 */
const int TASK_NUM = sizeof(tasks)/sizeof(OSTcb); /* Compute total number of tasks 		 */
/***************************************************************************************/



/***************************************************************************************
 *  Change the following code if needed
****************************************************************************************/
OSFlag test1_FLAG = {.flag = 0x00};
OSFlag test2_FLAG = {.flag = 0x00};


uint8_t keyScan(void){
	if(!HAL_GPIO_ReadPin(GPIOC,KEY0_Pin))
	{
//		OSDelay(1);
//		if(!HAL_GPIO_ReadPin(GPIOC,KEY0_Pin)) 
			return key0_press;
		//else return no_press;
	}
	else if(!HAL_GPIO_ReadPin(GPIOC,KEY1_Pin))
	{
//		OSDelay(1);
//		if(!HAL_GPIO_ReadPin(GPIOC,KEY0_Pin)) 
		return key1_press;
		//else return no_press;
	}
	else return no_press;
}


void taska(void) {
	while(1) {
//		GPIOD->BSRR = (uint32_t)GPIO_PIN_2 << 16u;
//		OLED_ShowString(0,0,"1");
//		
//		OSDelay(10);
//		GPIOD->BSRR = GPIO_PIN_2;
//		OLED_ShowString(0,0,".");
//	
		if(keyScan()==1) {
			OSFlagPost(&test1_FLAG,0x01,OS_FLAG_SET_ALL);
		}		
		if(test1_FLAG.flag == 0x00) {
			OLED_ShowString(0,0,"0");
		} else if(test1_FLAG.flag == 0x01)
		{
			OLED_ShowString(0,0,"1");
		} else if(test1_FLAG.flag == 0x02)
		{
			OLED_ShowString(0,0,"2");
		} else if(test1_FLAG.flag == 0x03) {
			OLED_ShowString(0,0,"3");
		}
		OSDelay(10);
	
	}
}


void taskb(void) {
	while(1) {
		OSFlagPend(&test1_FLAG,0x03,OS_FLAG_GET_CLR);
//		GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16u;
		OLED_ShowString(10,0,"2");
//		OSDelay(10);
//		GPIOA->BSRR = GPIO_PIN_8;
//		OLED_ShowString(10,0,".");
//		
//		OSDelay(10);
	}
}


void taskc(void) {
	while(1) {
//		OLED_ShowString(20,0,"3");
//		OSDelay(100);
//		OLED_ShowString(20,0,".");
//		OSDelay(100);
		
		if(keyScan()==2) {
			OSFlagPost(&test1_FLAG,0x02,OS_FLAG_SET_ALL);	
		}
		OSDelay(10);		
	}
}

void taskd(void) {
	while(1) {
		OLED_ShowString(30,0,"4");
		OSDelay(100);
		OLED_ShowString(30,0,".");
		OSDelay(100);
	}
}


void taske(void) {
	while(1) {
		OLED_ShowString(40,0,"5");
		OSDelay(100);
		OLED_ShowString(40,0,".");
		OSDelay(100);
	}
}

