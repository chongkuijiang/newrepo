#ifndef __USART_H
#define __USART_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f10x.h"

#define  NUM 60
	 
typedef union
{
	uint8_t  Bit[2];
	uint16_t Dat;
}CRC16_t;
typedef struct
{
	__IO uint8_t  code;  //state
	__IO uint8_t  msg;   //报警标志
	__IO uint16_t timeOut;  //报警时间
	
	__IO uint16_t PV;
	__IO uint16_t SV;
//待机
	uint16_t stbySV;
//同步设定温度	
    uint16_t tempSV; //temp sv set
	uint16_t SVdata;  //sv data
	uint8_t  reSend; // re send fig
	uint8_t  isSV; //sv set fig
	uint8_t  isfish; // sv is sended
//寄存器设定	
	uint8_t  wNum;    //send times
	uint8_t  isWrie; //reg set fig
	uint8_t  writeId; //reg id
	uint16_t writeData; // reg data
	
}Alarm_t;
extern Alarm_t alarmTab[100];


typedef struct
{	
	volatile uint8_t  Rxp;
	volatile uint8_t  Rxfig;
	volatile uint16_t RxDelay;
    uint8_t  Rxbuf[20];	
}Rx_t;
extern Rx_t Rx[2];

#define RXLEN  1024
typedef struct
{	
	volatile uint8_t  Rxfig;
	volatile uint16_t RxDelay;	
    volatile uint16_t Rxp;
	char     Rxbuf[RXLEN];
}Rx3_t;
extern Rx3_t Rx3;

enum ErrorCode{
NoError,
ALARM_H,
ALARM_L,
TC_OP,
F1_OP,
F2_OP,
HT_OP,
TC_ST,
TC_RE,
HT_ST,
TR_ST,
HC_RE,
OFLI
};



extern __IO uint8_t isCsy,isMs;
extern __IO uint16_t delay,send,dealySet,isRx,Csytim;

//------------------------------------------
void uart_init(void);
CRC16_t CreatCRC16( uint8_t * data, uint8_t len);	
void SerialReceive_Callback(Rx_t* rx);
void Serial_3_Receive();
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


