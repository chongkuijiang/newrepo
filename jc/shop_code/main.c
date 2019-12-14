


#include "stm32f10x.h"
#include "sys.h"
#include "bsb_usart.h"	
#include "bsb_gpio.h"
#include "crc.h"



//-----------------------------------------------------------------------
__IO uint16_t send;


typedef struct
{
    uint16_t nameId;
	uint8_t count;
	uint8_t out;
	uint8_t sm;
	uint8_t sa;
	uint8_t sb;
}data_t;


typedef struct
{
	uint8_t address;
	uint8_t channels;
	data_t  data[18];
	
}sys_data_t;   
 


void IWDG_Config(uint8_t prv ,uint16_t rlv);
void Gpio_Init(void);
void sendquse(void);
void SerialReceive_Callback(uint8_t* rxbuf,uint16_t Size);

//-----------------------------------------------------------------------	
void usart_init(void);
int main(void)
{
    NVIC_Configuration();	
	initSystick(); 
	Usart_Init();
    Gpio_Init();
	IWDG_Config(IWDG_Prescaler_64 ,100); //Tout = prv/40 * rlv (s)
    Config_ReceiveCallback(U2,SerialReceive_Callback);
    while(1)
	{   
		
		  if(send==0)
			 sendquse();

        IWDG_ReloadCounter();		  
	}  
	
}

//-----------------------------------------------------------------------	
void sendmodbusasi()
{
	uint8_t txbuf[100];
	txbuf[0] = ':';
}


//void SendDMA_485(Rx485_t* Ux,uint16_t txlen)
//{
//	DMA_InitTypeDef DMA_InitStructure;   	
//	GPIO_SetBits(Ux->gtr, Ux->ptr);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(Ux->usart->DR));	
//	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Ux->txbuf;	
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
//	DMA_InitStructure.DMA_BufferSize = txlen;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
////	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	 
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
//	DMA_Init(USART_DMA, &DMA_InitStructure);
//	DMA_Cmd (USART_DMA,ENABLE);			
//	DMA_ITConfig(USART_DMA,DMA_IT_TC,ENABLE);  
//	USART_DMACmd(Ux->usart, USART_DMAReq_Tx, ENABLE);
//}



void sendCMD(uint8_t address)
{
	uint8_t Txbuf[8];
	CRC16_t crc;

	Txbuf[0] = address;
	Txbuf[1] = 0xaa;	 //≤È—Ø
	crc = CreatCRC16(Txbuf, 2);
	Txbuf[2] = crc.Bit[1];
	Txbuf[3] = crc.Bit[0];

    SendTI(U2,Txbuf,4);
 //  SendNormal(U2,Txbuf,4);


}

//---------------------------------------------------------------------------
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{		
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();	
}
void SerialReceive_Callback(uint8_t* rxbuf,uint16_t Size)
{
	CRC16_t crc;	
	if(rxbuf[1]==0x5a) //adr,5a,state,pv,sv,crc
	{
		crc = CreatCRC16(rxbuf, 15);
		if((rxbuf[15] == crc.Bit[1])&&(rxbuf[16] == crc.Bit[0]))
		{								
			
		}
	}
	else if(rxbuf[1]==0x5b) //adr,5b,
	{
		crc = CreatCRC16(rxbuf, 3);
		if((rxbuf[3] == crc.Bit[1])&&(rxbuf[4] == crc.Bit[0]))
		{								

		}
	}
  
}

//----------------------------------------------------------------------
void Gpio_Init(void)
{
	//port,pin,mode,outLevel,Trigger
	gpio_t cofig[] ={
	{PB,Pin2,Out_PP,Bit_RESET,None},{PB,Pin11,Out_PP,Bit_RESET,None},{PA,Pin12,Out_PP,Bit_RESET,None},
	{PB,Pin5,IPU,Bit_SET,None},{PB,Pin10,IN_FLOATING,Bit_SET,None},};
	GPIO_Configuration(cofig,GPIO_SIZE(cofig));		
}
//----------------------------------------------------------------------


//----------------------------------------------------------------------




//--------------------------------------------------------------------------
void sendquse(void)  
{
	sendCMD(1);   //cmd
	send = 42; //2.2ms
}












