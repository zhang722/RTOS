#ifndef OS_TYPES_H
#define OS_TYPES_H

typedef signed char             int8;
typedef signed short            int16;
typedef signed int              int32;

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;


typedef void(*ptask)(void); 	 /* Pointer to task function */

/*
 * Flag struct
 */
struct flag
{
	uint32 flag;
};
typedef struct flag OSFlag;


/*
 * Task control block(TCB)
 */
struct tcb {
  uint32 * stk;                /* Pointer to top of stack  		*/
  uint32   stkSize;            /* Size of stack 					 		*/
  ptask    task;               /* Pointer to task function 		*/
  uint8    state;              /* State of Task 					 		*/
  uint32   prio;               /* Prio of task 						 		*/
	uint32   prioLE;						 /* Prio in little end			    */
  uint32   ticks;              /* Total ticks to delay 		 		*/
	OSFlag * pflag;							 /* Pointer to flag				   		*/
	uint32	 flag;							 /* Flag to wait 								*/
};
typedef struct tcb OSTcb;


struct taskList 
{
  OSTcb * tcb;
};
typedef struct taskList OSList;

#endif

