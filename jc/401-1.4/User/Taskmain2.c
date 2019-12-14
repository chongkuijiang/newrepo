#include "stm32f10x.h"
#include "userApp.h"
void ResetManualTurn(void);
UCHAR ModeShiftKeyResponse(UCHAR num);
void UpKeyResponse(UCHAR num);
void ModeKeyResponse(UCHAR num);
void ModeKeyUnlashResponse(UCHAR num);
void DownKeyResponse(UCHAR num);
void ShiftKeyResponse(UCHAR num);
void ShiftKeyUnlashResponse(UCHAR num);
UCHAR PressTimeCount[5]={0,0,0,0,0};


void Task2(void)
{
		
		if((ModeKey)&&(ShiftKey)) 
		{
			if(PressTimeCount[4]<255) PressTimeCount[4]++;
			PressTimeCount[4]=ModeShiftKeyResponse(PressTimeCount[4]);
			SYS.HomeBackTime=250;
		}
		else if(UpKey) 
		{
			if(PressTimeCount[0]<255) PressTimeCount[0]++;
			if(SYS.DataSet[LOC]&&SYS.LockCount) 
			{
				if(((SYS.SetMenu!=1)||(SYS.SetStep!=7))&&((SYS.SetMenu!=0)||(SYS.SetStep!=0)||(SYS.DataSet[LOC]==2))) SYS.LockCount=LOCKTIME;
			}
			UpKeyResponse(PressTimeCount[0]);
			SYS.HomeBackTime=250;
		}
		else if(DownKey) 
		{
			if(PressTimeCount[1]<255) PressTimeCount[1]++;
			if(SYS.DataSet[LOC]&&SYS.LockCount) 
			{
				if(((SYS.SetMenu!=1)||(SYS.SetStep!=7))&&((SYS.SetMenu!=0)||(SYS.SetStep!=0)||(SYS.DataSet[LOC]==2))) SYS.LockCount=LOCKTIME;
			}
			DownKeyResponse(PressTimeCount[1]);
			SYS.HomeBackTime=250;
		}
		else if(ModeKey) {if(PressTimeCount[2]<255) PressTimeCount[2]++;ModeKeyResponse(PressTimeCount[2]);SYS.HomeBackTime=250;}
		else if(ShiftKey) {if(PressTimeCount[3]<255) PressTimeCount[3]++;ShiftKeyResponse(PressTimeCount[3]);SYS.HomeBackTime=250;}
		if(!UpKey) PressTimeCount[0]=0;
		if(!DownKey) PressTimeCount[1]=0;
		if(PressTimeCount[4]&&(!ModeKey)&&(!ShiftKey))
		{
			PressTimeCount[4]=0;
			PressTimeCount[2]=0;
			PressTimeCount[3]=0;
		}
		if(!ModeKey) 
		{
			if(PressTimeCount[4]==0) ModeKeyUnlashResponse(PressTimeCount[2]);
			PressTimeCount[2]=0;
		}
		if(!ShiftKey) 
		{
			if(PressTimeCount[4]==0) ShiftKeyUnlashResponse(PressTimeCount[3]);
			PressTimeCount[3]=0;
		}
}
UINT UpDownUnsignedValue(UINT LowLimit,UINT HighLimit,UINT dataValue,UCHAR PointEffer,BOOL IncOrDec)
{
	UINT i=1;
	if(PointEffer&&(!SYS.DataSet[DPSW])) i=10;
	if(!(SYS.BlinkState&0x70)) SYS.BlinkState=0x80;
	else if(SYS.BlinkState&0x40) i=i*10;
	else if(SYS.BlinkState&0x20) i=i*100;
	else if(SYS.BlinkState&0x10) i=i*1000;
	if(IncOrDec)
	{
		if((dataValue+i)<=HighLimit) dataValue+=i;
		else dataValue=HighLimit;
	}
	else
	{
		if(((short)dataValue-i)>=LowLimit) dataValue-=i;
		else dataValue=LowLimit;
	}
	return dataValue;
}

