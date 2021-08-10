#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "oled.h"

uint32 taskA_Stk[TASKA_STK_SIZE];
uint32 taskB_Stk[TASKB_STK_SIZE];
uint32 taskIdle_Stk[TASKB_STK_SIZE];

#define task1_STK_SIZE 256
#define task2_STK_SIZE 256
#define taskidle_STK_SIZE 256

#define task1_PRIO 0
#define task2_PRIO 1
#define taskidle_PRIO 31

ptask  name[TASK_NUM] = {taska, taskb, OSTaskIdle}; 
uint32 size[TASK_NUM] = {task1_STK_SIZE , task2_STK_SIZE,  taskidle_STK_SIZE};
uint32 prio[TASK_NUM] = {task1_PRIO,     task2_PRIO,     taskidle_PRIO};

#define OSTaskDeclare() \
taskDeclare(taska);\
taskDeclare(taskb);\

#define taskDeclare(NAME)\
OStcb NAME##TCB;\
__align(8) CPU_STK NAME##Stk[NAME##_STK_SIZE];\
void NAME(void *p)
	

OStcb taskA = {.prio = (uint32)1<<31};
OStcb taskB = {.prio = (uint32)1<<30};
OStcb taskIdle = {.prio = (uint32)1<<29};




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


