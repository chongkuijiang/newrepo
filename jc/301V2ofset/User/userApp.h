

#ifndef __USER_APP_H_
#define __USER_APP_H_

#include "stm32f10x.h"
#include <stdlib.h>


#define LOCKTIME 3
#define	LED_C_on	   		SegLed[8]|=0x0100
#define	LED_C_off	   		SegLed[8]&=0xfe00
#define	LED_F_on			SegLed[8]|=0x0200
#define	LED_F_off 		    SegLed[8]&=0xfd00
#define	LED_out_on 	   	    SegLed[8]|=0x0200
#define	LED_out_off 	   	SegLed[8]&=0xfd00
#define	LED_soft_on  	   	SegLed[8]|=0x0100
#define	LED_soft_off	   	SegLed[8]&=0xfe00
#define	LED_auto_on		 	SegLed[8]|=0x0400
#define	LED_auto_off  	   	SegLed[8]&=0xfb00
#define	LED_stby_on 		SegLed[8]|=0x0800
#define	LED_stby_off	    SegLed[8]&=0xf700
#define	LED_manu_on 	    SegLed[8]|=0x1000
#define	LED_manu_off 	    SegLed[8]&=0xef00
#define	LED_J_on	   		SegLed[8]|=0x2000
#define	LED_J_off	   		SegLed[8]&=0xdf00
#define	LED_K_on			SegLed[8]|=0x4000
#define	LED_K_off 		    SegLed[8]&=0xbf00

#define ON 1
#define OFF 0
#define TR_ON  GPIOB->BSRR = GPIO_Pin_8
#define TR_OFF GPIOB->BRR = GPIO_Pin_8
#define KeyStatus 	(GPIOB->IDR & GPIO_Pin_0)

enum NumNameSet{
	EE,   In,   C_F,  DPSW, HSCI,   TURN,      BUZ,       LOC, ID, AL_H,
	AL_L, HC_H, HC_L, SO_T, SO_R,   ST_T,      ST_R,      P,   P1, P2,
   HT_Z, HT_A, ET_Z, ET_A, Tem_SV, Current_Z, DisErrNum, OFS,
   Error};
enum NumNameRead{Tem_HT,Tem_PV,Standby_SV,Tem_sensor,Tem_E,Env_sensor,EnvPID,Current,Rate,RateD};
enum EnumRunningState{softStartup,autoTurn,autorunning,manual,standby};
enum EnumErrorCode{NoError,ALARM_H,ALARM_L,TC_OP,F1_OP,F2_OP,HT_OP,TC_ST,TC_RE,HT_ST,TR_ST};
//-----------------------------------------------------------------------------------------------

typedef unsigned short UINT;
typedef	unsigned char  BOOL;	
typedef	unsigned char  UCHAR;	
typedef signed short INT;
struct Auto_Turn
{
	s32 NSV;
	s32 Ymax1;
	s32 Ymax2;
	s32 Ymin1;
	s32 Ymin2;
	u16 softUpCounter;
	u16 T;
	u16 t1;
	s32 TemCompare[3];
	u8 step;
	u8 upDownFlag;
	u8 checkStep;
	s16 checkTemBak[8];
};
struct Auto_Turn_all
{
	u32 TemBak;
	u32 TemBak1;
	u32 TemNSV;
	u32 buff;
	u16 TimeBak[10];
	u16 t;
	u16 t1;
};
struct systemData
{
	UCHAR SetMenu;
	UCHAR SetStep;
	UINT BlinkState;
	UINT BlinkNum;
	u16 LockCount;
	UINT WriteState;
	UCHAR RunningMode;
	u16 ManualRate;
	s16 DataSet[50];
	s16 DataGet[20];
	u16 StandbyTime;
	float p;
	s32 TemPID;
	s32 TemSV;
	s32 TemPV;
	s16 SV_Dis;
	UINT TemBakAD;
	UINT timerCycle[5];
	u8 ErrorCode;
	
	UINT AD_Value[3];
	u32 AD_AddValue[3]; 
	u32 TemOffSetValue;
	u32 TemVoltage;
	u32 RunTimeOver;
	UINT AD_num0;
	UINT AD_num1;
	UINT AD_num2;
	
	BOOL AutoFlag;
	BOOL TurnFlag;
	UINT PWM_GetPeriod1;
	UINT PWM_GetPeriod2;
	u8 	PeriodCycle1;
	u8 	PeriodCycle2;
	u8 	GetPeriodCount;
	UINT PWM_Period;
	UINT SSR_Count;
	UINT SsrStart;
	UINT SsrStartBuff;
	UINT ZeroCount;
	UINT ZeroCountBuff;

	UINT HomeBackTime;
	BOOL KeyDownFlag;
	s32 testData0;
	s32 testData1;
	s32 testData2;
};

//-----------------------------------------------------------------------------------------------
#ifdef USER_Globals
#define EXT_USER
#else
#define EXT_USER extern
#endif
float DataConverter(int P,int e0,int e1,int sv,int pv);
s16 ModData1(s32 data);
s16 ModData2(s32 data1,s32 data2,s32 data3);
void BSP_Init(void);
UINT ConverTime(UINT data1,UINT data2,UINT data3);
s16 Get1(s32 data1);
s16 Get1ToGet2(s16 data1,s16 data2,UCHAR i);
s32 Get2(s32 data1,s16 data2);
EXT_USER struct systemData SYS;
EXT_USER struct Auto_Turn TurnData,TurnData2;
EXT_USER struct Auto_Turn_all TurnDataAll;
EXT_USER UINT SegLed[10],LED[10],NumLed,DivNumPV,SYScycle,KeyResCycle,power;
EXT_USER u8 ErrSaveData[20],UpOrDownToSV_Flag,disCheck,divflag,ModeKey,ShiftKey,UpKey,DownKey,LEDcount,PvCount,CountStep;

/*
STM32F10X_MD,USE_STDPERIPH_DRIVER
..\..\Libraries\CMSIS\CM3\CoreSupport;..\..\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x;..\..\Libraries\STM32F10x_StdPeriph_Driver\inc;..\..\User
CopyHex_Flash.bat
copy Objects\ACR301M.hex ..\HEX\ACR301M.hex
*/



#endif 