////////////////////////////////////////////////////////////////////////////////////////////
UINT UpDownTimeValue(UINT LowLimit,UINT HighLimit,UINT dataValue,BOOL IncOrDec)
{
	UINT i=1;
	if(!(SYS.BlinkState&0x70)) SYS.BlinkState=0x80;
	else if(SYS.BlinkState&0x40) i=i*10;
	else if(SYS.BlinkState&0x20) i=i*100;
	if(IncOrDec)
	{
		dataValue+=i;
		if(dataValue%100>59) dataValue+=40;
		if(dataValue>HighLimit) dataValue=HighLimit;
	}
	else
	{
		if(dataValue>i) dataValue-=i;
		else dataValue=0;
		if(dataValue%100>59) dataValue-=40;
		if(dataValue<LowLimit) dataValue=LowLimit;
	}
	return dataValue;
}
////////////////////////////////////////////////////////////////////////////////////////////


UCHAR ModeShiftKeyResponse(UCHAR num)
{
	if(num==3)
	{
		SYS.BlinkNum=0;
		SYS.BlinkState=0;
		if(SYS.SetMenu)
		{
			SYS.SetMenu=0;
			if(SYS.RunningMode==0) 	SYS.SetStep=0;
			else if(SYS.RunningMode==1) SYS.SetStep=3;
			else   SYS.SetStep=4;
			num=255;
		}
	}
	else if(num==50)
	{
		if(SYS.SetMenu==0) 
		{
			SYS.SetMenu=2;
			SYS.SetStep=1;
			SYS.BlinkNum=0;
			SYS.BlinkState=0;
		}
	}
	return num;
}
void ModeKeyResponse(UCHAR num)
{
	if(num==50)
	{
		SYS.BlinkNum=0;
		SYS.BlinkState=0;
		if(SYS.SetMenu==0) {SYS.SetMenu=1;SYS.SetStep=1;}
	}
}
void ModeKeyUnlashResponse(UCHAR num)
{
	if((num>2)&&(num<40))
	{
		SYS.BlinkNum=0;
		SYS.BlinkState=0;
		switch(SYS.SetMenu)
		{
			case 0:
				if(SYS.RunningMode%10==0)//Auto mode
				{
					if(SYS.SetStep<2) SYS.SetStep++;
					else  SYS.SetStep=0;
				}
				else if(SYS.RunningMode%10==1)	 SYS.SetStep=3;	//Standby mode
				else if(SYS.RunningMode%10==2)
				{
					if(SYS.SetStep==4) SYS.SetStep=2;//Manual mode
					else SYS.SetStep=4;
				}
				break;
			case 1:
				if(SYS.SetStep<8) SYS.SetStep++;
				else  SYS.SetStep=0;
				break;
			case 2:
				if(SYS.SetStep<10) {if(++SYS.SetStep==8) SYS.DataSet[DisErrNum]=1;}
				else  SYS.SetStep=0;
				break;
			case 4:
				if(SYS.SetStep<3) SYS.SetStep++;
				else  SYS.SetStep=0;
				break;
			default:break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void ShiftKeyResponse(UCHAR num)
{
  	if(num==50)
  	{
		if(SYS.SetMenu==0)
		{
			SYS.BlinkNum=0;
			SYS.BlinkState=0;
			if(SYS.RunningMode<2) 
			{
				if(++SYS.RunningMode==1) SYS.DataGet[Standby_SV]=SYS.DataSet[Tem_SV]*SYS.DataSet[ST_R]/100;
			}
			else SYS.RunningMode=0;
			if(SYS.RunningMode==0) 	SYS.SetStep=0;
			else if(SYS.RunningMode==1) SYS.SetStep=3;
			else SYS.SetStep=4;
		}
	}
}
void ShiftKeyUnlashResponse(UCHAR num)
{
	if((num>2)&&(num<40))
	{
		if(replan ==1) {replan =0; SYS.ErrorCode =NoError; return;}
		SYS.BlinkNum=0;
		switch((UINT)SYS.SetMenu*100+SYS.SetStep)
		{
			case 0:	  case 3:  case 401: case 402: case 403:
					if(SYS.BlinkState==0x80) SYS.BlinkState=0x40;
					else if(SYS.BlinkState==0x40) SYS.BlinkState=0x20;
					else if(SYS.BlinkState==0x20) SYS.BlinkState=0x10;
					else if(SYS.BlinkState==0x10) SYS.BlinkState=0x80;
					else  SYS.BlinkState=0x40;
					break;
			case 4:   case 201: case 202: case 203: case 204: case 206:
			case 207: case 208:	case 108:
					if(SYS.BlinkState==0x80) SYS.BlinkState=0x40;
					else if(SYS.BlinkState==0x40) SYS.BlinkState=0x20;
					else if(SYS.BlinkState==0x20) SYS.BlinkState=0x80;
					else SYS.BlinkState=0x40;
					break;
			case 205: 
					if(SYS.BlinkState==0x40) SYS.BlinkState=0x80;
					else SYS.BlinkState=0x40;
					break;
			default: break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void UpKeyResponse(UCHAR num)
{	
	s16 dataBuff=0;
  	if(num==2) 
  	{
		SYS.BlinkNum=0;
  	  	switch((UINT)SYS.SetMenu*100+SYS.SetStep)
		{
			case 0:
					if(SYS.DataSet[LOC]==2) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[C_F]==0) SYS.SV_Dis=UpDownUnsignedValue(400,4500,SYS.SV_Dis,1,1);
					else SYS.SV_Dis=UpDownUnsignedValue(1040,8420,SYS.SV_Dis,1,1);
					if(SYS.DataSet[C_F]==0) dataBuff=SYS.SV_Dis;
					else  dataBuff=(SYS.SV_Dis-320)*5/9;
					SYS.DataSet[Tem_SV]=dataBuff;
					SYS.WriteState=Tem_SV;
                    svDelay = 50; //5s
					if(SYS.AutoFlag==3) ResetManualTurn();
					break;
			case 3:	if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[C_F]==0)SYS.DataGet[Standby_SV]=UpDownUnsignedValue(400,SYS.SV_Dis*99/100,SYS.DataGet[Standby_SV],1,1);
					else SYS.DataGet[Standby_SV]=UpDownUnsignedValue(1040,SYS.SV_Dis*99/100,SYS.DataGet[Standby_SV],1,1);
					//SYS.DataSet[ST_R]=SYS.DataGet[Standby_SV]*100/SYS.DataSet[Tem_SV];
					break;
			case 4:
					if(SYS.ErrorCode==HT_ST) break;
					if(SYS.ErrorCode==TR_ST) break;
					SYS.ManualRate=UpDownUnsignedValue(0,999,SYS.ManualRate,1,1);
					break;
			case 100:
					SYS.SetMenu=2;
					break;
			case 101:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[In]==0) SYS.DataSet[In]=1;
					else SYS.DataSet[In]=0;
					SYS.BlinkState|=0xf0;
					SYS.WriteState=In;
					break;
			case 102:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[C_F]==0) 
					{
						SYS.DataSet[C_F]=1;
						SYS.SV_Dis=SYS.DataSet[Tem_SV]*9/5+320;
					}
					else 
					{
						SYS.DataSet[C_F]=0;
						SYS.SV_Dis=SYS.DataSet[Tem_SV];
					}
					//SYS.FlagCF=1;
					DivNumPV=0;
					SYS.BlinkState|=0xf0;
					SYS.WriteState=C_F;
					break;
			case 103:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[DPSW]==0) SYS.DataSet[DPSW]=1;
					else 
					{
						SYS.DataSet[DPSW]=0;
						dataBuff=SYS.DataSet[Tem_SV]/10;
						SYS.DataSet[Tem_SV]=dataBuff*10;
						if(SYS.DataSet[C_F]==0) SYS.SV_Dis=SYS.DataSet[Tem_SV];
						else SYS.SV_Dis=9*SYS.DataSet[Tem_SV]/5+320;
					}
					SYS.BlinkState|=0xf0;
					SYS.WriteState=DPSW;
					break;
			case 104:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[HSCI]==1) SYS.DataSet[HSCI]=0;
					else SYS.DataSet[HSCI]=1;
					SYS.BlinkState|=0xf0;
					SYS.WriteState=HSCI;
					break;
			case 105:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[TURN]==0) SYS.DataSet[TURN]=1;
					else if(SYS.AutoFlag!=3)SYS.DataSet[TURN]=0;
					SYS.BlinkState|=0xf0;
					//SYS.WriteState=TURN;
					break;
			case 106:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[BUZ]==0) SYS.DataSet[BUZ]=1;
					else SYS.DataSet[BUZ]=0;
					SYS.BlinkState|=0xf0;
					SYS.WriteState=BUZ;
					break;
			case 107:
					SYS.BlinkState|=0xf0;
					if(SYS.DataSet[LOC]<2) SYS.DataSet[LOC]++;
					else  SYS.DataSet[LOC]=0;
					SYS.WriteState=LOC;
					break;
			case 108:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[ID]=UpDownUnsignedValue(0,1000,SYS.DataSet[ID],0,1);
//					if(SYS.DataSet[ID]<256) SYS.WriteState=ID;
//					else SYS.WriteState=0;
					break; 
			case 200:
					SYS.SetMenu=1;
					break;
			case 201:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[AL_H]=UpDownUnsignedValue(30,999,SYS.DataSet[AL_H],0,1);
					SYS.WriteState=AL_H;
					break;
			case 202:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[AL_L]=UpDownUnsignedValue(30,999,SYS.DataSet[AL_L],0,0);
					SYS.WriteState=AL_L;
					break;
			case 203:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[HC_H]=UpDownUnsignedValue(100,250,SYS.DataSet[HC_H],0,1);
					SYS.WriteState=HC_H;
					break;
			case 204:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[HC_L]=UpDownUnsignedValue(0,10,SYS.DataSet[HC_L],0,1);
					SYS.WriteState=HC_L;
					break;
			case 205:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[SO_T]=UpDownUnsignedValue(0,30,SYS.DataSet[SO_T],0,1);
					SYS.WriteState=SO_T;
					break;
			case 206:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[SO_R]=UpDownUnsignedValue(100,500,SYS.DataSet[SO_R],0,1);
					SYS.WriteState=SO_R;
					break;
			case 207:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[ST_T]=UpDownTimeValue(0,959,SYS.DataSet[ST_T],1);
					SYS.WriteState=ST_T;
					break;
			case 208:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[ST_R]=UpDownUnsignedValue(0,99,SYS.DataSet[ST_R],0,1);
					SYS.WriteState=ST_R;
					break;
			case 209:
					SYS.DataSet[DisErrNum]=UpDownUnsignedValue(1,20,SYS.DataSet[DisErrNum],0,1);
					SYS.BlinkState=0x00;
					break;
			case 300:
					SYS.SetMenu=4;
					break;
			case 400:
					SYS.SetMenu=1;
					break;
			case 401:
					SYS.DataSet[HT_Z]=UpDownUnsignedValue(0,200,SYS.DataSet[HT_Z],0,1);
					SYS.WriteState=HT_Z;
					break;
			case 402:
				SYS.DataSet[HT_A]=UpDownUnsignedValue(800,1200,SYS.DataSet[HT_A],0,1);
					SYS.WriteState=HT_A;
					break;
			case 403:
				   //SYS.DataSet[ET_Z]=UpDownUnsignedValue(900,1100,SYS.DataSet[ET_Z],0,1);
                                SYS.DataSet[ET_Z]=UpDownUnsignedValue(700,1300,SYS.DataSet[ET_Z],0,1);

                                   
					SYS.WriteState=ET_Z;
					break;
			default: break;
		}
  	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void DownKeyResponse(UCHAR num)
{
	s16 dataBuff=0;
	if(num==2)
	{
		SYS.BlinkNum=0;
		switch((UINT)SYS.SetMenu*100+SYS.SetStep)
		{
			case 0:	
					if(SYS.DataSet[LOC]==2) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[C_F]==0) SYS.SV_Dis=UpDownUnsignedValue(400,4500,SYS.SV_Dis,1,0);
					else SYS.SV_Dis=UpDownUnsignedValue(1040,8420,SYS.SV_Dis,1,0);
					if(SYS.DataSet[C_F]==0) dataBuff=SYS.SV_Dis;
					else  dataBuff=(SYS.SV_Dis-320)*5/9;
					SYS.DataSet[Tem_SV]=dataBuff;
					SYS.WriteState=Tem_SV;
                    svDelay = 50; //5s
					if(SYS.AutoFlag==3) ResetManualTurn();
					break;
			case 3:	if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					if(SYS.DataSet[C_F]==0)SYS.DataGet[Standby_SV]=UpDownUnsignedValue(400,SYS.SV_Dis*99/100,SYS.DataGet[Standby_SV],1,0);
					else SYS.DataGet[Standby_SV]=UpDownUnsignedValue(1040,SYS.SV_Dis*99/100,SYS.DataGet[Standby_SV],1,0);
                    //SYS.DataSet[ST_R]=SYS.DataGet[Standby_SV]*100/SYS.DataSet[Tem_SV];
					break;
			case 4:
					SYS.ManualRate=UpDownUnsignedValue(0,999,SYS.ManualRate,1,0);
					break;
			case 100:
					if(SYS.DataSet[ID]==888) SYS.SetMenu=4;
					else SYS.SetMenu=2;
					break;
			case 107:
					SYS.BlinkState|=0xf0;
					if(SYS.DataSet[LOC]) SYS.DataSet[LOC]--;
					else  SYS.DataSet[LOC]=2;
					SYS.WriteState=LOC;
					break;
			case 108:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[ID]=UpDownUnsignedValue(0,1000,SYS.DataSet[ID],0,0);
//					if(SYS.DataSet[ID]<256) SYS.WriteState=ID;
//					else SYS.WriteState=0;
					break;
			case 200:
					SYS.SetMenu=1;
					break;
			case 201:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[AL_H]=UpDownUnsignedValue(30,999,SYS.DataSet[AL_H],0,0);
					SYS.WriteState=AL_H;
					break;
			case 202:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[AL_L]=UpDownUnsignedValue(30,999,SYS.DataSet[AL_L],0,1);
					SYS.WriteState=AL_L;
					break;
			case 203:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[HC_H]=UpDownUnsignedValue(100,250,SYS.DataSet[HC_H],0,0);
					SYS.WriteState=HC_H;
					break;
			case 204:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[HC_L]=UpDownUnsignedValue(0,10,SYS.DataSet[HC_L],0,0);
					SYS.WriteState=HC_L;
					break;
			case 205:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[SO_T]=UpDownUnsignedValue(0,30,SYS.DataSet[SO_T],0,0);
					SYS.WriteState=SO_T;
					break;
			case 206:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[SO_R]=UpDownUnsignedValue(100,500,SYS.DataSet[SO_R],0,0);
					SYS.WriteState=SO_R;
					break;
			case 207:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[ST_T]=UpDownTimeValue(0,959,SYS.DataSet[ST_T],0);
					SYS.WriteState=ST_T;
					break;
			case 208:if(SYS.DataSet[LOC]) {SYS.LockCount=LOCKTIME;break;}
					SYS.DataSet[ST_R]=UpDownUnsignedValue(0,99,SYS.DataSet[ST_R],0,0);
					SYS.WriteState=ST_R;
					break;
			case 209:
					SYS.DataSet[DisErrNum]=UpDownUnsignedValue(1,20,SYS.DataSet[DisErrNum],0,0);
					SYS.BlinkState=0x00;
					break;
			case 300:
					SYS.SetMenu=2;
					break;
			case 400:
					SYS.SetMenu=2;
					break;
			case 401:
					SYS.DataSet[HT_Z]=UpDownUnsignedValue(0,200,SYS.DataSet[HT_Z],0,0);
					SYS.WriteState=HT_Z;
					break;
			case 402:
				SYS.DataSet[HT_A]=UpDownUnsignedValue(800,1200,SYS.DataSet[HT_A],0,0);
					SYS.WriteState=HT_A;
					break;
			case 403:
				//	SYS.DataSet[ET_Z]=UpDownUnsignedValue(900,1100,SYS.DataSet[ET_Z],0,0);
                            SYS.DataSet[ET_Z]=UpDownUnsignedValue(700,1300,SYS.DataSet[ET_Z],0,0);


				SYS.WriteState=ET_Z;
					break;
			default: break;
		}
	}
}

