/***********************************************************************
文件名称：EXIT.C
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "main.h"	

extern unsigned char LouShui_FLAG1;      //阀箱漏水开关量信号1

/***********************************************************************
函数名称：void EXTI_Configuration(void)
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	/* 使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);

	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{	
		LouShui_FLAG1 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9);
		EXTI_ClearITPendingBit(EXTI_Line9);      /* 清除中断标志位 */		
	}
}

