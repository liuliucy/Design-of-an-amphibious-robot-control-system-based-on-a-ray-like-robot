#include "main.h"

#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

//复位DHT11

void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);    //PB11的初始化	
}

void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);    //PB11的初始化		
}

void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
  DHT11_DQ_OUT_0; 	//拉低DQ
  delay_ms(20);    	//拉低至少18ms
  DHT11_DQ_OUT_1; 	//DQ=1 
	bsp_DelayUS(30);     	//主机拉高20~40us
}
//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
unsigned char DHT11_Check(void) 	   
{   
	unsigned char retry=0;
	DHT11_IO_IN();//SET INPUT	 
  while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		bsp_DelayUS(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
  while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		bsp_DelayUS(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//从DHT11读取一个位
//返回值：1/0
unsigned char DHT11_Read_Bit(void) 			 
{
 	unsigned char retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		bsp_DelayUS(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		bsp_DelayUS(1);
	}
	bsp_DelayUS(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}
//从DHT11读取一个字节
//返回值：读到的数据
unsigned char DHT11_Read_Byte(void)    
{        
  unsigned char i,dat;
  dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：1,正常;0,读取失败
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi)    
{        
 	unsigned char buf[5];
	unsigned char i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 0;
	return 1;	    
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在  	 
unsigned char DHT11_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	DHT11_Rst();
	return DHT11_Check();
}

/* 定义GPIO端口 */
#define RCC_DQ		RCC_AHB1Periph_GPIOC
#define PORT_DQ		GPIOC
#define PIN_DQ		GPIO_Pin_8

#if 0 /* 库函数方式 */
	#define DQ_0()		GPIO_ResetBits(PORT_DQ, PIN_DQ)
	#define DQ_1()		GPIO_SetBits(PORT_DQ, PIN_DQ)

	/* 判断DQ输入是否为低 */
	#define DQ_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DQ, PIN_DQ) == Bit_RESET)
#else	/* 直接操作寄存器，提高速度 */
	#define DQ_0()		PORT_DQ->BSRRH = PIN_DQ
	#define DQ_1()		PORT_DQ->BSRRL = PIN_DQ

	/* 判断DQ输入是否为低 */
	#define DQ_IS_LOW()	((PORT_DQ->IDR & PIN_DQ) == 0)
#endif

/*
*********************************************************************************************************
*	函 数 名: bsp_InitDS18B20
*	功能说明: 配置STM32的GPIO和SPI接口，用于连接 DS18B20
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitDS18B20(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_DQ, ENABLE);

	DQ_1();

	/* 配置DQ为开漏输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		/* 设为开漏模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = PIN_DQ;
	GPIO_Init(PORT_DQ, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_Reset
*	功能说明: 复位DS18B20。 拉低DQ为低，持续最少480us，然后等待
*	形    参: 无
*	返 回 值: 0 失败； 1 表示成功
*********************************************************************************************************
*/
uint8_t DS18B20_Reset(void)
{
	/*
		复位时序, 见DS18B20 page 15

		首先主机拉低DQ，持续最少 480us
		然后释放DQ，等待DQ被上拉电阻拉高，约 15-60us
		DS18B20 将驱动DQ为低 60-240us， 这个信号叫 presence pulse  (在位脉冲,表示DS18B20准备就绪 可以接受命令)
		如果主机检测到这个低应答信号，表示DS18B20复位成功
	*/

	uint8_t i;
	uint16_t k;

//	DISABLE_INT();/* 禁止全局中断 */

	/* 复位2次，如果失败则返回0 */
	for (i = 0; i < 2; i++)
	{
		DQ_0();				/* 拉低DQ */
		bsp_DelayUS(520);	/* 延迟 520uS， 要求这个延迟大于 480us */
		DQ_1();				/* 释放DQ */

		bsp_DelayUS(15);	/* 等待15us */

		/* 检测DQ电平是否为低 */
		for (k = 0; k < 10; k++)
		{
			if (DQ_IS_LOW())
			{
				break;
			}
			bsp_DelayUS(10);	/* 等待65us */
		}
		if (k >= 10)
		{
			continue;		/* 失败 */
		}

		/* 等待DS18B20释放DQ */
		for (k = 0; k < 30; k++)
		{
			if (!DQ_IS_LOW())
			{
				break;
			}
			bsp_DelayUS(10);	/* 等待65us */
		}
		if (k >= 30)
		{
			continue;		/* 失败 */
		}

		break;
	}

//	ENABLE_INT();	/* 使能全局中断 */

	bsp_DelayUS(5);

	if (i >= 2)
	{
		return 0;
	}

	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_WriteByte
*	功能说明: 向DS18B20写入1字节数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DS18B20_WriteByte(uint8_t _val)
{
	/*
		写数据时序, 见DS18B20 page 16
	*/
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		DQ_0();
		bsp_DelayUS(2);

		if (_val & 0x01)
		{
			DQ_1();
		}
		else
		{
			DQ_0();
		}
		bsp_DelayUS(60);
		DQ_1();
		bsp_DelayUS(2);
		_val >>= 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadByte
*	功能说明: 向DS18B20读取1字节数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t DS18B20_ReadByte(void)
{
	/*
		写数据时序, 见DS18B20 page 16
	*/
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		read >>= 1;

		DQ_0();
		bsp_DelayUS(3);
		DQ_1();
		bsp_DelayUS(3);

		if (DQ_IS_LOW())
		{
			;
		}
		else
		{
			read |= 0x80;
		}
		bsp_DelayUS(60);
	}

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadTempReg
*	功能说明: 读温度寄存器的值（原始数据）
*	形    参: 无
*	返 回 值: 温度寄存器数据 （除以16得到 1摄氏度单位, 也就是小数点前面的数字)
*********************************************************************************************************
*/
int16_t DS18B20_ReadTempReg(void)
{
	uint8_t temp1, temp2;

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0xcc);	/* 发命令 */
	DS18B20_WriteByte(0x44);	/* 发转换命令 */

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0xcc);	/* 发命令 */
	DS18B20_WriteByte(0xbe);

	temp1 = DS18B20_ReadByte();	/* 读温度值低字节 */
	temp2 = DS18B20_ReadByte();	/* 读温度值高字节 */

	return ((temp2 << 8) | temp1);	/* 返回16位寄存器值 */
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadID
*	功能说明: 读DS18B20的ROM ID， 总线上必须只有1个芯片
*	形    参: _id 存储ID
*	返 回 值: 0 表示失败， 1表示检测到正确ID
*********************************************************************************************************
*/
uint8_t DS18B20_ReadID(uint8_t *_id)
{
	uint8_t i;

	/* 总线复位 */
	if (DS18B20_Reset() == 0)
	{
		return 0;
	}

	DS18B20_WriteByte(0x33);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		_id[i] = DS18B20_ReadByte();
	}

	DS18B20_Reset();		/* 总线复位 */
	
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: DS18B20_ReadTempByID
*	功能说明: 读指定ID的温度寄存器的值（原始数据）
*	形    参: 无
*	返 回 值: 温度寄存器数据 （除以16得到 1摄氏度单位, 也就是小数点前面的数字)
*********************************************************************************************************
*/
int16_t DS18B20_ReadTempByID(uint8_t *_id)
{
	uint8_t temp1, temp2;
	uint8_t i;

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0x55);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteByte(_id[i]);
	}
	DS18B20_WriteByte(0x44);	/* 发转换命令 */

	DS18B20_Reset();		/* 总线复位 */

	DS18B20_WriteByte(0x55);	/* 发命令 */
	for (i = 0; i < 8; i++)
	{
		DS18B20_WriteByte(_id[i]);
	}	
	DS18B20_WriteByte(0xbe);

	temp1 = DS18B20_ReadByte();	/* 读温度值低字节 */
	temp2 = DS18B20_ReadByte();	/* 读温度值高字节 */

	return ((temp2 << 8) | temp1);	/* 返回16位寄存器值 */
}
/***********************************************************************
函数名称：unsigned char Read_ROM_ID(unsigned char * data)
功    能：读1820的ROM id 返回1则读成功，id在data所指位置，返回0失败，将data指定地址清0
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
unsigned char Read_ROM_ID(unsigned char * data)
{
	/* 读 ROM ID */
	uint8_t id[8];
	uint8_t ret;
	ret = DS18B20_ReadID(id);
	if (ret == 0)
	{
		memset(data, 0, 8);
		return 0;
	}
	else
	{
		memcpy(data, id, 8);
		return 1;
	}
}

