#ifndef TASKS_H
#define TASKS_H

#include "types.h"

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024

extern uint32 taskA_Stk[TASKA_STK_SIZE];
extern uint32 taskB_Stk[TASKB_STK_SIZE];

extern struct xtos_task_struct taskA;
extern struct xtos_task_struct taskB;

void taskSwitch(void);	
void taska(void);
void taskb(void);

#endif

