### 1 简单的实时嵌入式操作系统
* 参考ucos
* 最大优势：足够简单、代码量很小
* 支持最多31个任务，不支持时间片
* 支持基本的操作系统功能，如任务挂起和恢复、事件。其他的不支持

### 2 将本OS添加到工程 
1. 将OS文件夹复制到工程文件夹下
2. 在keil中将OS及其中文件添加到工程
3. 将OS添加到Include Paths，使keil能找到os.h等头文件
4. 将stm32f1xx_it.c中的PendSV_Handler函数的定义删除
    （或者在PendSV_Handler前加上__weak关键字）
5. 在main.c里引入os.h
6. 在stm32f1xx_it.c中引入os.h
7. 在SysTick_Handler函数的结尾处调用OSTimeTick()
8. 在main函数的while(1)循环开始前添加
    ```
    OSInit();
    OSStart();
    ```
9. 如果用了HAL库，则在main函数`SystemClock_Config();`后 添加
    ```
    SysTick->CTRL = 0x00;
    ```
    防止滴答定时器在系统开始前产生中断

### 3 使用OS
#### 1 创建任务
仅需两步就可创建新任务：
1. 在tasks.h里声明任务tcb：
```
#define taska_SIZE 256
#define taskb_SIZE 256

#define taska_PRIO 0
#define taskb_PRIO 1

#define DECLARE_TASKS()\
DECLARE_A_TASK(taska);\
DECLARE_A_TASK(taskb);\

#define DECLARE_TCBS()\
DECLARE_TCB(taska),\
DECLARE_TCB(taskb)

```
这样就创建了两个任务：taska和taskb。
如果想要创建新的名为test的任务，需要添加四处：
a. `#define test_SIZE 256` 
b. `#define test_PRIO 3`
c. `DECLARE_A_TASK(test);\`
d. `DECLARE_TCB(taska),\`
2. 在tasks.c里添加任务入口函数：
```
void test(void) {
    while(1) {
        ...
    }
}
```
注意名称与前面一致。这样就创建好了一个叫做test的任务。
#### 2 任务挂起和恢复
```
void OSTaskSuspend(OSTcb * ptcb);   //任务挂起
void OSTaskResume(OSTcb * ptcb);    //任务恢复
```
#### 3 事件
事件可用来任务之间同步
```
void OSFlagPost(OSFlag * pflag, uint32 flag, uint32 opt);   //发布事件
void OSFlagPend(OSFlag * pflag, uint32 flag, uint32 opt);   //等待事件
```
如
```
void taska(void) {
    while(1) {
        OSFlagPost(&test1_FLAG, 0x01, OS_FLAG_SET_ANY);
        OSDelay(10);
    }
}


void taskb(void) {
    while(1) {
        OSFlagPend(&test1_FLAG, 0x01, OS_FLAG_GET_CLR);
    }
}
```
代表taska发布0x01的事件，taskb接受0x01的事件。这样一来，taskb就和taska保持同步了。