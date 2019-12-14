#define USER_Globals

#include "stm32f10x.h"
#include "userApp.h"
UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR );
void Task3(void);
#include "TableLED.txt"
BOOL disUpFlag=0;
void ConverToChar(UINT data,UINT *resultAddress,UCHAR pointNum,UCHAR PointEffer)
{
	UCHAR ps[4]={0,0,0,0};
	if(data>=10000) data=9999;
	if(PointEffer&&!SYS.DataSet[DPSW]) data/=10;
	ps[0]=(UCHAR)(data/1000%10)+0x30;
	ps[1]=(UCHAR)(data/100%10)+0x30;
	ps[2]=(UCHAR)(data/10%10)+0x30;
	ps[3]=(UCHAR)(data%10)+0x30;
	if((pointNum==1)&&(SYS.BlinkState==0))
	{
	
		if(ps[0]==0x30) resultAddress[0]=ascii[' '];
		else resultAddress[0]=ascii[ps[0]];
		if((ps[1]==0x30)&&(resultAddress[0]==ascii[' '])) resultAddress[1]=ascii[' '];
		else resultAddress[1]=ascii[ps[1]];
		resultAddress[2]=ascii[ps[2]]|0x8000;
		resultAddress[3]=ascii[ps[3]];
	}
	else if((pointNum==2)&&(SYS.BlinkState==0))
	{
		if(ps[0]==0x30) resultAddress[0]=ascii[' '];
		else resultAddress[0]=ascii[ps[0]];
		resultAddress[1]=ascii[ps[1]]|0x8000;
		resultAddress[2]=ascii[ps[2]];
		resultAddress[3]=ascii[ps[3]];
	}
	else if((pointNum==3)&&(SYS.BlinkState==0))
	{
		resultAddress[0]=ascii[ps[0]]|0x8000;
		resultAddress[1]=ascii[ps[1]];
		resultAddress[2]=ascii[ps[2]];
		resultAddress[3]=ascii[ps[3]];
	}
	else if(SYS.BlinkState&0xf0)
	{
		resultAddress[0]=ascii[ps[0]];
		resultAddress[1]=ascii[ps[1]];
		resultAddress[2]=ascii[ps[2]];
		resultAddress[3]=ascii[ps[3]];
		if(pointNum==1) resultAddress[2]|=0x8000;
		else if(pointNum==2) resultAddress[1]|=0x8000;
		else if(pointNum==3) resultAddress[0]|=0x8000;
	}
	else
	{
		if(ps[0]==0x30) resultAddress[0]=ascii[' '];
		else resultAddress[0]=ascii[ps[0]];
		if((ps[1]==0x30)&&(resultAddress[0]==ascii[' '])) resultAddress[1]=ascii[' '];
		else resultAddress[1]=ascii[ps[1]];
		if((ps[2]==0x30)&&(resultAddress[1]==ascii[' '])) resultAddress[2]=ascii[' '];
		else resultAddress[2]=ascii[ps[2]];
		resultAddress[3]=ascii[ps[3]];
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
void ConverToCharPV(UINT data,UINT *resultAddress,UCHAR pointNum,UCHAR PointEffer)
{
	UCHAR ps[4]={0,0,0,0};
	if(PointEffer&&!pointNum) data/=10;
	ps[0]=(UCHAR)(data/1000%10)+0x30;
	ps[1]=(UCHAR)(data/100%10)+0x30;
	ps[2]=(UCHAR)(data/10%10)+0x30;
	ps[3]=(UCHAR)(data%10)+0x30;
	if(pointNum==1)
	{
		if(ps[0]==0x30) resultAddress[0]=ascii[' '];
		else resultAddress[0]=ascii[ps[0]];
		if((ps[1]==0x30)&&(resultAddress[0]==ascii[' '])) resultAddress[1]=ascii[' '];
		else resultAddress[1]=ascii[ps[1]];
		resultAddress[2]=ascii[ps[2]]|0x8000;
		resultAddress[3]=ascii[ps[3]];
	}
	else if(pointNum==2)
	{
		if(ps[0]==0x30) resultAddress[0]=ascii[' '];
		else resultAddress[0]=ascii[ps[0]];
		resultAddress[1]=ascii[ps[1]]|0x8000;
		resultAddress[2]=ascii[ps[2]];
		resultAddress[3]=ascii[ps[3]];
	}
	else
	{
		if(ps[0]==0x30) resultAddress[0]=ascii[' '];
		else resultAddress[0]=ascii[ps[0]];
		if((ps[1]==0x30)&&(resultAddress[0]==ascii[' '])) resultAddress[1]=ascii[' '];
		else resultAddress[1]=ascii[ps[1]];
		if((ps[2]==0x30)&&(resultAddress[1]==ascii[' '])) resultAddress[2]=ascii[' '];
		else resultAddress[2]=ascii[ps[2]];
		resultAddress[3]=ascii[ps[3]];
	}
}

void DisErrorSave(u8 num)
{
	u8 i=0,j=0;
	i=num/10+0x30;
	j=num%10+0x30;
	SegLed[4]=ascii[i];
	SegLed[5]=ascii[j];
	SegLed[6]=ascii['-'];
	i=Error+num-1;
	if(SYS.DataSet[i]>10) SegLed[7]=ascii['0'];
	else if(SYS.DataSet[i]==10) {SegLed[6]=ascii['-']|ascii['1'];SegLed[7]=ascii['0'];}
	else SegLed[7]=ascii[SYS.DataSet[i]+0x30];
}
void DataFile(UINT sequence)
{	
	s16 dataBuff=SYS.DataGet[Tem_PV];
	if(dataBuff>5000) {dataBuff=0;if(SYS.ErrorCode<3) SYS.ErrorCode=TC_OP;}//TC open
	else 
	{
		if(SYS.ErrorCode==TC_OP) SYS.ErrorCode=0;
		if(SYS.DataSet[C_F]) dataBuff=9*dataBuff/5+320;
		if(SYS.DataSet[DPSW]==0) dataBuff+=5;
	}
	switch(sequence)
	{
		case 0: 
				ConverToCharPV(abs(dataBuff),&SegLed[0],SYS.DataSet[DPSW],1);
				if(dataBuff<0) SegLed[0]=ascii['-'];
		    if((SYS.ErrorCode==TC_OP)&&(SYS.SetMenu==0)) SegLed[0]=SegLed[1]=SegLed[2]=SegLed[3]=0x0100;
				ConverToChar(SYS.SV_Dis,&SegLed[4],SYS.DataSet[DPSW],1);
		//ConverToChar(divflag,&SegLed[4],SYS.DataSet[DPSW],1);
				break;
		case 1:		
				ConverToCharPV(abs(dataBuff),&SegLed[0],SYS.DataSet[DPSW],1);
		    if(dataBuff<0) SegLed[0]=ascii['-'];
		    if((SYS.ErrorCode==TC_OP)&&(SYS.SetMenu==0)) SegLed[0]=SegLed[1]=SegLed[2]=SegLed[3]=0x0100;
				ConverToChar(SYS.DataGet[Rate],&SegLed[4],1,0);
				SegLed[4]=ascii['O'];
				break;
		case 2:
				ConverToCharPV(abs(dataBuff),&SegLed[0],SYS.DataSet[DPSW],1);
		    if(dataBuff<0) SegLed[0]=ascii['-'];
		    if((SYS.ErrorCode==TC_OP)&&(SYS.SetMenu==0)) SegLed[0]=SegLed[1]=SegLed[2]=SegLed[3]=0x0100;
		        if(SYS.DataGet[Current] >= 1000)
				    ConverToChar(SYS.DataGet[Current]/10,&SegLed[4],1,0);//
				else
					ConverToChar(SYS.DataGet[Current],&SegLed[4],2,0);//
				SegLed[4]=ascii['C'];
				break;
		case 3:
				ConverToCharPV(abs(dataBuff),&SegLed[0],SYS.DataSet[DPSW],1);
		    if(dataBuff<0) SegLed[0]=ascii['-'];
		    if((SYS.ErrorCode==TC_OP)&&(SYS.SetMenu==0)) SegLed[0]=SegLed[1]=SegLed[2]=SegLed[3]=0x0100;
				ConverToChar(SYS.DataGet[Standby_SV],&SegLed[4],SYS.DataSet[DPSW],1);
				break;
		case 4:
				ConverToCharPV(abs(dataBuff),&SegLed[0],SYS.DataSet[DPSW],1);
				if(dataBuff<0) SegLed[0]=ascii['-'];
		    if((SYS.ErrorCode==TC_OP)&&(SYS.SetMenu==0)) SegLed[0]=SegLed[1]=SegLed[2]=SegLed[3]=0x0100;
				ConverToChar(SYS.ManualRate,&SegLed[4],1,0);
				SegLed[4]=ascii['A'];
				break;
		case 100:
				SegLed[0]=ascii['_'];
				SegLed[1]=ascii['S'];
				SegLed[2]=ascii['E'];
				SegLed[3]=ascii['T'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				SegLed[6]=ascii[' '];
				SegLed[7]=ascii['1'];
				break;
		case 101:
				SegLed[0]=ascii[' '];
				SegLed[1]=ascii[' '];
				SegLed[2]=ascii['I'];
				SegLed[3]=ascii['N'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				SegLed[6]=ascii[' '];
				if(SYS.DataSet[In]) SegLed[7]=ascii['K'];
				else SegLed[7]=ascii['J'];
				break;
		case 102:
				SegLed[0]=ascii[' '];
				SegLed[1]=ascii['C'];
				SegLed[2]=ascii['-'];
				SegLed[3]=ascii['F'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				SegLed[6]=ascii[' '];
				if(SYS.DataSet[C_F]) SegLed[7]=ascii['F'];
				else SegLed[7]=ascii['C'];
				break;
		case 103:
				SegLed[0]=ascii['U'];
				SegLed[1]=ascii['N'];
				SegLed[2]=ascii['I'];
				SegLed[3]=ascii['T'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				if(SYS.DataSet[DPSW]) 
				{
					SegLed[6]=ascii['0']|0x8000;
					SegLed[7]=ascii['1'];
				}

				else
				{
					SegLed[6]=ascii['1'];
					SegLed[7]=ascii['0'];
				}
				break;
		case 104:
				SegLed[0]=ascii['H'];
				SegLed[1]=ascii['S'];
				SegLed[2]=ascii['C'];
				SegLed[3]=ascii['I'];
				SegLed[4]=ascii[' '];
				if(SYS.DataSet[HSCI]) 
				{
					SegLed[5]=ascii['S'];
					SegLed[6]=ascii['S'];
					SegLed[7]=ascii['R'];
				}
				else
				{
					SegLed[5]=ascii['P'];
					SegLed[6]=ascii['W'];
					SegLed[7]=ascii['M'];
				}
				break;
		case 105:
				SegLed[0]=ascii['T'];
				SegLed[1]=ascii['U'];
				SegLed[2]=ascii['R'];
				SegLed[3]=ascii['N'];
				SegLed[4]=ascii[' '];
				if(SYS.DataSet[TURN]) 
				{
					SegLed[5]=ascii[' '];
					SegLed[6]=ascii['O'];
					SegLed[7]=ascii['N'];
				}

				else
				{
					SegLed[5]=ascii['O'];
					SegLed[6]=ascii['F'];
					SegLed[7]=ascii['F'];
				}
				break;
		case 106:
				SegLed[0]=ascii['B'];
				SegLed[1]=ascii['U'];
				SegLed[2]=ascii['Z'];
				SegLed[3]=ascii['Z'];
				SegLed[4]=ascii[' '];
				if(SYS.DataSet[BUZ]) 
				{
					SegLed[5]=ascii[' '];
					SegLed[6]=ascii['O'];
					SegLed[7]=ascii['N'];
				}

				else
				{
					SegLed[5]=ascii['O'];
					SegLed[6]=ascii['F'];
					SegLed[7]=ascii['F'];
				}
				break;
		case 107:
				SegLed[0]=ascii[' '];
				SegLed[1]=ascii['L'];
				SegLed[2]=ascii['O'];
				SegLed[3]=ascii['C'];
				SegLed[4]=ascii[' '];
				if(SYS.DataSet[LOC]==0) 
				{
					SegLed[5]=ascii['O'];
					SegLed[6]=ascii['F'];
					SegLed[7]=ascii['F'];
					
				}
				else if(SYS.DataSet[LOC]==1) 
				{
					SegLed[5]=ascii['O'];
					SegLed[6]=ascii['N'];
					SegLed[7]=ascii['1'];
				}
				else
				{
					SegLed[5]=ascii['O'];
					SegLed[6]=ascii['N'];
					SegLed[7]=ascii['2'];
				}
				break;
		case 108:
				SegLed[0]=ascii[' '];
				SegLed[1]=ascii[' '];
				SegLed[2]=ascii['I'];
				SegLed[3]=ascii['D'];
				ConverToChar(SYS.DataSet[ID],&SegLed[4],0,0);
				SegLed[4]=ascii[' '];
				break;
		case 200:
				SegLed[0]=ascii['_'];
				SegLed[1]=ascii['S'];
				SegLed[2]=ascii['E'];
				SegLed[3]=ascii['T'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				SegLed[6]=ascii[' '];
				SegLed[7]=ascii['2'];
				break;
		case 201:
				SegLed[0]=ascii['A'];
				SegLed[1]=ascii['L'];
				SegLed[2]=ascii['-'];
				SegLed[3]=ascii['H'];
				ConverToChar(SYS.DataSet[AL_H],&SegLed[4],1,0);
				SegLed[4]=ascii[' '];
				
				break;
		case 202:
				SegLed[0]=ascii['A'];
				SegLed[1]=ascii['L'];
				SegLed[2]=ascii['-'];
				SegLed[3]=ascii['L'];
				ConverToChar(SYS.DataSet[AL_L],&SegLed[4],1,0);
				SegLed[4]=ascii['-'];
				
				break;
		case 203:
				SegLed[0]=ascii['H'];
				SegLed[1]=ascii['C'];
				SegLed[2]=ascii['-'];
				SegLed[3]=ascii['H'];
				ConverToChar(SYS.DataSet[HC_H],&SegLed[4],1,0);
				SegLed[4]=ascii['C'];
				break;
		case 204:
				SegLed[0]=ascii['H'];
				SegLed[1]=ascii['C'];
				SegLed[2]=ascii['-'];
				SegLed[3]=ascii['L'];
				ConverToChar(SYS.DataSet[HC_L],&SegLed[4],1,0);
				//ConverToChar(TurnData2.Ymin2/100,&SegLed[4],0,0);
				SegLed[4]=ascii['C'];
				break;
		
		case 205:
				SegLed[0]=ascii['S'];
				SegLed[1]=ascii['O'];
				SegLed[2]=ascii['F'];
				SegLed[3]=ascii['T'];
				ConverToChar(SYS.DataSet[SO_T],&SegLed[4],0,0);
				SegLed[4]=ascii['T'];
				SegLed[5]=ascii[' '];
				break;
		case 206:
				SegLed[0]=ascii['S'];
				SegLed[1]=ascii['O'];
				SegLed[2]=ascii['F'];
				SegLed[3]=ascii['T'];
				ConverToChar(SYS.DataSet[SO_R],&SegLed[4],1,0);
				SegLed[4]=ascii['A'];
				break;
		case 207:
				SegLed[0]=ascii['S'];
				SegLed[1]=ascii['T'];
				SegLed[2]=ascii['B'];
				SegLed[3]=ascii['Y'];
				ConverToChar(SYS.DataSet[ST_T],&SegLed[4],2,0);
				SegLed[4]=ascii['T'];
				break;
		case 208:
				SegLed[0]=ascii['S'];
				SegLed[1]=ascii['T'];
				SegLed[2]=ascii['B'];
				SegLed[3]=ascii['Y'];
				ConverToChar(SYS.DataSet[ST_R],&SegLed[4],2,0);
				SegLed[4]=ascii['P'];
				break;
		case 209:
				SegLed[0]=ascii['-'];
				SegLed[1]=ascii['E'];
				SegLed[2]=ascii['R'];
				SegLed[3]=ascii['-'];
				DisErrorSave(SYS.DataSet[DisErrNum]);
				break;
		case 300:
				SegLed[0]=ascii['_'];
				SegLed[1]=ascii['S'];
				SegLed[2]=ascii['E'];
				SegLed[3]=ascii['T'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				SegLed[6]=ascii[' '];
				SegLed[7]=ascii['3'];
				break;
		case 301:
				SegLed[0]=ascii[' '];
				SegLed[1]=ascii[' '];
				SegLed[2]=ascii[' '];
				SegLed[3]=ascii['1'];
				break;
		case 302:
				SegLed[0]=ascii[' '];
				SegLed[1]=ascii[' '];
				SegLed[2]=ascii[' '];
				SegLed[3]=ascii['2'];
				break;
		case 303: break;
		case 304: break;	
		case 400:
				SegLed[0]=ascii['_'];
				SegLed[1]=ascii['S'];
				SegLed[2]=ascii['E'];
				SegLed[3]=ascii['T'];
				SegLed[4]=ascii[' '];
				SegLed[5]=ascii[' '];
				SegLed[6]=ascii[' '];
				SegLed[7]=ascii['4'];
				break;
		case 401:
				ConverToCharPV(SYS.TemVoltage/10,&SegLed[0],2,0);
				ConverToChar(SYS.DataSet[HT_Z],&SegLed[4],0,0);
				break;
		case 402:
				ConverToCharPV(SYS.TemVoltage/10,&SegLed[0],2,0);
				ConverToChar(SYS.DataSet[HT_A],&SegLed[4],3,0);
				break;
		case 403:
				ConverToChar(abs(SYS.DataGet[Env_sensor]/10),&SegLed[0],1,0);
				if(SYS.DataGet[Env_sensor]<0) 
				{
					ConverToChar(abs(SYS.DataGet[Env_sensor]/100),&SegLed[0],0,0);
					SegLed[1]=ascii['-'];
				}
				SegLed[0]=ascii['E'];
				ConverToChar(SYS.DataSet[ET_Z],&SegLed[4],0,0);
				break;
		default:
				break;
		
	}
}
void DataInit(void)
{
	UCHAR i=0;
	SYS.DataSet[TURN]=OFF;
	SYS.DataSet[ET_Z]=800;
	SYS.DataSet[HT_Z]=0;
	SYS.DataSet[HT_A]=1000;
	SYS.DataSet[P]=20;
	SYS.DataSet[DPSW]=ON;
	SYS.DataSet[BUZ]=ON;
	SYS.DataSet[LOC]=0;
	SYS.DataSet[Tem_SV]=2000;
	SYS.DataSet[AL_H]=500;
	SYS.DataSet[AL_L]=500;
	SYS.DataSet[HC_H]=160;
	SYS.DataSet[HC_L]=2;
	SYS.DataSet[SO_T]=1;
	SYS.DataSet[SO_R]=200;
	SYS.DataSet[ST_T]=100;
	SYS.DataSet[ST_R]=50;
	for(i=0;i<20;i++) SYS.DataSet[Error+i]=0;
	SYS.DataSet[EE]=0xaa;
	CommIIC(SYS.DataSet,100,0,0);
}
void LoadData(void)
{
	SYS.SSR_Count=0;
	SYS.SsrStartBuff=SYS.SsrStart=20000;
	SYS.PWM_Period=100;
	if(CommIIC(SYS.DataSet,2,0,1)==0)
	{
		if(SYS.DataSet[EE]==0xaa) CommIIC(SYS.DataSet,100,0,1);
		else DataInit();
	}
	if(SYS.DataSet[ID]>255) SYS.DataSet[ID]=0;
	SYS.DataSet[TURN]=0;
	SYS.StandbyTime=SYS.StandbyTime=SYS.DataSet[ST_T]/100*3600+SYS.DataSet[ST_T]%100*60;
	if(SYS.DataSet[C_F]==0) SYS.SV_Dis=SYS.DataSet[Tem_SV];
	else SYS.SV_Dis=9*SYS.DataSet[Tem_SV]/5+320;
}
//void HardFault_Handler(void)  __attribute__ ((alias("HardFaultException")));
//void SysTick_Handler(void)    __attribute__ ((alias("SysTickHandler")));
//void HardFaultException(void){}
UINT DisplayAddress[9]={0x0008,0x0004,0x0002,0x0001,0x0080,0x0040,0x0020,0x0010,0x0100};
void Task2(void);
void Txcom(uint16_t rad);
void SysTickProc(void) //0.1ms
{

	if(trun > 0) trun --;
	if(stTime >0) stTime --;		
	if(disUpFlag==0) return;
	if(SYScycle<20000) SYScycle++;	
	if(comRx >0) { comRx --; if(comRx == 0) Rxcom();}
	if(comTx >0) comTx --; else Txcom(SYScycle);
	switch(NumLed)
	{
		case 0: {GPIOC->ODR =LED[0];GPIOC->BSRR = DisplayAddress[0];break;}
		case 3: GPIOC->ODR = 0x0000;break;
		case 10: {GPIOC->ODR =LED[1];GPIOC->BSRR = DisplayAddress[1];break;}
		case 13: GPIOC->ODR = 0x0000;break;
		case 20: {GPIOC->ODR =LED[2];GPIOC->BSRR = DisplayAddress[2];break;}
		case 23: GPIOC->ODR = 0x0000;break;
		case 30: {GPIOC->ODR =LED[3];GPIOC->BSRR = DisplayAddress[3];break;}
		case 33: GPIOC->ODR = 0x0000;break;
		case 40:{GPIOC->ODR =LED[4];GPIOC->BSRR = DisplayAddress[4];break;}
		case 49:GPIOC->ODR = 0x0000;break;
		case 50:{GPIOC->ODR =LED[5];GPIOC->BSRR = DisplayAddress[5];break;}
		case 59:GPIOC->ODR = 0x0000;break;		 
		case 60:{GPIOC->ODR =LED[6];GPIOC->BSRR = DisplayAddress[6];break;}
		case 69:GPIOC->ODR = 0x0000;break;
		case 70:{GPIOC->ODR =LED[7];GPIOC->BSRR = DisplayAddress[7];break;}
		case 79:GPIOC->ODR = 0x0000;break;
		case 80:{GPIOC->ODR =LED[8];GPIOA->BSRR = GPIO_Pin_8;}break;
		case 83:{GPIOC->ODR = 0x0000;GPIOA->BRR = GPIO_Pin_8;break;}
		case 90:GPIOC->CRH&=0xffff0000;GPIOC->CRH|=0x4444;break;
		case 92:if(GPIOC->IDR & GPIO_Pin_8) ShiftKey=1;
				else  ShiftKey=0;
				if(GPIOC->IDR & GPIO_Pin_9) UpKey=1;
				else UpKey=0;
				if(GPIOC->IDR & GPIO_Pin_10) ModeKey=1;
				else ModeKey=0;
				if(GPIOC->IDR & GPIO_Pin_11) DownKey=1;
				else DownKey=0;
				GPIOC->CRH&=0xffff0000;GPIOC->CRH|=0x2222;
				break;
		default: break;
	}
	if(NumLed>=92) NumLed=0;
	else NumLed++;
	if(SYS.PWM_GetPeriod1<9999) SYS.PWM_GetPeriod1++;	
	if(KeyResCycle<300) KeyResCycle++;
	else
	{
		KeyResCycle=0;
		Task2();
	}
  
}
void DisError(u8 num)
{
	switch(num)
	{
		case TC_OP : 	if(SYS.RunningMode!=2) 
						{SegLed[0]=ascii['T'];SegLed[1]=ascii['C']|0x8000;SegLed[2]=ascii['O'];SegLed[3]=ascii['P'];}break;
		case TC_ST :	if(SYS.RunningMode!=2)
						{SegLed[0]=ascii['T'];SegLed[1]=ascii['C']|0x8000;SegLed[2]=ascii['S'];SegLed[3]=ascii['T'];}break;
		case TC_RE :	if(SYS.RunningMode!=2)
						{SegLed[0]=ascii['T'];SegLed[1]=ascii['C']|0x8000;SegLed[2]=ascii['R'];SegLed[3]=ascii['E'];}break;
		case ALARM_H:	SegLed[0]=ascii['A'];SegLed[1]=ascii['L'];		 SegLed[2]=ascii['-'];SegLed[3]=ascii['H'];break;
		case ALARM_L:	SegLed[0]=ascii['A'];SegLed[1]=ascii['L'];		 SegLed[2]=ascii['-'];SegLed[3]=ascii['L'];break;
		case HT_OP :	SegLed[0]=ascii['H'];SegLed[1]=ascii['T']|0x8000;SegLed[2]=ascii['O'];SegLed[3]=ascii['P'];break;
		case HT_ST :	SegLed[0]=ascii['H'];SegLed[1]=ascii['T']|0x8000;SegLed[2]=ascii['S'];SegLed[3]=ascii['T'];break;
		case TR_ST :	SegLed[0]=ascii['T'];SegLed[1]=ascii['R']|0x8000;SegLed[2]=ascii['S'];SegLed[3]=ascii['T'];break;
		case F1_OP :	SegLed[0]=ascii['F'];SegLed[1]=ascii['U']|0x8000;SegLed[2]=ascii['-'];SegLed[3]=ascii['1'];break;
		case F2_OP :	SegLed[0]=ascii['F'];SegLed[1]=ascii['U']|0x8000;SegLed[2]=ascii['-'];SegLed[3]=ascii['2'];break;
		default:    break;
	}
}

void Buzzer(BOOL command)
{
	if(command==ON) GPIOB->BSRR = GPIO_Pin_11;
	else GPIOB->BRR = GPIO_Pin_11;
}
void StartDisplay(void);
void Task1(void);

void stTest(void);

int main(void)
{
	
	static UCHAR T3Cycle=0;
	UCHAR i=0,n=0;comTx = 5000;
	BSP_Init();;
//	Delay(20000);
	stTest();
	disUpFlag=1;
	LED[0]=ascii['F'];
	LED[1]=ascii['U'];
	LED[2]=ascii['Z'];
	LED[3]=ascii['Y'];
	LED[4]=ascii['I'];
	LED[5]=ascii['C'];
	LED[6]=ascii['-'];	
	if(SYS.DataSet[In]) LED[7]=ascii['K'];
	else LED[7]=ascii['J'];
	LoadData();	
	StartDisplay();
	SYS.AD_AddValue[0]=SYS.AD_num0=SYS.AD_AddValue[1]=SYS.AD_num1=SYS.AD_AddValue[2]=SYS.AD_num2=0;
	SYScycle=0;
	LED_auto_on;
	
	while(1)
	{		
		while(SYScycle<1000);	
		SYScycle=0;   //100MS
		if(T3Cycle<10) //1s
		T3Cycle++;
		else 
		T3Cycle=1;
		if((T3Cycle==5)||(T3Cycle==10)) 
			Task3();     //0.5s
		if(T3Cycle&0x01) continue; //200ms
		       Task1(); 
	  switch(SYS.SetMenu)
		{
			case 0:  DataFile(SYS.SetStep);          break;
			case 1:  DataFile(100+SYS.SetStep);      break;
			case 2:  DataFile((UINT)200+SYS.SetStep);break;
			case 3:  DataFile(300+SYS.SetStep);      break;
			case 4:  DataFile(400+SYS.SetStep);      break;
		    default: break;
		}
		if(SYS.ErrorCode)
		{
			if(n<5) {if(SYS.SetMenu==0) DisError(SYS.ErrorCode);}
			if((n==0)&&SYS.DataSet[BUZ]) Buzzer(ON);
			else if(n==5) Buzzer(OFF);
			if(n<10) n++;
			else n=0;
		}
		else {n=0;Buzzer(OFF);}
		if(SYS.BlinkState)
		{
			for(i=0;i<8;i++)
			{
				if(SYS.BlinkState&(0x0001<<i)) 
				{
					if((SYS.BlinkNum&0x01)==0) LED[i]=SegLed[i];
					else if((SYS.BlinkNum&0x01)==1) LED[i]=ascii[' '];
				}
				else  LED[i]=SegLed[i];
			}
			if(SYS.BlinkNum++>=25) {SYS.BlinkState=0;SYS.BlinkNum=0;}
		}
		else
		{
			for(i=0;i<8;i++)  LED[i]=SegLed[i];
			SYS.BlinkNum=0;
		}
		LED[8]=SegLed[8];
		LED[9]=SegLed[9];
		if(SYS.LockCount)
		{
			SYS.LockCount--;
			LED[4]=ascii[' '];
			LED[5]=ascii['L'];
			LED[6]=ascii['O'];
			LED[7]=ascii['C'];
		}
		if(SYS.HomeBackTime)
		{
			if((--SYS.HomeBackTime==0)&&(SYS.SetMenu<3)&&SYS.SetMenu)
			{
				SYS.BlinkNum=0;
				SYS.BlinkState=0;
				SYS.SetMenu=0;
				if(SYS.RunningMode==0) 	SYS.SetStep=0;
				else if(SYS.RunningMode==1) SYS.SetStep=3;
				else if(SYS.RunningMode==2) SYS.SetStep=4;
				else SYS.SetStep=0;
			}
		}
	}
}

