#ifndef OS_H
#define OS_H

#include "types.h"

// xtos任务入口
typedef void(*xtos_task)(void);


/*
 * xtos_task_struct - 任务描述符
 */
struct xtos_task_struct {
    uint32 *pTopOfStack;   /* 栈顶地址 */
};

/*
************************************************************************************************************************
*                                         TASK LIST
*
* Description: Structure of tcb list
************************************************************************************************************************
*/

#define MAX_TASK_NUM 64

/*List node*/
struct listItem {
	uint32 itemValue; 					/*Help to sort in order*/
	struct listItem * next;			/*Pointer to next item*/
	struct listItem * previous;	/*Pointer to previous item*/
	void * owner;								/*Pointer to a tcb struct who owns this list item*/
	void * container;						/*Pointer to a list who owns this list item*/
};
typedef struct listItem OSListItem;

/*List root*/
struct list {
	uint32 num; 								/*Number of list items*/
	struct listItem * index; 		/*Index of a list item*/
	struct listItem * endItem; 	/*The last item of list*/
};
typedef struct list OSList;


extern struct xtos_task_struct *gp_xtos_cur_task;
extern struct xtos_task_struct *gp_xtos_next_task;

extern int OSLock(void);	/*From os_asm.asm*/
extern void OSUnlock(int key);
extern void OSStart(void);
extern void OSContextSwitch(void);
extern void OSPendSV_Handler(void);

void OSCreateTask(struct xtos_task_struct *tcb, xtos_task task, uint32 *stk);
void OSDelay(uint32 ticks);
void OSHistroyTask(void);

#endif

