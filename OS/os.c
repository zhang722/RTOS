#include "os.h"
#include "stm32f1xx_hal.h"
#include "tasks.h"

struct xtos_task_struct *gp_xtos_cur_task;
struct xtos_task_struct *gp_xtos_next_task;

/*
 * xtos_task_finished - 任务结束后的回调函数
 */
void xtos_distroy_task() {
    // to do...
    while(1){}
}
/*
 * xtos_create_task - 创建一个任务，初始化任务栈空间
 *
 * @tcb: 任务描述符
 * @task: 任务入口函数
 * @stk: 任务栈顶
 */
void OSCreateTask(struct xtos_task_struct * tcb, xtos_task task, uint32 * stk) {
    uint32  *pstk;
    pstk = stk;
    pstk = (uint32 *)((uint32)(pstk) & 0xFFFFFFF8uL);


    *(--pstk) = (uint32)0x01000000uL; // xPSR
    *(--pstk) = (uint32)task;         // Entry Point
    *(--pstk) = (uint32)xtos_distroy_task; // R14 (LR)
    *(--pstk) = (uint32)0x12121212uL; // R12
    *(--pstk) = (uint32)0x03030303uL; // R3
    *(--pstk) = (uint32)0x02020202uL; // R2
    *(--pstk) = (uint32)0x01010101uL; // R1
    *(--pstk) = (uint32)0x00000000u;  // R0

    *(--pstk) = (uint32)0x11111111uL; // R11
    *(--pstk) = (uint32)0x10101010uL; // R10
    *(--pstk) = (uint32)0x09090909uL; // R9
    *(--pstk) = (uint32)0x08080808uL; // R8
    *(--pstk) = (uint32)0x07070707uL; // R7
    *(--pstk) = (uint32)0x06060606uL; // R6
    *(--pstk) = (uint32)0x05050505uL; // R5
    *(--pstk) = (uint32)0x04040404uL; // R4

    tcb->pTopOfStack = pstk;
}


/*
OS_time
*/
void OSDelay(uint32 ticks) {
	uint32 a = HAL_GetTick();
	uint32 delta = HAL_GetTick() - a;

	while(delta < ticks) {
		taskSwitch();
		delta = HAL_GetTick() - a;
	}
}

/*
************************************************************************************************************************
*                                         TASK LIST
*
* Description: Inplementation of tcb list
************************************************************************************************************************
*/

/*
 * Initialise item
 */
void listItemInit(OSListItem * const item) {
	item -> container = NULL;	/*Container is null, indicating that the node has not inserted any list*/
}

/*
 *  
 */
void listInit( OSList * const list )
{
    list -> index = ( OSListItem * ) & ( list -> endItem );						/* Point the index to the last node */  
    list -> endItem -> itemValue  = MAX_TASK_NUM;											/* Set itemValue to max, */
																																			/* ensure endItem is the last node */ 
    list->endItem -> next = ( OSListItem * ) & ( list->endItem );			/* Next is itself */
    list->endItem -> previous = ( OSListItem * ) & ( list->endItem );	/* Previous is itself, */ 
																																			/* indicating the list is empty */
    list->num = 0U;																										/* Indicates that the list is empty */
}

