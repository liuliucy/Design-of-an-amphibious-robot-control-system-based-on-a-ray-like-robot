#include "main.h"
#include "string.h"
/***********************************************************************
函数名称：tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：TCP数据接收和发送
注    意：这是一个回调函数，当一个TCP段到达这个连接时会被调用
***********************************************************************/

extern unsigned char TCPaccept[14];
extern unsigned char TCPsend[31];
extern unsigned char TCP_FLAG;
extern unsigned char CANsend[8];   //CAN发送的正确数据
unsigned short int temperature = 0xff;            //DHT11温度数据
unsigned char CAN_FLAG = 0;           //CAN接收正确数据的标志位
unsigned char buff[57];
unsigned char rom_id[8] = {0};
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */


static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	unsigned char i = 0;
	struct pbuf *p_temp = p;
	if(p_temp != NULL)
	{	
		tcp_recved(pcb, p_temp->tot_len);                   //TCP数据接收
		
    memcpy(TCPaccept,p_temp->payload,p_temp->len);
		
		if(TCPaccept[0] == 0xcd && TCPaccept[1] == 0x0d && TCPaccept[13] == 0x55)
		{
        TCP_FLAG = 1;       //校验成功，包头0xcd、数据长度13字节：12位数据位+1位校验位�
			}
	
    if( TCP_FLAG == 1)
		{
			TCP_FLAG = 0;
			TCPsend[0] = 0xcd;                       //TCPsend[0]--TCPsend[1] TCP发送包头+数据长度
			TCPsend[1] = 0x32;
			TCPsend[2] = (float)temperature/16 ; 
			for(i=0;i<12;i++)                      //TCPsend[11]--TCPsend[22] 姿态角数据
				{
					 TCPsend[i+3] = buff[i+7]; 					
				}
				for(i=0;i<12;i++)                      
				{  
					 TCPsend[i+15] = buff[i+31];   //TCPsend[23]--TCPsend[34] 加速度计数据
				}	
				

			TCPsend[27]=(Pressure)&255;						//深度低八位
			TCPsend[28]=(Pressure>>8)&255;				//深度高八位
			TCPsend[29] = 0x55;                     //TCPsend[50]  包尾数据

				while(p_temp != NULL)	
		  {				
		   	tcp_write(pcb,TCPsend,31,TCP_WRITE_FLAG_COPY); 	// TCP数据发送 
			  tcp_output(pcb);
			  p_temp = p_temp->next;
		  }		
			
			for(i=0;i<4;i++)
			{
				 CANsend[1+i] = TCPaccept[2+i];       //将主控板接收的推进器控制数据转发至从控制板
			}
			CANsend[5]=Pressure&255;
			CANsend[6]=	(Pressure>>8)&255;
			CANsend[0] = 0xcd;  //CAN包头
			CANsend[7] = 0x55;  //CAN包尾
			can1_tx(0xa1);
		}
	}
	else
	{
		tcp_close(pcb); 											/* 作为TCP服务器不应主动关闭这个连接？ */
	}
	/* 释放该TCP段 */
	pbuf_free(p); 	
	err = ERR_OK;
	return err;
}


/***********************************************************************
函数名称：tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：回调函数
注    意：这是一个回调函数，当一个连接已经接受时会被调用
***********************************************************************/
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_server_recv); 				/* 设置TCP段到时的回调函数 */
	err = ERR_OK;
	return err;
}


/***********************************************************************
函数名称：TCP_server_init(void)
功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态

***********************************************************************/
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	/*****************************************************/
	pcb = tcp_new(); 								/* 建立通信的TCP控制块(pcb) */
	tcp_bind(pcb,IP_ADDR_ANY,TCP_SERVER_PORT); 	    /* 绑定本地IP地址和端口号（作为tcp服务器） */
	pcb = tcp_listen(pcb); 							/* 进入监听状态 */
	tcp_accept(pcb,tcp_server_accept); 			    /* 设置有连接请求时的回调函数 */
}
void Task_DHT11(void *pdata)
{
  bsp_InitDS18B20();
	DISABLE_INT();/* 禁止全局中断 */
  if(Read_ROM_ID(rom_id) == 0)
	{
		  TCPsend[2] = 0x99 ;				  //DS18B20读取ID失败	
	}
	ENABLE_INT();	/* 使能全局中断 */
  while(1)
	{
	   DISABLE_INT();/* 禁止全局中断 */
		 temperature = DS18B20_ReadTempByID(rom_id); 
	   ENABLE_INT();	/* 使能全局中断 */
		 OSTimeDlyHMSM(0, 0, 1, 0);//挂起5ms，以便其他线程运行
	}			
}
