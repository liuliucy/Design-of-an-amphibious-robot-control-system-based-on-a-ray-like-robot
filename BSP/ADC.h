#ifndef _ADC_H_
#define _ADC_H_
#include<stdint.h>
#define ADC_CHANNLE_NUM					16

extern uint16_t ADC_RCVTab[ADC_CHANNLE_NUM];
void ADC_Configuration(void);

unsigned short int Get_Adc(unsigned char ch); 				                    //获得某个通道值 
unsigned short int Get_Adc_Average(unsigned char ch,unsigned char times);//得到某个通道给定次数采样的平均值  

#endif
