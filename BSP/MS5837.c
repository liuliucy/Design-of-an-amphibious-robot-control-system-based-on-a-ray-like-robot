#include "MS5837.h"
#include "math.h"
#include "SysTick.h"
//#include "dac.h"

//��ѹ��״̬��
#define SCTemperature    0x01	  //��ʼ�¶�ת��
#define CTemperatureing  0x02 //����ת���¶�
#define SCPressure  		 0x03	    //��ʼ��ѹת��
#define SCPressureing    0x04	  //����ת����ѹ


 
/*
C1 ѹ�������� SENS|T1
C2  ѹ������  OFF|T1
C3	�¶�ѹ��������ϵ�� TCS
C4	�¶�ϵ����ѹ������ TCO
C5	�ο��¶� T|REF
C6 	�¶�ϵ�����¶� TEMPSENS
*/
uint32_t  Cal_C[7];	        //���ڴ��PROM�е�6������1-6

double OFF_;
float Aux;
/*
dT ʵ�ʺͲο��¶�֮��Ĳ���
TEMP ʵ���¶�	
*/
uint64_t dT,TEMP;
uint8_t NEW_TEMP;
/*
OFF ʵ���¶Ȳ���
SENS ʵ���¶�������
*/
uint64_t OFf,SENS;
uint32_t D1_Pres,D2_Temp;	// ����ѹ��ֵ,�����¶�ֵ

uint32_t Pressure,Pressure_old,qqp,Wdodo;				//����ѹ
uint32_t TEMP2,T2,OFF2,SENS2;	//�¶�У��ֵ
uint32_t Pres_BUFFER[20];     //������
uint32_t Temp_BUFFER[10];     //������
uint32_t depth;




/*******************************************************************************
  * @��������	MS561101BA_RESET
  * @����˵��   ��λMS5611
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void MS583703BA_RESET(void)
{
		IIC_Start();
		IIC_Send_Byte(0xEC);//CSB�ӵأ�������ַ��0XEE������ 0X77
	  IIC_Wait_Ack();
    IIC_Send_Byte(0x1E);//���͸�λ����
	  IIC_Wait_Ack();
    IIC_Stop();
	
}
/*******************************************************************************
  * @��������	MS5611_init
  * @����˵��   ��ʼ��5611
  * @�������  	��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
u8 MS5837_init(void)
 {	 
  u8  inth,intl;
  int i;
  for (i=1;i<=6;i++) 
	{
 
		IIC_Start();
    IIC_Send_Byte(0xEC);
		IIC_Wait_Ack();
		IIC_Send_Byte(0xA0 + (i*2));
		IIC_Wait_Ack();
    IIC_Stop();
		bsp_DelayUS(5);
		IIC_Start();
		IIC_Send_Byte(0xEC+0x01);  //�������ģʽ
		bsp_DelayUS(1);
		IIC_Wait_Ack();
		inth = IIC_Read_Byte(1);  		//��ACK�Ķ�����
		bsp_DelayUS(1);
		intl = IIC_Read_Byte(0); 			//���һ���ֽ�NACK
		IIC_Stop();
    Cal_C[i] = (((uint16_t)inth << 8) | intl);
	}
	 return !Cal_C[0];
 }


/**************************ʵ�ֺ���********************************************
*����ԭ��:unsigned long MS561101BA_getConversion(void)
*��������:    ��ȡ MS561101B ��ת����� 
*******************************************************************************/
unsigned long MS583703BA_getConversion(uint8_t command)
{
 
			unsigned long conversion = 0;
			u8 temp[3];
	
	    IIC_Start();
			IIC_Send_Byte(0xEC); 		//д��ַ
			IIC_Wait_Ack();
			IIC_Send_Byte(command); //дת������
			IIC_Wait_Ack();
			IIC_Stop();

			bsp_DelayUS(10000);
			IIC_Start();
			IIC_Send_Byte(0xEC); 		//д��ַ
			IIC_Wait_Ack();
			IIC_Send_Byte(0);				// start read sequence
			IIC_Wait_Ack();
			IIC_Stop();
		 
			IIC_Start();
			IIC_Send_Byte(0xEC+0x01);  //�������ģʽ
			IIC_Wait_Ack();
			temp[0] = IIC_Read_Byte(1);  //��ACK�Ķ�����  bit 23-16
			temp[1] = IIC_Read_Byte(1);  //��ACK�Ķ�����  bit 8-15
			temp[2] = IIC_Read_Byte(0);  //��NACK�Ķ����� bit 0-7
			IIC_Stop();
			
			conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
			return conversion;
 
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:void MS561101BA_GetTemperature(void)
*��������:    ��ȡ �¶�ת����� 
*******************************************************************************/

void MS583703BA_getTemperature(void)
{
	
	D2_Temp = MS583703BA_getConversion(0x58);
	bsp_DelayUS(10000);
	dT=D2_Temp - (((uint32_t)Cal_C[5])*256);//657e87
	TEMP=2000+dT*((uint32_t)Cal_C[6])/8388608;
}

///***********************************************
//  * @brief  ��ȡ��ѹ
//  * @param  None
//  * @retval None
//************************************************/
void MS583703BA_getPressure(void)
{
	D1_Pres= MS583703BA_getConversion(0x48);
//	bsp_DelayUS(1000);
	
	OFF_=(uint32_t)Cal_C[2]*65536+((uint32_t)Cal_C[4]*dT)/128;
	SENS=(uint32_t)Cal_C[1]*32768+((uint32_t)Cal_C[3]*dT)/256;

	if(TEMP<2)  // low temp
	{
		Aux = (2000-TEMP)*(2000-TEMP);
		T2 = 3*(dT*dT) /0x80000000; 
		OFF2 = 1.5*Aux;
		SENS2 = 5*Aux/8;
		
	
		OFF_ = OFF_ - OFF2;
		SENS = SENS - SENS2;	
	}
	 else{
	  T2=2*(dT*dT)/137438953472;
		OFF2 = 1*Aux/16;
		SENS2 = 0;
		OFF_ = OFF_ - OFF2;
		SENS = SENS - SENS2;	
		 
	 }
  Pressure= ((D1_Pres*SENS/2097152-OFF_)/8192)/10;
	TEMP=(TEMP-T2)/100;
//	NEW_TEMP=TEMP;
	Wdodo=4095*(Pressure-900)/2600;
//	DAC_SetChannel1Data(DAC_Align_12b_R,Wdodo); 
}


