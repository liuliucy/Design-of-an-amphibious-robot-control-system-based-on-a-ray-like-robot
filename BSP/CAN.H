#ifndef _CAN_H_
#define _CAN_H_

#define CAN_BAUD_NUM    18		//可用配置波特率个数
#include "stm32f4xx.h"

void CAN1_Configuration(void);
void can1_tx(unsigned int ID);
void CAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure);
#endif
