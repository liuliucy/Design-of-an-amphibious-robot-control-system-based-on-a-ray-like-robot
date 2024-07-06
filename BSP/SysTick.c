/* Includes ------------------------------------------------------------------*/
#include "main.h"

 __IO uint32_t TimingDelay;
unsigned char time_flag = 0;
  
/***********************************************************************
函数名称：SysTick_Configuration(void)
功    能： 1ms中断一次
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
函数名称：Delay(__IO uint32_t nTime)
功    能： 延时，单位ms
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
函数名称：TimingDelay_Decrement(void)
功    能： 延时计数，
***********************************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
/***********************************************************************
函数名称：SysTick_Handler(void)
功    能： SysTick中断函数
***********************************************************************/
void SysTick_Handler(void)
{	
	
	TimingDelay_Decrement();//用于延时
	
}

void mydelay_ms(__IO uint32_t n)
{
	volatile INT32U i,j;
	//STM32F407ZG 168MHz 延时
	while(n--){
		for(i=0;i<47;i++){
			for(j=0;j<900;j++);
		}
	}
}
