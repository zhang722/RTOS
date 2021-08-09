#include "stdio.h"
#include "stdlib.h"

// #define MAX_PRIO 32
typedef unsigned long  uint32;
typedef unsigned char  uint8;

// #define Ready   1
// #define Run     2
// #define Suspend 3


// struct tcb {
//   uint32 * stk                /* Pointer to top of stack */
//   uint32   stkSize            /* Size of stack */
//   uint8    state              /* State of Task */
//   //uint32   pid;               /* ID of task */
//   uint32    prio;              /* Prio of task */
//   uint32   tickRemain         /* Number of ticks remaining */
//   uint32   ticks;             /* Total ticks to delay */
// };
// typedef struct tcb OSTcb;

// struct tcbRdyList
// {
//   OSTcb * tcb;  /* Pointer to tcb */
// };


// void OS_PrioInsert(uint32 prio)
// {
//   OSPrioTbl |= prio;
// }

// uint32 * OSTaskStkInit(void * task,
//                        void * arg,
//                        uint32 * stkBase,
//                        uint32 * stkSize)
// {
//   uint32 * pstk = NULL;
//   pstk = &stkBase[stkSize];
//   pstk = (uint32 *)((uint32)(pstk) & 0xFFFFFFF8uL);

//   *(--pstk) = (uint32)0x01000000uL; // xPSR
//   *(--pstk) = (uint32)task;         // Entry Point
//   *(--pstk) = (uint32)xtos_distroy_task; // R14 (LR)
//   *(--pstk) = (uint32)0x12121212uL; // R12
//   *(--pstk) = (uint32)0x03030303uL; // R3
//   *(--pstk) = (uint32)0x02020202uL; // R2
//   *(--pstk) = (uint32)0x01010101uL; // R1
//   *(--pstk) = (uint32)0x00000000u;  // R0

//   *(--pstk) = (uint32)0x11111111uL; // R11
//   *(--pstk) = (uint32)0x10101010uL; // R10
//   *(--pstk) = (uint32)0x09090909uL; // R9
//   *(--pstk) = (uint32)0x08080808uL; // R8
//   *(--pstk) = (uint32)0x07070707uL; // R7
//   *(--pstk) = (uint32)0x06060606uL; // R6
//   *(--pstk) = (uint32)0x05050505uL; // R5
//   *(--pstk) = (uint32)0x04040404uL; // R4

//   return pstk;
// }

// void OSTaskCreate(OSTcb * tcb, 
//                   void * task,
//                   void * arg,
//                   uint32 prio,
//                   uint32 * stkBase,
//                   uint32 stkSize) 
// {
//   uint32 * psp = NULL;
//   psp = OSTaskStkInit(task,
//                        arg,
//                        stkBase,
//                        stkSize);
//   tcb->stk = psp;
//   tcb->stkSize = stkSize; 
//   tcb->prio = prio;
//   tcb->state = Ready;
//   OS_CRITICAL_ENTER();
//   OS_PrioInsert(prio);
//   OS_CRITICAL_EXIT();
  
// }

void printf_bin(int num)
{
    int i, j, k;
    unsigned char *p = (unsigned char*)&num + 3;//p先指向num后面第3个字节的地址，即num的最高位字节地址

    for (i = 0; i < 4; i++) //依次处理4个字节(32位）
    {
        j = *(p - i); //取每个字节的首地址，从高位字节到低位字节，即p p-1 p-2 p-3地址处
        for (int k = 7; k >= 0; k--) //处理每个字节的8个位，注意字节内部的二进制数是按照人的习惯存储！
        {
            if (j & (1 << k))//1左移k位，与单前的字节内容j进行或运算，如k=7时，00000000&10000000=0 ->该字节的最高位为0
                printf("1");
            else
                printf("0");
        }
        printf(" ");//每8位加个空格，方便查看
    }
    printf("\r\n");
}


int main(int argc, char const *argv[])
{
  uint32 prioTable = (uint32)0 | (uint32)1<<31 |(uint32)1<<30 | (uint32)1<<29;
  // uint32 prio = 1<<31;
  // prioTable |= prio;
  printf_bin(prioTable);
  return 0;
}

