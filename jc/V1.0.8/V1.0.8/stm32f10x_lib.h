/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_lib.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file includes the peripherals header files in the
*                      user application.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_LIB_H
#define __STM32F10x_LIB_H
#define KeUr (*((vu16 *)(0x4001380c)))
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

#ifdef _ADC
  #include "stm32f10x_adc.h"
#endif /*_ADC */

#ifdef _BKP
  #include "stm32f10x_bkp.h"
#endif /*_BKP */

#ifdef _CAN
  //#include "stm32f10x_can.h"
#endif /*_CAN */

#ifdef _CRC
  //#include "stm32f10x_crc.h"
#endif /*_CRC */

#ifdef _DAC
  //#include "stm32f10x_dac.h"
#endif /*_DAC */

#ifdef _DBGMCU
  //#include "stm32f10x_dbgmcu.h"
#endif /*_DBGMCU */

#ifdef _DMA
  #include "stm32f10x_dma.h"
#endif /*_DMA */

#ifdef _EXTI
  #include "stm32f10x_exti.h"
#endif /*_EXTI */

#ifdef _FLASH
  #include "stm32f10x_flash.h"
#endif /*_FLASH */

#ifdef _FSMC
  //#include "stm32f10x_fsmc.h"
#endif /*_FSMC */

#ifdef _GPIO
  #include "stm32f10x_gpio.h"
#endif /*_GPIO */

#ifdef _I2C
  //#include "stm32f10x_i2c.h"
#endif /*_I2C */

#ifdef _IWDG
  //#include "stm32f10x_iwdg.h"
#endif /*_IWDG */

#ifdef _NVIC
  #include "stm32f10x_nvic.h"
#endif /*_NVIC */

#ifdef _PWR
  //#include "stm32f10x_pwr.h"
#endif /*_PWR */

#ifdef _RCC
  #include "stm32f10x_rcc.h"
#endif /*_RCC */

#ifdef _RTC
  //#include "stm32f10x_rtc.h"
#endif /*_RTC */

#ifdef _SDIO
  #include "stm32f10x_sdio.h"
#endif /*_SDIO */

#ifdef _SPI
  //#include "stm32f10x_spi.h"
#endif /*_SPI */

#ifdef _SysTick
  #include "stm32f10x_systick.h"
#endif /*_SysTick */

#ifdef _TIM
  #include "stm32f10x_tim.h"
#endif /*_TIM */

#ifdef _USART
  #include "stm32f10x_usart.h"
#endif /*_USART */

#ifdef _WWDG
  //#include "stm32f10x_wwdg.h"
#endif /*_WWDG */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#include  <stdlib.h>
#include  "ucos_ii.h"
void debug(void);
#define ADC1_DR_Address    ((u32)0x4001244C)
#define LOCKTIME 10
#define ON 1
#define OFF 0
#define TCST_TIME 90
float DataConverter1(int P,int e0,int e1,int sv,int pv);
float DataConverter2(int P,int e0,int e1,int sv,int pv);
float DataConverter3(int P,int e0,int e1,int sv,int pv);
float DataConverter4(int P,int e0,int e1,int sv,int pv);
s16 ModData1(s32 data);
s16 ModData2(s32 data1,s32 data2,s32 data3);
void StartUp103x(u32 addNum);
UINT ConverTime(UINT data1,UINT data2,UINT data3);
s16 Get1(s32 data1);
s16 Get1ToGet2(s16 data1,s16 data2,UCHAR i);
s32 Get2(s32 data1,s16 data2);
s32 Get3(s32 data1,s16 data2);
s32 Get4(s32 data1,s16 data2);
s32 Get5(s32 data1,s16 data2);
void bps_init(void);
void TaskStartPID1(void *yydata);
void TaskStartPID2(void *yydata);
void TaskStartPID3(void *yydata);
void TaskStartPID4(void *yydata);
void TaskStartSaveData(void *yydata);
void TaskStartComm(void *yydata);
void TaskStartDisplay(void *yydata);
void TaskPID(void *id);
void delay(UINT num);
void Delay(UINT num);
void Relay(u8 channel,u8 command);
void PWM_OUT(u8 channel, u8 command);
void LED(u8 number,u8 command);
void DisLED(u8 number);
void ChTUP(s32 *p,BOOL *flag);
void SetInitData(void);
void LoadData(void);
s32 GetTemSV(s32 data);
void GetCommID(void);
void GetEnvTem(s16 *data);
void CCdataValue(unsigned char *data, unsigned int data1);
u8  Start(void);
void  Stop(void);
UCHAR RecAck(void);
void  NoAck(void);
void  Ack(void);
UCHAR Receive(void);
void  Send(UCHAR sendByte);
UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR );
void stTest_1(void);
void stTest_2(void);
void stTest_3(void);
void stTest_4(void);

#ifdef USER_Globals
#define EXT_USER
#else
#define EXT_USER extern
#endif
#define	TaskCreateDisplay()    	    OSTaskCreate(TaskStartDisplay, (void *)0, &TaskStartStkDisplay[449],10) //9
#define TaskCreatePID1()        	OSTaskCreate(TaskStartPID1, (void *)0, &TaskStartStkPID1[549],5); //4
#define TaskCreatePID2()        	OSTaskCreate(TaskStartPID2, (void *)0, &TaskStartStkPID2[549],6); //5
#define TaskCreatePID3()        	OSTaskCreate(TaskStartPID3, (void *)0, &TaskStartStkPID3[549],7); //6
#define TaskCreatePID4()        	OSTaskCreate(TaskStartPID4, (void *)0, &TaskStartStkPID4[549],8); //7
#define TaskRunPID()        	    OSTaskCreate(TaskPID,       (void *)0, &TaskStkPID[449],      9); //7
#define TaskCreateSaveData()        OSTaskCreate(TaskStartSaveData, (void *)0, &TaskStartStkSaveData[449],4) //8
EXT_USER OS_STK TaskStartStkPID1[550];
EXT_USER OS_STK TaskStartStkPID2[550];
EXT_USER OS_STK TaskStartStkPID3[550];
EXT_USER OS_STK TaskStartStkPID4[550];
EXT_USER OS_STK TaskStkPID[450];
EXT_USER OS_STK TaskStartStkSaveData[450];
EXT_USER OS_STK TaskStartStkDisplay[450];

#include "TableLED.txt"
EXT_USER INT8U *osErrCode;
EXT_USER OS_EVENT *SS1[5];
EXT_USER struct systemData SYS;
EXT_USER struct systemData0 SYS0;
EXT_USER u16 Reg3Data[40],Reg4Data[200];
EXT_USER struct ChannelData /*CHdata0,*/CHdata1,CHdata2,CHdata3,CHdata4;
EXT_USER struct Auto_Turn TurnData1,TurnData2,TurnData3,TurnData4;
EXT_USER struct systemData1 SYS1;
EXT_USER UCHAR allI,allN,alarmCount;

EXT_USER u16 startDelay1,startDelay2,startDelay3,startDelay4,checkOff,checkbak;
EXT_USER u8 checkError1,checkError2,checkError3,checkError4;

#endif /* __STM32F10x_LIB_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
