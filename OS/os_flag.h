#ifndef OS_FLAG_H
#define OS_FLAG_H

#include "types.h"
#include "tasks.h"

#define OS_FLAG_SET_ALL (uint32)0x00
#define OS_FLAG_SET_ANY (uint32)0x01

#define OS_FLAG_CONSUME  (uint32)0x02
#define OS_FLAG_NO_CNSM  (uint32)0x04

uint32 OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt);
void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt);


#endif

