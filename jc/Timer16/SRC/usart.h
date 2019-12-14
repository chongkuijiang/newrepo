#ifndef __USART_H
#define __USART_H
#include <jansson.h>

#ifdef __cplusplus
 extern "C" {
#endif 
	 
	 
//#include <stdio.h>	 
#include <stdint.h>

typedef union{
	struct{
		uint16_t b0 : 1;
		uint16_t b1 : 1;
		uint16_t b2 : 1;
		uint16_t b3 : 1;
		uint16_t b4 : 1;
		uint16_t b5 : 1;
		uint16_t b6 : 1;
		uint16_t b7 : 1;
		uint16_t b8 : 1;
		uint16_t b9 : 1;
		uint16_t b10 : 1;
		uint16_t b11 : 1;
		uint16_t b12 : 1;
		uint16_t b13 : 1;
		uint16_t b14 : 1;
		uint16_t b15 : 1;
	}bit;
	uint16_t dat;
}bit16_t;

typedef struct{
	uint8_t  step;
	uint8_t  mode;
	uint8_t  onOff;
	uint8_t  group;
	uint8_t  out;
	uint8_t  push;
	uint8_t  pOut;
	uint32_t delayOut;
	uint32_t outDelay;
	uint32_t delayOut2;
	uint32_t outDelay2;
	
}timer_t;
extern timer_t timer[16];

typedef struct{
    volatile uint32_t delayOut;
	volatile uint32_t outDelay;
	volatile uint32_t delayOut2;
	volatile uint32_t outDelay2;	
}delay_t;
extern delay_t delay[16];

typedef struct{
    volatile uint16_t delay;
	volatile uint8_t out;	
}delayOut_t;
extern delayOut_t dOut[6];

extern uint16_t time,rxifg,pushfig;
extern uint8_t butgroup,butopen,voltage,signal;
  
extern char Rxbuf1[2048];
extern uint8_t Txbuf1[2048];
extern uint16_t pr1,pt1,rxtim1,txlen1;

void uart_init(void);
uint8_t readSignal(void);
void load(int i);
void proctime(void);
void U1sendn(char* buf);
void GPIO_Configuration(void);
void  sendJson(void);
void procJson(const char *input);
void pushing(void);

//------------------------------------------

	 
#ifdef __cplusplus
}
#endif


#endif


