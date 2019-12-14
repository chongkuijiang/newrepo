#include <stm32f10x_lib.h>
void GetEnvTem(s16 *data)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
	s8 i=0;
    u8 ADnum=8;
	s32 dataBuff32s=0,dataBuff32s1=0;
	if(SYS.AD_num[ADnum]<100) return;
	OS_ENTER_CRITICAL();
	dataBuff32s=SYS.AD_AddValue[ADnum];
	dataBuff32s1=SYS.AD_num[ADnum];
	SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
	OS_EXIT_CRITICAL();
    dataBuff32s=dataBuff32s/dataBuff32s1;
	dataBuff32s=dataBuff32s*SYS.DataCalSet[ET_Z]*330/(25*4095-33*dataBuff32s);
	for(i=0;i<4;i++) data[i]=data[i+1];
	data[4]=Get1(dataBuff32s);
	Reg3Data[30]=data[4];
    dataBuff32s=0;
	for(i=0;i<5;i++) dataBuff32s+=data[i];
	SYS.Tem_E=dataBuff32s/5;
	if(SYS.Tem_E>7000) SYS.Tem_E=7000;
	if(SYS.Tem_E>0)
	{
		i=SYS.Tem_E/1000;
		if(SYS.Tem_E%1000) i++;
	}
	else i=1;
	SYS.TemOffSetValueJ=Get1ToGet2(1,SYS.Tem_E,i);
	SYS.TemOffSetValueK=Get1ToGet2(0,SYS.Tem_E,i);
}
void GetTem1(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=0;
	s32 dataBuff32s=0,buffTem=0,buff1Num=0;
	if(SYS.AD_num[ADnum]<100) return;
	OS_ENTER_CRITICAL();
	buffTem=SYS.AD_AddValue[ADnum];
	buff1Num=SYS.AD_num[ADnum];
	SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
	OS_EXIT_CRITICAL();
	CHdata1.TemBakAD=buffTem/buff1Num;
	dataBuff32s=(s32)buff1Num*SYS.DataCalSet[HT_Z1];	
	if(buffTem>dataBuff32s) buffTem-=dataBuff32s;
	else buffTem=0;
	CHdata1.TemVoltage=(float)buffTem*33*SYS.DataCalSet[HT_A1]/(4095*buff1Num);
    Reg3Data[26]=CHdata1.TemVoltage;
	if(CHdata1.DataSet[In]) dataBuff32s=CHdata1.TemVoltage+SYS.TemOffSetValueJ;
	else dataBuff32s=CHdata1.TemVoltage+SYS.TemOffSetValueK;
	CHdata1.TemPID=Get2(dataBuff32s,CHdata1.DataSet[In]);
	CHdata1.DataGet[Tem_sensor]=CHdata1.TemPID/10;
	if(abs(CHdata1.TemPID-CHdata1.TemPV)>4) CHdata1.TemPV=CHdata1.TemPID;						 
	if((CHdata1.UpOrDownToSV_Flag<3)&&(CHdata1.DivNumPV<1))
	{
		dataBuff32s=CHdata1.DataGet[Tem_sensor];
		if(CHdata1.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;	
		if(CHdata1.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				CHdata1.TcopFlag=1;
				if(CHdata1.ErrorCode<TC_OP) CHdata1.ErrorCode=TC_OP;
				if(((CHdata1.DataSet[OnOff]==0)||(CHdata1.RunningMode==3))&&(CHdata1.ErrorCode==TC_OP)) CHdata1.ErrorCode=CHdata1.RunningMode;
			}
			else 
			{
				if(CHdata1.ErrorCode==TC_OP) CHdata1.ErrorCode=CHdata1.RunningMode;
				CHdata1.TcopFlag=0;
			}
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				CHdata1.TcopFlag=1;
				if(CHdata1.ErrorCode<TC_OP) CHdata1.ErrorCode=TC_OP;
				if(((CHdata1.DataSet[OnOff]==0)||(CHdata1.RunningMode==3))&&(CHdata1.ErrorCode==TC_OP)) CHdata1.ErrorCode=CHdata1.RunningMode;
			}
			else
			{
				if(CHdata1.ErrorCode==TC_OP) CHdata1.ErrorCode=CHdata1.RunningMode;
				CHdata1.TcopFlag=0;
			}
		}
		if(dataBuff32s>0) CHdata1.DataGet[Tem_PV]=dataBuff32s;
		else CHdata1.DataGet[Tem_PV]=0;
	}	
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void GetTem2(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=1;
	s32 dataBuff32s=0,buffTem=0,buff1Num=0;
	if(SYS.AD_num[ADnum]<100) return;
	OS_ENTER_CRITICAL();
	buffTem=SYS.AD_AddValue[ADnum];
	buff1Num=SYS.AD_num[ADnum];
	SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
	OS_EXIT_CRITICAL();
	CHdata2.TemBakAD=buffTem/buff1Num;
	dataBuff32s=(s32)buff1Num*SYS.DataCalSet[HT_Z2];	
	if(buffTem>dataBuff32s) buffTem-=dataBuff32s;
	else buffTem=0;
	CHdata2.TemVoltage=(float)buffTem*33*SYS.DataCalSet[HT_A2]/(4095*buff1Num);
    Reg3Data[27]=CHdata2.TemVoltage;
	dataBuff32s=CHdata2.TemVoltage;
	if(CHdata2.DataSet[In]) dataBuff32s=CHdata2.TemVoltage+SYS.TemOffSetValueJ;
	else dataBuff32s=CHdata2.TemVoltage+SYS.TemOffSetValueK;
	CHdata2.TemPID=Get3(dataBuff32s,CHdata2.DataSet[In]);
	CHdata2.DataGet[Tem_sensor]=CHdata2.TemPID/10;
	if(abs(CHdata2.TemPID-CHdata2.TemPV)>4) CHdata2.TemPV=CHdata2.TemPID;								 
	if((CHdata2.UpOrDownToSV_Flag<3)&&(CHdata2.DivNumPV<1))
	{
		dataBuff32s=CHdata2.DataGet[Tem_sensor];
		if(CHdata2.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata2.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				CHdata2.TcopFlag=1;
				if(CHdata2.ErrorCode<TC_OP) CHdata2.ErrorCode=TC_OP;
				if(((CHdata2.DataSet[OnOff]==0)||(CHdata2.RunningMode==3))&&(CHdata2.ErrorCode==TC_OP)) CHdata2.ErrorCode=CHdata2.RunningMode;
			}
			else 
			{
				if(CHdata2.ErrorCode==TC_OP) CHdata2.ErrorCode=CHdata2.RunningMode;
				CHdata2.TcopFlag=0;
			}
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				CHdata2.TcopFlag=1;
				if(CHdata2.ErrorCode<TC_OP) CHdata2.ErrorCode=TC_OP;
				if(((CHdata2.DataSet[OnOff]==0)||(CHdata2.RunningMode==3))&&(CHdata2.ErrorCode==TC_OP)) CHdata2.ErrorCode=CHdata2.RunningMode;
			}
			else 
			{
				if(CHdata2.ErrorCode==TC_OP) CHdata2.ErrorCode=CHdata2.RunningMode;
				CHdata2.TcopFlag=0;
			}
		}
		if(dataBuff32s>0) CHdata2.DataGet[Tem_PV]=dataBuff32s;
		else CHdata2.DataGet[Tem_PV]=0;
	}	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetTem3(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=2;
	s32 dataBuff32s=0,buffTem=0,buff1Num=0;
	if(SYS.AD_num[ADnum]<100) return;
	OS_ENTER_CRITICAL();
	buffTem=SYS.AD_AddValue[ADnum];
	buff1Num=SYS.AD_num[ADnum];
	SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
	OS_EXIT_CRITICAL();
	CHdata3.TemBakAD=buffTem/buff1Num;
	dataBuff32s=(s32)buff1Num*SYS.DataCalSet[HT_Z3];	
	if(buffTem>dataBuff32s) buffTem-=dataBuff32s;
	else buffTem=0;
	CHdata3.TemVoltage=(float)buffTem*33*SYS.DataCalSet[HT_A3]/(4095*buff1Num);
    Reg3Data[28]=CHdata3.TemVoltage;
	dataBuff32s=CHdata3.TemVoltage;
	if(CHdata3.DataSet[In]) dataBuff32s=CHdata3.TemVoltage+SYS.TemOffSetValueJ;
	else dataBuff32s=CHdata3.TemVoltage+SYS.TemOffSetValueK;
	CHdata3.TemPID=Get4(dataBuff32s,CHdata3.DataSet[In]);
	CHdata3.DataGet[Tem_sensor]=CHdata3.TemPID/10;
	if(abs(CHdata3.TemPID-CHdata3.TemPV)>4) CHdata3.TemPV=CHdata3.TemPID;							 
	if((CHdata3.UpOrDownToSV_Flag<3)&&(CHdata3.DivNumPV<1))
	{
		dataBuff32s=CHdata3.DataGet[Tem_sensor];
		if(CHdata3.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata3.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				CHdata3.TcopFlag=1;
				if(CHdata3.ErrorCode<TC_OP) CHdata3.ErrorCode=TC_OP;
				if(((CHdata3.DataSet[OnOff]==0)||(CHdata3.RunningMode==3))&&(CHdata3.ErrorCode==TC_OP)) CHdata3.ErrorCode=CHdata3.RunningMode;
			}
			else 
			{
				if(CHdata3.ErrorCode==TC_OP) CHdata3.ErrorCode=CHdata3.RunningMode;
				CHdata3.TcopFlag=0;
			}
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				CHdata3.TcopFlag=1;
				if(CHdata3.ErrorCode<TC_OP) CHdata3.ErrorCode=TC_OP;
				if(((CHdata3.DataSet[OnOff]==0)||(CHdata3.RunningMode==3))&&(CHdata3.ErrorCode==TC_OP)) CHdata3.ErrorCode=CHdata3.RunningMode;
			}
			else 
			{
				if(CHdata3.ErrorCode==TC_OP) CHdata3.ErrorCode=CHdata3.RunningMode;
				CHdata3.TcopFlag=0;
			}
		}
		if(dataBuff32s>0) CHdata3.DataGet[Tem_PV]=dataBuff32s;
		else CHdata3.DataGet[Tem_PV]=0;
	}	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void GetTem4(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=3;
	s32 dataBuff32s=0,buffTem=0,buff1Num=0;
	if(SYS.AD_num[ADnum]<100) return;
	OS_ENTER_CRITICAL();
	buffTem=SYS.AD_AddValue[ADnum];
	buff1Num=SYS.AD_num[ADnum];
	SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
	OS_EXIT_CRITICAL();
	CHdata4.TemBakAD=buffTem/buff1Num;
	dataBuff32s=(s32)buff1Num*SYS.DataCalSet[HT_Z4];	
	if(buffTem>dataBuff32s) buffTem-=dataBuff32s;
	else buffTem=0;
	CHdata4.TemVoltage=(float)buffTem*33*SYS.DataCalSet[HT_A4]/(4095*buff1Num);
    Reg3Data[29]=CHdata4.TemVoltage;
	if(CHdata4.DataSet[In]) dataBuff32s=CHdata4.TemVoltage+SYS.TemOffSetValueJ;
	else dataBuff32s=CHdata4.TemVoltage+SYS.TemOffSetValueK;
	CHdata4.TemPID=Get5(dataBuff32s,CHdata4.DataSet[In]);
	CHdata4.DataGet[Tem_sensor]=CHdata4.TemPID/10;
    if(abs(CHdata4.TemPID-CHdata4.TemPV)>4) CHdata4.TemPV=CHdata4.TemPID;			 
	if((CHdata4.UpOrDownToSV_Flag<3)&&(CHdata4.DivNumPV<1))
	{
		dataBuff32s=CHdata4.DataGet[Tem_sensor];
		if(CHdata4.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata4.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				CHdata4.TcopFlag=1;
				if(CHdata4.ErrorCode<TC_OP) CHdata4.ErrorCode=TC_OP;
				if(((CHdata4.DataSet[OnOff]==0)||(CHdata4.RunningMode==3))&&(CHdata4.ErrorCode==TC_OP)) CHdata4.ErrorCode=CHdata4.RunningMode;
			}
			else 
			{	
				if(CHdata4.ErrorCode==TC_OP) CHdata4.ErrorCode=CHdata4.RunningMode;
				CHdata4.TcopFlag=0;
			}
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				CHdata4.TcopFlag=1;
				if(CHdata4.ErrorCode<TC_OP) CHdata4.ErrorCode=TC_OP;
				if(((CHdata4.DataSet[OnOff]==0)||(CHdata4.RunningMode==3))&&(CHdata4.ErrorCode==TC_OP)) CHdata4.ErrorCode=CHdata4.RunningMode;
			}
			else 
			{
				if(CHdata4.ErrorCode==TC_OP) CHdata4.ErrorCode=CHdata4.RunningMode;
				CHdata4.TcopFlag=0;
			}
		}
		if(dataBuff32s>0) CHdata4.DataGet[Tem_PV]=dataBuff32s;
		else CHdata4.DataGet[Tem_PV]=0;
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void GetCurrent1(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=4;
    u32 dataBuff1=0,dataBuff2=0;
	if(SYS.AD_num[ADnum]<100) return;
    OS_ENTER_CRITICAL();
    dataBuff1=SYS.AD_AddValue[ADnum];
    dataBuff2=SYS.AD_num[ADnum];
    SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
    OS_EXIT_CRITICAL();
	CHdata1.DataGet[Current]=dataBuff1*155/(dataBuff2*1000);
	CHdata1.DataGet[Current] *= 0.55;
	Reg3Data[6]=CHdata1.DataGet[Current]=CHdata1.DataGet[Current]*10;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void GetCurrent2(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=5;
    u32 dataBuff1=0,dataBuff2=0;
	if(SYS.AD_num[ADnum]<100) return;
    OS_ENTER_CRITICAL();
    dataBuff1=SYS.AD_AddValue[ADnum];
    dataBuff2=SYS.AD_num[ADnum];
    SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
    OS_EXIT_CRITICAL();
	CHdata2.DataGet[Current]=dataBuff1*155/(dataBuff2*1000);
	CHdata2.DataGet[Current] *= 0.55;
	Reg3Data[7]=CHdata2.DataGet[Current]=CHdata2.DataGet[Current]*10;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void GetCurrent3(void)
{
	#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=6;
    u32 dataBuff1=0,dataBuff2=0;
	if(SYS.AD_num[ADnum]<100) return;
    OS_ENTER_CRITICAL();
    dataBuff1=SYS.AD_AddValue[ADnum];
    dataBuff2=SYS.AD_num[ADnum];
    SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
    OS_EXIT_CRITICAL();
	CHdata3.DataGet[Current]=dataBuff1*155/(dataBuff2*1000);
	CHdata3.DataGet[Current] *= 0.55;
	Reg3Data[8]=CHdata3.DataGet[Current]=CHdata3.DataGet[Current]*10;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void GetCurrent4(void)
{
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    u8 ADnum=7;
    u32 dataBuff1=0,dataBuff2=0;
	if(SYS.AD_num[ADnum]<100) return;
    OS_ENTER_CRITICAL();
    dataBuff1=SYS.AD_AddValue[ADnum];
    dataBuff2=SYS.AD_num[ADnum];
    SYS.AD_AddValue[ADnum]=0;
	SYS.AD_num[ADnum]=0;
    OS_EXIT_CRITICAL();
	CHdata4.DataGet[Current]=dataBuff1*155/(dataBuff2*1000);
	CHdata4.DataGet[Current] *= 0.55;
	Reg3Data[9]=CHdata4.DataGet[Current]=CHdata4.DataGet[Current]*10;
}
void CheckOutShort1(void)
{
	u8 bak=0;
	CHdata1.DataGet[Rate]=0;
	CHdata1.SSR_Count=0;
	CHdata1.SsrStartBuff=CHdata1.SsrStart=20000;
	CHdata1.TR_OpenTime=65535;
	bak=CHdata1.DataSet[HSCI];
	CHdata1.DataSet[HSCI]=1; 
	OSTimeDly(2000); 
    GetCurrent1();
	if(CHdata1.DataGet[Current]>=40) 
	{
		OSTimeDly(5000);
	 	GetCurrent1();
		if(CHdata1.DataGet[Current]>=40) CHdata1.ErrorCode=TR_ST;
	}
	GetTem1(); 
	if(CHdata1.ErrorCode!=TR_ST)
	{
		while(1)
		{
		
      if(++CHdata1.DataGet[Rate]>=10) break;
			if(CHdata1.DataSet[OnOff]==0) {CHdata1.DataGet[Rate]=0;break;}
      OSTimeDly(3000);
			GetCurrent1();
			if(CHdata1.DataGet[Current]>=(CHdata1.DataGet[Rate]*10+100)) 
			{	
				CHdata1.ErrorCode=HT_ST;
				break;
			}	
		}
	}
	GetTem1(); 
	if(CHdata1.ErrorCode<HT_ST)
	{
		while(1)
		{
			if(CHdata1.DataGet[Rate]<100) CHdata1.DataGet[Rate]+=10;
			else break;
			if(CHdata1.DataSet[OnOff]==0) {CHdata1.DataGet[Rate]=0;break;}
			OSTimeDly(3000);
			GetCurrent1();
			if(CHdata1.DataGet[Current]>1000)	
			{
				CHdata1.ErrorCode=HT_ST;
				break;
			}
		}
	}
	CHdata1.DataSet[HSCI]=bak;
	if((CHdata1.DataSet[HSCI]==0)||(CHdata1.ErrorCode>=ALARM_H))
	{	
		CHdata1.DataGet[Rate]=0;
		CHdata1.SSR_Count=0;
		CHdata1.SsrStart=20000;
		PWM_OUT(1,OFF);
	}
	GetTem1(); 	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckOutShort2(void)
{
	u8 bak=0;
	CHdata2.DataGet[Rate]=0;
	CHdata2.SSR_Count=0;
	CHdata2.SsrStartBuff=CHdata2.SsrStart=20000;
	CHdata2.TR_OpenTime=65535;
	bak=CHdata2.DataSet[HSCI];
	CHdata2.DataSet[HSCI]=1;
	OSTimeDly(2000); 
    GetCurrent2();
	if(CHdata2.DataGet[Current]>=40) 
	{
		OSTimeDly(5000);
	 	GetCurrent2();
		if(CHdata2.DataGet[Current]>=40) CHdata2.ErrorCode=TR_ST;
	}
	GetTem2();
	if(CHdata2.ErrorCode!=TR_ST)
	{ 
		while(1)
		{
		
        	if(++CHdata2.DataGet[Rate]>=10) break;
			if(CHdata2.DataSet[OnOff]==0) {CHdata2.DataGet[Rate]=0;break;}
        	OSTimeDly(3000);
			GetCurrent2();
			if(CHdata2.DataGet[Current]>=(CHdata2.DataGet[Rate]*10+100)) 
			{
				CHdata2.ErrorCode=HT_ST;
				break;
			}	
		}
	}
	GetTem2(); 
	if(CHdata2.ErrorCode<HT_ST)
	{
		while(1)
		{
			if(CHdata2.DataGet[Rate]<100) CHdata2.DataGet[Rate]+=10;
			else break;
			if(CHdata2.DataSet[OnOff]==0) {CHdata2.DataGet[Rate]=0;break;}
			OSTimeDly(3000);
			GetCurrent2();
			if(CHdata2.DataGet[Current]>1000)	
			{
				CHdata2.ErrorCode=HT_ST;
				break;
			}
		}
	}
	CHdata2.DataSet[HSCI]=bak;
	if((CHdata2.DataSet[HSCI]==0)||(CHdata2.ErrorCode>=ALARM_H))
	{	
		CHdata2.DataGet[Rate]=0;
		CHdata2.SSR_Count=0;
		CHdata2.SsrStart=20000;
		CHdata2.TR_OpenTime=65535;
		PWM_OUT(2,OFF);
	}
	GetTem2(); 	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckOutShort3(void)
{
	u8 bak=0;
	CHdata3.DataGet[Rate]=0;
	CHdata3.SSR_Count=0;
	CHdata3.SsrStartBuff=CHdata3.SsrStart=20000;
	CHdata3.TR_OpenTime=65535;
	bak=CHdata3.DataSet[HSCI];
	CHdata3.DataSet[HSCI]=1;
	OSTimeDly(2000); 
    GetCurrent3();
	if(CHdata3.DataGet[Current]>=40) 
	{
		OSTimeDly(5000);
	 	GetCurrent3();
		if(CHdata3.DataGet[Current]>=40) CHdata3.ErrorCode=TR_ST;
	}
	GetTem3();
	if(CHdata3.ErrorCode!=TR_ST)
	{ 
		while(1)
		{
		
        	if(++CHdata3.DataGet[Rate]>=10) break;
			if(CHdata3.DataSet[OnOff]==0) {CHdata3.DataGet[Rate]=0;break;}
        	OSTimeDly(3000);
			GetCurrent3();
			if(CHdata3.DataGet[Current]>=(CHdata3.DataGet[Rate]*10+100)) 
			{
				CHdata3.ErrorCode=HT_ST;
				break;
			}	
		}
	}
	GetTem3(); 
	if(CHdata3.ErrorCode<HT_ST)
	{
		while(1)
		{
			if(CHdata3.DataGet[Rate]<100) CHdata3.DataGet[Rate]+=10;
			else break;
			if(CHdata3.DataSet[OnOff]==0) {CHdata3.DataGet[Rate]=0;break;}
			OSTimeDly(3000);
			GetCurrent3();
			if(CHdata3.DataGet[Current]>1000)	
			{
				CHdata3.ErrorCode=HT_ST;
				break;
			}
		}
	}
	CHdata3.DataSet[HSCI]=bak;
	if((CHdata3.DataSet[HSCI]==0)||(CHdata3.ErrorCode>=ALARM_H))
	{	
		CHdata3.DataGet[Rate]=0;
		CHdata3.SSR_Count=0;
		CHdata3.SsrStart=20000;
		CHdata3.TR_OpenTime=65535;
		PWM_OUT(3,OFF);
	}
	GetTem3(); 	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckOutShort4(void)
{
	u8 bak=0;
	CHdata4.DataGet[Rate]=0;
	CHdata4.SSR_Count=0;
	CHdata4.SsrStartBuff=CHdata4.SsrStart=20000;
	CHdata4.TR_OpenTime=65535;
	bak=CHdata4.DataSet[HSCI];
	CHdata4.DataSet[HSCI]=1; 
	OSTimeDly(2000); 
    GetCurrent4();
	if(CHdata4.DataGet[Current]>=40) 
	{
		OSTimeDly(5000);
	 	GetCurrent4();
		if(CHdata4.DataGet[Current]>=40) CHdata4.ErrorCode=TR_ST;//Check TRAC is short?
	}
	GetTem4();
	if(CHdata4.ErrorCode!=TR_ST)
	{
		while(1)
		{
        	if(++CHdata4.DataGet[Rate]>=10) break;
			if(CHdata4.DataSet[OnOff]==0) {CHdata4.DataGet[Rate]=0;break;}
        	OSTimeDly(3000);	
			GetCurrent4();
			if(CHdata4.DataGet[Current]>=(CHdata4.DataGet[Rate]*10+100)) 
			{
				CHdata4.ErrorCode=HT_ST;
				break;
			}	
		}
	}
	GetTem4();
	if(CHdata4.ErrorCode<HT_ST)
	{
		while(1)
		{
			if(CHdata4.DataGet[Rate]<100) CHdata4.DataGet[Rate]+=10;
			else break;
			if(CHdata4.DataSet[OnOff]==0) {CHdata4.DataGet[Rate]=0;break;}
			OSTimeDly(3000);
			GetCurrent4();
			if(CHdata4.DataGet[Current]>1000)	
			{
				CHdata4.ErrorCode=HT_ST;
				break;
			}
		}
	}
	CHdata4.DataSet[HSCI]=bak;
	if((CHdata4.DataSet[HSCI]==0)||(CHdata4.ErrorCode>=ALARM_H))
	{	
		CHdata4.DataGet[Rate]=0;
		CHdata4.SSR_Count=0;
		CHdata4.SsrStart=20000;
		CHdata4.TR_OpenTime=65535;
		PWM_OUT(4,OFF);
	}
	GetTem4();	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ResetManualTurn1(void)
{
	TurnData1.NSV=CHdata1.TemSV;
	TurnData1.Ymax1=0;
	TurnData1.Ymin1=100000;
	TurnData1.T=0;
	TurnData1.TemCompare[0]=TurnData1.TemCompare[1]=TurnData1.TemCompare[2]=CHdata1.TemPV/10;
	TurnData1.step=0;
	TurnData1.upDownFlag=3;
	CHdata1.UpOrDownToSV_Flag=0;
}
void ResetManualTurn2(void)
{
	TurnData2.NSV=CHdata2.TemSV;
	TurnData2.Ymax1=0;
	TurnData2.Ymin1=100000;
	TurnData2.T=0;
	TurnData2.TemCompare[0]=TurnData2.TemCompare[1]=TurnData2.TemCompare[2]=CHdata2.TemPV/10;
	TurnData2.step=0;
	TurnData2.upDownFlag=3;
	CHdata2.UpOrDownToSV_Flag=0;
}
void ResetManualTurn3(void)
{
	TurnData3.NSV=CHdata3.TemSV;
	TurnData3.Ymax1=0;
	TurnData3.Ymin1=100000;
	TurnData3.T=0;
	TurnData3.TemCompare[0]=TurnData3.TemCompare[1]=TurnData3.TemCompare[2]=CHdata3.TemPV/10;
	TurnData3.step=0;
	TurnData3.upDownFlag=0;
	CHdata3.UpOrDownToSV_Flag=0;
}
void ResetManualTurn4(void)
{
	TurnData4.NSV=CHdata4.TemSV;
	TurnData4.Ymax1=0;
	TurnData4.Ymin1=100000;
	TurnData4.T=0;
	TurnData4.TemCompare[0]=TurnData4.TemCompare[1]=TurnData4.TemCompare[2]=CHdata4.TemPV/10;
	TurnData4.step=0;
	TurnData4.upDownFlag=0;
	CHdata4.UpOrDownToSV_Flag=0;
}
void TaskPID(void *id)
{
	static u8 num=0;
	while(1)
	{
		
		if(++num >4) num =1;
		switch (num)
		{
			case 1: OSSemPost(SS1[1]);
				      break;
			case 2: OSSemPost(SS1[2]);
				      break;
			case 3: OSSemPost(SS1[3]);
				      break;
			case 4: OSSemPost(SS1[4]);
				      break;
			default:
				      break;
		}
		OSTimeDly(2498);
		
	}
}
void TaskStartPID1(void *yydata)
{
	u16 check_TCST_counter=TCST_TIME,check_TCRE_counter=3,check_TCST_temMax=5000,check_TCST_temMin=0,outBak=0;
	u16 softOut=0;
	s32 e[4]={0,0,0,0},dataBuff=0;
	float MVresult1=0;
	u8 i=0,HT_OP_Flag1=0,TRST_count=0,OnOffBak=1,status;
	yydata=yydata;
    Reg4Data[26]=CHdata1.RunningMode=1; 
    for(i=0;i<10;i++)
	{	
		OSTimeDly(2000);
		GetTem1();
		GetCurrent1();
		e[2]=e[1];e[1]=e[0];e[0]=CHdata1.TemPID;	
	}
	TurnData1.softUpCounter=CHdata1.DataSet[SO_T]*60+1;
	CHdata1.StandbyTime=CHdata1.DataSet[ST_T]*60;
	CHdata1.StandbyCount=CHdata1.StandbyTime;
	CHdata1.TemSV=GetTemSV(CHdata1.DataSet[Tem_SV]);
	softOut=CHdata1.DataSet[SO_R];
	OSTimeDly(1000);
	TaskRunPID();
	while(1)
	{
		//OSTimeDly(9995);
		OSSemPend(SS1[1],0,&status);
    DisLED(2);
    outBak=CHdata1.DataGet[Rate];
		GetTem1();	
		TurnData1.TemCompare[1]=TurnData1.TemCompare[2];
		TurnData1.TemCompare[2]=CHdata1.TemPV;
		ChTUP(&TurnData1.TemCompare[0],&TurnData1.upDownFlag);			
		e[3]=e[2];
		e[2]=e[1];
		e[1]=e[0];
		e[0]=CHdata1.TemPID;
        if(CHdata1.DataSet[OnOff]==0)
        {
            CHdata1.TR_OpenTime=65535;CHdata1.SSR_Count=20000;
            TurnData1.softUpCounter=CHdata1.DataSet[SO_T]*60+1;
			softOut=CHdata1.DataSet[SO_R];
            PWM_OUT(1,OFF);
            CHdata1.DataGet[Rate]=0;
            ResetManualTurn1();
            GetCurrent1();
            CHdata1.AutoFlag=0;
			Relay(1,OFF);
			CHdata1.DataSet[TURN]=OFF;
			OnOffBak=0;
            continue;
        }
		else if(OnOffBak==0)
		{
			CHdata1.ErrorCode=CHdata1.RunningMode;
			Relay(1,ON);
			if(CHdata1.RunningMode==1) 
			{
				CHdata1.RunningMode=0;
				CheckOutShort1();
				if(CHdata1.RunningMode==0) CHdata1.RunningMode=1;
			}
		}
		OnOffBak=CHdata1.DataSet[OnOff];
		if(CHdata1.RunningMode==1)
		{
			if(CHdata1.AutoFlag==0) 
			{
				if((CHdata1.TemPID+3500)>CHdata1.TemSV) 
				{
					CHdata1.AutoFlag=4;
					MVresult1=(CHdata1.TemSV-CHdata1.TemPID)*CHdata1.DataSet[P]/2;
				}
				else CHdata1.AutoFlag=1;
			}
         if(CHdata1.AutoFlag==1) 
			{	
				if(TurnData1.softUpCounter) 
				{
					if(softOut<995) softOut+=5;
					else softOut=999;
					CHdata1.DataGet[Rate]=softOut;
					TurnData1.softUpCounter--;                       
					if((CHdata1.TemPID+10200)>CHdata1.TemSV) TurnData1.softUpCounter=0;
					else if(CHdata1.TemPID>10000) TurnData1.softUpCounter=0;					
				}                    
				if(TurnData1.softUpCounter==0)
				{
					softOut=CHdata1.DataSet[SO_R];
					TurnData1.NSV=CHdata1.TemPV;
					CHdata1.AutoFlag=2;
					TurnData1.step=1;
				}
			}
			if(CHdata1.AutoFlag==2)
			{
				if(TurnData1.step==1)
				{
					dataBuff=CHdata1.TemPV-TurnData1.NSV;
					if(((dataBuff*20)>=CHdata1.TemSV)||(dataBuff>=1000))
					{
						TurnData1.step=2;
						TurnData1.T=0;
						TurnData1.buff=60000;
						TurnData1.NSV=CHdata1.TemPV;
					}
					CHdata1.DataGet[Rate]=999;
				}
				if(TurnData1.step==2)
				{
					CHdata1.DataGet[Rate]=0;
					if(TurnData1.T>=3)
					{
						dataBuff=55000*TurnData1.T/abs(CHdata1.TemPV-TurnData1.NSV);
						if(dataBuff<TurnData1.buff) TurnData1.buff=dataBuff;
						if(((dataBuff>TurnData1.buff))||(TurnData1.upDownFlag==0)||(TurnData1.upDownFlag==2))
						{
							if(CHdata1.DataSet[TURN]==ON) CHdata1.DataSet[TURN]=OFF; 
							CHdata1.AutoFlag=4;
							TurnData1.step=8;
							CHdata1.DataSet[P]=ModData1(TurnData1.buff);
							CHdata1.TurnFlag=1;
							Reg4Data[70]=CHdata1.DataSet[P];
							CommIIC(&CHdata1.DataSet[P],6,P<<1,0);
							MVresult1=0;
						}	
					}
					TurnData1.T++;
				}
			}
			if(CHdata1.AutoFlag==3)
			{
				if(TurnData1.step==0)
				{
					if(CHdata1.TemSV>CHdata1.TemPV)  CHdata1.DataGet[Rate]=999;
					else CHdata1.DataGet[Rate]=0;
					if(TurnData1.upDownFlag==1) TurnData1.step=11;
				}
				else 
				{
					if(CHdata1.TemSV>(CHdata1.TemPV+10))  CHdata1.DataGet[Rate]=999;
					else if(CHdata1.TemPV>(CHdata1.TemSV+10))  CHdata1.DataGet[Rate]=0;
					if(TurnData1.step>10)
					{
						if(TurnData1.step==11) {if(TurnData1.upDownFlag==0) TurnData1.step=12;}
						else if(TurnData1.step==12) {if(TurnData1.upDownFlag==1) TurnData1.step=13;}
						else if(TurnData1.step==13) {if(TurnData1.upDownFlag==0) TurnData1.step=14;}
						else if(TurnData1.step==14) {if(TurnData1.upDownFlag==1) {TurnData1.step=16;TurnData1.Ymin1=CHdata1.TemPV;}}
						//else if(TurnData1.step==15) {TurnData1.T++;if(TurnData1.upDownFlag==0) {TurnData1.step=16;TurnData1.Ymax2=CHdata1.TemPV;}}
						else if(TurnData1.step==16) 
						{
							if((TurnData1.upDownFlag==0)||(TurnData1.upDownFlag==2)) 
							{
								TurnData1.step=17;
								Reg4Data[34]=CHdata1.DataSet[TURN]=OFF;
								TurnData1.Ymax1=CHdata1.TemPV;
								CHdata1.AutoFlag=4;
								CHdata1.UpOrDownToSV_Flag=0;
								CHdata1.DataSet[P]=ModData2(TurnData1.T,TurnData1.Ymax1,TurnData1.Ymin1);
								TurnData1.T=(TurnData1.T*2+CHdata1.DataSet[P])/2;
								CHdata1.DataSet[I]=TurnData1.T;
								CHdata1.DataSet[D]=TurnData1.T/4;
								if((TurnData1.T%4)>2) CHdata1.DataSet[D]++;
								Reg4Data[70]=CHdata1.DataSet[P];
                Reg4Data[74]=CHdata1.DataSet[I];
                Reg4Data[78]=CHdata1.DataSet[D];
								CHdata1.TurnFlag=1;
								CommIIC(&CHdata1.DataSet[P],6,P<<1,0);
								MVresult1=(CHdata1.TemSV-CHdata1.TemPV)*CHdata1.DataSet[P]/2;
							}
							TurnData1.T++;
						}
						else TurnData1.step=11;
					}
					else  TurnData1.step=11;
				}
			}
			if(CHdata1.AutoFlag==4)
			{
				if((CHdata1.TemPV*20)<(CHdata1.TemSV*8))
				{
					TurnData1.step=0;
					CHdata1.AutoFlag=0;
				}
				else if(CHdata1.TurnFlag==0)
				{
					if(((CHdata1.TemPV*20)<(CHdata1.TemSV*13))||((CHdata1.TemSV-CHdata1.TemPV)>4500))
					{
						TurnData1.step=0;
						CHdata1.AutoFlag=0;
					}
				}
				if((CHdata1.DataSet[TURN]==ON)&&((CHdata1.TemSV-CHdata1.TemPV)<500))
				{
					CHdata1.AutoFlag=3;
					ResetManualTurn1();
				}		
				if(CHdata1.UpOrDownToSV_Flag==0)
				{
					if(CHdata1.TemPID>CHdata1.TemSV) CHdata1.UpOrDownToSV_Flag=2;
					else if(CHdata1.TemPID<CHdata1.TemSV) CHdata1.UpOrDownToSV_Flag=1;
					else  CHdata1.UpOrDownToSV_Flag=3;
				}
				else if(CHdata1.UpOrDownToSV_Flag==1)
				{
					if(CHdata1.TemPID>=CHdata1.TemSV) CHdata1.UpOrDownToSV_Flag=3;
				}
				else if(CHdata1.UpOrDownToSV_Flag==2)
				{
					if(CHdata1.TemPID<=CHdata1.TemSV) CHdata1.UpOrDownToSV_Flag=4;
				}
				MVresult1+= DataConverter1(CHdata1.DataSet[P],e[0],e[1],CHdata1.TemSV,CHdata1.TemPV);
				
				dataBuff=(CHdata1.TemSV-CHdata1.TemPV)*CHdata1.DataSet[P];
				if(MVresult1<0)
				{
						if(CHdata1.TemSV<CHdata1.TemPV)  {if(MVresult1<dataBuff) MVresult1=dataBuff;}
						else MVresult1=0;
						if(MVresult1<-10000) MVresult1=-10000; 
				}
				else if(CHdata1.TemPV>(CHdata1.TemSV+1000)) MVresult1=0;
				if(MVresult1>100000) MVresult1=100000;
				
				CHdata1.DataGet[Rate]=MVresult1/100;	
			}
		}
		else if(CHdata1.RunningMode==2) 
		{
			CHdata1.UpOrDownToSV_Flag=0;
			if(CHdata1.StandbyCount--)
			{
				if(CHdata1.DataGet[Tem_PV]/10>=Reg4Data[2]) CHdata1.DataGet[Rate]=0;
				else CHdata1.DataGet[Rate]=999;
				CHdata1.ManualRate=0;
			}
			else
			{
				CHdata1.DataGet[Rate]=0;
				CHdata1.RunningMode=1;
				MVresult1=0;
				TurnData1.step=0;
				CHdata1.AutoFlag=0;
				MVresult1=(CHdata1.TemSV-CHdata1.TemPID)*CHdata1.DataSet[P];
				CHdata1.StandbyCount=CHdata1.StandbyTime;
				Reg4Data[2]=CHdata1.SV_Dis=CHdata1.SV_Dis*100/CHdata1.DataSet[ST_R];
			}
		}
		else if(CHdata1.RunningMode==3) 
		{
			CHdata1.DataGet[Rate]=CHdata1.ManualRate;
			MVresult1=0;
			CHdata1.UpOrDownToSV_Flag=0;
			TurnData1.step=0;
			CHdata1.AutoFlag=0;
			MVresult1=(CHdata1.TemSV-CHdata1.TemPID)*CHdata1.DataSet[P];
		}
		else 
		{
			CHdata1.DataGet[Rate]=0;
			MVresult1=0;
			CHdata1.UpOrDownToSV_Flag=0;
			TurnData1.step=0;
			CHdata1.AutoFlag=0;
		}	
		if(CHdata1.DataGet[Rate]<0) CHdata1.DataGet[Rate]=0;
		else if(CHdata1.DataGet[Rate]>999) CHdata1.DataGet[Rate]=999;	
		switch(CHdata1.ErrorCode)
		{
			case TC_RE : case HT_ST:
			case TR_ST :
				     	CHdata1.DataGet[Rate]=0;
						Relay(1,OFF);                    
					 	break;
			case TC_OP : case TC_ST : case Fu_OP : 
						CHdata1.DataGet[Rate]=0;
						Relay(1,OFF);
						break;
			case ALARM_H : CHdata1.DataGet[Rate]=0;Relay(1,ON);break;
			default:	   Relay(1,ON);break;
		}
		if(CHdata1.DataSet[HSCI]==0) CHdata1.SSR_Count=0;
		GetCurrent1();
		if(!(GPIOC->IDR & GPIO_Pin_13)) check_TCRE_counter=23;
		else {if(--check_TCRE_counter==0) CHdata1.ErrorCode=TC_RE;}
		if(CHdata1.DataGet[Tem_PV]<1000)
		{
			check_TCST_counter--;
			if(check_TCST_temMax<CHdata1.TemBakAD)  check_TCST_temMax=CHdata1.TemBakAD;
			if(check_TCST_temMin>CHdata1.TemBakAD)  check_TCST_temMin=CHdata1.TemBakAD;
			if((outBak<990)||((check_TCST_temMax-check_TCST_temMin)>9))
			{
				check_TCST_counter=TCST_TIME;
				check_TCST_temMax=0;
				check_TCST_temMin=9000;
			}
			if(check_TCST_counter==0) 
			{ 
				if((check_TCST_temMax-check_TCST_temMin)<10) {if(CHdata1.ErrorCode<TC_ST) CHdata1.ErrorCode=TC_ST;}
				check_TCST_counter=TCST_TIME;
				check_TCST_temMin=9000;
				check_TCST_temMax=0;
				
			}
		}
		else 
		{
			check_TCST_counter=TCST_TIME;
			check_TCST_temMax=0;
			check_TCST_temMin=9000;
		}
		if((e[3]-e[0])>2000) 
		{
			if((e[3]>=e[2])&&(e[2]>=e[1])&&(e[1]>=e[0]))
			{
				if((CHdata1.ErrorCode<TC_OP)&&((e[3]-e[0])>e[3]/5)) 
				{
					CHdata1.TcShortFlag=1;
					CHdata1.TcShortCount=20;
				}
			}
		}
		if((CHdata1.TcShortFlag==1)&&CHdata1.TcShortCount) 
		{
			if(--CHdata1.TcShortCount==0) CHdata1.TcShortFlag=0;
			if((CHdata1.ErrorCode<TC_OP)&&((SYS.Tem_E*10+1000)>e[0])) {CHdata1.TcShortFlag=0;CHdata1.ErrorCode=TC_ST;}
		}
		if(CHdata1.ErrorCode==TC_ST) {if((SYS.Tem_E*10+1000)<e[0]) CHdata1.ErrorCode=CHdata1.RunningMode;}
		if((outBak>=500)&&(CHdata1.DataGet[Current]==0))
		{
			if(CHdata1.ErrorCode<TC_OP)
			{
				if(HT_OP_Flag1>5) CHdata1.ErrorCode=HT_OP;
				else HT_OP_Flag1++;
			}
			else HT_OP_Flag1=0;
		}
		else HT_OP_Flag1=0;
		if(CHdata1.ErrorCode==HT_OP) {if(CHdata1.DataGet[Current]) CHdata1.ErrorCode=CHdata1.RunningMode;}
		if(CHdata1.ErrorCode<TC_OP)
		{
			if(CHdata1.DataGet[Current]>CHdata1.DataSet[HC_H])	CHdata1.ErrorCode=HT_ST;
			if((CHdata1.DataGet[Rate]==0)&&(CHdata1.DataGet[Current]>=45)) {if(++TRST_count==16) CHdata1.ErrorCode=TR_ST;}
			else TRST_count=0;
			if(SYS.PWM_GetPeriod1>=5) CHdata1.ErrorCode=Fu_OP;
		}
	 	if((CHdata1.AutoFlag==4)&&(CHdata1.UpOrDownToSV_Flag>=3)&&(CHdata1.RunningMode==1)&&(CHdata1.ErrorCode<ALARM_H))
		{
			if(CHdata1.DataGet[Tem_PV]>(CHdata1.SV_Dis*10+CHdata1.DataSet[AL_H])) CHdata1.ErrorCode=ALARM_H;
			else if((CHdata1.DataGet[Tem_PV]+CHdata1.DataSet[AL_L])<CHdata1.SV_Dis*10)  CHdata1.ErrorCode=ALARM_L;
		}
		else
		{	
			if(CHdata1.ErrorCode==ALARM_H) {if(CHdata1.DataGet[Tem_PV]<=(CHdata1.SV_Dis*10+CHdata1.DataSet[AL_H])) CHdata1.ErrorCode=CHdata1.RunningMode;}
			else if(CHdata1.ErrorCode==ALARM_L) {if((CHdata1.DataGet[Tem_PV]+CHdata1.DataSet[AL_L])>=CHdata1.SV_Dis*10)  CHdata1.ErrorCode=CHdata1.RunningMode;}
			
		}
    }
}
void TaskStartPID2(void *yydata)
{
	u16 check_TCST_counter=TCST_TIME,check_TCRE_counter=3,check_TCST_temMax=5000,check_TCST_temMin=0,outBak=0;
	u16 softOut=0;
	s32 e[4]={0,0,0,0},dataBuff=0;
	float MVresult1=0;
	u8 i=0,HT_OP_Flag1=0,TRST_count=0,OnOffBak=1,status;
	yydata=yydata;
    Reg4Data[27]=CHdata2.RunningMode=1; 
    for(i=0;i<10;i++)
	{	
		OSTimeDly(2000);
		GetTem2();
		GetCurrent2();
		e[2]=e[1];e[1]=e[0];e[0]=CHdata2.TemPID;			
	}
	TurnData2.softUpCounter=CHdata2.DataSet[SO_T]*60+1;
	CHdata2.StandbyTime=CHdata2.DataSet[ST_T]*60;
	CHdata2.StandbyCount=CHdata2.StandbyTime;
	CHdata2.TemSV=GetTemSV(CHdata2.DataSet[Tem_SV]);
	softOut=CHdata2.DataSet[SO_R];
	OSTimeDly(1000);
	while(1)
	{
		//OSTimeDly(9995);
		OSSemPend(SS1[2],0,&status);
    outBak=CHdata2.DataGet[Rate];
		GetTem2();	
		TurnData2.TemCompare[1]=TurnData2.TemCompare[2];
		TurnData2.TemCompare[2]=CHdata2.TemPV;
		ChTUP(&TurnData2.TemCompare[0],&TurnData2.upDownFlag);		
		e[3]=e[2];
		e[2]=e[1];
		e[1]=e[0];
		e[0]=CHdata2.TemPID;
        if(CHdata2.DataSet[OnOff]==0)
        {
            CHdata2.TR_OpenTime=65535;CHdata2.SSR_Count=20000;
            TurnData2.softUpCounter=CHdata2.DataSet[SO_T]*60+1;
			softOut=CHdata2.DataSet[SO_R];
            PWM_OUT(2,OFF);
            CHdata2.DataGet[Rate]=0;
            ResetManualTurn2();
            GetCurrent2();
            CHdata2.AutoFlag=0;
			Relay(2,OFF);
			CHdata2.DataSet[TURN]=OFF;
			OnOffBak=0;
            continue;
        }
		else if(OnOffBak==0)
		{
			CHdata2.ErrorCode=CHdata2.RunningMode;
			Relay(2,ON);
			if(CHdata2.RunningMode==1) 
			{
				CHdata2.RunningMode=0;
				CheckOutShort2();
				if(CHdata2.RunningMode==0) CHdata2.RunningMode=1;
			}
		}
		OnOffBak=CHdata2.DataSet[OnOff];
		if(abs(e[0]-CHdata2.TemSV)>1000) CHdata2.UpOrDownToSV_Flag=0;
		if(CHdata2.RunningMode==1)
		{
			if(CHdata2.AutoFlag==0) 
			{
				if((CHdata2.TemPID+3500)>CHdata2.TemSV) 
				{
					CHdata2.AutoFlag=4;
					MVresult1=(CHdata2.TemSV-CHdata2.TemPID)*CHdata2.DataSet[P]/2;
				}
				else CHdata2.AutoFlag=1;
			}
            if(CHdata2.AutoFlag==1) 
			{	
				if(TurnData2.softUpCounter) 
				{
					if(softOut<995) softOut+=5;
					else softOut=999;
					CHdata2.DataGet[Rate]=softOut; 
					TurnData2.softUpCounter--;                    
					if((CHdata2.TemPID+10200)>CHdata2.TemSV) TurnData2.softUpCounter=0;
					else if(CHdata2.TemPID>10000) TurnData2.softUpCounter=0;					
				}                    
				if(TurnData2.softUpCounter==0)
				{
					softOut=CHdata2.DataSet[SO_R];
					TurnData2.NSV=CHdata2.TemPV;
					CHdata2.AutoFlag=2;
					TurnData2.step=1;
				}
			}
			if(CHdata2.AutoFlag==2)
			{
				if(TurnData2.step==1)
				{
					dataBuff=CHdata2.TemPV-TurnData2.NSV;
					if(((dataBuff*20)>=CHdata2.TemSV)||(dataBuff>=1000))
					{
						TurnData2.step=2;
						TurnData2.T=0;
						TurnData2.buff=60000;
						TurnData2.NSV=CHdata2.TemPV;
					}
					CHdata2.DataGet[Rate]=999;
				}
				if(TurnData2.step==2) 
				{
					CHdata2.DataGet[Rate]=0;
					if(TurnData2.T>=3)
					{
						dataBuff=55000*TurnData2.T/abs(CHdata2.TemPV-TurnData2.NSV);
						if(dataBuff<TurnData2.buff) TurnData2.buff=dataBuff;
						if(((dataBuff>TurnData2.buff))||(TurnData2.upDownFlag==0)||(TurnData2.upDownFlag==2))
						{
							if(CHdata2.DataSet[TURN]==ON) CHdata2.DataSet[TURN]=OFF; 
							CHdata2.AutoFlag=4;
							TurnData2.step=8;
							CHdata2.DataSet[P]=ModData1(TurnData2.buff);
							CHdata2.TurnFlag=1;
							Reg4Data[71]=CHdata2.DataSet[P];
							CommIIC(&CHdata2.DataSet[P],6,64+(P<<1),0);
							MVresult1=0;
						}	
					}
					TurnData2.T++;
				}
			}
			if(CHdata2.AutoFlag==3)
			{
				if(TurnData2.step==0)
				{
					if(CHdata2.TemSV>CHdata2.TemPV)  CHdata2.DataGet[Rate]=999;
					else CHdata2.DataGet[Rate]=0;
					if(TurnData2.upDownFlag==1) TurnData2.step=11;
				}
				else 
				{
					if(CHdata2.TemSV>(CHdata2.TemPV+10))  CHdata2.DataGet[Rate]=999;
					else if(CHdata2.TemPV>(CHdata2.TemSV+10))  CHdata2.DataGet[Rate]=0;
					if(TurnData2.step>10)
					{
						if(TurnData2.step==11) {if(TurnData2.upDownFlag==0) TurnData2.step=12;}
						else if(TurnData2.step==12) {if(TurnData2.upDownFlag==1) TurnData2.step=13;}
						else if(TurnData2.step==13) {if(TurnData2.upDownFlag==0) TurnData2.step=14;}
						else if(TurnData2.step==14) {if(TurnData2.upDownFlag==1) {TurnData2.step=16;TurnData2.Ymin1=CHdata2.TemPV;}}
						//else if(TurnData2.step==15) {TurnData2.T++;if(TurnData2.upDownFlag==0) {TurnData2.step=16;TurnData2.Ymax2=CHdata2.TemPV;}}
						else if(TurnData2.step==16) 
						{
							if((TurnData2.upDownFlag==0)||(TurnData2.upDownFlag==2)) 
							{
								TurnData2.step=17;
								Reg4Data[35]=CHdata2.DataSet[TURN]=OFF;
								TurnData2.Ymax1=CHdata2.TemPV;
								CHdata2.AutoFlag=4;
								CHdata2.UpOrDownToSV_Flag=0;
								CHdata2.DataSet[P]=ModData2(TurnData2.T,TurnData2.Ymax1,TurnData2.Ymin1);
								TurnData2.T=(TurnData2.T*2+CHdata2.DataSet[P])/2;
								CHdata2.DataSet[I]=TurnData2.T;
								CHdata2.DataSet[D]=TurnData2.T/4;
								if((TurnData2.T%4)>2) CHdata2.DataSet[D]++;
								Reg4Data[71]=CHdata2.DataSet[P];
                        		Reg4Data[75]=CHdata2.DataSet[I];
                        		Reg4Data[79]=CHdata2.DataSet[D];
								CHdata2.TurnFlag=1;
								CommIIC(&CHdata2.DataSet[P],6,64+(P<<1),0);
								MVresult1=(CHdata2.TemSV-CHdata2.TemPV)*CHdata2.DataSet[P]/2;
							}
							TurnData2.T++;
						}
						else TurnData2.step=11;
					}
					else TurnData2.step=11;
				}
			}
			if(CHdata2.AutoFlag==4)
			{
				if((CHdata2.TemPV*20)<(CHdata2.TemSV*8))
				{
					TurnData2.step=0;
					CHdata2.AutoFlag=0;
				}
				else if(CHdata2.TurnFlag==0)
				{
					if(((CHdata2.TemPV*20)<(CHdata2.TemSV*13))||((CHdata2.TemSV-CHdata2.TemPV)>4500))
					{
						TurnData2.step=0;
						CHdata2.AutoFlag=0;
					}
				}
				if((CHdata2.DataSet[TURN]==ON)&&((CHdata2.TemSV-CHdata2.TemPV)<500))
				{
					CHdata2.AutoFlag=3;
					ResetManualTurn2();
				}		
				if(CHdata2.UpOrDownToSV_Flag==0)
				{
					if(CHdata2.TemPID>CHdata2.TemSV) CHdata2.UpOrDownToSV_Flag=2;
					else if(CHdata2.TemPID<CHdata2.TemSV) CHdata2.UpOrDownToSV_Flag=1;
					else  CHdata2.UpOrDownToSV_Flag=3;
				}
				else if(CHdata2.UpOrDownToSV_Flag==1)
				{
					if(CHdata2.TemPID>=CHdata2.TemSV) CHdata2.UpOrDownToSV_Flag=3;
				}
				else if(CHdata2.UpOrDownToSV_Flag==2)
				{
					if(CHdata2.TemPID<=CHdata2.TemSV) CHdata2.UpOrDownToSV_Flag=4;
				}
				MVresult1+= DataConverter2(CHdata2.DataSet[P],e[0],e[1],CHdata2.TemSV,CHdata2.TemPV);	
				dataBuff=(CHdata2.TemSV-CHdata2.TemPV)*CHdata2.DataSet[P];
				if(MVresult1<0)
				{
						if(CHdata2.TemSV<CHdata2.TemPV)  {if(MVresult1<dataBuff) MVresult1=dataBuff;}
						else MVresult1=0;
						if(MVresult1<-10000) MVresult1=-10000; 
				}
				else if(CHdata2.TemPV>(CHdata2.TemSV+1000)) MVresult1=0;
				if(MVresult1>100000) MVresult1=100000;	
				CHdata2.DataGet[Rate]=MVresult1/100;
			}
		}
		else if(CHdata2.RunningMode==2) 
		{
			CHdata2.UpOrDownToSV_Flag=0;
			if(CHdata2.StandbyCount--)
			{
				if(CHdata2.DataGet[Tem_PV]/10>=Reg4Data[3]) CHdata2.DataGet[Rate]=0;
				else CHdata2.DataGet[Rate]=999;
				CHdata2.ManualRate=0;
			}
			else
			{
				CHdata2.DataGet[Rate]=0;
				CHdata2.RunningMode=1;
				MVresult1=0;
				TurnData2.step=0;
				CHdata2.AutoFlag=0;
				MVresult1=(CHdata2.TemSV-CHdata2.TemPID)*CHdata2.DataSet[P];
				CHdata2.StandbyCount=CHdata2.StandbyTime;
				Reg4Data[3]=CHdata2.SV_Dis=CHdata2.SV_Dis*100/CHdata2.DataSet[ST_R];
			}
		}
		else if(CHdata2.RunningMode==3) 
		{
			CHdata2.DataGet[Rate]=CHdata2.ManualRate;
			MVresult1=0;
			CHdata2.UpOrDownToSV_Flag=0;
			TurnData2.step=0;
			CHdata2.AutoFlag=0;
			MVresult1=(CHdata2.TemSV-CHdata2.TemPID)*CHdata2.DataSet[P];
		}
		else 
		{
			CHdata2.DataGet[Rate]=0;
			MVresult1=0;
			CHdata2.UpOrDownToSV_Flag=0;
			TurnData2.step=0;
			CHdata2.AutoFlag=0;
		}	
		if(CHdata2.DataGet[Rate]<0) CHdata2.DataGet[Rate]=0;
		else if(CHdata2.DataGet[Rate]>999) CHdata2.DataGet[Rate]=999;		
		switch(CHdata2.ErrorCode)
		{
			case TC_RE : case HT_ST:
			case TR_ST :
				     	CHdata2.DataGet[Rate]=0;
						Relay(2,OFF);
					 	break;
			case TC_OP : case TC_ST : case Fu_OP : 
						CHdata2.DataGet[Rate]=0;
						Relay(2,OFF);
						break;
			case ALARM_H : CHdata2.DataGet[Rate]=0;Relay(2,ON);break;
			default:	   Relay(2,ON);break;
		}
		if(CHdata2.DataSet[HSCI]==0) CHdata2.SSR_Count=0;
		GetCurrent2();
		if(!(GPIOE->IDR & GPIO_Pin_6)) check_TCRE_counter=23;
		else {if(--check_TCRE_counter==0) CHdata2.ErrorCode=TC_RE;}
		if(CHdata2.DataGet[Tem_PV]<1000)
		{
			check_TCST_counter--;
			if(check_TCST_temMax<CHdata2.TemBakAD)  check_TCST_temMax=CHdata2.TemBakAD;
			if(check_TCST_temMin>CHdata2.TemBakAD)  check_TCST_temMin=CHdata2.TemBakAD;
			if((outBak<990)||((check_TCST_temMax-check_TCST_temMin)>9))
			{
				check_TCST_counter=TCST_TIME;
				check_TCST_temMax=0;
				check_TCST_temMin=9000;
			}
			if(check_TCST_counter==0) 
			{ 
				if((check_TCST_temMax-check_TCST_temMin)<10) {if(CHdata2.ErrorCode<TC_ST) CHdata2.ErrorCode=TC_ST;}
				check_TCST_counter=TCST_TIME;
				check_TCST_temMin=9000;
				check_TCST_temMax=0;
				
			}
		}
		else 
		{
			check_TCST_counter=TCST_TIME;
			check_TCST_temMax=0;
			check_TCST_temMin=9000;
		}
		if((e[3]-e[0])>2000) 
		{
			if((e[3]>=e[2])&&(e[2]>=e[1])&&(e[1]>=e[0]))
			{
				if((CHdata2.ErrorCode<Fu_OP)&&((e[3]-e[0])>e[3]/5)) 
				{
					CHdata2.TcShortFlag=1;
					CHdata2.TcShortCount=20;
				}
			}
		}
		if((CHdata2.TcShortFlag==1)&&CHdata2.TcShortCount) 
		{
			if(--CHdata2.TcShortCount==0) CHdata2.TcShortFlag=0;
			if((CHdata2.ErrorCode<Fu_OP)&&((SYS.Tem_E*10+1000)>e[0])) {CHdata2.TcShortFlag=0;CHdata2.ErrorCode=TC_ST;}
		}
		if(CHdata2.ErrorCode==TC_ST) {if((SYS.Tem_E*10+1000)<e[0]) CHdata2.ErrorCode=CHdata2.RunningMode;}
		if((outBak>=500)&&(CHdata2.DataGet[Current]==0))
		{
			if(CHdata2.ErrorCode<Fu_OP)
			{
				if(HT_OP_Flag1>5) CHdata2.ErrorCode=HT_OP;
				else HT_OP_Flag1++;
			}
			else HT_OP_Flag1=0;
		}
		else HT_OP_Flag1=0;
		if(CHdata2.ErrorCode==HT_OP) {if(CHdata2.DataGet[Current]) CHdata2.ErrorCode=CHdata2.RunningMode;}
		if(CHdata2.ErrorCode<Fu_OP)
		{
			if(CHdata2.DataGet[Current]>CHdata2.DataSet[HC_H])	CHdata2.ErrorCode=HT_ST;
			if((CHdata2.DataGet[Rate]==0)&&(CHdata2.DataGet[Current]>=45)) {if(++TRST_count==16) CHdata2.ErrorCode=TR_ST;}
			else TRST_count=0;
			if(SYS.PWM_GetPeriod2>=5) CHdata2.ErrorCode=Fu_OP;
		}
	 	if((CHdata2.AutoFlag==4)&&(CHdata2.UpOrDownToSV_Flag>=3)&&(CHdata2.RunningMode==1)&&(CHdata2.ErrorCode<ALARM_H))
		{
			if(CHdata2.DataGet[Tem_PV]>(CHdata2.SV_Dis*10+CHdata2.DataSet[AL_H])) CHdata2.ErrorCode=ALARM_H;
			else if((CHdata2.DataGet[Tem_PV]+CHdata2.DataSet[AL_L])<CHdata2.SV_Dis*10)  CHdata2.ErrorCode=ALARM_L;
		}
		else
		{	
			if(CHdata2.ErrorCode==ALARM_H) {if(CHdata2.DataGet[Tem_PV]<=(CHdata2.SV_Dis*10+CHdata2.DataSet[AL_H])) CHdata2.ErrorCode=CHdata2.RunningMode;}
			else if(CHdata2.ErrorCode==ALARM_L) {if((CHdata2.DataGet[Tem_PV]+CHdata2.DataSet[AL_L])>=CHdata2.SV_Dis*10)  CHdata2.ErrorCode=CHdata2.RunningMode;}
			
		}
    }
}
void TaskStartPID3(void *yydata)
{
	u16 check_TCST_counter=TCST_TIME,check_TCRE_counter=3,check_TCST_temMax=5000,check_TCST_temMin=0,outBak=0;
	u16 softOut=0;
	s32 e[4]={0,0,0,0},dataBuff=0;
	float MVresult1=0;
	u8 i=0,HT_OP_Flag1=0,TRST_count=0,OnOffBak=1,status;
	yydata=yydata;
    Reg4Data[28]=CHdata3.RunningMode=1; 
    for(i=0;i<10;i++)
	{	
		OSTimeDly(2000);
		GetTem3();
		GetCurrent3();
		e[2]=e[1];e[1]=e[0];e[0]=CHdata3.TemPID;		
	}
	TurnData3.softUpCounter=CHdata3.DataSet[SO_T]*60+1;
	CHdata3.StandbyTime=CHdata3.DataSet[ST_T]*60;
	CHdata3.StandbyCount=CHdata3.StandbyTime;
	CHdata3.TemSV=GetTemSV(CHdata3.DataSet[Tem_SV]);
	softOut=CHdata3.DataSet[SO_R];
	OSTimeDly(1000);
	while(1)
	{
		//OSTimeDly(9995);
		OSSemPend(SS1[3],0,&status);
    outBak=CHdata3.DataGet[Rate];	
		GetTem3();
		TurnData3.TemCompare[1]=TurnData3.TemCompare[2];
		TurnData3.TemCompare[2]=CHdata3.TemPV;
		ChTUP(&TurnData3.TemCompare[0],&TurnData3.upDownFlag);	
		e[3]=e[2];
		e[2]=e[1];
		e[1]=e[0];
		e[0]=CHdata3.TemPID;
        if(CHdata3.DataSet[OnOff]==0)
        {
            CHdata3.TR_OpenTime=65535;CHdata3.SSR_Count=20000;
            TurnData3.softUpCounter=CHdata3.DataSet[SO_T]*60+1;
			softOut=CHdata3.DataSet[SO_R];
            PWM_OUT(3,OFF);
            CHdata3.DataGet[Rate]=0;
            ResetManualTurn3();
            GetCurrent3();
            CHdata3.AutoFlag=0;
			Relay(3,OFF);
			CHdata3.DataSet[TURN]=OFF;
			OnOffBak=0;
            continue;
        }
		else if(OnOffBak==0)
		{
			CHdata3.ErrorCode=CHdata3.RunningMode;
			Relay(3,ON);
			if(CHdata3.RunningMode==1) 
			{
				CHdata3.RunningMode=0;
				CheckOutShort3();
				if(CHdata3.RunningMode==0) CHdata3.RunningMode=1;
			}
		}
		OnOffBak=CHdata3.DataSet[OnOff];
		if(abs(e[0]-CHdata3.TemSV)>1000) CHdata3.UpOrDownToSV_Flag=0;
		if(CHdata3.RunningMode==1)
		{
			if(CHdata3.AutoFlag==0) 
			{
				if((CHdata3.TemPID+3500)>CHdata3.TemSV) 
				{
					CHdata3.AutoFlag=4;
					MVresult1=(CHdata3.TemSV-CHdata3.TemPID)*CHdata3.DataSet[P]/2;
				}
				else CHdata3.AutoFlag=1;
			}
            if(CHdata3.AutoFlag==1) 
			{	
				if(TurnData3.softUpCounter) 
				{
					if(softOut<995) softOut+=5;
					else softOut=999;
					CHdata3.DataGet[Rate]=softOut;
					TurnData3.softUpCounter--;                       
					if((CHdata3.TemPID+10200)>CHdata3.TemSV) TurnData3.softUpCounter=0;
					else if(CHdata3.TemPID>10000) TurnData3.softUpCounter=0;					
				}                    
				if(TurnData3.softUpCounter==0)
				{
					softOut=CHdata3.DataSet[SO_R];
					TurnData3.NSV=CHdata3.TemPV;
					CHdata3.AutoFlag=2;
					TurnData3.step=1;
				}
			}
			if(CHdata3.AutoFlag==2)
			{
				if(TurnData3.step==1)
				{
					dataBuff=CHdata3.TemPV-TurnData3.NSV;
					if(((dataBuff*20)>=CHdata3.TemSV)||(dataBuff>=1000))
					{
						TurnData3.step=2;
						TurnData3.T=0;
						TurnData3.buff=60000;
						TurnData3.NSV=CHdata3.TemPV;
					}
					CHdata3.DataGet[Rate]=999;
				}
				if(TurnData3.step==2)
				{
					CHdata3.DataGet[Rate]=0;
					if(TurnData3.T>=3)
					{
						dataBuff=55000*TurnData3.T/abs(CHdata3.TemPV-TurnData3.NSV);
						if(dataBuff<TurnData3.buff) TurnData3.buff=dataBuff;
						if(((dataBuff>TurnData3.buff))||(TurnData3.upDownFlag==0)||(TurnData3.upDownFlag==2))
						{
							if(CHdata3.DataSet[TURN]==ON) CHdata3.DataSet[TURN]=OFF; 
							CHdata3.AutoFlag=4;
							TurnData3.step=8;
							CHdata3.DataSet[P]=ModData1(TurnData3.buff);
							CHdata3.TurnFlag=1;
							Reg4Data[72]=CHdata3.DataSet[P];
							CommIIC(&CHdata3.DataSet[P],6,128+(P<<1),0);
							MVresult1=0;
						}	
					}
					TurnData3.T++;
				}
			}
			if(CHdata3.AutoFlag==3)
			{
				if(TurnData3.step==0)
				{
					if(CHdata3.TemSV>CHdata2.TemPV)  CHdata3.DataGet[Rate]=999;
					else CHdata3.DataGet[Rate]=0;
					if(TurnData3.upDownFlag==1) TurnData3.step=11;
				}
				else 
				{
					if(CHdata3.TemSV>(CHdata3.TemPV+10))  CHdata3.DataGet[Rate]=999;
					else if(CHdata3.TemPV>(CHdata3.TemSV+10))  CHdata3.DataGet[Rate]=0;
					if(TurnData3.step>10)
					{
						if(TurnData3.step==11) {if(TurnData3.upDownFlag==0) TurnData3.step=12;}
						else if(TurnData3.step==12) {if(TurnData3.upDownFlag==1) TurnData3.step=13;}
						else if(TurnData3.step==13) {if(TurnData3.upDownFlag==0) TurnData3.step=14;}
						else if(TurnData3.step==14) {if(TurnData3.upDownFlag==1) {TurnData3.step=16;TurnData3.Ymin1=CHdata3.TemPV;}}
						//else if(TurnData3.step==15) {TurnData3.T++;if(TurnData3.upDownFlag==0) {TurnData3.step=16;TurnData3.Ymax2=CHdata3.TemPV;}}
						else if(TurnData3.step==16) 
						{
							if((TurnData3.upDownFlag==0)||(TurnData3.upDownFlag==2)) 
							{
								TurnData3.step=17;
								Reg4Data[36]=CHdata3.DataSet[TURN]=OFF;
								TurnData3.Ymax1=CHdata3.TemPV;
								CHdata3.AutoFlag=4;
								CHdata3.UpOrDownToSV_Flag=0;
								CHdata3.DataSet[P]=ModData2(TurnData3.T,TurnData3.Ymax1,TurnData3.Ymin1);
								TurnData3.T=(TurnData3.T*2+CHdata3.DataSet[P])/2;
								CHdata3.DataSet[I]=TurnData3.T;
								CHdata3.DataSet[D]=TurnData3.T/4;
								if((TurnData3.T%4)>2) CHdata3.DataSet[D]++;
								Reg4Data[72]=CHdata3.DataSet[P];
                        		Reg4Data[76]=CHdata3.DataSet[I];
                        		Reg4Data[80]=CHdata3.DataSet[D];
								CHdata3.TurnFlag=1;
								CommIIC(&CHdata3.DataSet[P],6,128+(P<<1),0);
								MVresult1=(CHdata3.TemSV-CHdata2.TemPV)*CHdata3.DataSet[P]/2;
							}
							TurnData3.T++;
						}
						else TurnData3.step=11;
					}
					else TurnData3.step=11;
				}
			}
			if(CHdata3.AutoFlag==4)
			{
				if((CHdata3.TemPV*20)<(CHdata3.TemSV*8))
				{
					TurnData3.step=0;
					CHdata3.AutoFlag=0;
				}
				else if(CHdata3.TurnFlag==0)
				{
					if(((CHdata3.TemPV*20)<(CHdata3.TemSV*13))||((CHdata3.TemSV-CHdata3.TemPV)>4500))
					{
						TurnData3.step=0;
						CHdata3.AutoFlag=0;
					}
				}
				if((CHdata3.DataSet[TURN]==ON)&&((CHdata3.TemSV-CHdata3.TemPV)<500))
				{
					CHdata3.AutoFlag=3;
					ResetManualTurn3();
				}		
				if(CHdata3.UpOrDownToSV_Flag==0)
				{
					if(CHdata3.TemPID>CHdata3.TemSV) CHdata3.UpOrDownToSV_Flag=2;
					else if(CHdata3.TemPID<CHdata3.TemSV) CHdata3.UpOrDownToSV_Flag=1;
					else  CHdata3.UpOrDownToSV_Flag=3;
				}
				else if(CHdata3.UpOrDownToSV_Flag==1)
				{
					if(CHdata3.TemPID>=CHdata3.TemSV) CHdata3.UpOrDownToSV_Flag=3;
				}
				else if(CHdata3.UpOrDownToSV_Flag==2)
				{
					if(CHdata3.TemPID<=CHdata3.TemSV) CHdata3.UpOrDownToSV_Flag=4;
				}
				MVresult1+= DataConverter3(CHdata3.DataSet[P],e[0],e[1],CHdata3.TemSV,CHdata3.TemPV);
				dataBuff=(CHdata3.TemSV-CHdata1.TemPV)*CHdata3.DataSet[P];
				if(MVresult1<0)
				{
						if(CHdata3.TemSV<CHdata3.TemPV)  {if(MVresult1<dataBuff) MVresult1=dataBuff;}
						else MVresult1=0;
						if(MVresult1<-10000) MVresult1=-10000; 
				}
				else if(CHdata3.TemPV>(CHdata3.TemSV+1000)) MVresult1=0;
				if(MVresult1>100000) MVresult1=100000;
				CHdata3.DataGet[Rate]=MVresult1/100;	
		    }
		}
		else if(CHdata3.RunningMode==2) 
		{
			CHdata3.UpOrDownToSV_Flag=0;
			if(CHdata3.StandbyCount--)
			{
				if(CHdata3.DataGet[Tem_PV]/10>=Reg4Data[4]) CHdata3.DataGet[Rate]=0;
				else CHdata3.DataGet[Rate]=999;
				CHdata3.ManualRate=0;
			}
			else
			{
				CHdata3.DataGet[Rate]=0;
				CHdata3.RunningMode=1;
				MVresult1=0;
				TurnData3.step=0;
				CHdata3.AutoFlag=0;
				MVresult1=(CHdata3.TemSV-CHdata3.TemPID)*CHdata3.DataSet[P];
				CHdata3.StandbyCount=CHdata3.StandbyTime;
				Reg4Data[4]=CHdata3.SV_Dis=CHdata3.SV_Dis*100/CHdata3.DataSet[ST_R];
			}
		}
		else if(CHdata3.RunningMode==3) 
		{
			CHdata3.DataGet[Rate]=CHdata3.ManualRate;
			MVresult1=0;
			CHdata3.UpOrDownToSV_Flag=0;
			TurnData3.step=0;
			CHdata3.AutoFlag=0;
			MVresult1=(CHdata3.TemSV-CHdata3.TemPID)*CHdata3.DataSet[P];
		}
		else 
		{
			CHdata3.DataGet[Rate]=0;
			MVresult1=0;
			CHdata3.UpOrDownToSV_Flag=0;
			TurnData3.step=0;
			CHdata3.AutoFlag=0;
		}
		if(CHdata3.DataGet[Rate]<0) CHdata3.DataGet[Rate]=0;
		else if(CHdata3.DataGet[Rate]>999) CHdata3.DataGet[Rate]=999;		
		switch(CHdata3.ErrorCode)
		{
			case TC_RE : case HT_ST:
			case TR_ST :
				     	CHdata3.DataGet[Rate]=0;
						Relay(3,OFF);
					 	break;
			case TC_OP : case TC_ST : case Fu_OP : 
						CHdata3.DataGet[Rate]=0;
						Relay(3,OFF);
						break;
			case ALARM_H : CHdata3.DataGet[Rate]=0;Relay(3,ON);break;
			default:	   Relay(3,ON);break;
		}
		if(CHdata3.DataSet[HSCI]==0) CHdata3.SSR_Count=0;
		GetCurrent3();
		if(!(GPIOC->IDR & GPIO_Pin_14)) check_TCRE_counter=23;
		else {if(--check_TCRE_counter==0) CHdata3.ErrorCode=TC_RE;}
		if(CHdata3.DataGet[Tem_PV]<1000)
		{
			check_TCST_counter--;
			if(check_TCST_temMax<CHdata3.TemBakAD)  check_TCST_temMax=CHdata3.TemBakAD;
			if(check_TCST_temMin>CHdata3.TemBakAD)  check_TCST_temMin=CHdata3.TemBakAD;
			if((outBak<990)||((check_TCST_temMax-check_TCST_temMin)>9))
			{
				check_TCST_counter=TCST_TIME;
				check_TCST_temMax=0;
				check_TCST_temMin=9000;
			}
			if(check_TCST_counter==0) 
			{ 
				if((check_TCST_temMax-check_TCST_temMin)<10) {if(CHdata3.ErrorCode<TC_ST) CHdata3.ErrorCode=TC_ST;}
				check_TCST_counter=TCST_TIME;
				check_TCST_temMin=9000;
				check_TCST_temMax=0;
				
			}
		}
		else 
		{
			check_TCST_counter=TCST_TIME;
			check_TCST_temMax=0;
			check_TCST_temMin=9000;
		}
		if((e[3]-e[0])>1000) 
		{
			if((e[3]>=e[2])&&(e[2]>=e[1])&&(e[1]>=e[0]))
			{
				if((CHdata3.ErrorCode<Fu_OP)&&((e[3]-e[0])>e[3]/5)) 
				{
					CHdata3.TcShortFlag=1;
					CHdata3.TcShortCount=TCST_TIME;
				}
			}
		}
		if((CHdata3.TcShortFlag==1)&&CHdata3.TcShortCount) 
		{
			if(--CHdata3.TcShortCount==0) CHdata3.TcShortFlag=0;
			if((CHdata3.ErrorCode<Fu_OP)&&((SYS.Tem_E*10+1000)>e[0])) {CHdata3.TcShortFlag=0;CHdata3.ErrorCode=TC_ST;}
		}
		if(CHdata3.ErrorCode==TC_ST) {if((SYS.Tem_E*10+1000)<e[0]) CHdata3.ErrorCode=CHdata3.RunningMode;}
		if((outBak>=500)&&(CHdata3.DataGet[Current]==0))
		{
			if(CHdata3.ErrorCode<Fu_OP)
			{
				if(HT_OP_Flag1>5) CHdata3.ErrorCode=HT_OP;
				else HT_OP_Flag1++;
			}
			else HT_OP_Flag1=0;
		}
		else HT_OP_Flag1=0;
		if(CHdata3.ErrorCode==HT_OP) {if(CHdata3.DataGet[Current]) CHdata3.ErrorCode=CHdata3.RunningMode;}
		if(CHdata3.ErrorCode<Fu_OP)
		{
			if(CHdata3.DataGet[Current]>CHdata3.DataSet[HC_H])	CHdata3.ErrorCode=HT_ST;
			if((CHdata3.DataGet[Rate]==0)&&(CHdata3.DataGet[Current]>=45)) {if(++TRST_count==16) CHdata3.ErrorCode=TR_ST;}
			else TRST_count=0;
			if(SYS.PWM_GetPeriod3>=5) CHdata3.ErrorCode=Fu_OP;
		}
	 	if((CHdata3.AutoFlag==4)&&(CHdata3.UpOrDownToSV_Flag>=3)&&(CHdata3.RunningMode==1)&&(CHdata3.ErrorCode<ALARM_H))
		{
			if(CHdata3.DataGet[Tem_PV]>(CHdata3.SV_Dis*10+CHdata3.DataSet[AL_H])) CHdata3.ErrorCode=ALARM_H;
			else if((CHdata3.DataGet[Tem_PV]+CHdata3.DataSet[AL_L])<CHdata3.SV_Dis*10)  CHdata3.ErrorCode=ALARM_L;
		}
		else
		{	
			if(CHdata3.ErrorCode==ALARM_H) {if(CHdata3.DataGet[Tem_PV]<=(CHdata3.SV_Dis*10+CHdata3.DataSet[AL_H])) CHdata3.ErrorCode=CHdata3.RunningMode;}
			else if(CHdata3.ErrorCode==ALARM_L) {if((CHdata3.DataGet[Tem_PV]+CHdata3.DataSet[AL_L])>=CHdata3.SV_Dis*10)  CHdata3.ErrorCode=CHdata3.RunningMode;}
			
		}
   }
}
void TaskStartPID4(void *yydata)
{
	u16 check_TCST_counter=TCST_TIME,check_TCRE_counter=3,check_TCST_temMax=5000,check_TCST_temMin=0,outBak=0;
	u16 softOut=0;
	s32 e[4]={0,0,0,0},dataBuff=0;
	float MVresult1=0;
	u8 i=0,HT_OP_Flag1=0,TRST_count=0,OnOffBak=1,status;
	yydata=yydata;
    Reg4Data[29]=CHdata4.RunningMode=1;
    for(i=0;i<10;i++)
	{	
		OSTimeDly(2000);
		GetTem4();
		GetCurrent4();
		e[2]=e[1];e[1]=e[0];e[0]=CHdata4.TemPID;			
	}
	TurnData4.softUpCounter=CHdata4.DataSet[SO_T]*60+1;
	CHdata4.StandbyTime=CHdata4.DataSet[ST_T]*60;
	CHdata4.StandbyCount=CHdata4.StandbyTime;
	CHdata4.TemSV=GetTemSV(CHdata4.DataSet[Tem_SV]);
	softOut=CHdata4.DataSet[SO_R];
	OSTimeDly(1000);
	while(1)
	{ 
//		OSTimeDly(9995);
		OSSemPend(SS1[4],0,&status);
    outBak=CHdata4.DataGet[Rate];
		GetTem4();	
		TurnData4.TemCompare[1]=TurnData4.TemCompare[2];
		TurnData4.TemCompare[2]=CHdata4.TemPV;
		ChTUP(&TurnData4.TemCompare[0],&TurnData4.upDownFlag);			
		e[3]=e[2];
		e[2]=e[1];
		e[1]=e[0];
		e[0]=CHdata4.TemPID;
        if(CHdata4.DataSet[OnOff]==0)
        {
            CHdata4.TR_OpenTime=65535;CHdata4.SSR_Count=20000;
            TurnData4.softUpCounter=CHdata4.DataSet[SO_T]*60+1;
			softOut=CHdata4.DataSet[SO_R];
            PWM_OUT(4,OFF);
            CHdata4.DataGet[Rate]=0;
            ResetManualTurn4();
            GetCurrent4();
            CHdata4.AutoFlag=0;
			Relay(4,OFF);
			CHdata4.DataSet[TURN]=OFF;
			OnOffBak=0;
            continue;
        }
		else if(OnOffBak==0)
		{
			CHdata4.ErrorCode=CHdata4.RunningMode;
			Relay(4,ON);
			if(CHdata4.RunningMode==1) 
			{
				CHdata4.RunningMode=0;
				CheckOutShort4();
				if(CHdata4.RunningMode==0) CHdata4.RunningMode=1;
			}
		}
		OnOffBak=CHdata4.DataSet[OnOff];
		if(abs(e[0]-CHdata4.TemSV)>1000) CHdata4.UpOrDownToSV_Flag=0;
		if(CHdata4.RunningMode==1)
		{
			if(CHdata4.AutoFlag==0) 
			{
				if((CHdata4.TemPID+3500)>CHdata4.TemSV) 
				{
					CHdata4.AutoFlag=4;
					MVresult1=(CHdata4.TemSV-CHdata4.TemPID)*CHdata4.DataSet[P]/2;
				}
				else CHdata4.AutoFlag=1;
			}
            if(CHdata4.AutoFlag==1) 
			{	
				if(TurnData4.softUpCounter) 
				{
					if(softOut<995) softOut+=5;
					else softOut=999;
					CHdata4.DataGet[Rate]=softOut;
					TurnData4.softUpCounter--;                       
					if((CHdata4.TemPID+10200)>CHdata4.TemSV) TurnData4.softUpCounter=0;
					else if(CHdata4.TemPID>10000) TurnData4.softUpCounter=0;					
				}                    
				if(TurnData4.softUpCounter==0)
				{
					softOut=CHdata4.DataSet[SO_R];
					TurnData4.NSV=CHdata4.TemPV;	
					CHdata4.AutoFlag=2;
					TurnData4.step=1;
				}
			}
			if(CHdata4.AutoFlag==2)
			{
				if(TurnData4.step==1)
				{
					dataBuff=CHdata4.TemPV-TurnData4.NSV;
					if(((dataBuff*20)>=CHdata4.TemSV)||(dataBuff>=1000))
					{
						TurnData4.step=2;
						TurnData4.T=0;
						TurnData4.buff=60000;
						TurnData4.NSV=CHdata4.TemPV;
					}
					CHdata4.DataGet[Rate]=999;
				}
				if(TurnData4.step==2)
				{
					CHdata4.DataGet[Rate]=0;
					if(TurnData4.T>=3)
					{
						dataBuff=55000*TurnData4.T/abs(CHdata4.TemPV-TurnData4.NSV);
						if(dataBuff<TurnData4.buff) TurnData4.buff=dataBuff;
						if(((dataBuff>TurnData4.buff))||(TurnData4.upDownFlag==0)||(TurnData4.upDownFlag==2))
						{
							if(CHdata4.DataSet[TURN]==ON) CHdata4.DataSet[TURN]=OFF; 
							CHdata4.AutoFlag=4;
							TurnData4.step=8;
							CHdata4.DataSet[P]=ModData1(TurnData4.buff);
							CHdata4.TurnFlag=1;
							Reg4Data[73]=CHdata4.DataSet[P];
							CommIIC(&CHdata4.DataSet[P],6,192+(P<<1),0);
							MVresult1=0;
						}	
					}
					TurnData4.T++;
				}
			}
			if(CHdata4.AutoFlag==3)
			{
				if(TurnData4.step==0)
				{
					if(CHdata4.TemSV>CHdata4.TemPV)  CHdata4.DataGet[Rate]=999;
					else CHdata4.DataGet[Rate]=0;
					if(TurnData4.upDownFlag==1) TurnData4.step=11;
				}
				else 
				{
					if(CHdata4.TemSV>(CHdata4.TemPV+10))  CHdata4.DataGet[Rate]=999;
					else if(CHdata4.TemPV>(CHdata4.TemSV+10))  CHdata4.DataGet[Rate]=0;
					if(TurnData4.step>10)
					{
						if(TurnData4.step==11) {if(TurnData4.upDownFlag==0) TurnData4.step=12;}
						else if(TurnData4.step==12) {if(TurnData4.upDownFlag==1) TurnData4.step=13;}
						else if(TurnData4.step==13) {if(TurnData4.upDownFlag==0) TurnData4.step=14;}
						else if(TurnData4.step==14) {if(TurnData4.upDownFlag==1) {TurnData4.step=16;TurnData4.Ymin1=CHdata4.TemPV;}}
						//else if(TurnData4.step==15) {TurnData4.T++;if(TurnData4.upDownFlag==0) {TurnData4.step=16;TurnData4.Ymax2=CHdata4.TemPV;}}
						else if(TurnData4.step==16) 
						{
							if((TurnData4.upDownFlag==0)||(TurnData4.upDownFlag==2))
							{
								TurnData4.step=17;
								Reg4Data[37]=CHdata4.DataSet[TURN]=OFF;
								TurnData4.Ymax1=CHdata4.TemPV;
								CHdata4.AutoFlag=4;
								CHdata4.UpOrDownToSV_Flag=0;
								CHdata4.DataSet[P]=ModData2(TurnData4.T,TurnData4.Ymax1,TurnData4.Ymin1);
								TurnData4.T=(TurnData4.T*2+CHdata4.DataSet[P])/2;
								CHdata4.DataSet[I]=TurnData4.T;
								CHdata4.DataSet[D]=TurnData4.T/4;
								if((TurnData4.T%4)>2) CHdata4.DataSet[D]++;
								Reg4Data[73]=CHdata4.DataSet[P];
                Reg4Data[77]=CHdata4.DataSet[I];
                Reg4Data[81]=CHdata4.DataSet[D];
								CHdata4.TurnFlag=1;
								CommIIC(&CHdata4.DataSet[P],6,192+(P<<1),0);
								MVresult1=(CHdata4.TemSV-CHdata4.TemPV)*CHdata4.DataSet[P]/2;
							}
							TurnData4.T++;
						}
						else TurnData4.step=11;
					}
					else TurnData4.step=11;
				}
			}
			if(CHdata4.AutoFlag==4)
			{
				if((CHdata4.TemPV*20)<(CHdata4.TemSV*8))
				{
					TurnData4.step=0;
					CHdata4.AutoFlag=0;
				}
				else if(CHdata4.TurnFlag==0)
				{
					if(((CHdata4.TemPV*20)<(CHdata4.TemSV*13))||((CHdata4.TemSV-CHdata4.TemPV)>4500))
					{
						TurnData4.step=0;
						CHdata4.AutoFlag=0;
					}
				}
				if((CHdata4.DataSet[TURN]==ON)&&((CHdata4.TemSV-CHdata4.TemPV)<500))
				{
					CHdata4.AutoFlag=3;
					ResetManualTurn4();
				}		
				if(CHdata4.UpOrDownToSV_Flag==0)
				{
					if(CHdata4.TemPID>CHdata4.TemSV) CHdata4.UpOrDownToSV_Flag=2;
					else if(CHdata4.TemPID<CHdata4.TemSV) CHdata4.UpOrDownToSV_Flag=1;
					else  CHdata4.UpOrDownToSV_Flag=3;
				}
				else if(CHdata4.UpOrDownToSV_Flag==1)
				{
					if(CHdata4.TemPID>=CHdata4.TemSV) CHdata4.UpOrDownToSV_Flag=3;
				}
				else if(CHdata4.UpOrDownToSV_Flag==2)
				{
					if(CHdata4.TemPID<=CHdata4.TemSV) CHdata4.UpOrDownToSV_Flag=4;
				}
				MVresult1+= DataConverter4(CHdata4.DataSet[P],e[0],e[1],CHdata4.TemSV,CHdata4.TemPV);
				dataBuff=(CHdata4.TemSV-CHdata4.TemPV)*CHdata4.DataSet[P];
				if(MVresult1<0)
				{
						if(CHdata4.TemSV<CHdata4.TemPV)  {if(MVresult1<dataBuff) MVresult1=dataBuff;}
						else MVresult1=0;
						if(MVresult1<-10000) MVresult1=-10000; 
				}
				else if(CHdata4.TemPV>(CHdata4.TemSV+1000)) MVresult1=0;
				if(MVresult1>100000) MVresult1=100000;
				CHdata4.DataGet[Rate]=MVresult1/100;	
			}
		}
		else if(CHdata4.RunningMode==2) 
		{
			CHdata4.UpOrDownToSV_Flag=0;
			if(CHdata4.StandbyCount--)
			{
				if(CHdata4.DataGet[Tem_PV]/10>=Reg4Data[5]) CHdata4.DataGet[Rate]=0;
				else CHdata4.DataGet[Rate]=999;
				CHdata4.ManualRate=0;
			}
			else
			{
				CHdata4.DataGet[Rate]=0;
				CHdata4.RunningMode=1;
				MVresult1=0;
				TurnData4.step=0;
				CHdata4.AutoFlag=0;
				MVresult1=(CHdata4.TemSV-CHdata4.TemPID)*CHdata4.DataSet[P];
				CHdata4.StandbyCount=CHdata4.StandbyTime;
				Reg4Data[5]=CHdata4.SV_Dis=CHdata4.SV_Dis*100/CHdata4.DataSet[ST_R];
			}
		}
		else if(CHdata4.RunningMode==3) 
		{
			CHdata4.DataGet[Rate]=CHdata4.ManualRate;
			MVresult1=0;
			CHdata4.UpOrDownToSV_Flag=0;
			TurnData4.step=0;
			CHdata4.AutoFlag=0;
			MVresult1=(CHdata4.TemSV-CHdata4.TemPID)*CHdata4.DataSet[P];
		}
		else 
		{
			CHdata4.DataGet[Rate]=0;
			MVresult1=0;
			CHdata4.UpOrDownToSV_Flag=0;
			TurnData4.step=0;
			CHdata4.AutoFlag=0;
		}	
		if(CHdata4.DataGet[Rate]<0) CHdata4.DataGet[Rate]=0;
		else if(CHdata4.DataGet[Rate]>999) CHdata4.DataGet[Rate]=999;	
		switch(CHdata4.ErrorCode)
		{
			case TC_RE : case HT_ST:
			case TR_ST :
				     	CHdata4.DataGet[Rate]=0;
						Relay(4,OFF);
					 	break;
			case TC_OP : case TC_ST : case Fu_OP : 
						CHdata4.DataGet[Rate]=0;
						Relay(4,OFF);
						break;
			case ALARM_H : CHdata4.DataGet[Rate]=0;Relay(4,ON);break;
			default:	   Relay(4,ON);break;
		}
		if(CHdata4.DataSet[HSCI]==0) CHdata4.SSR_Count=0;
		GetCurrent4();
		if(!(GPIOC->IDR & GPIO_Pin_15)) check_TCRE_counter=23;
		else {if(--check_TCRE_counter==0) CHdata4.ErrorCode=TC_RE;}
		if(CHdata4.DataGet[Tem_PV]<1000)
		{
			check_TCST_counter--;
			if(check_TCST_temMax<CHdata4.TemBakAD)  check_TCST_temMax=CHdata4.TemBakAD;
			if(check_TCST_temMin>CHdata4.TemBakAD)  check_TCST_temMin=CHdata4.TemBakAD;
			if((outBak<990)||((check_TCST_temMax-check_TCST_temMin)>9))
			{
				check_TCST_counter=TCST_TIME;
				check_TCST_temMax=0;
				check_TCST_temMin=9000;
			}
			if(check_TCST_counter==0) 
			{ 
				if((check_TCST_temMax-check_TCST_temMin)<10) {if(CHdata4.ErrorCode<TC_ST) CHdata4.ErrorCode=TC_ST;}
				check_TCST_counter=TCST_TIME;
				check_TCST_temMin=9000;
				check_TCST_temMax=0;
				
			}
		}
		else 
		{
			check_TCST_counter=TCST_TIME;
			check_TCST_temMax=0;
			check_TCST_temMin=9000;
		if((e[3]-e[0])>1000) 
		{
			if((e[3]>=e[2])&&(e[2]>=e[1])&&(e[1]>=e[0]))
			{
				if((CHdata4.ErrorCode<3)&&((e[3]-e[0])>e[3]/5)) 
				{
					CHdata4.TcShortFlag=1;
					CHdata4.TcShortCount=20;
				}
			}
		}
	}
		if((CHdata4.TcShortFlag==1)&&CHdata4.TcShortCount) 
		{
			if(--CHdata4.TcShortCount==0) CHdata4.TcShortFlag=0;
			if((CHdata4.ErrorCode<Fu_OP)&&((SYS.Tem_E*10+1000)>e[0])) {CHdata4.TcShortFlag=0;CHdata4.ErrorCode=TC_ST;}
		}
		if(CHdata4.ErrorCode==TC_ST) {if((SYS.Tem_E*10+1000)<e[0]) CHdata4.ErrorCode=CHdata4.RunningMode;}
		if((outBak>=500)&&(CHdata4.DataGet[Current]==0))
		{
			if(CHdata4.ErrorCode<Fu_OP)
			{
				if(HT_OP_Flag1>5) CHdata4.ErrorCode=HT_OP;
				else HT_OP_Flag1++;
			}
			else HT_OP_Flag1=0;
		}
		else HT_OP_Flag1=0;
		if(CHdata4.ErrorCode==HT_OP) {if(CHdata4.DataGet[Current]) CHdata4.ErrorCode=CHdata4.RunningMode;}
		if(CHdata4.ErrorCode<Fu_OP)
		{
			if(CHdata4.DataGet[Current]>CHdata4.DataSet[HC_H])	CHdata4.ErrorCode=HT_ST;
			if((CHdata4.DataGet[Rate]==0)&&(CHdata4.DataGet[Current]>=45)) {if(++TRST_count==16) CHdata4.ErrorCode=TR_ST;}
			else TRST_count=0;
			if(SYS.PWM_GetPeriod4>=5) CHdata4.ErrorCode=Fu_OP;
		}
	 	if((CHdata4.AutoFlag==4)&&(CHdata4.UpOrDownToSV_Flag>=3)&&(CHdata4.RunningMode==1)&&(CHdata4.ErrorCode<ALARM_H))
		{
			if(CHdata4.DataGet[Tem_PV]>(CHdata4.SV_Dis*10+CHdata4.DataSet[AL_H])) CHdata4.ErrorCode=ALARM_H;
			else if((CHdata4.DataGet[Tem_PV]+CHdata4.DataSet[AL_L])<CHdata4.SV_Dis*10)  CHdata4.ErrorCode=ALARM_L;
		}
		else
		{	
			if(CHdata4.ErrorCode==ALARM_H) {if(CHdata4.DataGet[Tem_PV]<=(CHdata4.SV_Dis*10+CHdata4.DataSet[AL_H])) CHdata4.ErrorCode=CHdata4.RunningMode;}
			else if(CHdata4.ErrorCode==ALARM_L) {if((CHdata4.DataGet[Tem_PV]+CHdata4.DataSet[AL_L])>=CHdata4.SV_Dis*10)  CHdata4.ErrorCode=CHdata4.RunningMode;}
			
		}
   }
}
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&&TIM_IT_Update)
	{
		PWM_OUT(1,ON);
		TIM2->SR=~TIM_IT_Update;
	}	
}
void TIM3_IRQHandler(void)
{
	if(TIM3->SR&&TIM_IT_Update)
	{
		PWM_OUT(2,ON);
		TIM3->SR=~TIM_IT_Update;
	}	
}
/////////////////////////////////////////////////////////////////////////////////////////////
void TIM4_IRQHandler(void)
{
    if(TIM4->SR&&TIM_IT_Update)
	{
		PWM_OUT(3,ON);
		TIM4->SR=~TIM_IT_Update;
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR&&TIM_IT_Update)
	{
		PWM_OUT(4,ON);
		TIM1->SR=~TIM_IT_Update;
	}
}
void EXTI0_IRQHandler(void)
{
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
    OS_ENTER_CRITICAL();
	TIM1->EGR|=0x0001;
	TIM2->EGR|=0x0001;
	TIM3->EGR|=0x0001;
	TIM4->EGR|=0x0001;
	OSIntNesting++;
    if(EXTI->PR&EXTI_Line0)
    {
	    EXTI->PR = EXTI_Line0;
        if(CHdata1.DataSet[HSCI])
        {	
			if(CHdata1.DataGet[Rate]<999) PWM_OUT(1,OFF);	
			TIM2->SR=~TIM_IT_Update;
			TIM2->ARR=ConverTime(CHdata1.DataGet[Rate],SYS.PWM_Period,1);
            if(CHdata1.DataGet[Rate]>0) TIM2->CR1|=0x0001;
			else TIM2->CR1&=0xfffe;	
        }
        else 
        {
			if(CHdata1.SSR_Count==0) {CHdata1.SsrStartBuff=ConverTime(CHdata1.DataGet[Rate],SYS.PWM_Period,0);PWM_OUT(1,OFF);}
			if(CHdata1.SSR_Count++==CHdata1.SsrStartBuff) PWM_OUT(1,ON);
        }
        if(CHdata2.DataSet[HSCI])
        {
			if(CHdata2.DataGet[Rate]<999)PWM_OUT(2,OFF);
			TIM3->ARR=ConverTime(CHdata2.DataGet[Rate],SYS.PWM_Period,1);
			TIM3->SR=~TIM_IT_Update;
            if(CHdata2.DataGet[Rate]>0) TIM3->CR1|=0x0001;
			else TIM3->CR1&=0xfffe;
        }
        else 
        {
            if(CHdata2.SSR_Count==0) {CHdata2.SsrStartBuff=ConverTime(CHdata2.DataGet[Rate],SYS.PWM_Period,0);PWM_OUT(2,OFF);}
			if(CHdata2.SSR_Count++==CHdata2.SsrStartBuff) PWM_OUT(2,ON);
        }
        if(CHdata3.DataSet[HSCI])
        {
			if(CHdata3.DataGet[Rate]<999) PWM_OUT(3,OFF);
			TIM4->ARR=ConverTime(CHdata3.DataGet[Rate],SYS.PWM_Period,1);
			TIM4->SR=~TIM_IT_Update;
            if(CHdata3.DataGet[Rate]>0) TIM4->CR1|=0x0001;
			else TIM4->CR1&=0xfffe;
        }
        else 
        {
            if(CHdata3.SSR_Count==0) {CHdata3.SsrStartBuff=ConverTime(CHdata3.DataGet[Rate],SYS.PWM_Period,0);PWM_OUT(3,OFF);}
			if(CHdata3.SSR_Count++==CHdata3.SsrStartBuff) PWM_OUT(3,ON);
        }
        if(CHdata4.DataSet[HSCI])
        {
			if(CHdata4.DataGet[Rate]<999) PWM_OUT(4,OFF);
			TIM1->ARR=ConverTime(CHdata4.DataGet[Rate],SYS.PWM_Period,1);
			TIM1->SR=~TIM_IT_Update;
            if(CHdata4.DataGet[Rate]>0) TIM1->CR1|=0x0001;
			else TIM1->CR1&=0xfffe;
        }
        else 
        {
            if(CHdata4.SSR_Count==0) {CHdata4.SsrStartBuff=ConverTime(CHdata4.DataGet[Rate],SYS.PWM_Period,0);PWM_OUT(4,OFF);}
			if(CHdata4.SSR_Count++==CHdata4.SsrStartBuff) PWM_OUT(4,ON);
        }
		CHdata1.ZeroCount++;
		CHdata2.ZeroCount++;
		CHdata3.ZeroCount++;
		CHdata4.ZeroCount++;
		if(SYS.GetPeriodCount<100)
		{
			if(++SYS.PeriodCycle==10)	
			{
				if(SYS.GetPeriodCount>10) SYS.PWM_Period=(100000/SYS.PWM_GetPeriod+SYS.PWM_Period)/2;
				else SYS.PWM_Period=100000/SYS.PWM_GetPeriod;
				if(SYS.PWM_Period<=110) SYS.PWM_Period=100;
				else SYS.PWM_Period=120;
				SYS.GetPeriodCount++;
				SYS.PWM_GetPeriod=0;
				SYS.PeriodCycle=0;
			}
		}
    }
	OS_EXIT_CRITICAL();
    OSIntExit();
}			

