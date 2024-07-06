#ifndef __MYIIC_H
#define __MYIIC_H
#include "stdint.h"
#include "stm32f4xx.h"


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/10 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

   	   		   
//IO��������
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}

//IO��������	 
//#define IIC_SCL    PBout(8) //SCL
//#define IIC_SDA    PBout(9) //SDA	 
//#define READ_SDA   PBin(9)  //����SDA 
#define IIC_SCL 		GPIOB , GPIO_Pin_8
#define IIC_SDA  		GPIOB , GPIO_Pin_9
#define IIC_SCL_0 	GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define IIC_SCL_1 	GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define IIC_SDA_0 	GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define IIC_SDA_1 	GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define IIC_SDA_READ 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define IIC_SDA_OUT 	GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9)
 
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);								//����IIC��ʼ�ź�
void IIC_Stop(void);	  						//����IICֹͣ�ź�
void IIC_Send_Byte(unsigned char txd);					//IIC����һ���ֽ�
unsigned char IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void IIC_Ack(void);									//IIC����ACK�ź�
void IIC_NAck(void);								//IIC������ACK�ź�

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	 

#endif














