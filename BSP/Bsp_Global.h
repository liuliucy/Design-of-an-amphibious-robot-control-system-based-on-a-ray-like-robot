/*
 * �ļ�·����
 *      BSP/Bsp_Global.h
 * �޸�ʱ�䣺
 *      2013-09-14
 * ���ߣ�
 *      ET TEAM (c) http://et.ecuster.com
 * ��ϵ��
 *      ecuster_et@163.com
 */
#ifndef _BSP_BSP_GLOBAL_H_
#define _BSP_BSP_GLOBAL_H_

#include "stm32f4xx_gpio.h"

#define RS232_REC_BUFF_SIZE				57

#define RS232_START_FLAG1	'T'			//RS232һ��������ʼ��־1 
#define RS232_START_FLAG2	'M'			//RS232һ��������ʼ��־2 
#define RS232_START_FLAG3	0x0f			//RS232һ��������ʼ��־3 
#define RS232_START_FLAG4	0x01			//RS232һ��������ʼ��־4 
#define RS232_START_FLAG5	0x00			//RS232һ��������ʼ��־5 
#define RS232_START_FLAG6	0x31			//RS232һ��������ʼ��־6 

#define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_7)	//����ʹ��
#define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_7) 	//����ʹ��
#define RS485_REC_BUFF_SIZE				18
#define RS485_START_FLAG1	'{'			//RS485һ��������ʼ��־1 
#define RS485_START_FLAG2 '}'
#define RS485_END_FLAG1   '['
#define RS485_END_FLAG2	  ']'			//RS485һ�����ݽ�����־1

#define Light_OFF 		  GPIO_ResetBits(GPIOD , GPIO_Pin_15)
#define Light_ON 		    GPIO_SetBits(GPIOD , GPIO_Pin_15)

void BSP_Init(void);
extern unsigned char RS485_DATA_FLAG;    //RS485������ȷ�ߵ����ݵı�־λ
extern volatile unsigned char RS485_buff[RS485_REC_BUFF_SIZE] ;//���ڽ�������
extern volatile unsigned int RS485_rec_counter ;//����RS485���ռ���

extern unsigned char RS232_DATA_FLAG;    //RS232������ȷ��ȼ����ݵı�־λ
extern volatile unsigned char RS232_buff[114] ;//���ڽ�������
extern volatile unsigned int RS232_rec_counter ;//����RS232���ռ���

void RS485_Send_Data(unsigned char *send_buff,unsigned int length);
static void RS485_Delay(unsigned int nCount);
void USART485_Configuration(void);
void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void USART232_Configuration(void);
void Light_pin_Configuration(void);

#endif
