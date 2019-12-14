#include "stm32f10x.h"
#include "userApp.h"

extern UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR );

void GetTemSV(void)
{
	SYS.TemSV=(s32)SYS.DataSet[Tem_SV]*10;
}
void CheckTemUpDown(s32 *p)
{
	if((p[1]>p[0])&&(p[2]>p[0])) 
	{	
		if(p[1]>p[2]) p[0]=p[2];
		else  p[0]=p[1];
		TurnData.upDownFlag=1;
	}
	else if((p[0]>p[1])&&(p[0]>p[2]))  
	{
		if(p[1]>p[2]) p[0]=p[1];
		else  p[0]=p[2];
		TurnData.upDownFlag=0;
	}
	else if((p[0]==p[1])&&(p[0]==p[2])) TurnData.upDownFlag=3;
}

void ResetManualTurn(void)
{
	TurnData2.Ymax1=0;
	TurnData2.Ymax2=0;
	TurnData2.Ymin1=0;
	TurnData2.Ymin2=0;
	TurnData2.T=0;
	TurnData2.t1=0;
	TurnData2.TemCompare[0]=TurnData2.TemCompare[1]=TurnData2.TemCompare[2]=SYS.TemPV/10;
	TurnData2.step=0;
	TurnData.upDownFlag=4;
	UpOrDownToSV_Flag=0;
}
void EXTI9_5_IRQHandler(void)//301
{  	
	if(EXTI->PR&EXTI_Line6)
	{
		EXTI->PR = EXTI_Line6;
		if((!SYS.DataSet[HSCI])||(CountStep<100))
		{	
			if(SYS.DataGet[Rate]<999)  
			{	
				TIM2->EGR|=0x0001;	
				TIM2->ARR=ConverTime(SYS.DataGet[Rate],SYS.PWM_Period,1);
				TIM2->SR=~TIM_IT_Update;
				if(SYS.DataGet[Rate]) TIM2->CR1|=0x0001;
				TR_OFF;
			}
			else TR_ON;
		}
		else
		{	
			if(SYS.SSR_Count==0) {SYS.SsrStartBuff=ConverTime(SYS.DataGet[Rate],SYS.PWM_Period,0);TR_OFF;}
			if(SYS.SSR_Count++==SYS.SsrStartBuff) TR_ON;
			TIM2->EGR|=0x0001;
			TIM2->SR=~TIM_IT_Update;
		}
		if(SYS.GetPeriodCount<100)
		{
			if(++SYS.PeriodCycle1==10)	
			{
				if(SYS.GetPeriodCount>10) SYS.PWM_Period=(100000/SYS.PWM_GetPeriod1+SYS.PWM_Period)/2;
				else SYS.PWM_Period=100000/SYS.PWM_GetPeriod1;
				if(SYS.PWM_Period<=110) SYS.PWM_Period=100;
				else SYS.PWM_Period=120;
				SYS.ZeroCountBuff=SYS.PWM_Period/5;
				SYS.GetPeriodCount++;
				SYS.PWM_GetPeriod1=0;
				SYS.PeriodCycle1=0;
			}
		}
	}		
}
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&TIM_IT_Update)
	{
		TR_ON;
		TIM2->SR=~TIM_IT_Update;
	}
}
void DMA1_Channel1_IRQHandler(void)
{ 
       SYS.AD_AddValue[0]+=SYS.AD_Value[0];
	SYS.AD_AddValue[1]+=SYS.AD_Value[1];
	SYS.AD_AddValue[2]+=SYS.AD_Value[2];
	SYS.AD_num0++;
	SYS.AD_num1++;
	SYS.AD_num2++;
    DMA1->IFCR|=0x01;
}

