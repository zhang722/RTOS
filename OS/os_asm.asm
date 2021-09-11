	EXTERN OSTCBCurPtr
	EXTERN OSTCBNextPtr

	EXPORT OSStart
	EXPORT OSContextSwitch
	EXPORT PendSV_Handler
	EXPORT OSLock
	EXPORT OSUnlock
	EXPORT CPU_CntLeadZeros
		
NVIC_INT_CTRL   EQU     0xE000ED04                              ; Interrupt control state register.
NVIC_SYSPRI14   EQU     0xE000ED22                              ; System priority register (priority 14).
NVIC_PENDSV_PRI EQU         0xFFFF                              ; PendSV priority value (lowest).
NVIC_PENDSVSET  EQU     0x10000000                              ; Value to trigger PendSV exception.


	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8
		

CPU_CntLeadZeros
	CLZ     R0, R0                          ; Count leading zeros
	BX      LR

OSLock
	MRS     R0, PRIMASK         ; 保存中断屏蔽寄存器PRIMASK到R0中
	CPSID   I                   ; 关闭中断
	BX      LR
	
OSUnlock
	MSR     PRIMASK, R0         ; 把R0的值写回到PRIMASK恢复进入临界区前的状态
	BX      LR
	
OSStart
    LDR     R0, =NVIC_SYSPRI14                                  ; Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                                              ; Set the PSP to 0 for initial context switch call
    MSR     PSP, R0

    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                                                   ; Enable interrupts at processor level
	

OSContextSwitch
	LDR     R0, =NVIC_INT_CTRL										; 触发PendSV,进行上下文切换
	LDR     R1, =NVIC_PENDSVSET
	STR     R1, [R0]
	BX      LR


PendSV_Handler
	CPSID I         ;关中断
	MRS   R0, PSP   ;将PSP协寄存器移动到R0
	CBZ   R0, OSPendSVHandler_nosave    ;判断并分支，若R0=0，则说明是第一次切换，跳转到相应函数

	;如果是f4，则取消注释
	;TST   R14, #0x10 ;TST即按位与
	;IT    EQ          ;IT即If-Then,EQ即IT块中的语句需要加上的条件
	;VSTMDBEQ  R0!, {S16-S31}  ;VSTM即将VFP中的寄存器批量移动到Registers Bank
	;,DB代表每次移动前将待写入地址减4,!代表将减4后的地址重新写入R0,EQ是IT块中要加上的条件

	SUBS    R0, R0, #0x20   ;将R0减去0x20后保存至R0
	STM     R0, {R4-R11}    ;将R4-R11保存至R0指向的堆栈,参数省略代表之后递增

	LDR     R1, =OSTCBCurPtr  ;OSTCBCurPtr是全局变量
	LDR     R1, [R1]
	STR     R0, [R1]

OSPendSVHandler_nosave
	LDR   R0, =OSTCBCurPtr   ; gp_xtos_cur_task = gp_xtos_next_task
	LDR   R1, =OSTCBNextPtr
	LDR   R2, [R1]
	STR   R2, [R0]

	LDR   R0, [R2]   ; R0 = gp_xtos_next_task->pTopOfStack
	LDM   R0, {R4-R11}   ; 装载下文的Callee Saved Registers
	ADDS  R0, R0, #0x20

	;如果是f4，则取消注释
	;TST   R14, #0x10   ; 根据EXC_RETURN的bit4, 判定是否开启了FPU
	;IT    EQ    ; 若开启了, 则装载S16-S31
	;VLDMIAEQ R0!, {S16-S31} 

	MSR   PSP, R0   ; 更新PSP = gp_xtos_next_task->pTopOfStack
	ORR   LR, LR, #0x04   ; 确保返回时使用PSP作为栈指针

	CPSIE I   ; 开中断
	BX    LR
	NOP
	END
	  
