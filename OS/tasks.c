#include "gpio.h"
#include "os.h"
#include "tasks.h"
#include "os_flag.h"


/***************************************************************************************
 *  Do not change the following code
****************************************************************************************/
DECLARE_TASKS();			
OSTcb tasks[] = {DECLARE_TCBS()};									/* Define tcbs 											 */
const int TASK_NUM = sizeof(tasks)/sizeof(OSTcb); /* Compute total number of tasks 		 */
/***************************************************************************************/



/***************************************************************************************
 *  Change the following code if needed
****************************************************************************************/
OSFlag test1_FLAG = {.flag = 0x00};
OSFlag test2_FLAG = {.flag = 0x00};

OSTaskTest testFps = {0};


void taska(void) {
	while(1) {
		testFps.taskaCnt++;
		OSDelay(10);
	
	}
}


void taskb(void) {
	while(1) {
		OSFlagPend(&test1_FLAG,0x03,OS_FLAG_GET_CLR);
		testFps.taskbCnt++;
	}
}


void taskc(void) {
	while(1) {
		testFps.taskcCnt++;
		OSDelay(10);		
	}
}


void taskd(void) {
	while(1) {
		testFps.taskdCnt++;
		OSDelay(500);
	}
}


void taske(void) {
	while(1) {
		testFps.taskaFps = testFps.taskaCnt;
		testFps.taskbFps = testFps.taskbCnt;
		testFps.taskcFps = testFps.taskcCnt;
		testFps.taskdFps = testFps.taskdCnt;
		testFps.taskaCnt = 0;
		testFps.taskbCnt = 0;
		testFps.taskcCnt = 0;
		testFps.taskdCnt = 0;
		OSDelay(OS_TICK_HZ);
	}
}

