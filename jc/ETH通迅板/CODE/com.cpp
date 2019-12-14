
#include "com.h"
#include <String.h>

//--------------------------------------------------------
// Use Configuration Wizard in Context Menu 
// <o0> Ê¹ÄÜCRC16 <0=>NO  <1=>YES
#define EN_CRC16    1
#if EN_CRC16
 #include "CRC.c"
#endif
// end of configuration section 
//======================================================== 
UsartSend::UsartSend(USART_TypeDef* port)
{
	usart = port;
	buf = 0;
	p = 0;
	len = 0;
	run = 0;	
}
//-------------------------------------------------
void UsartSend::sendISR()
{
	if(run)
	{
		p++;
		USART_SendData(usart,buf[p]);				
		if(p == len) { p = 0; run = 0;}
	}
}
//------------------------------------------------
void UsartSend::startSend(uint8_t *txbuf,uint8_t  buflen)
{
	for(p=0;p<buflen;p++)
	{
		USART_SendData(usart, txbuf[p]);
		while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET);	
	}
}
//-------------------------------------------------
void UsartSend::startIsrSend(uint8_t *txbuf,uint8_t  buflen)
{
	p = 0;
	buf = txbuf;
	len = buflen;
	run = 1;
	USART_SendData(usart, buf[p]);
}
//===================================================================================
UsartReceive::UsartReceive(USART_TypeDef* port,uint8_t id,uint8_t* pbuf,uint8_t len)
{
	usart = port;
	buf = pbuf;
	p = 0;
	timeOut = 0;	
	bufsize = len-1;
	comId = id;
}
//--------------------------------------------------
void UsartReceive::receiveISR(void)
{
	buf[p] = USART_ReceiveData(usart);
	if(p < bufsize) p++;
	timeOut = 3;	
}
//--------------------------------------------------
extern "C" {void SerialReceive_Callback(uint8_t id,uint8_t* rxbuf,uint8_t len);}
void UsartReceive::receiveExec(void)
{
	if(timeOut >0)
	{
		timeOut --;
		if(timeOut == 0)
		{				
		    SerialReceive_Callback(comId,buf,p);				
			memset(buf,0,p);
			p = 0; 
		}
	}	
}
//--------------------------------------------------

