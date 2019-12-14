#include <stm32f10x_lib.h>
void delay(UINT num)
{
	UCHAR i;
	while(num--)
	{
		for(i=0;i<100;i++);
	}
}
void Delay(UINT num)
{
	UCHAR i;
	while(num--)
	{
		for(i=0;i<255;i++);
	}
}
void Relay(u8 channel,u8 command)
{
	if(channel==1)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_10;
		else GPIOC->BRR=GPIO_Pin_10;
	}
	else if(channel==2)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_11;
		else GPIOC->BRR=GPIO_Pin_11;
	}
	else if(channel==3)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_12;
		else GPIOC->BRR=GPIO_Pin_12;
	}
	else if(channel==4)
	{
		if(command) GPIOD->BSRR=GPIO_Pin_2;
		else GPIOD->BRR=GPIO_Pin_2;
	}
	OSTimeDly(500);
}
void PWM_OUT(u8 channel, u8 command)
{
	if(channel==1)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_9;
		else GPIOC->BRR=GPIO_Pin_9;
	}
	else if(channel==2)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_8;
		else GPIOC->BRR=GPIO_Pin_8;
	}
	else if(channel==3)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_7;
		else GPIOC->BRR=GPIO_Pin_7;
	}
	else if(channel==4)
	{
		if(command) GPIOC->BSRR=GPIO_Pin_6;
		else GPIOC->BRR=GPIO_Pin_6;
	}
}
void LED(u8 number,u8 command)
{
    if(number==1)
    {
        if(command==ON) GPIO_ResetBits(GPIOB,GPIO_Pin_14);
        else GPIO_SetBits(GPIOB,GPIO_Pin_14);
    }
    else if(number==2)
    {
        if(command==ON) GPIO_ResetBits(GPIOB,GPIO_Pin_13);
        else GPIO_SetBits(GPIOB,GPIO_Pin_13);
    }
    else 
    {
        if(command==ON) GPIO_ResetBits(GPIOB,GPIO_Pin_12);
        else GPIO_SetBits(GPIOB,GPIO_Pin_12);
    }
}
void DisLED(u8 number)
{
    if(number==1)
    {
        if ((GPIOB->ODR & GPIO_Pin_14) != (u32)Bit_RESET) GPIOB->BRR = GPIO_Pin_14;
        else GPIOB->BSRR = GPIO_Pin_14;
    }
    else if(number==2)
    {
        if ((GPIOB->ODR & GPIO_Pin_13) != (u32)Bit_RESET) GPIOB->BRR = GPIO_Pin_13;
        else GPIOB->BSRR = GPIO_Pin_13;
    }
    else
    {
        if ((GPIOB->ODR & GPIO_Pin_12) != (u32)Bit_RESET) GPIOB->BRR = GPIO_Pin_12;
        else GPIOB->BSRR = GPIO_Pin_12;
    } 
}

