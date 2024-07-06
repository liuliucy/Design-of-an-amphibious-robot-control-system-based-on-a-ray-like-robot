#ifndef __MYIIC_H
#define __MYIIC_H
#include "stdint.h"
#include "stm32f4xx.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

   	   		   
//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}

//IO操作函数	 
//#define IIC_SCL    PBout(8) //SCL
//#define IIC_SDA    PBout(9) //SDA	 
//#define READ_SDA   PBin(9)  //输入SDA 
#define IIC_SCL 		GPIOB , GPIO_Pin_8
#define IIC_SDA  		GPIOB , GPIO_Pin_9
#define IIC_SCL_0 	GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define IIC_SCL_1 	GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define IIC_SDA_0 	GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define IIC_SDA_1 	GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define IIC_SDA_READ 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define IIC_SDA_OUT 	GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9)
 
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);								//发送IIC开始信号
void IIC_Stop(void);	  						//发送IIC停止信号
void IIC_Send_Byte(unsigned char txd);					//IIC发送一个字节
unsigned char IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char IIC_Wait_Ack(void); 							//IIC等待ACK信号
void IIC_Ack(void);									//IIC发送ACK信号
void IIC_NAck(void);								//IIC不发送ACK信号

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	 

#endif