void GetEnvTem(s16 *data)
{
	s8 i=0;
	s32 dataBuff32s=0,dataBuff32s1=0;
	dataBuff32s=SYS.AD_AddValue[1];
	dataBuff32s1=SYS.AD_num1;
	if(dataBuff32s1>100) SYS.AD_AddValue[1]=SYS.AD_num1=0;
	else return;
	dataBuff32s=dataBuff32s/dataBuff32s1;
	dataBuff32s=dataBuff32s*SYS.DataSet[ET_Z]*10/(4095*25/33-dataBuff32s);
	for(i=0;i<9;i++) data[i]=data[i+1];
	data[9]=Get1(dataBuff32s);
	SYS.DataGet[Env_sensor]=data[9];
	dataBuff32s=0;
	for(i=0;i<10;i++) dataBuff32s+=data[i];	
	dataBuff32s=dataBuff32s/10;
	if(SYS.DataGet[Tem_E]<dataBuff32s) SYS.DataGet[Tem_E]+=2;
	else if(SYS.DataGet[Tem_E]>dataBuff32s) SYS.DataGet[Tem_E]-=2;
	if(SYS.DataGet[Tem_E]>6800) SYS.DataGet[Tem_E]=6800;
	else if(SYS.DataGet[Tem_E]<-2000) SYS.DataGet[Tem_E]=-2000; 
	if(SYS.DataGet[Tem_E]>0)
	{
		i=SYS.DataGet[Tem_E]/1000;
		if(SYS.DataGet[Tem_E]%1000) i++;
	}
	else i=1;
	SYS.TemOffSetValue=Get1ToGet2(SYS.DataSet[In],SYS.DataGet[Tem_E],i);
}
void GetTem(s32 *data)
{
	static s16 svBak=0,pvBak=0;
	static float PvOut=0.0;
	float buffF=0.0;
	s32 dataT[3]={0,0,0};
	s8 i=0,DivCount=0;
	s32 dataBuff32s=0,buffTem=0,buffNum=0;
	buffTem=SYS.AD_AddValue[0];
	buffNum=SYS.AD_num0;
	if(buffNum>100) SYS.AD_AddValue[0]=SYS.AD_num0=0;
	else return;
	SYS.TemBakAD=buffTem/buffNum;
	dataBuff32s=(s32)buffNum*SYS.DataSet[HT_Z];	
	if(buffTem>dataBuff32s) buffTem-=dataBuff32s;
	else buffTem=0;
	SYS.TemVoltage=(float)buffTem*32*SYS.DataSet[HT_A]/(4096*buffNum);
	dataBuff32s=SYS.TemVoltage+SYS.TemOffSetValue;
	for(i=0;i<4;i++) data[i]=data[i+1];
	data[4]=Get2(dataBuff32s,SYS.DataSet[In]);
	dataBuff32s=0;
	for(i=0;i<5;i++) dataBuff32s+=data[i];
	dataBuff32s=dataBuff32s/50;
	SYS.TemPID=dataBuff32s;
	if(abs(dataBuff32s-SYS.TemPV)>4) SYS.TemPV=dataBuff32s;
	dataBuff32s=SYS.DataGet[Tem_sensor];
	SYS.DataGet[Tem_sensor]=SYS.TemPV/10;

	dataBuff32s=abs(SYS.DataGet[Tem_PV]-dataBuff32s);
	if(dataBuff32s>100) dataBuff32s=100;
	DivCount=dataBuff32s;
	if(SYS.AutoFlag==4)
	{
		if(svBak!=SYS.DataSet[Tem_SV]) {disCheck=1;UpOrDownToSV_Flag=0;}
		else if(((pvBak>=SYS.DataSet[Tem_SV])&&(SYS.DataGet[Tem_sensor]<=SYS.DataSet[Tem_SV]))
			||((pvBak<=SYS.DataSet[Tem_SV])&&(SYS.DataGet[Tem_sensor]>=SYS.DataSet[Tem_SV]))) 
		{
			divflag=1;
			UpOrDownToSV_Flag=2;
		}
	} 
	else disCheck=1;
	pvBak=SYS.DataGet[Tem_sensor];
	svBak=SYS.DataSet[Tem_SV];
	dataBuff32s=abs(SYS.DataGet[Tem_sensor]-SYS.DataSet[Tem_SV]);
	if(dataBuff32s>50) {disCheck=1;if(UpOrDownToSV_Flag==2) UpOrDownToSV_Flag=1;}	
	if(dataBuff32s>100) {PvOut=SYS.DataGet[Tem_sensor];disCheck=divflag=0;}
	else if(disCheck==1)
	{
		if(DivCount>2) PvOut=(float)(PvOut*DivCount+5*SYS.DataGet[Tem_sensor])/(float)(DivCount+5);
		else {PvOut=SYS.DataGet[Tem_sensor];disCheck=0;}
		divflag=0;
	}
	else if(SYS.AutoFlag==4)
	{
		if(divflag)
		{
			dataT[0]=SYS.DataGet[Tem_sensor]-SYS.DataSet[Tem_SV];
			if(dataT[0]>50) {dataT[2]=dataT[0]-50;dataT[0]=50;}
			else if(dataT[0]<-50) {dataT[2]=dataT[0]+50;dataT[0]=-50;}
			else dataT[2]=0;
        	if(dataT[0]>=10) {dataT[1]=dataT[0]-10;dataT[0]=10;}
        	else if(dataT[0]<=-10) {dataT[1]=dataT[0]+10;dataT[0]=-10;}
        	else dataT[1]=0;
			dataBuff32s=dataT[0]/5+dataT[1]/3+dataT[2]+SYS.DataSet[Tem_SV];
		}
		else dataBuff32s=SYS.DataGet[Tem_sensor];
		dataT[0]=abs(SYS.DataGet[Tem_PV]-dataBuff32s);

		if(dataT[0]>5) buffF=0.25;
		else if(dataT[0]>3) buffF=0.2;
		else if(dataT[0]>2) buffF=0.15;
		else buffF=0.1;
		if(UpOrDownToSV_Flag==0) PvOut=SYS.DataGet[Tem_sensor];
		else if(dataBuff32s<SYS.DataGet[Tem_PV]) PvOut=PvOut-buffF;
		else if(dataBuff32s>SYS.DataGet[Tem_PV]) PvOut=PvOut+buffF;
	}
	else {PvOut=SYS.DataGet[Tem_sensor];divflag=0;}
	SYS.DataGet[Tem_PV]=(s16)(PvOut+0.9);
}
void Delay(UINT num);
s32 Tem[5]={0,0,0,0,0};
s16 Env[10]={0,0,0,0,0,0,0,0,0,0};
#include "TableLED.txt"
void StartDisplay(void)
{
	UCHAR i=0;
	
	LED[0]=ascii['I'];
	LED[1]=ascii['C'];
	LED[2]=ascii['-'];
	if(SYS.DataSet[In]) LED[3]=ascii['K'];
	else LED[3]=ascii['J'];
	
	if(SYS.DataSet[HSCI])
	{
		LED[4]=ascii[' '];
		LED[5]=ascii['S']|0x8000;
		LED[6]=ascii['S']|0x8000;
		LED[7]=ascii['R']|0x8000;
	}
	else
	{
		LED[4]=ascii[' '];
		LED[5]=ascii['P']|0x8000;
		LED[6]=ascii['W']|0x8000;
		LED[7]=ascii['M']|0x8000;
	}
	for(i=0;i<10;i++)
	{
		Delay(2500);
		GetEnvTem(&Env[0]);
		SYS.DataGet[Tem_E]=SYS.DataGet[Env_sensor];
		GetTem(&Tem[0]);
	}
	LED[0]=ascii['C']|0x8000;
	LED[1]=ascii['H']|0x8000;
	LED[2]=ascii[(UCHAR)(SYS.DataSet[ID]/10)+0x30];
	LED[3]=ascii[(UCHAR)(SYS.DataSet[ID]%10)+0x30];

	
	LED[4]=ascii['C'];
	LED[5]=ascii['F'];
	LED[6]=ascii['-'];
	if(SYS.DataSet[C_F]) LED[7]=ascii['F'];
	else LED[7]=ascii['C'];
	for(i=0;i<10;i++)
	{
		Delay(3800);
		GetEnvTem(&Env[0]);
		SYS.DataGet[Tem_E]=SYS.DataGet[Env_sensor];
		GetTem(&Tem[0]);
	}
}
void GetCurrent(void)
{
	uint16_t resues = SYS.AD_AddValue[2]/(SYS.AD_num2*2);
	if(resues<7) resues = 0; else resues -= 7;
	
	 if(resues < 450)
	   SYS.DataGet[Current]= 1.34 * resues; 
    else
	   SYS.DataGet[Current]= 1.210 * resues; 
	SYS.AD_AddValue[2]=0;
	SYS.AD_num2=0;
	
//	SYS.DataGet[Current]=SYS.AD_AddValue[2]/(SYS.AD_num2);//10
//	//SYS.DataGet[Current]=SYS.AD_AddValue[2]/(SYS.AD_num2*4);
//	if(SYS.DataGet[Current]) SYS.DataGet[Current]--;
//	SYS.AD_AddValue[2]=0;
//	SYS.AD_num2=0;
}
void Task1(void) //100ms
{
	static u16 check_TCST_counter=180,check_TCRE_counter=5,outBak=0,check_TCST_TemBak=0;
	static float MVresult1=0;
	static s32 e[3]={0,0,0,},dataBuff=0;
	static u16 StandbyCount=0;
	static u8 i=0,HT_OP_Flag=0,F1_OP_Flag=0,F2_OP_Flag=0,HTST_count=0,TRST_count=0;
	static u16 lastRate,lastP;
	if(CountStep<100) 
	{
		CountStep++;
		
		if(CountStep==1) GetCurrent();
		else if(CountStep==2);
		else if(CountStep==3) {GetCurrent();if(SYS.DataGet[Current]>=300) {SYS.ErrorCode=TR_ST;CountStep=32;}}
		else if(CountStep<30)
		{	
			GetCurrent();
			if(SYS.DataGet[Current]>SYS.DataSet[HC_H]*5) {SYS.ErrorCode=HT_ST;CountStep=32;SYS.DataGet[Rate]=0;} 
			else if(SYS.DataGet[Rate]<10) SYS.DataGet[Rate]++;
			else if(SYS.DataGet[Rate]<100) SYS.DataGet[Rate]+=10;
			else {CountStep=30;if(SYS.DataGet[Current]==0) SYS.DataGet[Rate]=500;}
		}
		else if(CountStep==32) {GetCurrent();if((SYS.DataGet[Current]==0)&&(GPIOB->IDR & GPIO_Pin_1)&&(GPIOB->IDR & GPIO_Pin_2)) SYS.ErrorCode=HT_OP;}
		else if(CountStep==33) 
		{
			GetCurrent();
			e[2]=e[1];
			e[1]=e[0];
			e[0]=SYS.TemPID;
		}
		else if(CountStep==34) 
		{
			GetCurrent();
			e[2]=e[1];
			e[1]=e[0];
			e[0]=SYS.TemPID;
		}
		else if(CountStep==35) 
		{
			e[2]=e[1];
			e[1]=e[0];
			e[0]=SYS.TemPID;
			disCheck=0;
			TurnData.softUpCounter=SYS.DataSet[SO_T]*60*5+1;
			StandbyCount=SYS.StandbyTime*5;
			GetCurrent();
			check_TCST_TemBak=SYS.TemPID;
			lastRate = SYS.DataGet[Rate];
			SYS.DataGet[Rate] = 999;
		}
      else if(CountStep==39)
		{
			GetCurrent();
			power = (float)SYS.DataGet[Current]*220.0/100;
         SYS.DataGet[Rate] = lastRate;
		   CountStep=100;
		}
		GetEnvTem(&Env[0]);
		SYS.DataGet[Tem_E]=SYS.DataGet[Env_sensor];
		GetTem(&Tem[0]);
		UpOrDownToSV_Flag=0;
		
	}
	else if(CountStep>=100)
	{
		GetTemSV();
		//for(i=0;i<5;i++)
		{	
                    outBak=SYS.DataGet[Rate];
			GetEnvTem(&Env[0]);
			GetTem(&Tem[0]);
		if(i==4)
		{
			e[2]=e[1];
			e[1]=e[0];
			e[0]=SYS.TemPID;
		}
		if(i==3)
		{
			TurnData.TemCompare[1]=TurnData.TemCompare[2];
			if(abs(TurnData.TemCompare[2]-SYS.TemPV)>9) TurnData.TemCompare[2]=SYS.TemPV;
			CheckTemUpDown(&TurnData.TemCompare[0]);
		}
		if(SYS.RunningMode==0)
		{
			if(SYS.AutoFlag==0) 
			{	
				if((SYS.TemSV-SYS.TemPV)<=1500) 
				{
					SYS.AutoFlag=4;
					TurnData.upDownFlag=0;
					MVresult1=0;
				}
				else SYS.AutoFlag=1;
			}
			if(SYS.AutoFlag)
			{	
				if(SYS.AutoFlag==1) 
				{	
					if(TurnData.softUpCounter) 
					{
						if(SYS.DataGet[Rate]<999) SYS.DataGet[Rate]=SYS.DataSet[SO_R]+(SYS.DataSet[SO_T]*60-TurnData.softUpCounter/5);
						TurnData.softUpCounter--;
						if((SYS.TemPV*2)>=SYS.TemSV) TurnData.softUpCounter=0;
						if(SYS.TemPV>10000)  TurnData.softUpCounter=0;
					}
					if(TurnData.softUpCounter==0) 
					{
						if((SYS.TemPV*2)>=SYS.TemSV)
						{
							TurnData.step=1;
							SYS.AutoFlag=2;
							TurnDataAll.TemBak=SYS.TemPV;			
							SYS.DataGet[Rate]=999;
						}
						if(SYS.DataGet[Rate]<998) SYS.DataGet[Rate]+=2;	
						else SYS.DataGet[Rate]=999;	
					}
				}
				if(SYS.AutoFlag==2)
				{	
					if(TurnData.step==1)
					{
						dataBuff=SYS.TemPV-TurnDataAll.TemBak;
						if(((dataBuff*20)>=SYS.TemSV)||(dataBuff>=1000))
						{
							TurnData.step=2;
							TurnDataAll.t=0;
							TurnDataAll.buff=60000;
							TurnDataAll.TemBak=SYS.TemPV;
						}
						SYS.DataGet[Rate]=999;
					}
					if(TurnData.step==2) //up1
					{
						SYS.DataGet[Rate]=0;
						if(TurnDataAll.t>=15)
						{
							dataBuff=11000*TurnDataAll.t/abs(SYS.TemPV-TurnDataAll.TemBak);
							if(dataBuff<TurnDataAll.buff) TurnDataAll.buff=dataBuff;
							if(((dataBuff>TurnDataAll.buff))||(TurnData.upDownFlag==0)||(TurnData.upDownFlag==3))
							{
								if(SYS.DataSet[TURN]==ON) SYS.DataSet[TURN]=OFF; 
								SYS.AutoFlag=4;
								TurnData.step=8;
								SYS.DataSet[P]=ModData1(TurnDataAll.buff);
								SYS.TurnFlag=1;
								if(power < 300) SYS.DataSet[P] *= 0.6;
								if(power < 700) {
									  SYS.DataSet[P] *= 0.7;
										if(SYS.DataSet[P] > 30)
										SYS.DataSet[P] = 28;
									}
								
								CommIIC(&SYS.DataSet[P],2,P<<1,0);
								TurnDataAll.t1=0;
								MVresult1=0;
							}	
						}
						TurnDataAll.t++;
					}
				}
				else if(SYS.AutoFlag==3)
				{	
					if(TurnData2.step==0)
					{
						if(SYS.TemSV>SYS.TemPV)  SYS.DataGet[Rate]=999;
						else SYS.DataGet[Rate]=0;
						if(TurnData.upDownFlag==1) TurnData2.step=11;
					}
					else 
					{
						if(SYS.TemSV>(SYS.TemPV+10))  SYS.DataGet[Rate]=999;
						else if(SYS.TemPV>(SYS.TemSV+10))  SYS.DataGet[Rate]=0;
						if(TurnData2.step>10)
						{
							if(TurnData2.step==11) {if(TurnData.upDownFlag==0) TurnData2.step=12;}
							else if(TurnData2.step==12) {if(TurnData.upDownFlag==1) TurnData2.step=13;}
							else if(TurnData2.step==13) {if(TurnData.upDownFlag==0) TurnData2.step=14;}
							else if(TurnData2.step==14) {if(TurnData.upDownFlag==1) {TurnData2.step=16;TurnData2.Ymin1=SYS.TemPV;}}
							else if(TurnData2.step==16) 
							{
								TurnData2.T++;
								if(TurnData.upDownFlag==0)
								{
									TurnData2.step=17;
									SYS.DataSet[TURN]=OFF;
									TurnData2.Ymax1=SYS.TemPV;
									SYS.AutoFlag=4;
									lastP = SYS.DataSet[P];
									SYS.DataSet[P]=ModData2(TurnData2.T,TurnData2.Ymax1,TurnData2.Ymin1);									
									SYS.TurnFlag=1;									
									if(power < 300) SYS.DataSet[P] *= 0.5;
									if(power < 700) {
										SYS.DataSet[P] *= 0.6;
										if(SYS.DataSet[P] > 30)
										SYS.DataSet[P] = 28;
									}
									if((SYS.DataSet[P]==0)||(SYS.DataSet[P]>3*lastP)) SYS.DataSet[P] = lastP;
									CommIIC(&SYS.DataSet[P],2,P<<1,0);
									MVresult1=(SYS.TemSV-SYS.TemPV)*SYS.DataSet[P]/2;
								}
							}
							else TurnData2.step=11;
						}
						else TurnData2.step=11;
					}					
				}
				if(SYS.AutoFlag==4)
			 	{
					if((SYS.TemPV*2)<SYS.TemSV)
					{
						TurnData.step=0;
						SYS.AutoFlag=0;
					}
					else if(SYS.TurnFlag==0)
					{
						if(((SYS.TemPV*20)<(SYS.TemSV*13))||((SYS.TemSV-SYS.TemPV)>4500))
						{
							TurnData.step=0;
							SYS.AutoFlag=0;
						}
					}
					if((SYS.DataSet[TURN]==ON)&&((SYS.TemSV-SYS.TemPV)<500))
					{
						SYS.AutoFlag=3;
						ResetManualTurn();
						TurnData.checkStep=0;
					}		
					else if(i==4)
					{
						MVresult1+= DataConverter(SYS.DataSet[P],e[0],e[1],SYS.TemSV,SYS.TemPV);
						dataBuff=(SYS.TemSV-SYS.TemPV)*SYS.DataSet[P];
						if(MVresult1<0)
						{
							if(SYS.TemSV<SYS.TemPV)  {if(MVresult1<dataBuff) MVresult1=dataBuff;}
							else MVresult1=0;
							if(MVresult1<-10000) MVresult1=-10000; 
						}
						else if(SYS.TemPV>(SYS.TemSV+1000)) MVresult1=0;
						SYS.DataGet[Rate]=MVresult1/100;
						if(MVresult1>100000) MVresult1=100000;
						if(SYS.DataGet[Rate]<0) SYS.DataGet[Rate]=0;
						else if(SYS.DataGet[Rate]>999) SYS.DataGet[Rate]=999;
					}	
				}
		 	}
		StandbyCount=SYS.StandbyTime*5;
		}
		else if(SYS.RunningMode==1) 
		{	
			if(StandbyCount--)
			{
				if(SYS.DataGet[Tem_sensor]>=SYS.DataGet[Standby_SV]) SYS.DataGet[Rate]=0;
				else SYS.DataGet[Rate]=999;
				SYS.ManualRate=0;
			}
			else  {SYS.RunningMode=0;if(SYS.SetMenu==0) SYS.SetStep=0;}
			MVresult1=0;
			TurnData.step=0;
			TurnData.checkStep=0;
			SYS.AutoFlag=0;
			MVresult1=0;
		}
		else if(SYS.RunningMode==2) 
		{	
			SYS.DataGet[Rate]=SYS.ManualRate;
			MVresult1=0;
			TurnData.step=0;
			TurnData.checkStep=0;
			SYS.AutoFlag=0;
			MVresult1=0;
			if((SYS.ErrorCode==TC_OP)||(SYS.ErrorCode==TC_ST)||(SYS.ErrorCode==TC_RE)) SYS.ErrorCode=NoError;
		}
		else 
		{	
			SYS.DataGet[Rate]=0;
			MVresult1=0;
			TurnData.step=0;
			SYS.AutoFlag=0;
		}
		switch(SYS.ErrorCode)
		{
			case HT_ST: case TR_ST :
					     SYS.DataGet[Rate]=0;
						 break;
			case TC_OP : case TC_ST : case TC_RE : 
						if(SYS.RunningMode!=2) SYS.DataGet[Rate]=0;	break;
			case ALARM_H : case F1_OP : case F2_OP : case HT_OP:  
						SYS.DataGet[Rate]=0;break;
			default: break;
		}
	}
		
	  if(i==5) i=0;
	  else {i++;return;}
	  
	  if(SYS.DataSet[HSCI]) SYS.SSR_Count=0;
		GetCurrent();
		if(SYS.AutoFlag!=4) UpOrDownToSV_Flag=0;
		//****************************************************************Check TC_RE
		if((GPIOA->IDR & GPIO_Pin_12)&&((SYS.TemPID-SYS.DataGet[Tem_E])<500))
		{
			if(check_TCRE_counter) check_TCRE_counter--;
			if((check_TCRE_counter==0)&&(SYS.ErrorCode==NoError)) SYS.ErrorCode=TC_RE;
		}
		else
		{
			if(check_TCRE_counter<5) check_TCRE_counter++;
			if((SYS.ErrorCode==TC_RE)&&(check_TCRE_counter>=5)) SYS.ErrorCode=0;
		}
		//****************************************************************Check TC_ST
		if((SYS.TemPID-SYS.DataGet[Tem_E])<200)
		{
			if((SYS.DataGet[Rate]>=950)&&(SYS.ErrorCode!=HT_OP)&&(SYS.ErrorCode!=F1_OP)&&(SYS.ErrorCode!=F2_OP))
			{
				if(check_TCST_TemBak>SYS.TemPID) check_TCST_TemBak=SYS.TemPID;
				if(++check_TCST_counter>=120)
				{
					if((SYS.TemPID-check_TCST_TemBak)<100) if(SYS.ErrorCode<F1_OP) SYS.ErrorCode=TC_ST;
					check_TCST_counter=0;
				}
			}
			else  {check_TCST_counter=0;check_TCST_TemBak=SYS.TemPID;}
			if((e[1]-e[0])>500)	{if(SYS.ErrorCode<F1_OP) SYS.ErrorCode=TC_ST;}
		}
		else 
		{
			check_TCST_counter=0;
			check_TCST_TemBak=SYS.TemPID;
			if(SYS.ErrorCode==TC_ST) SYS.ErrorCode=0;	
		}
		//****************************************************************Check HT_OP

		if((outBak>=500)&&(SYS.DataGet[Current]==0)&&(SYS.ErrorCode!=F1_OP)&&(SYS.ErrorCode!=F2_OP))
		{
		
			if(HT_OP_Flag>5) {HT_OP_Flag=0;SYS.ErrorCode=HT_OP;}
			else HT_OP_Flag++;
		}
		else HT_OP_Flag=0;
		if((SYS.DataGet[Current]>0)&&(SYS.ErrorCode==HT_OP)) SYS.ErrorCode=0;

		//**************************************************************************************FU_OP
		if(!(GPIOB->IDR & GPIO_Pin_1)) {if(F1_OP_Flag++>3) SYS.ErrorCode=F1_OP;}//Check fause is open?
		else {F1_OP_Flag=0; if(SYS.ErrorCode==F1_OP) SYS.ErrorCode=0;}
		if(!(GPIOB->IDR & GPIO_Pin_2)) {if(F2_OP_Flag++>3) SYS.ErrorCode=F2_OP;}//Check fause is open?
		else {F2_OP_Flag=0; if(SYS.ErrorCode==F2_OP) SYS.ErrorCode=0;}
		//****************************************************************Check HT_ST,TR_ST

		if(SYS.DataGet[Current]>SYS.DataSet[HC_H]*10)	{if(++HTST_count==3) SYS.ErrorCode=HT_ST;SYS.ManualRate=0;}//Check heater is short?
		else HTST_count=0;
		
		if((outBak==0)&&(SYS.DataGet[Current]>=25)) {if(++TRST_count==3) SYS.ErrorCode=TR_ST;SYS.ManualRate=0;}//Check TRAC is short?
		else TRST_count=0;

		//****************************************************************Check AL_H,AL_L
		if((SYS.AutoFlag==4)&&(SYS.RunningMode==0)&&UpOrDownToSV_Flag)
		{
			if(SYS.ErrorCode==NoError)
			{
				if(SYS.DataGet[Tem_PV]>(SYS.DataSet[Tem_SV]+SYS.DataSet[AL_H])) SYS.ErrorCode=ALARM_H;
				else if((SYS.DataGet[Tem_PV]+SYS.DataSet[AL_L])<SYS.DataSet[Tem_SV])  SYS.ErrorCode=ALARM_L;
			}
			else if(SYS.ErrorCode==ALARM_H) {if(SYS.DataGet[Tem_PV]<=(SYS.DataSet[Tem_SV]+SYS.DataSet[AL_H])) SYS.ErrorCode=NoError;}
			else if(SYS.ErrorCode==ALARM_L) {if((SYS.DataGet[Tem_PV]+SYS.DataSet[AL_L])>=SYS.DataSet[Tem_SV])  SYS.ErrorCode=NoError;}
		}
		else if((SYS.ErrorCode==ALARM_H)||(SYS.ErrorCode==ALARM_L)) SYS.ErrorCode=NoError;
	 }

}
