#ifndef OS_FLAG_H
#define OS_FLAG_H

#include "os_types.h"
#include "tasks.h"

#define OS_FLAG_SET_ALL  (uint32)0x00	/* 所有位都置位 						*/
#define OS_FLAG_SET_ANY  (uint32)0x01	/* 只要有某一标志置位即可 	*/

#define OS_FLAG_GET_CLR  (uint32)0x02	/* 获取后清除相关标志 			*/
#define OS_FLAG_NO_CLR   (uint32)0x04	/* 获取后不清除 						*/


void OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt);
void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt);


#endif

