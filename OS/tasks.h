#ifndef TASKS_H
#define TASKS_H

#include "types.h"

/***********************************************
 *
 *  You should change the following four places:
 *  SIZE: 					the stack size of task
 *  PRIO: 					the prio of task 
 *  DECLARE_A_TASK: declare stack of task
 *  DECLARE_TCB: 		declare tcb of task
 *
***********************************************/
#define taska_SIZE 256
#define taskb_SIZE 256
#define taskc_SIZE 256
#define taskd_SIZE 256

#define taska_PRIO 0
#define taskb_PRIO 1
#define taskc_PRIO 2
#define taskd_PRIO 3

#define DECLARE_TASKS()\
DECLARE_A_TASK(taska);\
DECLARE_A_TASK(taskb);\
DECLARE_A_TASK(taskc);\
DECLARE_A_TASK(taskd);

#define DECLARE_TCBS()\
DECLARE_TCB(taska),\
DECLARE_TCB(taskb),\
DECLARE_TCB(taskc),\
DECLARE_TCB(taskd)

/**********************************************/





/**********************************************
 *  Do not change the following code
**********************************************/
#define DECLARE_TCB(NAME)\
{.stk = &NAME##_STK[NAME##_SIZE - 1], .task = NAME, .stkSize = NAME##_SIZE, .prio = NAME##_PRIO}


#define DECLARE_A_TASK(NAME)\
__align(8) uint32 NAME##_STK[NAME##_SIZE];\
extern uint32 NAME##_STK[NAME##_SIZE];\
void NAME(void)

extern OSTcb tasks[];
extern const int TASK_NUM;

#endif

