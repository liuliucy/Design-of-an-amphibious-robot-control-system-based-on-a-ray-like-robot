/* Includes ------------------------------------------------------------------*/
#include "main.h"

 __IO uint32_t TimingDelay;
unsigned char time_flag = 0;
  
/***********************************************************************
�������ƣ�SysTick_Configuration(void)
��    �ܣ� 1ms�ж�һ��
***********************************************************************/
void SysTick_Configuration(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}

/***********************************************************************
�������ƣ�Delay(__IO uint32_t nTime)
��    �ܣ� ��ʱ����λms
***********************************************************************/
void Delay(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;
	while(TimingDelay != 0);
}
void delay_ms(__IO uint32_t nTime)
{ 
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void delay_us(__IO uint32_t nTime)
{ 
	if (SysTick_Config(SystemCoreClock / 1000000))
	{ 
		/* Capture error */ 
		while (1);
	}	
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void bsp_DelayUS(__IO uint32_t nTime)
{ 
	unsigned int i = 0;
	
	while(nTime --)
	{
		for(i = 0;i < 100;i ++)
		{
			;
		}
	}
}
/***********************************************************************
�������ƣ�TimingDelay_Decrement(void)
��    �ܣ� ��ʱ������
***********************************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/***********************************************************************
�������ƣ�SysTick_Handler(void)
��    �ܣ� SysTick�жϺ���
***********************************************************************/
void SysTick_Handler(void)
{	
	
	TimingDelay_Decrement();//������ʱ
	
}

void mydelay_ms(__IO uint32_t n)
{
	volatile INT32U i,j;
	//STM32F407ZG 168MHz ��ʱ
	while(n--){
		for(i=0;i<47;i++){
			for(j=0;j<900;j++);
		}
	}
}
