
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