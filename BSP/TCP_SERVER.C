#include "main.h"
#include "string.h"
/***********************************************************************
º¯ÊıÃû³Æ£ºtcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
¹¦    ÄÜ£ºTCPÊı¾İ½ÓÊÕºÍ·¢ËÍ
×¢    Òâ£ºÕâÊÇÒ»¸ö»Øµ÷º¯Êı£¬µ±Ò»¸öTCP¶Îµ½´ïÕâ¸öÁ¬½ÓÊ±»á±»µ÷ÓÃ
***********************************************************************/

extern unsigned char TCPaccept[14];
extern unsigned char TCPsend[31];
extern unsigned char TCP_FLAG;
extern unsigned char CANsend[8];   //CAN·¢ËÍµÄÕıÈ·Êı¾İ
unsigned short int temperature = 0xff;            //DHT11ÎÂ¶ÈÊı¾İ
unsigned char CAN_FLAG = 0;           //CAN½ÓÊÕÕıÈ·Êı¾İµÄ±êÖ¾Î»
unsigned char buff[57];
unsigned char rom_id[8] = {0};
#define ENABLE_INT()	__set_PRIMASK(0)	/* Ê¹ÄÜÈ«¾ÖÖĞ¶Ï */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ½ûÖ¹È«¾ÖÖĞ¶Ï */


static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	unsigned char i = 0;
	struct pbuf *p_temp = p;
	if(p_temp != NULL)
	{	
		tcp_recved(pcb, p_temp->tot_len);                   //TCPÊı¾İ½ÓÊÕ
		
    memcpy(TCPaccept,p_temp->payload,p_temp->len);
		
		if(TCPaccept[0] == 0xcd && TCPaccept[1] == 0x0d && TCPaccept[13] == 0x55)
		{
        TCP_FLAG = 1;       //Ğ£Ñé³É¹¦£¬°üÍ·0xcd¡¢Êı¾İ³¤¶È13×Ö½Ú£º12Î»Êı¾İÎ»+1Î»Ğ£ÑéÎ»¡
			}
	
    if( TCP_FLAG == 1)
		{
			TCP_FLAG = 0;
			TCPsend[0] = 0xcd;                       //TCPsend[0]--TCPsend[1] TCP·¢ËÍ°üÍ·+Êı¾İ³¤¶È
			TCPsend[1] = 0x32;
			TCPsend[2] = (float)temperature/16 ; 
			for(i=0;i<12;i++)                      //TCPsend[11]--TCPsend[22] ×ËÌ¬½ÇÊı¾İ
				{
					 TCPsend[i+3] = buff[i+7]; 					
				}
				for(i=0;i<12;i++)                      
				{  
					 TCPsend[i+15] = buff[i+31];   //TCPsend[23]--TCPsend[34] ¼ÓËÙ¶È¼ÆÊı¾İ
				}	
				

			TCPsend[27]=(Pressure)&255;						//Éî¶ÈµÍ°ËÎ»
			TCPsend[28]=(Pressure>>8)&255;				//Éî¶È¸ß°ËÎ»
			TCPsend[29] = 0x55;                     //TCPsend[50]  °üÎ²Êı¾İ

				while(p_temp != NULL)	
		  {				
		   	tcp_write(pcb,TCPsend,31,TCP_WRITE_FLAG_COPY); 	// TCPÊı¾İ·¢ËÍ 
			  tcp_output(pcb);
			  p_temp = p_temp->next;
		  }		
			
			for(i=0;i<4;i++)
			{
				 CANsend[1+i] = TCPaccept[2+i];       //½«Ö÷¿Ø°å½ÓÊÕµÄÍÆ½øÆ÷¿ØÖÆÊı¾İ×ª·¢ÖÁ´Ó¿ØÖÆ°å
			}
			CANsend[5]=Pressure&255;
			CANsend[6]=	(Pressure>>8)&255;
			CANsend[0] = 0xcd;  //CAN°üÍ·
			CANsend[7] = 0x55;  //CAN°üÎ²
			can1_tx(0xa1);
		}
	}
	else
	{
		tcp_close(pcb); 											/* ×÷ÎªTCP·şÎñÆ÷²»Ó¦Ö÷¶¯¹Ø±ÕÕâ¸öÁ¬½Ó£¿ */
	}
	/* ÊÍ·Å¸ÃTCP¶Î */
	pbuf_free(p); 	
	err = ERR_OK;
	return err;
}


/***********************************************************************
º¯ÊıÃû³Æ£ºtcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
¹¦    ÄÜ£º»Øµ÷º¯Êı
×¢    Òâ£ºÕâÊÇÒ»¸ö»Øµ÷º¯Êı£¬µ±Ò»¸öÁ¬½ÓÒÑ¾­½ÓÊÜÊ±»á±»µ÷ÓÃ
***********************************************************************/
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* ÉèÖÃ»Øµ÷º¯ÊıÓÅÏÈ¼¶£¬µ±´æÔÚ¼¸¸öÁ¬½ÓÊ±ÌØ±ğÖØÒª,´Ëº¯Êı±ØĞëµ÷ÓÃ*/
	tcp_recv(pcb,tcp_server_recv); 				/* ÉèÖÃTCP¶Îµ½Ê±µÄ»Øµ÷º¯Êı */
	err = ERR_OK;
	return err;
}


/***********************************************************************
º¯ÊıÃû³Æ£ºTCP_server_init(void)
¹¦    ÄÜ£ºÍê³ÉTCP·şÎñÆ÷µÄ³õÊ¼»¯£¬Ö÷ÒªÊÇÊ¹µÃTCPÍ¨Ñ¶¿ì½øÈë¼àÌı×´Ì¬

***********************************************************************/
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	/*****************************************************/
	pcb = tcp_new(); 								/* ½¨Á¢Í¨ĞÅµÄTCP¿ØÖÆ¿é(pcb) */
	tcp_bind(pcb,IP_ADDR_ANY,TCP_SERVER_PORT); 	    /* °ó¶¨±¾µØIPµØÖ·ºÍ¶Ë¿ÚºÅ£¨×÷Îªtcp·şÎñÆ÷£© */
	pcb = tcp_listen(pcb); 							/* ½øÈë¼àÌı×´Ì¬ */
	tcp_accept(pcb,tcp_server_accept); 			    /* ÉèÖÃÓĞÁ¬½ÓÇëÇóÊ±µÄ»Øµ÷º¯Êı */
}
void Task_DHT11(void *pdata)
{
  bsp_InitDS18B20();
	DISABLE_INT();/* ½ûÖ¹È«¾ÖÖĞ¶Ï */
  if(Read_ROM_ID(rom_id) == 0)
	{
		  TCPsend[2] = 0x99 ;				  //DS18B20¶ÁÈ¡IDÊ§°Ü	
	}
	ENABLE_INT();	/* Ê¹ÄÜÈ«¾ÖÖĞ¶Ï */
  while(1)
	{
	   DISABLE_INT();/* ½ûÖ¹È«¾ÖÖĞ¶Ï */
		 temperature = DS18B20_ReadTempByID(rom_id); 
	   ENABLE_INT();	/* Ê¹ÄÜÈ«¾ÖÖĞ¶Ï */
		 OSTimeDlyHMSM(0, 0, 1, 0);//¹ÒÆğ5ms£¬ÒÔ±ãÆäËûÏß³ÌÔËĞĞ
	}			
}
