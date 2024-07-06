/***********************************************************************
�ļ����ƣ�main.C
��    �ܣ�TCP������ģʽ�շ����ԣ�����UC0S II����ϵͳ
ʵ��ƽ̨������STM32F407VET6 ������
��汾  ��V1.0.0 
***********************************************************************/
#include "main.h"
#include "232.h"

int main(void)
{
	//��ʼ������
	BSP_Init();
	//��ʼ��ϵͳ
	OSInit();
	//������������
	OSTaskCreate(	Task_StartUp,               		    				//ָ����������ָ��
                  	(void *) 0,												//����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��
					(OS_STK *)&Stk_TaskStartUp[TASK_STARTUP_STK_SIZE - 1],	//���������Ķ�ջ��ջ��ָ��   �Ӷ����µݼ�
					(INT8U) OS_USER_PRIO_LOWEST);							//�������������ȼ�  

	//���ļ�������0  
	OSTimeSet(0);

	//����UCOS-II�ں�
	OSStart();

	return 0;
}
//���������ջ
OS_STK Stk_TaskStartUp[TASK_STARTUP_STK_SIZE];

void Task_StartUp(void *pdata)
{

	//��ʼ��UCOSʱ��
	//OS_TICKS_PER_SEC Ϊ UCOS-II ÿ�������
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
