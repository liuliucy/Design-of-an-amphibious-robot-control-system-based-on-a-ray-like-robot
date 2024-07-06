#include "main.h"  
#include "232.h"



u8 USART3_RX_BUF[USART3_REC_LEN];				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u16 USART3_RX_STA=0; 
u8 gps_flag;
u16 gps_len;
//��̫�������շ�ǰ��������,���յ��Ĵӿذ�CAN���ݡ�485�յ��Ĺߵ����ݡ�232�յ���������ݡ�DTH11��ʪ�ȴ���������������TCP���Ͱ�
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);        //�ܽ�PA9����ΪUSART1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;        
	GPIO_Init(GPIOD, &GPIO_InitStructure);                                                                                                                 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);
	
	USART_ClockInit(USART3,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = 9600; //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; //��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure); 

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);        ///////�����ж�ʹ��
	USART_ClearITPendingBit(USART3, USART_IT_TC);//����ж�TCλ
	USART_Cmd(USART3,ENABLE);//���ʹ�ܴ���
}

//RS232�������ݵ��жϷ������
void USART3_IRQHandler(void)  
{
	static u8 k=0; 
	u8 res;	
	OSIntEnter();
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		res=USART3->DR; 	
			if(res=='$')    //�ж��Ƿ��յ�$
		 {	
			gps_flag=0;     //GPS��־λ��0
			USART3_RX_STA=0;    //���ձ�ʶ����0
			USART3_RX_STA|=0x1000;    //��־����λ��1
			USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //���յ������ݴ���BUFF
			USART3_RX_STA++;     //��1
		 }
		 else if(USART3_RX_STA&0x1000)   //��־����λ�Ƿ���1
		{  

		  if(k==5)    //�ж��յ��ĺ���λ
			{
				if(USART3_RX_BUF[1]=='G'&&USART3_RX_BUF[2]=='N'&&USART3_RX_BUF[3]=='G'&&USART3_RX_BUF[4]=='G'&&USART3_RX_BUF[5]=='A')   //�ж��Ƿ�����Ҫ�����ݶ�
				{ 
          if(res==0X0D)			 //�ж��Ƿ��յ�������ʶ��
					 { 
						 USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //�����ݴ���BUFF
						 USART3_RX_STA++;							  //��1
             gps_len=USART3_RX_STA&0X0FFF;   //���ݳ���
						 USART3_RX_STA=0;     //���ձ�־λ��0
						 k=0;                  //��0
						 gps_flag=1;           //GPS���ձ�־λ��1					
					 }
					else
					 {  
						 USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //δ�յ�������ʶ���������ݱ�������
						 USART3_RX_STA++;		
				   }
				}
					else 	 
				{
								 USART3_RX_STA=0;   //����λ�жϲ������½���
					      	k=0;   // ��0
				}						
			}
		else 
		 {
			USART3_RX_BUF[USART3_RX_STA&0X0FFF]=res ;  //���պ���λ
			USART3_RX_STA++;
			k++;
		 }
	 }		
 }
}



 void gps_send(void)
{

		if(gps_flag==1)		//GPS���������
	{
	  gps_flag=0;		   	//������һ�ν���


	}
}
	


