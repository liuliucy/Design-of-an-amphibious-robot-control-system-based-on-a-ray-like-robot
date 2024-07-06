#include "main.h"  
#include "math.h"

//#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
//#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

OS_STK Stk_Task_LED[TASK_TEST_LED_STK_SIZE];
OS_STK Stk_Task_TCP_Server[TASK_TCP_SERVER_STK_SIZE];
OS_STK Stk_Task_TCPData_Processing[TASK_TCPData_Processing_STK_SIZE];
OS_STK Stk_Task_Light[TASK_Light_STK_SIZE];
OS_STK Stk_Task_DHT11[TASK_DHT11_STK_SIZE];

unsigned char i,k;                		  //����forѭ��

unsigned char CANaccept[8];           //CAN���յ���ȷ����
unsigned char CANsend[8];             //CAN���͵���ȷ����
unsigned char TCPaccept[14];          //TCP���ܵ���ȷ����
unsigned char TCPsend[31];            //TCP���͵����ݰ�

unsigned char TCP_FLAG = 0;           //TCP������ȷ���ݵı�־λ

unsigned char RS232_DATA_FLAG = 0;    //RS232������ȷ��ȼ����ݵı�־λ

extern volatile unsigned char RS232_buff[114] ;//���ڽ�������
extern volatile unsigned int RS232_rec_counter ;//����RS232���ռ���
//unsigned char buff[57];

unsigned char DHT11_FLAG = 0;         //DHT11���ݲɼ���Ϻ�ı�־λ
//unsigned char rom_id[8] = {0};
//unsigned short int temperature = 0xff;            //DHT11�¶�����
unsigned char humidity = 0xff;               //DHT11ʪ������

unsigned char LouShui_FLAG1 = 0;      //©ˮ�������ź�1


//LED��˸����
void Task_LED(void *pdata)
{		
	LED_Configuration(); 
    while(1)
	{
		
		GPIO_ToggleBits(LED1);	
		GPIO_ToggleBits(LED2);
		GPIO_ToggleBits(LED3);
		OSTimeDlyHMSM(0, 0, 0, 100);		
	}
}
//tcp�������շ�����
void Task_TCP_Server(void *pdata)
{
	__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
	ETH_BSP_Config();
	LwIP_Init();

	/* TCP_server Init */
	TCP_server_init();
	while(1)
	{  
		LocalTime += 2;
		LwIP_Periodic_Handle(LocalTime);
		OSTimeDlyHMSM(0, 0, 0, 20);//����5ms���Ա������߳�����
	}
}
//��̫�������շ�ǰ��������,���յ��Ĵӿذ�CAN���ݡ�485�յ��Ĺߵ����ݡ�232�յ���������ݡ�DTH11��ʪ�ȴ���������������TCP���Ͱ�
void Task_TCPData_Processing(void *pdata)
{
	CAN1_Configuration();
	Light_pin_Configuration();
	GPS_init(9600);
	USART232_Configuration();
	IIC_Init();	         //��ʼ��IIC PC11 PC12����
	mydelay_ms(50);
	MS583703BA_RESET();	 // Reset Device  ��λMS5837
	mydelay_ms(50);
	MS5837_init();	     //��ʼ��MS5837
  while(1)
	{
//			MS583703BA_getTemperature();						 //��ȡ�¶�
			MS583703BA_getPressure();                //��ȡ�����
			if(TCPaccept[10] == 0xa9)
		 {
			  Light_ON;
		 }	
     else Light_OFF ;	
//			TCPsend[0] = 0xcd;                       //TCPsend[0]--TCPsend[1] TCP���Ͱ�ͷ+���ݳ���
//			TCPsend[1] = 0x32;
		
//			if(CAN_FLAG == 1)
//			{
//				CAN_FLAG = 0;
//				for(i=0;i<8;i++)                       //TCPsend[2]--TCPsend[9] �ӿ��ư巢��2�ֽڵ�Դ��ѹ+2�ֽڵ���+2�ֽ��¶�+1�ֽ�©ˮ+1�ֽ�FT
//				{
//					TCPsend[i+2] = 0;
//				}
//			}
//			
//			TCPsend[10] = (float)temperature/16 ;    //TCPsend[10]  �����¶ȴ洢��TCP���Ͱ��̶�λ��			
//			

//				for(i=0;i<12;i++)                      //TCPsend[11]--TCPsend[22] ��̬������
//				{
//					 TCPsend[i+11] = buff[i+7]; 					
//				}
//				for(i=0;i<12;i++)                      
//				{  
//					 TCPsend[i+23] = buff[i+31];   //TCPsend[23]--TCPsend[34] ���ٶȼ�����
//				}	
//					for(i=0;i<11;i++)
//				{
//					TCPsend[i+36]=0;				
//				}
//			
//			TCPsend[47]=TEMP;											//��ȼ��¶�
//			TCPsend[48]=(Pressure)&255;						//��ȵͰ�λ
//			TCPsend[49]=(Pressure>>8)&255;				//��ȸ߰�λ
//			TCPsend[50] = 0x55;                     //TCPsend[50]  ��β����
			
			OSTimeDlyHMSM(0, 0, 0, 100);//��200ms���Ա������߳�����
			}
}



//ˮ�µƿ�������  TCPaccept[0]Ϊ����ˮ�µƶ����Ŀ���λ
//void Task_Light(void *pdata)
//{
//	Light_pin_Configuration();
//  while(1)
//	{
//		 if(TCPaccept[10] == 0xa9)
//		 {
//			  Light_ON;
//		 }	
//     else Light_OFF ;		 
//		 OSTimeDlyHMSM(0, 0, 0, 100);//����5ms���Ա������߳�����
//	}			
//}

//DHT11����DS18B20��ʪ�����ݲɼ�
//void Task_DHT11(void *pdata)
//{
//  bsp_InitDS18B20();
//	DISABLE_INT();/* ��ֹȫ���ж� */
//  if(Read_ROM_ID(rom_id) == 0)
//	{
//		  TCPsend[8] = 0x99 ;				  //DS18B20��ȡIDʧ��	
//	}
//	ENABLE_INT();	/* ʹ��ȫ���ж� */
//  while(1)
//	{
//	   DISABLE_INT();/* ��ֹȫ���ж� */
//		 temperature = DS18B20_ReadTempByID(rom_id); 
//	   ENABLE_INT();	/* ʹ��ȫ���ж� */
//		 OSTimeDlyHMSM(0, 0, 1, 0);//����5ms���Ա������߳�����
//	}			
//}


