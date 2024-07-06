/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#ifndef _APP_TASK_TEST_H_
#define _APP_TASK_TEST_H_

#define TASK_TEST_LED_STK_SIZE	400
#define TASK_TCP_SERVER_STK_SIZE	4000
#define TASK_TCPData_Processing_STK_SIZE 400
#define TASK_CAN_STK_SIZE 400
#define TASK_ADC_STK_SIZE 400
#define TASK_Light_STK_SIZE 400
#define TASK_DHT11_STK_SIZE 600

extern OS_STK Stk_Task_LED[TASK_TEST_LED_STK_SIZE];
extern OS_STK Stk_Task_TCP_Server[TASK_TCP_SERVER_STK_SIZE];
extern OS_STK Stk_Task_TCPData_Processing[TASK_TCPData_Processing_STK_SIZE];
extern OS_STK Stk_Task_CAN[TASK_CAN_STK_SIZE];
extern OS_STK Stk_Task_ADC[TASK_ADC_STK_SIZE];
extern OS_STK Stk_Task_Light[TASK_Light_STK_SIZE];
extern OS_STK Stk_Task_DHT11[TASK_DHT11_STK_SIZE];

void Task_LED(void *pdata);
void Task_TCP_Server(void *pdata);
void Task_TCPData_Processing(void *pdata);
void Task_CAN(void *pdata);
void Task_ADC(void *pdata);
void Task_Light(void *pdata);
void Task_DHT11(void *pdata);

#endif
