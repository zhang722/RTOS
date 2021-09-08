#include "stdio.h"
#include "stdlib.h"

// #define MAX_PRIO 32
typedef unsigned int  uint32;
typedef unsigned char  uint8;

#define OS_FLAG_SET_ALL (uint32)0x00
#define OS_FLAG_SET_ANY (uint32)0x01

#define OS_FLAG_CONSUME  (uint32)0x02
#define OS_FLAG_NO_CNSM  (uint32)0x04

typedef struct 
{
  uint32   flag;
  uint32 * pflag;
}OSTcb;



typedef struct 
{
  uint32 flag;
}OSFlag;

OSFlag OSCurFlag;
OSTcb OSCurTcb = {0,(uint32)&OSCurFlag};

void OSFlagCreate(OSFlag * pflag, uint32 flag) 
{
  
  pflag->flag = flag;
  
}



uint32 OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt) 
{
  uint32 flagRdy;
  uint32 taskFlag;
  pflag->flag |= flag;

  taskFlag = OSCurTcb.pflag->flag;
  flagRdy = (uint32)(pflag->flag & taskFlag);
  
  
  return pflag->flag;
}


void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt) {
 
  if(opt == OS_FLAG_CONSUME) {
    pflag->flag &= ~(OSCurTcb.pflag->flag);
  } 
  else {
    pflag->flag &= ~(OSCurTcb.pflag->flag);
  }

}

int main(int argc, char const *argv[])
{
  /* code */
  return 0;
}