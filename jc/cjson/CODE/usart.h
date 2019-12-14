#ifndef __USART_H
#define __USART_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f10x.h"

#define BUSZ  ((GPIOA->IDR&(1<<2))&&(GPIOA->IDR&(1<<3)))
	 
typedef union
{
	uint8_t  Bit[2];
	uint16_t Dat;
}CRC16_t;
typedef struct
{
	__IO uint8_t  code;
	__IO uint8_t  msg;
	__IO uint16_t timeOut;
	
	__IO uint16_t PV;
	__IO uint16_t SV;
   uint16_t tempSV;
	uint16_t SVdata;
	uint8_t  reSend;
	uint8_t  isSV;
	uint8_t  isfish;
	uint8_t  isWrie;
	uint8_t  writeId;
	uint16_t writeData;
}Alarm_t;

typedef struct
{
	uint8_t code;	
	uint16_t PV;
	uint16_t SV;	
	uint8_t  on_off;
	uint8_t  cf;
	uint16_t Iout;
	uint16_t Oout;
	uint16_t Pout;		
	uint8_t  updata;
}data_t;

extern  __IO uint8_t Msg;
extern  uint8_t Rxbuf1[2048],rxtim1;
extern  uint8_t Txbuf1[1024],txlen1;
extern  uint16_t pr1,pt1;



//------------------------------------------
void uart_init(void);
void initGPIO(void);
CRC16_t CreatCRC16( uint8_t * data, uint8_t len);	 
void SerialReceive_Callback(void);

void IWDG_Config(uint8_t prv ,uint16_t rlv);
	


#ifdef __cplusplus
}
#endif

#endif


