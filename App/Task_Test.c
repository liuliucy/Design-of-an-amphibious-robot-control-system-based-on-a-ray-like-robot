#include "main.h"  
#include "math.h"

//#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
//#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

OS_STK Stk_Task_LED[TASK_TEST_LED_STK_SIZE];
OS_STK Stk_Task_TCP_Server[TASK_TCP_SERVER_STK_SIZE];
OS_STK Stk_Task_TCPData_Processing[TASK_TCPData_Processing_STK_SIZE];
OS_STK Stk_Task_Light[TASK_Light_STK_SIZE];
OS_STK Stk_Task_DHT11[TASK_DHT11_STK_SIZE];

unsigned char i,k;                		  //用于for循环

unsigned char CANaccept[8];           //CAN接收的正确数据
unsigned char CANsend[8];             //CAN发送的正确数据
unsigned char TCPaccept[14];          //TCP接受的正确数据
unsigned char TCPsend[31];            //TCP发送的数据包

unsigned char TCP_FLAG = 0;           //TCP接收正确数据的标志位

unsigned char RS232_DATA_FLAG = 0;    //RS232接收正确深度计数据的标志位

extern volatile unsigned char RS232_buff[114] ;//用于接收数据
extern volatile unsigned int RS232_rec_counter ;//用于RS232接收计数
//unsigned char buff[57];

unsigned char DHT11_FLAG = 0;         //DHT11数据采集完毕后的标志位
//unsigned char rom_id[8] = {0};
//unsigned short int temperature = 0xff;            //DHT11温度数据
unsigned char humidity = 0xff;               //DHT11湿度数据

unsigned char LouShui_FLAG1 = 0;      //漏水开关量信号1


//LED闪烁任务
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
//tcp服务器收发任务
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
		OSTimeDlyHMSM(0, 0, 0, 20);//挂起5ms，以便其他线程运行
	}
}
//以太网数据收发前后打包处理,将收到的从控板CAN数据、485收到的惯导数据、232收到的深度数据、DTH11温湿度传感器数据整理至TCP发送包
void Task_TCPData_Processing(void *pdata)
{
	CAN1_Configuration();
	Light_pin_Configuration();
	GPS_init(9600);
	USART232_Configuration();
	IIC_Init();	         //初始化IIC PC11 PC12口子
	mydelay_ms(50);
	MS583703BA_RESET();	 // Reset Device  复位MS5837
	mydelay_ms(50);
	MS5837_init();	     //初始化MS5837
  while(1)
	{
//			MS583703BA_getTemperature();						 //获取温度
			MS583703BA_getPressure();                //获取大气�
			if(TCPaccept[10] == 0xa9)
		 {
			  Light_ON;
		 }	
     else Light_OFF ;	
//			TCPsend[0] = 0xcd;                       //TCPsend[0]--TCPsend[1] TCP发送包头+数据长度
//			TCPsend[1] = 0x32;
		
//			if(CAN_FLAG == 1)
//			{
//				CAN_FLAG = 0;
//				for(i=0;i<8;i++)                       //TCPsend[2]--TCPsend[9] 从控制板发的2字节电源电压+2字节电流+2字节温度+1字节漏水+1字节FT
//				{
//					TCPsend[i+2] = 0;
//				}
//			}
//			
//			TCPsend[10] = (float)temperature/16 ;    //TCPsend[10]  舱内温度存储到TCP发送包固定位置			
//			

//				for(i=0;i<12;i++)                      //TCPsend[11]--TCPsend[22] 姿态角数据
//				{
//					 TCPsend[i+11] = buff[i+7]; 					
//				}
//				for(i=0;i<12;i++)                      
//				{  
//					 TCPsend[i+23] = buff[i+31];   //TCPsend[23]--TCPsend[34] 加速度计数据
//				}	
//					for(i=0;i<11;i++)
//				{
//					TCPsend[i+36]=0;				
//				}
//			
//			TCPsend[47]=TEMP;											//深度计温度
//			TCPsend[48]=(Pressure)&255;						//深度低八位
//			TCPsend[49]=(Pressure>>8)&255;				//深度高八位
//			TCPsend[50] = 0x55;                     //TCPsend[50]  包尾数据
			
			OSTimeDlyHMSM(0, 0, 0, 100);//挂200ms，以便其他线程运行
			}
}



//水下灯控制任务  TCPaccept[0]为控制水下灯动作的控制位
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
//		 OSTimeDlyHMSM(0, 0, 0, 100);//挂起5ms，以便其他线程运行
//	}			
//}

//DHT11或者DS18B20温湿度数据采集
//void Task_DHT11(void *pdata)
//{
//  bsp_InitDS18B20();
//	DISABLE_INT();/* 禁止全局中断 */
//  if(Read_ROM_ID(rom_id) == 0)
//	{
//		  TCPsend[8] = 0x99 ;				  //DS18B20读取ID失败	
//	}
//	ENABLE_INT();	/* 使能全局中断 */
//  while(1)
//	{
//	   DISABLE_INT();/* 禁止全局中断 */
//		 temperature = DS18B20_ReadTempByID(rom_id); 
//	   ENABLE_INT();	/* 使能全局中断 */
//		 OSTimeDlyHMSM(0, 0, 1, 0);//挂起5ms，以便其他线程运行
//	}			
//}


