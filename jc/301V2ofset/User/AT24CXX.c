#include "stm32f10x.h"
#include "userApp.h"

#define  set_SDA   GPIOA->BSRR=GPIO_Pin_0
#define  clr_SDA   GPIOA->BRR=GPIO_Pin_0
#define  set_SCL   GPIOA->BSRR=GPIO_Pin_1
#define  clr_SCL   GPIOA->BRR=GPIO_Pin_1
#define  set_WP	   GPIOA->BSRR=GPIO_Pin_2
#define  clr_WP    GPIOA->BRR=GPIO_Pin_2
#define SCL_read GPIOA->IDR & GPIO_Pin_1
#define SDA_read GPIOA->IDR & GPIO_Pin_0

void Delay(UINT num);
void delay(UINT num)
{
	UCHAR i;
	while(num--)
	{
		for(i=0;i<63;i++);
	}
}

void I2C_delay(void)
{
	u8 i=10; 
	while(i) { i--; } 
}
u8 Start(void)
{ 
  set_SDA;
  delay(100);
  set_SCL;
  delay(100);
  if(!SDA_read) return 1;
  clr_SDA;
  delay(100);
  if(SDA_read) return 1;
  clr_SCL;
  delay(100);
  return 0;
}
void Stop(void)
{ 
  clr_SCL;
  delay(100);
  clr_SDA;
  delay(100);
  set_SCL;
  delay(100);
  set_SDA;
  delay(100);
}

UCHAR RecAck(void)
{ 
	u8 errtime = 225;
	clr_SCL;
	I2C_delay();
	set_SDA;
	I2C_delay();
  	set_SCL;
  	I2C_delay();
  	while(SDA_read) 
	{
		if(--errtime==0) {Stop();return 1;}
	}
	clr_SCL;
	I2C_delay();
	return 0;
}

void Ack(void)
{ 
	clr_SCL;
	I2C_delay();
  	clr_SDA;
  	I2C_delay();
  	set_SCL;
  	I2C_delay();
  	clr_SCL;
  	I2C_delay();
}

void NoAck(void)
{
	clr_SCL;
	I2C_delay();
  	set_SDA;
	I2C_delay();
  	set_SCL;
  	I2C_delay();
  	clr_SCL;
	I2C_delay();
}

void Send(UCHAR sendByte)
{ 
  UCHAR j=8;
  while(j--)
  { 
	clr_SCL;
	I2C_delay();
	if(sendByte&0x80) set_SDA;
	else clr_SDA;
	I2C_delay();
	sendByte<<=1;
	set_SCL;
	I2C_delay();
  }
  clr_SCL;
}

UCHAR Receive(void)
{ 
  UCHAR receiveByte=0,i=8;
  while(i--)
  { 
  	receiveByte<<=1; 
    clr_SCL;
    I2C_delay();   
	set_SCL;
	I2C_delay(); 
    if(SDA_read) receiveByte|=0x01;
  }  
  clr_SCL;  
  return(receiveByte);
}

UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR )
{
	UCHAR deviceAddress=0,x=0,i=2,nu,*p=0;
	UINT address;
	while(i--)
	{
		address=startAddress;
		if(address<256) deviceAddress=0xA0;
		else if(address<512) deviceAddress=0xA2;
		else if(address<768) deviceAddress=0xA4;
		else if(address<1024) deviceAddress=0xA6;
		else if(address<1280) deviceAddress=0xA8;
		else if(address<1536) deviceAddress=0xAA;
		else if(address<1792) deviceAddress=0xAC;
		else if(address<2048) deviceAddress=0xAE;
		p=(UCHAR *)data;
		nu=number;
		x=0;
		if(WR)
		{
			while(nu--)
   	 		{
				if(address<256) deviceAddress=0xA0;
				else if(address<512) deviceAddress=0xA2;
				else if(address<768) deviceAddress=0xA4;
				else if(address<1024) deviceAddress=0xA6;
				else if(address<1280) deviceAddress=0xA8;
				else if(address<1536) deviceAddress=0xAA;
				else if(address<1792) deviceAddress=0xAC;
				else if(address<2048) deviceAddress=0xAE;	
				Start();
				Send(deviceAddress);
				x+=RecAck();
				Send((UCHAR)address);
				x+=RecAck();
				Start();
				Send(deviceAddress+1);
				x+=RecAck();
				*p++=Receive();
      			address++;
				NoAck();
				Stop();	
	    	}		
		}
		else
		{
			x+=Start();
			Send(deviceAddress);
			x+=RecAck();
			Send((UCHAR)address);
			x+=RecAck();
			clr_WP;
	  		while(--nu)
	  		{
		  		Send(*p++);
	    		x+=RecAck(); 
	    		address++;
				if(address%16==0)
				{
					Stop();
					Delay(10);
					if(address<256) deviceAddress=0xA0;
					else if(address<512) deviceAddress=0xA2;
					else if(address<768) deviceAddress=0xA4;
					else if(address<1024) deviceAddress=0xA6;
					else if(address<1280) deviceAddress=0xA8;
					else if(address<1536) deviceAddress=0xAA;
					else if(address<1792) deviceAddress=0xAC;
					else if(address<2048) deviceAddress=0xAE;
					Start();
					Send(deviceAddress);
					x+=RecAck();
					Send((UCHAR)address);
					x+=RecAck();
				}
	 		}
			Send(*p++);
			x+=RecAck();
	 	 	Stop();
	 	 	set_WP;
	  		Delay(10);
		}
		if(!x) break;
		
	}
	data=p;
	return x; 	
}

void Task3(void)
{
	static u16 i=0,bak=0;
		if((SYS.BlinkNum==0)&&(SYS.BlinkState==0))
		{
			if(SYS.WriteState)
			{
			 	CommIIC(&SYS.DataSet[SYS.WriteState],2,SYS.WriteState<<1,0);
				if(SYS.WriteState==ST_T) SYS.StandbyTime=SYS.DataSet[ST_T]/100*3600+SYS.DataSet[ST_T]%100*60;
				else if(SYS.WriteState==ST_R) SYS.DataGet[Standby_SV]=SYS.DataSet[Tem_SV]*SYS.DataSet[ST_R]/100;
				else if(SYS.WriteState==DPSW) {if(SYS.DataSet[DPSW]==0) CommIIC(&SYS.DataSet[Tem_SV],2,Tem_SV<<1,0);}
				SYS.WriteState=0;
			}
		}
		if(SYS.ErrorCode!=NoError)
		{
			switch(SYS.ErrorCode)
			{
				case ALARM_H: bak=4;break;
				case ALARM_L: bak=5;break;
				case F1_OP: bak=9;break;
				case F2_OP: bak=10;break;
				case HT_OP: bak=6;break;
				case TC_OP: bak=1;break;
				case TC_ST: bak=2;break;
				case TC_RE: bak=3;break;
				case HT_ST: bak=7;break;
				case TR_ST: bak=8;break;
				default: bak=0;break;
			}
			if(bak!=SYS.DataSet[Error])
			{
				for(i=0;i<19;i++) SYS.DataSet[Error+19-i]=SYS.DataSet[Error+18-i];
				SYS.DataSet[Error]=bak;
				CommIIC(&SYS.DataSet[Error],40,Error<<1,0);
			}
		}

		if(SYS.DataSet[C_F]) {LED_C_off;LED_F_on;}
		else {LED_F_off;LED_C_on;}
		if(SYS.DataSet[In]) {LED_J_off;LED_K_on;}
		else {LED_K_off;LED_J_on;}
		if(SYS.DataGet[Rate]) LED_out_on;
		else LED_out_off;
		if(SYS.RunningMode==0)
		{
			LED_manu_off;
			if(SYS.AutoFlag==1)	LED_soft_on;
			else LED_soft_off;
			if(SegLed[8]&0x0400) LED_auto_off;
			else LED_auto_on;
		}
		else if(SYS.RunningMode==1)
		{
			LED_soft_off;
			LED_auto_off;
			LED_stby_on;
		}
		else if(SYS.RunningMode==2)
		{
			LED_soft_off;
			LED_stby_off;
			LED_manu_on;
		}
		else
		{
			LED_soft_off;
			LED_auto_off;
			LED_stby_off;
			LED_manu_off;
		} 
}
