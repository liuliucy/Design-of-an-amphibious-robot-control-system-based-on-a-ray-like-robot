#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "core_cm4.h"

extern unsigned char time_flag;

void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void delay_ms(__IO uint32_t nTime);
void delay_us(__IO uint32_t nTime);
void bsp_DelayUS(__IO uint32_t nTime);
void mydelay_ms(__IO uint32_t n);
#endif
