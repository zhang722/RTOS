#ifndef OS_FLAG_H
#define OS_FLAG_H

#include "os_types.h"
#include "tasks.h"

#define OS_FLAG_SET_ALL  (uint32)0x00	/* ����λ����λ 						*/
#define OS_FLAG_SET_ANY  (uint32)0x01	/* ֻҪ��ĳһ��־��λ���� 	*/

#define OS_FLAG_GET_CLR  (uint32)0x02	/* ��ȡ�������ر�־ 			*/
#define OS_FLAG_NO_CLR   (uint32)0x04	/* ��ȡ����� 						*/


void OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt);
void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt);


#endif