void ChTUP(s32 *p,BOOL *flag)
{
	if((p[1]>p[0])&&(p[2]>p[0])) 
	{	
		p[0]=(p[1]+p[2])/2;
		*flag=1;
	}
	else if((p[0]>p[1])&&(p[0]>p[2]))  
	{
		p[0]=(p[1]+p[2])/2;
		*flag=0;
	}
	else if((p[0]==p[1])&&(p[0]==p[2])) *flag=2;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadData(void)
{
	u8 iwdFig;
	iwdFig = 0;
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET) {
		RCC_ClearFlag();
		iwdFig = 1;
	}
    SYS.PWM_Period=100;  
	TIM1->ARR=TIM2->ARR=TIM3->ARR=TIM4->ARR=65535;
    CHdata1.TR_OpenTime=CHdata2.TR_OpenTime=CHdata3.TR_OpenTime=CHdata4.TR_OpenTime=65535;	
	CHdata1.SSR_Count=0;
    CHdata1.SsrStartBuff=CHdata1.SsrStart=20000;
	CHdata1.TR_OpenTime=65535;
	CHdata2.SSR_Count=0;
    CHdata2.SsrStartBuff=CHdata2.SsrStart=20000;
	CHdata2.TR_OpenTime=65535;
	CHdata3.SSR_Count=0;
    CHdata3.SsrStartBuff=CHdata3.SsrStart=20000;
	CHdata3.TR_OpenTime=65535;
	CHdata4.SSR_Count=0;
    CHdata4.SsrStartBuff=CHdata4.SsrStart=20000;
	CHdata4.TR_OpenTime=65535;
	CHdata1.TurnFlag=0;
	CHdata2.TurnFlag=0;
	CHdata3.TurnFlag=0;
	CHdata4.TurnFlag=0;	
	
	if(CommIIC(&CHdata1.DataSet[Nop0],2,0,1)==0) 
	{
		if(CHdata1.DataSet[Nop0]==0x5a2)//5a2
		{
		 	CommIIC(CHdata1.DataSet,60,0,1);
			Delay(100);
            CommIIC(CHdata2.DataSet,60,64,1);
			Delay(100);
            CommIIC(CHdata3.DataSet,60,128,1);
			Delay(100);
            CommIIC(CHdata4.DataSet,60,192,1);
			Delay(100);
            CommIIC(SYS.DataCalSet,20,256,1);
		}
		else 
		{
			CHdata1.DataSet[Nop0]=0x5a2;
            SetInitData();
			CommIIC(CHdata1.DataSet,60,0,0);
			Delay(300);
            CommIIC(CHdata2.DataSet,60,64,0);
			Delay(300);
            CommIIC(CHdata3.DataSet,60,128,0);
			Delay(300);
            CommIIC(CHdata4.DataSet,60,192,0);
			Delay(300);
            CommIIC(SYS.DataCalSet,20,256,0);
		}
	}
	if(SYS.ID>72) SYS.ID=72;
    if(CHdata1.DataSet[C_F]==0) CHdata1.SV_Dis=CHdata1.DataSet[Tem_SV];
	else CHdata1.SV_Dis=9*CHdata1.DataSet[Tem_SV]/5+32;
    if(CHdata2.DataSet[C_F]==0) CHdata2.SV_Dis=CHdata2.DataSet[Tem_SV];
	else CHdata2.SV_Dis=9*CHdata2.DataSet[Tem_SV]/5+32;
    if(CHdata3.DataSet[C_F]==0) CHdata3.SV_Dis=CHdata3.DataSet[Tem_SV];
	else CHdata3.SV_Dis=9*CHdata3.DataSet[Tem_SV]/5+32;
    if(CHdata4.DataSet[C_F]==0) CHdata4.SV_Dis=CHdata4.DataSet[Tem_SV];
	else CHdata4.SV_Dis=9*CHdata4.DataSet[Tem_SV]/5+32;
	
    Reg4Data[2]=CHdata1.SV_Dis;
	  if(iwdFig==1){
			Reg4Data[6]=CHdata1.DataSet[OnOff];
		}
		else{
			Reg4Data[6]=CHdata1.DataSet[OnOff]=0;
		}
	  CHdata1.RunningMode=0; //Reg4Data[6]=CHdata1.DataSet[OnOff];
    Reg4Data[10]=CHdata1.DataSet[In];
    Reg4Data[14]=CHdata1.DataSet[C_F];
    Reg4Data[18]=CHdata1.DataSet[HSCI];
    Reg4Data[22]=1;
    Reg4Data[26]=CHdata1.RunningMode;
    Reg4Data[38]=CHdata1.DataSet[AL_H];
	Reg4Data[42]=CHdata1.DataSet[AL_L];
    Reg4Data[46]=CHdata1.DataSet[ST_T];
	Reg4Data[50]=CHdata1.DataSet[ST_R];
    Reg4Data[54]=CHdata1.DataSet[HC_H];
	Reg4Data[58]=CHdata1.DataSet[HC_L];
    Reg4Data[62]=CHdata1.DataSet[SO_T];
	Reg4Data[66]=CHdata1.DataSet[SO_R]; 	
	Reg4Data[70]=CHdata1.DataSet[P];
	Reg4Data[74]=0;
    Reg4Data[78]=0;
    Reg4Data[82]=CHdata1.ManualRate;
    Reg4Data[86]=CHdata1.DataSet[SampFreq];
    Reg4Data[94]=CHdata1.DataSet[Man1];
		
    Reg4Data[3]=CHdata2.SV_Dis;
		if(iwdFig==1){
			Reg4Data[7]=CHdata2.DataSet[OnOff];
		}
		else{
			Reg4Data[7]=CHdata2.DataSet[OnOff]=0;
		}
    CHdata2.RunningMode=0;
    Reg4Data[11]=CHdata2.DataSet[In];
    Reg4Data[15]=CHdata2.DataSet[C_F];
    Reg4Data[19]=CHdata2.DataSet[HSCI];
    Reg4Data[23]=1;
    Reg4Data[27]=CHdata2.RunningMode;
    Reg4Data[39]=CHdata2.DataSet[AL_H];
	Reg4Data[43]=CHdata2.DataSet[AL_L];
    Reg4Data[47]=CHdata2.DataSet[ST_T];
	Reg4Data[51]=CHdata2.DataSet[ST_R];
    Reg4Data[55]=CHdata2.DataSet[HC_H];
	Reg4Data[59]=CHdata2.DataSet[HC_L];
    Reg4Data[63]=CHdata2.DataSet[SO_T];
	Reg4Data[67]=CHdata2.DataSet[SO_R]; 	
	Reg4Data[71]=CHdata2.DataSet[P];
	Reg4Data[75]=0;
    Reg4Data[79]=0;
    Reg4Data[83]=CHdata2.ManualRate;
    Reg4Data[87]=CHdata2.DataSet[SampFreq];
    SYS.PowerValue=Reg4Data[90]=CHdata2.DataSet[Nop2];
    Reg4Data[95]=CHdata2.DataSet[Man1];
		
    Reg4Data[4]=CHdata3.SV_Dis;
		if(iwdFig==1){
			Reg4Data[8]=CHdata3.DataSet[OnOff];
		}
		else{
			Reg4Data[8]=CHdata3.DataSet[OnOff]=0;
		}
    CHdata3.RunningMode=0;
    Reg4Data[12]=CHdata3.DataSet[In];
    Reg4Data[16]=CHdata3.DataSet[C_F];
    Reg4Data[20]=CHdata3.DataSet[HSCI];
    Reg4Data[24]=1;
    Reg4Data[28]=CHdata3.RunningMode;
    Reg4Data[40]=CHdata3.DataSet[AL_H];
	Reg4Data[44]=CHdata3.DataSet[AL_L];
    Reg4Data[48]=CHdata3.DataSet[ST_T];
	Reg4Data[52]=CHdata3.DataSet[ST_R];
    Reg4Data[56]=CHdata3.DataSet[HC_H];
	Reg4Data[60]=CHdata3.DataSet[HC_L];
    Reg4Data[64]=CHdata3.DataSet[SO_T];
	Reg4Data[68]=CHdata3.DataSet[SO_R]; 	
	Reg4Data[72]=CHdata3.DataSet[P];
	Reg4Data[76]=0;
    Reg4Data[80]=0;
    Reg4Data[84]=CHdata3.ManualRate;
    Reg4Data[88]=CHdata3.DataSet[SampFreq];
    Reg4Data[96]=CHdata3.DataSet[Man1];
		
    Reg4Data[5]=CHdata4.SV_Dis;
		if(iwdFig==1){
			Reg4Data[9]=CHdata4.DataSet[OnOff];
		}
		else{
			Reg4Data[9]=CHdata4.DataSet[OnOff]=0;
		}
    CHdata4.RunningMode=0;
    Reg4Data[13]=CHdata4.DataSet[In];
    Reg4Data[17]=CHdata4.DataSet[C_F];
    Reg4Data[21]=CHdata4.DataSet[HSCI];
    Reg4Data[25]=1;
    Reg4Data[29]=CHdata4.RunningMode;
    Reg4Data[41]=CHdata4.DataSet[AL_H];
	Reg4Data[45]=CHdata4.DataSet[AL_L];
    Reg4Data[49]=CHdata4.DataSet[ST_T];
	Reg4Data[53]=CHdata4.DataSet[ST_R];
    Reg4Data[57]=CHdata4.DataSet[HC_H];
	Reg4Data[61]=CHdata4.DataSet[HC_L];
    Reg4Data[65]=CHdata4.DataSet[SO_T];
	Reg4Data[69]=CHdata4.DataSet[SO_R]; 	
	Reg4Data[73]=CHdata4.DataSet[P];
	Reg4Data[77]=0;
    Reg4Data[81]=0;
    Reg4Data[85]=CHdata4.ManualRate;
    Reg4Data[89]=CHdata4.DataSet[SampFreq];
    Reg4Data[97]=CHdata4.DataSet[Man1];
	Reg4Data[90]=SYS.PWM_Period;
	Reg4Data[91]=220;
	
	Reg4Data[93]=CHdata4.DataSet[Nop2];	
	
	Reg4Data[94]=CHdata1.ManualRate;
    Reg4Data[95]=CHdata2.ManualRate;
    Reg4Data[96]=CHdata3.ManualRate;
    Reg4Data[97]=CHdata4.ManualRate;
    Reg4Data[110]=SYS.DataCalSet[ET_Z];
    Reg4Data[111]=SYS.DataCalSet[HT_Z1];
    Reg4Data[112]=SYS.DataCalSet[HT_A1];
    Reg4Data[113]=SYS.DataCalSet[HT_Z2];
    Reg4Data[114]=SYS.DataCalSet[HT_A2];
    Reg4Data[115]=SYS.DataCalSet[HT_Z3];
    Reg4Data[116]=SYS.DataCalSet[HT_A3];
    Reg4Data[117]=SYS.DataCalSet[HT_Z4];
    Reg4Data[118]=SYS.DataCalSet[HT_A4];
	CHdata1.StandbyTime=CHdata1.DataSet[ST_T]*60;
    CHdata2.StandbyTime=CHdata2.DataSet[ST_T]*60;
    CHdata3.StandbyTime=CHdata3.DataSet[ST_T]*60;
    CHdata4.StandbyTime=CHdata4.DataSet[ST_T]*60;
	Reg4Data[34]=CHdata1.DataSet[TURN]=0;
	Reg4Data[34]=CHdata2.DataSet[TURN]=0;
	Reg4Data[34]=CHdata3.DataSet[TURN]=0;
	Reg4Data[34]=CHdata4.DataSet[TURN]=0;
	Reg4Data[119] = 0;
	Reg4Data[120] = CHdata1.DataSet[RisTem];
	Reg4Data[121] = CHdata2.DataSet[RisTem];
	Reg4Data[122] = CHdata3.DataSet[RisTem];
	Reg4Data[123] = CHdata4.DataSet[RisTem];
	
	Reg4Data[124] = CHdata1.DataSet[RisTime];
	Reg4Data[125] = CHdata2.DataSet[RisTime];
	Reg4Data[126] = CHdata3.DataSet[RisTime];
	Reg4Data[127] = CHdata4.DataSet[RisTime];
	
   Reg4Data[128] = CHdata1.DataSet[RisRang];
	Reg4Data[129] = CHdata2.DataSet[RisRang];
	Reg4Data[130] = CHdata3.DataSet[RisRang];
	Reg4Data[131] = CHdata4.DataSet[RisRang];
	
	Reg4Data[132] = ACC[0].risSW = 0;
	Reg4Data[133] = ACC[1].risSW = 0;
	Reg4Data[134] = ACC[2].risSW = 0;
	Reg4Data[135] = ACC[3].risSW = 0;

}
void SetInitData(void)
{
   CHdata1.DataSet[Tem_SV]=200;
   CHdata1.DataSet[In]=1;
   CHdata1.DataSet[HSCI]=1;
	CHdata1.DataSet[TURN]=ON;	
	CHdata1.DataSet[P]=60;
	CHdata1.DataSet[I]=60;
	CHdata1.DataSet[D]=15;
	CHdata1.DataSet[AL_H]=500;
	CHdata1.DataSet[AL_L]=500;
	CHdata1.DataSet[HC_H]=1600;
	CHdata1.DataSet[HC_L]=20;
	CHdata1.DataSet[SO_T]=1;
	CHdata1.DataSet[SO_R]=300;
	CHdata1.DataSet[ST_T]=100;
	CHdata1.DataSet[ST_R]=50;

    CHdata2.DataSet[In]=1;
    CHdata2.DataSet[HSCI]=1;
    CHdata2.DataSet[TURN]=ON;	
	CHdata2.DataSet[P]=60;
	CHdata2.DataSet[I]=60;
	CHdata2.DataSet[D]=15;
	CHdata2.DataSet[Tem_SV]=200;
	CHdata2.DataSet[AL_H]=500;
	CHdata2.DataSet[AL_L]=500;
	CHdata2.DataSet[HC_H]=1600;
	CHdata2.DataSet[HC_L]=20;
	CHdata2.DataSet[SO_T]=1;
	CHdata2.DataSet[SO_R]=300;
	CHdata2.DataSet[ST_T]=100;
	CHdata2.DataSet[ST_R]=50;
	
    CHdata3.DataSet[In]=1;
    CHdata3.DataSet[HSCI]=1;
    CHdata3.DataSet[TURN]=ON;	
	CHdata3.DataSet[P]=60;
	CHdata3.DataSet[I]=60;
	CHdata3.DataSet[D]=15;
	CHdata3.DataSet[Tem_SV]=200;
	CHdata3.DataSet[AL_H]=500;
	CHdata3.DataSet[AL_L]=500;
	CHdata3.DataSet[HC_H]=1600;
	CHdata3.DataSet[HC_L]=20;
	CHdata3.DataSet[SO_T]=1;
	CHdata3.DataSet[SO_R]=300;
	CHdata3.DataSet[ST_T]=100;
	CHdata3.DataSet[ST_R]=50;
    CHdata4.DataSet[In]=1;
    CHdata4.DataSet[HSCI]=1;
    CHdata4.DataSet[TURN]=ON;	
	CHdata4.DataSet[P]=60;
	CHdata4.DataSet[I]=60;
	CHdata4.DataSet[D]=15;
	CHdata4.DataSet[Tem_SV]=200;
	CHdata4.DataSet[AL_H]=500;
	CHdata4.DataSet[AL_L]=500;
	CHdata4.DataSet[HC_H]=1600;
	CHdata4.DataSet[HC_L]=20;
	CHdata4.DataSet[SO_T]=1;
	CHdata4.DataSet[SO_R]=300;
	CHdata4.DataSet[ST_T]=100;
	CHdata4.DataSet[ST_R]=50;
    SYS.DataCalSet[ET_Z]=800;
    SYS.DataCalSet[HT_Z1]=SYS.DataCalSet[HT_Z2]=SYS.DataCalSet[HT_Z3]=SYS.DataCalSet[HT_Z4]=0;
	SYS.DataCalSet[HT_A1]=SYS.DataCalSet[HT_A2]=SYS.DataCalSet[HT_A3]=SYS.DataCalSet[HT_A4]=980;
	CHdata1.DataSet[RisTem] = 0;
	CHdata2.DataSet[RisTem] = 0;
	CHdata3.DataSet[RisTem] = 0;
	CHdata4.DataSet[RisTem] = 0;
	CHdata1.DataSet[RisTime] = 0;
	CHdata2.DataSet[RisTime] = 0;
	CHdata3.DataSet[RisTime] = 0;
	CHdata4.DataSet[RisTime] = 0;
   CHdata1.DataSet[RisRang] = 0;
	CHdata2.DataSet[RisRang] = 0;
	CHdata3.DataSet[RisRang] = 0;
	CHdata4.DataSet[RisRang] = 0;
}
void GetCommID(void)
{
    unsigned char dataBuff[5]={251,252,253,254,255},i=0;
    while(!((dataBuff[0]==dataBuff[1])||(dataBuff[1]==dataBuff[2])||(dataBuff[2]==dataBuff[3])||(dataBuff[3]==dataBuff[4])))
    {
        dataBuff[i]=GPIOC->IDR|0xffC1;
        if(i>3) i=0;
        else i++; 
        OSTimeDly(100);
    }
	SYS.ID=0;
	if(!(dataBuff[0]&0x02)) SYS.ID+=16;
	if(!(dataBuff[0]&0x04)) SYS.ID+=8;
	if(!(dataBuff[0]&0x08)) SYS.ID+=4;
	if(!(dataBuff[0]&0x10)) SYS.ID+=2;
	if(!(dataBuff[0]&0x20)) SYS.ID+=1;
}
s32 GetTemSV(s32 data)
{
	data=data*100;
    return data;
}
void DMAChannel1_IRQHandler(void)
{ 
    OSIntNesting++;
        SYS.AD_AddValue[0]+=SYS.AD_Value[0];
	    SYS.AD_AddValue[1]+=SYS.AD_Value[1];
    	SYS.AD_AddValue[2]+=SYS.AD_Value[2];
        SYS.AD_AddValue[3]+=SYS.AD_Value[3];
        SYS.AD_AddValue[4]+=SYS.AD_Value[4];
        SYS.AD_AddValue[5]+=SYS.AD_Value[5];
        SYS.AD_AddValue[6]+=SYS.AD_Value[6];
        SYS.AD_AddValue[7]+=SYS.AD_Value[7];
        SYS.AD_AddValue[8]+=SYS.AD_Value[8];
        SYS.AD_num[0]++;
	    SYS.AD_num[1]++;
        SYS.AD_num[2]++;
        SYS.AD_num[3]++;
        SYS.AD_num[4]++;
        SYS.AD_num[5]++;
        SYS.AD_num[6]++;
        SYS.AD_num[7]++;
        SYS.AD_num[8]++;
		DMA1->IFCR|=0x01;  
    OSIntExit();
}
