#ifndef _ADC_H_
#define _ADC_H_
#include<stdint.h>
#define ADC_CHANNLE_NUM					16

extern uint16_t ADC_RCVTab[ADC_CHANNLE_NUM];
void ADC_Configuration(void);

unsigned short int Get_Adc(unsigned char ch); 				                    //���ĳ��ͨ��ֵ 
unsigned short int Get_Adc_Average(unsigned char ch,unsigned char times);//�õ�ĳ��ͨ����������������ƽ��ֵ  

#endif
