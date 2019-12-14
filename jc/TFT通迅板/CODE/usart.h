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
	__IO uint8_t  code;  //state
	__IO uint8_t  msg;   //������־
	__IO uint16_t timeOut;  //����ʱ��
	
	__IO uint16_t PV;
	__IO uint16_t SV;
//����
	uint16_t stbySV;
//ͬ���趨�¶�	
    uint16_t tempSV; //temp sv set
	uint16_t SVdata;  //sv data
	uint8_t  reSend; // re send fig
	uint8_t  isSV; //sv set fig
	uint8_t  isfish; // sv is sended
//�Ĵ����趨	
	uint8_t  wNum;    //send times
	uint8_t  isWrie; //reg set fig
	uint8_t  writeId; //reg id
	uint16_t writeData; // reg data
	
}Alarm_t;

typedef struct  //03-���ּĴ���
{
	uint16_t  updata;
	uint16_t  code;	
	uint16_t  PV;
	uint16_t  SV;	
	uint16_t  on_off;
	uint16_t  cf;
	uint16_t  Iout;
	uint16_t  Oout;
	uint16_t  Pout;		
	
}data_t;

typedef struct
{
	uint16_t  workSV;   //�����¶��趨
    uint16_t  stbySV;	//�����¶��趨
	uint16_t  workDsp;  //�����¶���ʾ
    uint16_t  stbyDsp;	//�����¶���ʾ	
	
	uint8_t   stby_m;  //m
	uint8_t   stby_s;  //s
	
	uint8_t   chSV;    //�����¶ȸı��־
	uint8_t   chST;    //�����¶ȸı��־
	uint8_t   chM;    //
	uint8_t   chS;    //
	uint8_t   chC;    //��λ
	uint8_t   chE;    //
	uint8_t   chSyn;  //ͬ��
	
	uint16_t  stbyDelay; //������ʱ����
	uint8_t   cf;        //�¶ȵ�λ
	uint8_t   isStby;     //�Ƿ��ڴ���״̬
	uint8_t   enable;     //���ܿ���
	uint8_t   SyncEnable;  //һ��ͬ������
	
	uint8_t   workMode;    //����ģʽ
	uint8_t   sigStby;     //�����ź�
	uint8_t   sigpower;    //�����ź�
	uint8_t   outArrive;   //�¶ȵ���
	uint8_t   outAlarm;    //�������
	uint8_t   cn_en;       //��Ӣ��
}Stby_t;
extern Stby_t Stby;

typedef struct
{
	uint8_t  Rxbuf[20];
	volatile uint8_t  Rxp;
	volatile uint8_t  Rxfig;
	volatile uint16_t RxDelay;	
}Rx_t;
extern Rx_t Rx[3];

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



extern __IO uint8_t isCsy;
extern __IO uint16_t delay,send,dealySet,isRx,Csytim;

//------------------------------------------
void uart_init(void);
void comsend(uint8_t address,uint8_t cmd,uint8_t data); 
CRC16_t CreatCRC16( uint8_t * data, uint8_t len);	
void SerialReceive_Callback(Rx_t* rx);
void Serial_3_Receive(Rx_t* rx);
void initGPIO(void);
void onAlarm(void);
void alarmExec(void);
void sendquse(void);
void syncExec(void);
void IWDG_Config(uint8_t prv ,uint16_t rlv);
void AT24CXX_Init(void);	


#ifdef __cplusplus
}
#endif

#endif


