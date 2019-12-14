
#include "Heartbeat.h"

//-------------------------------------------------
Heartbeat::Heartbeat(usart_t usart,uint8_t cycletime,uint8_t *txbuf,uint8_t  buflen)
{	
	heartbeatTime = cycletime; //ms
	timeOut = cycletime;
	scanPeriod = 50;    //´Î	
	isScan = 1;
	stackTop = 0;
	stackPointer = 0;
	address = 0;
	Usart = usart;
	tx.buf = txbuf;
	tx.p = 0;
	tx.buflen = buflen;
}
//-------------------------------------------------
void Heartbeat::send(void)
{	
	if(isScan)
	{		
		address++;		
	}
	else
	{		address = ChStack[stackPointer];
		if(address != 0) stackPointer++;
		else stackPointer = 0;		
				
	}
	if(tx.p == 0) USART_SendData(Usart,address);		
	else USART_SendData(Usart,tx.buf[tx.p-1]);
	if(++tx.p == tx.buflen) tx.p = 0;
}



