/*
 * 文件路径：
 *      BSP/Bsp_Global.h
 * 修改时间：
 *      2013-09-14
 * 作者：
 *      ET TEAM (c) http://et.ecuster.com
 * 联系：
 *      ecuster_et@163.com
 */
#ifndef _BSP_BSP_GLOBAL_H_
#define _BSP_BSP_GLOBAL_H_

#include "stm32f4xx_gpio.h"

#define RS232_REC_BUFF_SIZE				57

#define RS232_START_FLAG1	'T'			//RS232一桢数据起始标志1 
#define RS232_START_FLAG2	'M'			//RS232一桢数据起始标志2 
#define RS232_START_FLAG3	0x0f			//RS232一桢数据起始标志3 
#define RS232_START_FLAG4	0x01			//RS232一桢数据起始标志4 
#define RS232_START_FLAG5	0x00			//RS232一桢数据起始标志5 
#define RS232_START_FLAG6	0x31			//RS232一桢数据起始标志6 

#define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_7)	//接收使能
#define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_7) 	//发送使能
#define RS485_REC_BUFF_SIZE				18
#define RS485_START_FLAG1	'{'			//RS485一桢数据起始标志1 
#define RS485_START_FLAG2 '}'
#define RS485_END_FLAG1   '['
#define RS485_END_FLAG2	  ']'			//RS485一桢数据结束标志1

#define Light_OFF 		  GPIO_ResetBits(GPIOD , GPIO_Pin_15)
#define Light_ON 		    GPIO_SetBits(GPIOD , GPIO_Pin_15)

void BSP_Init(void);
extern unsigned char RS485_DATA_FLAG;    //RS485接收正确惯导数据的标志位
extern volatile unsigned char RS485_buff[RS485_REC_BUFF_SIZE] ;//用于接收数据
extern volatile unsigned int RS485_rec_counter ;//用于RS485接收计数

extern unsigned char RS232_DATA_FLAG;    //RS232接收正确深度计数据的标志位
extern volatile unsigned char RS232_buff[114] ;//用于接收数据
extern volatile unsigned int RS232_rec_counter ;//用于RS232接收计数

void RS485_Send_Data(unsigned char *send_buff,unsigned int length);
static void RS485_Delay(unsigned int nCount);
void USART485_Configuration(void);
void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void USART232_Configuration(void);
void Light_pin_Configuration(void);

#endif
