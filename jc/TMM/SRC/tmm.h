
#ifndef __USART_H
#define __USART_H

#include <stdint.h>

typedef union{
	struct{
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
	}bit;
	uint8_t dat;
}bit8_t;


extern __IO uint16_t ADC_Value[8];

void U1sendn(char* buf);
void U1sendn(char* buf);
void U1send(uint8_t* buf,uint8_t buflen);
void U1send(uint8_t* buf,uint8_t buflen);

void uart_init(void);
void ADCx_Init(void);
void json_send(void);



#endif


