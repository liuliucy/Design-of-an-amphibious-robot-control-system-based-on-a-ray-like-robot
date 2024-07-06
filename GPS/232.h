#ifndef _232_H_
#define _232_H_


//����3��غ궨��
#define USART3_REC_LEN		 120				//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		800					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern unsigned char  USART3_RX_BUF[USART3_REC_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern unsigned char  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern unsigned short USART3_RX_STA;   						//��������״̬
extern unsigned char gps_flag;
extern unsigned short gps_len;

void GPS_init(unsigned int bound);				//����3��ʼ�� 
void gps_send(void);
#endif
