/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
 
#ifndef _MAIN_H_
#define _MAIN_H_


//添加必要的头文件
#include "stm32f4xx.h" 
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "lwip/tcp.h"
#include "netconf.H"
#include "M_Type_P.h"
#include "M_CMSIS_P.h" 
#include "M_Global.h"
#include "Task_Startup.h"
#include "Bsp_Global.h"
#include "Task_Test.h"
#include "led.h"   
#include "TCP_SERVER.h"
#include "DHT11.h"
#include "NVIC.h"
#include "CAN.h"
#include "ADC.h"
#include "M_Delay_P.h"
#include "EXIT.h"
#include "tcp.h"
#include "SysTick.h"
#include "stdio.H"
#include "MS5837.h"
#include "myiic.h"
#include "math.h"

#include <string.h>

//ET Module版本号
#define M_VERSION	100
#define M_DEV_MCU  1
#define RMII_MODE  1

//全局初始化函数声明
void M_Global_init(void);
void Delay(uint32_t nCount);


#endif
