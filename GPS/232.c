#include "main.h"  
#include "232.h"



u8 USART3_RX_BUF[USART3_REC_LEN];				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u16 USART3_RX_STA=0; 
u8 gps_flag;
u16 gps_len;
//以太网数据收发前后打包处理,将收到的从控板CAN数据、485收到的惯导数据、232收到的深度数据、DTH11温湿度传感器数据整理至TCP发送包
void GPS_init(u32 bound)
{ 
	
	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	USART_DeInit(USART3);
	
	USART_StructInit(&USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);        //管脚PA9复用为USART1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        
	GPIO_Init(GPIOD, &GPIO_InitStructure);                                                                                                                 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);
	
	USART_ClockInit(USART3,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = 9600; //设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; //无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure); 

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);        ///////接收中断使能
	USART_ClearITPendingBit(USART3, USART_IT_TC);//清除中断TC位
	USART_Cmd(USART3,ENABLE);//最后使能串口
}

//RS232接收数据的中断服务程序
void USART3_IRQHandler(void)  
{
	static u8 k=0; 
	u8 res;	
	OSIntEnter();
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		res=USART3->DR; 	
			if(res=='$')    //判断是否收到$
		 {	
			gps_flag=0;     //GPS标志位清0
			USART3_RX_STA=0;    //接收标识符清0
			USART3_RX_STA|=0x1000;    //标志高四位置1
			USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //将收到的数据存入BUFF
			USART3_RX_STA++;     //加1
		 }
		 else if(USART3_RX_STA&0x1000)   //标志高四位是否置1
		{  

		  if(k==5)    //判断收到的后五位
			{
				if(USART3_RX_BUF[1]=='G'&&USART3_RX_BUF[2]=='N'&&USART3_RX_BUF[3]=='G'&&USART3_RX_BUF[4]=='G'&&USART3_RX_BUF[5]=='A')   //判断是否是想要的数据段
				{ 
          if(res==0X0D)			 //判断是否收到结束标识符
					 { 
						 USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //将数据存入BUFF
						 USART3_RX_STA++;							  //加1
             gps_len=USART3_RX_STA&0X0FFF;   //数据长度
						 USART3_RX_STA=0;     //接收标志位置0
						 k=0;                  //清0
						 gps_flag=1;           //GPS接收标志位置1					
					 }
					else
					 {  
						 USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //未收到结束标识符，将数据保存起来
						 USART3_RX_STA++;		
				   }
				}
					else 	 
				{
								 USART3_RX_STA=0;   //后五位判断不对重新接收
					      	k=0;   // 清0
				}						
			}
		else 
		 {
			USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //接收后五位
			USART3_RX_STA++;
			k++;
		 }
	 }		
 }
}



 void gps_send(void)
{

		if(gps_flag==1)		//GPS接收完成了
	{
	  gps_flag=0;		   	//启动下一次接收


	}
}
	


