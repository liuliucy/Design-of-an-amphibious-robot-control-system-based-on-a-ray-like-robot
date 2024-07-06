#include "myiic.h"
#include "stm32f4xx.h"


static void i2c_Delay(void)
{
	uint8_t i;

	/*��
		�����߼������ǲ���I2CͨѶʱ��Ƶ��
    ����������CPU��Ƶ168MHz ��MDK���뻷����1���Ż�
  
		�����ԣ�ѭ������Ϊ20~250ʱ����ͨѶ����

	*/
	for (i = 0; i < 40; i++);
}
static void i2c_Delay2(void)
{
	uint8_t i;

	/*��
		�����߼������ǲ���I2CͨѶʱ��Ƶ��
    ����������CPU��Ƶ168MHz ��MDK���뻷����1���Ż�
  
		�����ԣ�ѭ������Ϊ20~250ʱ����ͨѶ����

	*/
	for (i = 0; i < 20; i++);
}
static void i2c_Delay1(void)
{
	uint8_t i;

	/*��
		�����߼������ǲ���I2CͨѶʱ��Ƶ��
    ����������CPU��Ƶ168MHz ��MDK���뻷����1���Ż�
  
		�����ԣ�ѭ������Ϊ20~250ʱ����ͨѶ����

	*/
	for (i = 0; i < 10; i++);
}

//��ʼ��IIC
void IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
 
	IIC_SCL_1;
	IIC_SDA_1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_1;	  	  
	IIC_SCL_1;
	i2c_Delay();
 	IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
	i2c_Delay();
	IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_0;
	IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
	i2c_Delay();
	IIC_SCL_1; 
	IIC_SDA_1;//����I2C���߽����ź�
	i2c_Delay();						   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
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
	i2c_Delay2();  //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1;
	i2c_Delay2(); 
		IIC_SCL_0;	
	i2c_Delay2();
	}	 
}
 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}



























