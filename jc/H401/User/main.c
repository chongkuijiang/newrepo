#define USER_Globals

#include "stm32f10x.h"
#include "userApp.h"
UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR );
void Task3(void);
#include "TableLED.txt"
BOOL disUpFlag=0;

void DataInit(void)
{
	UCHAR i=0;
	SYS.DataSet[TURN]=OFF;
	SYS.DataSet[ET_Z]=800;
	SYS.DataSet[HT_Z]=0;
	SYS.DataSet[HT_A]=1000;
	SYS.DataSet[P]=20;
	SYS.DataSet[DPSW]=ON;
	SYS.DataSet[ON_OFF]=ON;
	SYS.DataSet[STSV]=1500;
	SYS.DataSet[SV_WK]=SYS.DataSet[Tem_SV]=2000;
	SYS.DataSet[AL_H]=500;
	SYS.DataSet[AL_L]=500;
	SYS.DataSet[HC_H]=160;
	SYS.DataSet[HC_L]=2;
	SYS.DataSet[SO_T]=1;
	SYS.DataSet[SO_R]=200;
	SYS.DataSet[EN]=1;
	SYS.DataSet[ID]=0;
	for(i=0;i<20;i++) SYS.DataSet[Error+i]=0;
	SYS.DataSet[EE]=0xaa;
	CommIIC(SYS.DataSet,100,0,0);
    
}
void LoadData(void)
{
	SYS.SSR_Count=0;
	SYS.SsrStartBuff=SYS.SsrStart=20000;
	SYS.PWM_Period=100;
    version = 12;
	if(CommIIC(SYS.DataSet,2,0,1)==0)
	{
		if(SYS.DataSet[EE]==0xaa) CommIIC(SYS.DataSet,100,0,1);
		else DataInit();
	}
	if(SYS.DataSet[ID]>255) SYS.DataSet[ID]=0;
	SYS.DataSet[TURN]=0;

	if(SYS.DataSet[C_F]==0) {
        SYS.SV_Dis=SYS.DataSet[Tem_SV];
        SYS.stSV_Dis = SYS.DataSet[STSV];
	}else {
        SYS.SV_Dis=9*SYS.DataSet[Tem_SV]/5+320;
        SYS.stSV_Dis = 9*SYS.DataSet[STSV]/5+320;
    }
}

void Task2(void);
void SysTickProc(void) //0.1ms
{
//    static uint8_t key;
	if(trun > 0) trun --;
	if(stTime >0) stTime --;	
   if(comRx >0) { comRx --; if(comRx == 0) Rxcom();}

	if(disUpFlag==0) return;
	if(SYScycle<20000) SYScycle++;		
	if(SYS.PWM_GetPeriod1<9999) SYS.PWM_GetPeriod1++;	

}

void stDelay(){
	int i = 3000;
	while(i-->0);
}
void StartDisplay(void);
void Task1(void);

void stTest(void);
uint8_t getID(void);

void IWDG_Config(uint8_t prv ,uint16_t rlv);
int main(void)     
{
	static UCHAR T3Cycle=0;
	BSP_Init();
    LED_WORK_ON;
    LED_ERR_OFF;
	comAddrss = getID();
    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET) {
		RCC_ClearFlag();
		disUpFlag=1;		
		LoadData();         	
     SYS.ErrorCode = NoError;		 
	   goto runed;
	}
	LoadData(); 
    SYS.DataSet[ID] = comAddrss;
    SYS.RunningMode = 3;  //stop
    stDelay();	
	checkOff = (GPIOB->IDR & GPIO_Pin_5)? 0:1;  //ID7-PB5  
	
	if(checkOff) stTest();      	
	disUpFlag=1;	
	StartDisplay();
	SYS.AD_AddValue[0]=SYS.AD_num0=SYS.AD_AddValue[1]=SYS.AD_num1=SYS.AD_AddValue[2]=SYS.AD_num2=0;
    SYScycle=0;
   runed:	   
    IWDG_Config(IWDG_Prescaler_64 ,250); //400
	while(1)
	{	
		while(!SYS.DataSet[ON_OFF]){  //enable
			SYS.DataGet[Rate]=0;
            GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			IWDG_ReloadCounter();
		}	
        
		while(SYScycle<1000);	
		SYScycle=0;   //100MS
        
		if(T3Cycle<10) //1s
		T3Cycle++;
		else  
		T3Cycle=1;
        
		if((T3Cycle==5)||(T3Cycle==10)) 
		{	
		    Task3();   
		} //0.5s
		if(T3Cycle&0x01) continue; //200ms-400
               
		       Task1(); 
     
		
		
		IWDG_ReloadCounter();
	}
}

