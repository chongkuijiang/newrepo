#ifndef __COM_H
#define __COM_H
#include "stm32f10x.h"


//--------------------------------------------------------
 class UsartSend{
	private:
	uint8_t* buf;
	uint8_t  p;
	uint8_t  len;
	uint8_t  run;
	USART_TypeDef*  usart;
	public:
	UsartSend(USART_TypeDef* port);
	void sendISR(void);
	void startIsrSend(uint8_t *txbuf,uint8_t  buflen);
	void startSend(uint8_t *txbuf,uint8_t  buflen);
	void startDmaSend(uint8_t *txbuf,uint8_t  buflen);
};
//-------------------------------------------------------
class UsartReceive{
	private:
	uint8_t* buf;
	uint8_t  bufsize;
	uint8_t  timeOut;
	uint8_t  p;
	uint8_t  comId;
	USART_TypeDef*  usart;
	public:
	UsartReceive(USART_TypeDef* port,uint8_t id,uint8_t* pbuf,uint8_t  len);
	void receiveISR(void);
	void receiveExec(void);

};



#endif

