#ifndef _DHT11_H_
#define _DHT11_H_

#include <stdint.h>

#define DHT11_DQ_OUT_0  GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define DHT11_DQ_OUT_1  GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define DHT11_DQ_IN GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)

void bsp_InitDS18B20(void);
int16_t DS18B20_ReadTempReg(void);
int16_t DS18B20_ReadTempByID(uint8_t *_id);
uint8_t DS18B20_ReadID(uint8_t *_id);
unsigned char Read_ROM_ID(unsigned char * data);

unsigned char DHT11_Init(void);//初始化DHT11
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi);//读取温湿度
unsigned char DHT11_Read_Byte(void);//读出一个字节
unsigned char DHT11_Read_Bit(void);//读出一个位
unsigned char DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    

#endif
