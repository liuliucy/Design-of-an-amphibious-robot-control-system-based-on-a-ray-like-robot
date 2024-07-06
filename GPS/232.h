#ifndef _232_H_
#define _232_H_


//串口3相关宏定义
#define USART3_REC_LEN		 120				//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		800					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

extern unsigned char  USART3_RX_BUF[USART3_REC_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern unsigned char  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern unsigned short USART3_RX_STA;   						//接收数据状态
extern unsigned char gps_flag;
extern unsigned short gps_len;

void GPS_init(unsigned int bound);				//串口3初始化 
void gps_send(void);
#endif
