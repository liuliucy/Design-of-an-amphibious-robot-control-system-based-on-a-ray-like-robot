#include "myiic.h"
#include "stm32f4xx.h"


static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频168MHz ，MDK编译环境，1级优化
  
		经测试，循环次数为20~250时都能通讯正常

	*/
	for (i = 0; i < 40; i++);
}
static void i2c_Delay2(void)
{
	uint8_t i;

	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频168MHz ，MDK编译环境，1级优化
  
		经测试，循环次数为20~250时都能通讯正常

	*/
	for (i = 0; i < 20; i++);
}
static void i2c_Delay1(void)
{
	uint8_t i;

	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频168MHz ，MDK编译环境，1级优化
  
		经测试，循环次数为20~250时都能通讯正常

	*/
	for (i = 0; i < 10; i++);
}

//初始化IIC
void IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
 
	IIC_SCL_1;
	IIC_SDA_1;
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	i2c_Delay();
 	IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	i2c_Delay();
	IIC_SCL_0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_0;
	IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
	i2c_Delay();
	IIC_SCL_1; 
	IIC_SDA_1;//发送I2C总线结束信号
	i2c_Delay();						   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_1;	i2c_Delay1();;	   
	IIC_SCL_1;	i2c_Delay1();;	 
	while(IIC_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_0;
		i2c_Delay2();
	IIC_SCL_1;
	i2c_Delay2();
	IIC_SCL_0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_1;
	i2c_Delay2();
	IIC_SCL_1;
	i2c_Delay();
	IIC_SCL_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
    if(txd&0x80)
			{
				IIC_SDA_1;
			}				
			else
			{
				IIC_SDA_0;
			}
			txd<<=1;
	i2c_Delay2();  //对TEA5767这三个延时都是必须的
		IIC_SCL_1;
	i2c_Delay2(); 
		IIC_SCL_0;	
	i2c_Delay2();
	}	 
}
 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_0; 
        	i2c_Delay2();
		IIC_SCL_1;
        receive<<=1;
        if(IIC_SDA_READ)receive++;   
			i2c_Delay1(); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























