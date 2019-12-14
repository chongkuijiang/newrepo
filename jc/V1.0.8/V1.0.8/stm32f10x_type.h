/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_type.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the common data types used for the
*                      STM32F10x firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_TYPE_H
#define __STM32F10x_TYPE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;
typedef unsigned short UINT;
typedef	unsigned char  BOOL;	
typedef	unsigned char  UCHAR;
typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

//0 1设置温度 2开关 3传感器类型   5pwm或者ssr  6软启动开关  9报警温度上限 10报警温度下限 11软启动时间 12软启动输出 13电流上限 14电流下限 15待机时间 16待机输出百分比
enum NumNameSet{Nop0,
	Tem_SV, //2,3,4,5
	OnOff,  //6,7,8,9
	In,     //10,11,12,13
	C_F,   //14,15,16,17
	HSCI,  //18,19,20,21
	SWSF,  //22,23,24,25
	Nop1,  //mode//26,27,28,29
	TURN,  //34,35,36,37
	AL_H,  //38,39,40,41
	AL_L,  //42,43,44,45
	SO_T,  //46,47,48,49
	SO_R,  //50,51,52,53
	HC_H,  //54,55,56,57
	HC_L,  //58,59,60,61
	ST_T,  //62,63,64,65
	ST_R,  //66,67,68,69
	P,    //70,71,72,73
	I,    //74,75,76,77
	D,    //78,79,80,81
	SampFreq, //86,87,88,89
    Nop2,
    Man1,   //82,83,84,85
    DisErrNum,
    Error
};
enum NumNameSetCal{ET_Z,HT_Z1,HT_A1,HT_Z2,HT_A2,HT_Z3,HT_A3,HT_Z4,HT_A4};
enum NumNameRead{Tem_PV,Current,Rate,Power,Status,Tem_HT,Tem_sensor};
enum EnumErrorCode{READY,STOP,SOFTSTARTUP,AUTOTURN,AUTO,STANDBY,MANUAL,
ALARM_H=11,
ALARM_L,
TC_OP,
TC_RE,
TC_ST,
HT_OP,
HT_ST,
TR_ST,
Fu_OP,
HC_RE //20
};
struct Auto_Turn
{
	s32 NSV;
	s32 buff;
	s32 Ymax1;
	s32 Ymin1;
	u16 softUpCounter;
	u16 T;
	s32 TemCompare[3];
	u8 step;
	BOOL upDownFlag;
};
struct ChannelData
{
    UCHAR RunningMode;
	u16 ManualRate;
	s16 DataSet[32];
	s16 DataGet[20];
    u16 StandbyTime;
    u16 TR_OpenTime;
	u16 TemBakAD;
	s32 TemPID;
    u32 TemVoltage;
    s16 SV_Dis;
    s32 TemSV;
	s32 TemPV;
	s32 P;
    u8 ErrorCode;
    BOOL AutoFlag;
	BOOL TcopFlag;
    UINT SSR_Count;
	UINT SsrStart;
	UINT SsrStartBuff;
    UINT ZeroCount;
    BOOL TcShortFlag;
	u8 TcShortCount;
    u8 UpOrDownToSV_Flag;
	u8 TurnFlag;
    u16 StandbyCount;
    UINT NumLed;
    UINT DivNumPV;
    s16 TemBakPV[21];
};
struct systemData
{
    u8 ID;
    u16 PowerValue;
    s16 DataCalSet[10];
    s16 Tem_E;
	UINT WriteState;	
    u32 TemOffSetValueJ;
	u32 TemOffSetValueK;			
	UINT AD_Value[18];
	u32 AD_AddValue[9]; 	
	UINT AD_num[9];
    UINT PWM_GetPeriod;
	UINT PWM_GetPeriod1;
	UINT PWM_GetPeriod2;
    UINT PWM_GetPeriod3;
	UINT PWM_GetPeriod4;
	u8 GetPeriodCount;
	u8 PeriodCycle;
	UINT PWM_Period;	
	s32 testData1;
};

struct systemData1
{
	UCHAR Cata1;
	UCHAR Cata2;
	UCHAR CommID;
	UCHAR *Cata3;
	UCHAR	CNt1;
	UCHAR	CNt2;
	u16	CNt3;
  UCHAR CNF;
	UCHAR Cata4;
	UINT Cata5;
	BOOL CNF1;
//	UCHAR Cata6[40];
	UCHAR Cata6[400];
	UCHAR Cata7[200];	
};
struct systemData0
{
	UCHAR data1;
	UCHAR	data2;
	UCHAR data3;
	UCHAR data4;
	UCHAR NUM1[10];
	UCHAR NUM2[40];
	UCHAR NUM3[20];		
};
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __STM32F10x_TYPE_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
