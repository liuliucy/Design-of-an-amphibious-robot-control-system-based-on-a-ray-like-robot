#include "main.h"  
#include "math.h"

//#define ENABLE_INT()	__set_PRIMASK(0)	/* Ê¹ÄÜÈ«¾ÖÖĞ¶Ï */
//#define DISABLE_INT()	__set_PRIMASK(1)	/* ½ûÖ¹È«¾ÖÖĞ¶Ï */

OS_STK Stk_Task_LED[TASK_TEST_LED_STK_SIZE];
OS_STK Stk_Task_TCP_Server[TASK_TCP_SERVER_STK_SIZE];
OS_STK Stk_Task_TCPData_Processing[TASK_TCPData_Processing_STK_SIZE];
OS_STK Stk_Task_Light[TASK_Light_STK_SIZE];
OS_STK Stk_Task_DHT11[TASK_DHT11_STK_SIZE];

unsigned char i,k;                		  //ÓÃÓÚforÑ­»·

unsigned char CANaccept[8];           //CAN½ÓÊÕµÄÕıÈ·Êı¾İ
unsigned char CANsend[8];             //CAN·¢ËÍµÄÕıÈ·Êı¾İ
unsigned char TCPaccept[14];          //TCP½ÓÊÜµÄÕıÈ·Êı¾İ
unsigned char TCPsend[31];            //TCP·¢ËÍµÄÊı¾İ°ü

unsigned char TCP_FLAG = 0;           //TCP½ÓÊÕÕıÈ·Êı¾İµÄ±êÖ¾Î»

unsigned char RS232_DATA_FLAG = 0;    //RS232½ÓÊÕÕıÈ·Éî¶È¼ÆÊı¾İµÄ±êÖ¾Î»

extern volatile unsigned char RS232_buff[114] ;//ÓÃÓÚ½ÓÊÕÊı¾İ
extern volatile unsigned int RS232_rec_counter ;//ÓÃÓÚRS232½ÓÊÕ¼ÆÊı
//unsigned char buff[57];

unsigned char DHT11_FLAG = 0;         //DHT11Êı¾İ²É¼¯Íê±ÏºóµÄ±êÖ¾Î»
//unsigned char rom_id[8] = {0};
//unsigned short int temperature = 0xff;            //DHT11ÎÂ¶ÈÊı¾İ
unsigned char humidity = 0xff;               //DHT11Êª¶ÈÊı¾İ

unsigned char LouShui_FLAG1 = 0;      //Â©Ë®¿ª¹ØÁ¿ĞÅºÅ1


