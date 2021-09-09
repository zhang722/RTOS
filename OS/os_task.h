#ifndef OS_TASK_H
#define OS_TASK_H

#include "os_types.h"
/**********************************************
 *  Do NOT change the following code
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

