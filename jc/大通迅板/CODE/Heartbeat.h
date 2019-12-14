#ifndef __HEARTBEAT_H
#define __HEARTBEAT_H
#include "stm32f10x.h"
//#include <stdint.h>

#define usart_t   USART_TypeDef*

typedef struct{
	uint8_t* buf;
	uint8_t  p;
	uint8_t  buflen;
	uint8_t  run;
}TxRxbuf_t;





class Heartbeat{
   private:
	uint8_t  ChStack[255];
	uint8_t  stackTop;
	uint8_t  stackPointer;
	uint8_t  heartbeatTime;
	uint8_t  scanPeriod;
	uint8_t  isScan;
    uint8_t  address;  
    usart_t  Usart;       
    TxRxbuf_t tx;
    
   public:
	uint16_t timeOut;	   
	Heartbeat(usart_t usart,uint8_t cycletime,uint8_t *txbuf,uint8_t  buflen);
    void send(void);
};

#endif

