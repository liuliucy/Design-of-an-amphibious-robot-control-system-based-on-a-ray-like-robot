/***********************************************************************
�ļ����ƣ�
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "main.h"
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
extern unsigned char RS232_DATA_FLAG;    //RS232������ȷ��ȼ����ݵı�־λ
volatile unsigned char RS232_buff[114];//���ڽ�������
volatile unsigned int RS232_rec_counter = 0;//����RS232���ռ���

extern unsigned char k;
extern unsigned char buff[57];
/*
 * ���ܣ�
 *       �弶��ʼ��
 * ������
 *       ��
 * ���أ�
 *       ��
 * ˵����
 *       ������ģ���г�ʼ���ľ���ʹ��ģ���ʼ��
 */
void BSP_Init(void)
{
	NVIC_Configuration();
//	SCB->VTOR=FLASH_BASE|0x20000;
}


void USART232_Configuration(void)
{ 
	
	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	USART_DeInit(USART1);
	
	USART_StructInit(&USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);        //�ܽ�PA9����ΪUSART1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                                                                                                 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	USART_ClockInit(USART1,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = 115200; //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; //��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure); 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        ///////�����ж�ʹ��
	USART_ClearITPendingBit(USART1, USART_IT_TC);//����ж�TCλ
	USART_Cmd(USART1,ENABLE);//���ʹ�ܴ���
}

//RS232�������ݵ��жϷ������
void USART1_IRQHandler(void)  
{
	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		RS232_buff[RS232_rec_counter] = USART1->DR;//
		RS232_rec_counter ++;

	}	
				if(RS232_rec_counter >= 57)	//ֻ�н��յ�2���������ϲ����ж�
		{
			if(RS232_buff[RS232_rec_counter - 56] == RS232_START_FLAG1 && RS232_buff[RS232_rec_counter - 55] == RS232_START_FLAG2 && 
				 RS232_buff[RS232_rec_counter - 54] == RS232_START_FLAG3 && RS232_buff[RS232_rec_counter - 53] == RS232_START_FLAG4 )	//֡��ʼ��־ 			
			{
				RS232_DATA_FLAG = 1;
			}
			else RS232_DATA_FLAG=0;
		}
	
			if(RS232_DATA_FLAG == 1)
		{
			for(k=0;k<57;k++)
			{
				buff[k]=RS232_buff[RS232_rec_counter - 56+k];
			}
			RS232_DATA_FLAG =0;
		}

		if(RS232_rec_counter == 113)//�������ջ�������С
		{
			RS232_rec_counter = 0;
		}
		OSIntExit();
}

//RS232�������ݷ���
void RS232_Send_Data(unsigned char *send_buff,unsigned int length)
{
 	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = send_buff[i];
		while((USART1->SR&0X40)==0);	
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}	
}




void Light_pin_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	Light_OFF;     //�ر�ˮ�µ�
}


