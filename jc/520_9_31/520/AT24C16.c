#include <stm32f10x_lib.h>
#define  set_SDA   GPIOB->BSRR=GPIO_Pin_7
#define  clr_SDA   GPIOB->BRR=GPIO_Pin_7
#define  set_SCL   GPIOB->BSRR=GPIO_Pin_6
#define  clr_SCL   GPIOB->BRR=GPIO_Pin_6
#define  set_WP	   GPIOB->BSRR=GPIO_Pin_5
#define  clr_WP    GPIOB->BRR=GPIO_Pin_5
#define SCL_read GPIOB->IDR & GPIO_Pin_6
#define SDA_read GPIOB->IDR & GPIO_Pin_7
void I2C_delay(void)
{
	u8 i=10;
	while(i) { i--; } 
}
void I2C_delay1(void)
{
	u8 i=20; 
	while(i) { i--; } 
}
u8 Start(void)
{ 
  set_SDA;
  I2C_delay1();
  set_SCL;
  I2C_delay1();
  if(!SDA_read) return 1; 
  clr_SDA;
  I2C_delay1();
  if(SDA_read) return 1;
  clr_SCL;
  I2C_delay1();
  return 0;
}
void Stop(void)
{ 
  clr_SCL;
  I2C_delay1();
  clr_SDA;
  I2C_delay1();
  set_SCL;
  I2C_delay1();
  set_SDA;
  I2C_delay1();
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

UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR )//for AT24C02~16,When WR=0 is write.
{
	UCHAR deviceAddress=0,x=0,i=2,*p=0;
	UINT address,nu;
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
					Delay(30);
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
	  		Delay(30);
		}
		if(!x) break;
		
	}
	data=p;
	return x;
     	
}