//LEDÉÁË¸ÈÎÎñ
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
//tcp·şÎñÆ÷ÊÕ·¢ÈÎÎñ
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
		OSTimeDlyHMSM(0, 0, 0, 20);//¹ÒÆğ5ms£¬ÒÔ±ãÆäËûÏß³ÌÔËĞĞ
	}
}
//ÒÔÌ«ÍøÊı¾İÊÕ·¢Ç°ºó´ò°ü´¦Àí,½«ÊÕµ½µÄ´Ó¿Ø°åCANÊı¾İ¡¢485ÊÕµ½µÄ¹ßµ¼Êı¾İ¡¢232ÊÕµ½µÄÉî¶ÈÊı¾İ¡¢DTH11ÎÂÊª¶È´«¸ĞÆ÷Êı¾İÕûÀíÖÁTCP·¢ËÍ°ü
void Task_TCPData_Processing(void *pdata)
{
	CAN1_Configuration();
	Light_pin_Configuration();
	GPS_init(9600);
	USART232_Configuration();
	IIC_Init();	         //³õÊ¼»¯IIC PC11 PC12¿Ú×Ó
	mydelay_ms(50);
	MS583703BA_RESET();	 // Reset Device  ¸´Î»MS5837
	mydelay_ms(50);
	MS5837_init();	     //³õÊ¼»¯MS5837
  while(1)
	{
//			MS583703BA_getTemperature();						 //»ñÈ¡ÎÂ¶È
			MS583703BA_getPressure();                //»ñÈ¡´óÆøÑ
			if(TCPaccept[10] == 0xa9)
		 {
			  Light_ON;
		 }	
     else Light_OFF ;	
//			TCPsend[0] = 0xcd;                       //TCPsend[0]--TCPsend[1] TCP·¢ËÍ°üÍ·+Êı¾İ³¤¶È
//			TCPsend[1] = 0x32;
		
//			if(CAN_FLAG == 1)
//			{
//				CAN_FLAG = 0;
//				for(i=0;i<8;i++)                       //TCPsend[2]--TCPsend[9] ´Ó¿ØÖÆ°å·¢µÄ2×Ö½ÚµçÔ´µçÑ¹+2×Ö½ÚµçÁ÷+2×Ö½ÚÎÂ¶È+1×Ö½ÚÂ©Ë®+1×Ö½ÚFT
//				{
//					TCPsend[i+2] = 0;
//				}
//			}
//			
//			TCPsend[10] = (float)temperature/16 ;    //TCPsend[10]  ²ÕÄÚÎÂ¶È´æ´¢µ½TCP·¢ËÍ°ü¹Ì¶¨Î»ÖÃ			
//			

//				for(i=0;i<12;i++)                      //TCPsend[11]--TCPsend[22] ×ËÌ¬½ÇÊı¾İ
//				{
//					 TCPsend[i+11] = buff[i+7]; 					
//				}
//				for(i=0;i<12;i++)                      
//				{  
//					 TCPsend[i+23] = buff[i+31];   //TCPsend[23]--TCPsend[34] ¼ÓËÙ¶È¼ÆÊı¾İ
//				}	
//					for(i=0;i<11;i++)
//				{
//					TCPsend[i+36]=0;				
//				}
//			
//			TCPsend[47]=TEMP;											//Éî¶È¼ÆÎÂ¶È
//			TCPsend[48]=(Pressure)&255;						//Éî¶ÈµÍ°ËÎ»
//			TCPsend[49]=(Pressure>>8)&255;				//Éî¶È¸ß°ËÎ»
//			TCPsend[50] = 0x55;                     //TCPsend[50]  °üÎ²Êı¾İ
			
			OSTimeDlyHMSM(0, 0, 0, 100);//¹Ò200ms£¬ÒÔ±ãÆäËûÏß³ÌÔËĞĞ
			}
}



//Ë®ÏÂµÆ¿ØÖÆÈÎÎñ  TCPaccept[0]Îª¿ØÖÆË®ÏÂµÆ¶¯×÷µÄ¿ØÖÆÎ»
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
//		 OSTimeDlyHMSM(0, 0, 0, 100);//¹ÒÆğ5ms£¬ÒÔ±ãÆäËûÏß³ÌÔËĞĞ
//	}			
//}

//DHT11»òÕßDS18B20ÎÂÊª¶ÈÊı¾İ²É¼¯
//void Task_DHT11(void *pdata)
//{
//  bsp_InitDS18B20();
//	DISABLE_INT();/* ½ûÖ¹È«¾ÖÖĞ¶Ï */
//  if(Read_ROM_ID(rom_id) == 0)
//	{
//		  TCPsend[8] = 0x99 ;				  //DS18B20¶ÁÈ¡IDÊ§°Ü	
//	}
//	ENABLE_INT();	/* Ê¹ÄÜÈ«¾ÖÖĞ¶Ï */
//  while(1)
//	{
//	   DISABLE_INT();/* ½ûÖ¹È«¾ÖÖĞ¶Ï */
//		 temperature = DS18B20_ReadTempByID(rom_id); 
//	   ENABLE_INT();	/* Ê¹ÄÜÈ«¾ÖÖĞ¶Ï */
//		 OSTimeDlyHMSM(0, 0, 1, 0);//¹ÒÆğ5ms£¬ÒÔ±ãÆäËûÏß³ÌÔËĞĞ
//	}			
//}


