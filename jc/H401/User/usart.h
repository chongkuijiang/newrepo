#ifndef __USART_H
#define __USART_H

#include <stdint.h>
//-----------------------------------------
typedef struct
{
	uint8_t * pRX;
	uint8_t * pTX;
	uint8_t  RXstate;
	uint8_t  TXstate;
	uint8_t  RXp;
	uint8_t  TXp;
	
}commont_t;


//------------------------------------------
void uart_init(void);

#endif


