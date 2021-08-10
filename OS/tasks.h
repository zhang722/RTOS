#ifndef TASKS_H
#define TASKS_H

#include "types.h"

#define TASK_NUM 3 /* Modify it if tasks created or deleted */

#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024

extern uint32 taskA_Stk[TASKA_STK_SIZE];
extern uint32 taskB_Stk[TASKB_STK_SIZE];
extern uint32 taskIdle_Stk[TASKB_STK_SIZE];

extern struct tcb taskA;
extern struct tcb taskB;
extern struct tcb taskIdle;

void taska(void);
void taskb(void);


#endif

