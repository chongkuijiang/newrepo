


#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"	
#include <String.h>
#include <stdlib.h>

#include "main.h"

//-----------------------------------------------------------------------


__IO uint8_t Msg;





//-----------------------------------------------------------------------	

int main(void)
{
	NVIC_Configuration();	
	initSystick(); 
	uart_init();
	initGPIO();
	IWDG_Config(IWDG_Prescaler_64 ,100); //Tout = prv/40 * rlv (s)
	while(1)
	{
      if(Msg){
				Msg = 0;
				if(rxtim1 >0) {rxtim1--; if(rxtim1 ==0) SerialReceive_Callback();}
				
				continue;
			}				
		
		
					
			IWDG_ReloadCounter();		  
	}
	
}

//-----------------------------------------------------------------------	


//-----------------------------------------------------------------------	
void writeData(uint8_t address,uint8_t reg,uint16_t data)
{
	static uint8_t Txbuf[7],i;
	CRC16_t crc;
	
	Txbuf[0] = address;
	Txbuf[1] = 0xab;  
	Txbuf[2] = reg;
	Txbuf[3] = data>>8;	
	Txbuf[4] = data;	
	crc = CreatCRC16(Txbuf, 5);
	Txbuf[5] = crc.Bit[1];
	Txbuf[6] = crc.Bit[0];
	for(i=0;i<7;i++){
	   USART_SendData(USART1, Txbuf[i]);
	   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
    }	
}
//----------------------------------------------------------------------		

//---------------------------------------------------------------------------
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{		
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();	
}
void SerialReceive_Callback(void)
{
	CRC16_t crc;
	int i;	
	uint8_t* rxbuf = Rxbuf1;
	pr1 = 0;    	
	
	if(rxbuf[1]==0x5a) 
	{
		crc = CreatCRC16(rxbuf, 15);
		if((rxbuf[15] == crc.Bit[1])&&(rxbuf[16] == crc.Bit[0]))
		{								
			
		}
	}
	else if(rxbuf[1]==0x5b) 
	{
		crc = CreatCRC16(rxbuf, 3);
		if((rxbuf[3] == crc.Bit[1])&&(rxbuf[4] == crc.Bit[0]))
		{	
		}
	}
   for(i=0;i<20;i++) rxbuf[i] = 0;
	
}

//----------------------------------------------------------------------
void initGPIO(void)
{
	GPIO_InitTypeDef portSet;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	
	//port1-16
	portSet.GPIO_Speed = GPIO_Speed_50MHz;
	portSet.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&portSet);	
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11);
	
	portSet.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&portSet);	
	GPIO_SetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15);
	
	portSet.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOC,&portSet);	
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	//485
	portSet.GPIO_Pin = GPIO_Pin_0;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&portSet);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	
	//buzzer
	portSet.GPIO_Pin = GPIO_Pin_8;
	portSet.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&portSet);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
    //led-red
    portSet.GPIO_Pin = GPIO_Pin_11;
	portSet.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&portSet);
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);	
    //led-blue
    portSet.GPIO_Pin = GPIO_Pin_12;
	portSet.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&portSet);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);	
	
	
    //24-220v	
	portSet.GPIO_Pin = GPIO_Pin_13;
	portSet.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&portSet);
	//input-	
	portSet.GPIO_Pin = GPIO_Pin_1;
	portSet.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&portSet);
	
}
//----------------------------------------------------------------------











