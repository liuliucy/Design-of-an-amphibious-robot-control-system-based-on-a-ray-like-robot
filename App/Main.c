/***********************************************************************
文件名称：main.C
功    能：TCP服务器模式收发测试，基于UC0S II操作系统
实验平台：基于STM32F407VET6 开发板
库版本  ：V1.0.0 
***********************************************************************/
#include "main.h"
#include "232.h"

int main(void)
{
	//初始化板子
	BSP_Init();
	//初始化系统
	OSInit();
	//创建启动任务
	OSTaskCreate(	Task_StartUp,               		    				//指向任务代码的指针
                  	(void *) 0,												//任务开始执行时，传递给任务的参数的指针
					(OS_STK *)&Stk_TaskStartUp[TASK_STARTUP_STK_SIZE - 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
					(INT8U) OS_USER_PRIO_LOWEST);							//分配给任务的优先级  

	//节拍计数器清0  
	OSTimeSet(0);

	//启动UCOS-II内核
	OSStart();

	return 0;
}
//启动任务堆栈
OS_STK Stk_TaskStartUp[TASK_STARTUP_STK_SIZE];

void Task_StartUp(void *pdata)
{

	//初始化UCOS时钟
	//OS_TICKS_PER_SEC 为 UCOS-II 每秒嘀嗒数
	SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC);
	 
	OSTaskCreate(Task_DHT11, (void *)0, &Stk_Task_DHT11[TASK_DHT11_STK_SIZE-1], OS_USER_PRIO_GET(6));
	OSTaskCreate(Task_TCP_Server, (void *)0, &Stk_Task_TCP_Server[TASK_TCP_SERVER_STK_SIZE-1], OS_USER_PRIO_GET(5));
  OSTaskCreate(Task_TCPData_Processing, (void *)0, &Stk_Task_TCPData_Processing[TASK_TCPData_Processing_STK_SIZE-1], OS_USER_PRIO_GET(7));
	//OSTaskCreate(Task_CAN, (void *)0, &Stk_Task_CAN[TASK_CAN_STK_SIZE-1], OS_USER_PRIO_GET(8));
	//OSTaskCreate(Task_Light, (void *)0, &Stk_Task_Light[TASK_Light_STK_SIZE-1], OS_USER_PRIO_GET(8));
	//OSTaskCreate(Task_ADC, (void *)0, &Stk_Task_ADC[TASK_ADC_STK_SIZE-1], OS_USER_PRIO_GET(10));
	OSTaskCreate(Task_LED, (void *)0, &Stk_Task_LED[TASK_TEST_LED_STK_SIZE-1], OS_USER_PRIO_GET(11));
	
	while (1)
	{
		OSTimeDlyHMSM(0, 0, 1, 0);//1000ms
	}
}
