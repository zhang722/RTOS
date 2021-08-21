#ifndef TYPES_H
#define TYPES_H


typedef signed char             int8;
typedef signed short            int16;
typedef signed int              int32;

typedef signed char const       cint8;
typedef signed short const      cint16;
typedef signed int const        cint32;

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;

typedef unsigned char const     ucint8;
typedef unsigned short const    ucint16;
typedef unsigned int const      ucint32;


typedef void(*ptask)(void); /* Pointer to task function */

/*
 * Task control block(TCB)
 */
struct tcb {
  uint32 * stk;                /* Pointer to top of stack */
  uint32   stkSize;            /* Size of stack */
  ptask    task;               /* Pointer to task function */
  uint8    state;              /* State of Task */
  uint32   prio;               /* Prio of task */
  uint32   ticks;              /* Total ticks to delay */
};
typedef struct tcb OStcb;

struct taskList 
{
  OStcb * tcb;
};
typedef struct taskList OSList;

#endif

