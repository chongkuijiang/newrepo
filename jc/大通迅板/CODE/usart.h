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

extern uint8_t Rxbuf2[20],Rxbuf1[10];
extern uint16_t comRx,comP,Rx1,P1;
extern __IO uint8_t isRxmain,isRxhmi,isCsy;
extern __IO uint16_t delay,send,dealySet,isRx,Csytim;

//------------------------------------------
void uart_init(void);
void comsend(uint8_t address,uint8_t cmd,uint8_t data); 
CRC16_t CreatCRC16( uint8_t * data, uint8_t len);	 
void SerialReceive_Callback(void);
void Serial1_Receive(void);
void initGPIO(void);
void onAlarm(void);
void alarmExec(void);
void sendquse(void);
void syncExec(void);
void IWDG_Config(uint8_t prv ,uint16_t rlv);
	


#ifdef __cplusplus
}
#endif

#endif


