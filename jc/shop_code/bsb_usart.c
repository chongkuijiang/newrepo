
#include "bsb_usart.h"
#include "bsb_gpio.h"
//------------------------------------------------------------------------------------------------------------------
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin; 
	uint32_t CLK;
}gpioCofig_t;
//----------------------------------------------------
typedef struct
{
	gpioCofig_t Tx;	
	gpioCofig_t Rx;
}usartGpio_t;
//----------------------------------------------------
typedef struct
{
	void (* USARTx_ClockCmd )(uint32_t,FunctionalState);
	uint32_t USARTx_Clk;
}usartClk_t;
//----------------------------------------------------
typedef struct
{
	USART_TypeDef* Com;
	usartGpio_t*  Pin;
	usartClk_t*   Clk;
	uint8_t     IRQn;
}usartId_t;
//-----------------------------------------------------
typedef struct
{
	USART_TypeDef* usart;
	uint8_t* txbuf;
	uint8_t* rxbuf;
	
	uint16_t rxlen;
	uint16_t txlen;
	
	uint16_t txp;
	uint16_t rxp;	
	
	int rxtime;
	ReceiveCallback callback;
	GPIO_TypeDef* gtr;
	uint16_t ptr;
	
}usart_t;

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

static usart_t* Udata[5];
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_1 
	static usartClk_t usart1_Clk={RCC_APB2PeriphClockCmd,RCC_APB2Periph_USART1};
	static usartGpio_t usart1_Pin[2]={
		   {{GPIOA,GPIO_Pin_9,RCC_APB2Periph_GPIOA},{GPIOA,GPIO_Pin_10,RCC_APB2Periph_GPIOA}}, //TX => PA9,RX => PA10
			 {{GPIOB,GPIO_Pin_6,RCC_APB2Periph_GPIOB},{GPIOB,GPIO_Pin_7,RCC_APB2Periph_GPIOB}}   //TX => PB6,RX => PB7
	};				
	static usartId_t usart1 = {USART1,usart1_Pin,&usart1_Clk,USART1_IRQn}; 
	static uint8_t TXbuf1[Txsize_1];
	static uint8_t RXbuf1[Rxsize_1];
	static usart_t Udata_1 = {.usart=USART1, .txbuf=TXbuf1, .rxbuf=RXbuf1, .rxlen=Rxsize_1,};
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_2 
	static usartClk_t usart2_Clk={RCC_APB1PeriphClockCmd,RCC_APB1Periph_USART2};
	static usartGpio_t usart2_Pin[]={
		   {{GPIOA,GPIO_Pin_2,RCC_APB2Periph_GPIOA},{GPIOA,GPIO_Pin_3,RCC_APB2Periph_GPIOA}}, //TX => PA2,RX => PA3
		   {{GPIOD,GPIO_Pin_5,RCC_APB2Periph_GPIOD},{GPIOD,GPIO_Pin_6,RCC_APB2Periph_GPIOD}}  //TX => PD5,RX => PD6
	};
	static usartId_t usart2 = {USART2,usart2_Pin,&usart2_Clk,USART2_IRQn}; 
	static uint8_t TXbuf2[Txsize_2];
	static uint8_t RXbuf2[Rxsize_2];
	static usart_t Udata_2 = {.usart=USART2, .txbuf=TXbuf2, .rxbuf=RXbuf2, .rxlen=Rxsize_2,};
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_3 
	static usartClk_t usart3_Clk={RCC_APB1PeriphClockCmd,RCC_APB1Periph_USART3};
	static usartGpio_t usart3_Pin[]={
		   {{GPIOB,GPIO_Pin_10,RCC_APB2Periph_GPIOB},{GPIOB,GPIO_Pin_11,RCC_APB2Periph_GPIOB}}, //TX => PB10,RX => PB11
		   {{GPIOC,GPIO_Pin_10,RCC_APB2Periph_GPIOC},{GPIOC,GPIO_Pin_11,RCC_APB2Periph_GPIOC}}, //TX => PC10,RX => PC11
		   {{GPIOD,GPIO_Pin_8,RCC_APB2Periph_GPIOD},{GPIOD,GPIO_Pin_9,RCC_APB2Periph_GPIOD}}    //TX => PD8,RX => PD9
	};
	static usartId_t usart3 = {USART3,usart3_Pin,&usart3_Clk,USART3_IRQn}; 
	static uint8_t TXbuf3[Txsize_3];
	static uint8_t RXbuf3[Rxsize_3];
	static usart_t Udata_3 = {.usart=USART3, .txbuf=TXbuf3, .rxbuf=RXbuf3, .rxlen=Rxsize_3,};
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_4 
	static usartClk_t usart4_Clk={RCC_APB1PeriphClockCmd,RCC_APB1Periph_UART4};
	static usartGpio_t usart4_Pin[]={
		   {{GPIOC,GPIO_Pin_10,RCC_APB2Periph_GPIOC},{GPIOC,GPIO_Pin_11,RCC_APB2Periph_GPIOC}}, //TX => PC10,RX => PC11
	};
	static usartId_t usart4 = {UART4,usart4_Pin,&usart4_Clk,UART4_IRQn}; 
	static uint8_t TXbuf4[Txsize_4];
	static uint8_t RXbuf4[Rxsize_4];
	static usart_t Udata_4 = {.usart=UART4, .txbuf=TXbuf4, .rxbuf=RXbuf4, .rxlen=Rxsize_4,};
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_5 
	static usartClk_t usart5_Clk={RCC_APB1PeriphClockCmd,RCC_APB1Periph_UART5};
	static usartGpio_t usart5_Pin[]={
		   {{GPIOC,GPIO_Pin_12,RCC_APB2Periph_GPIOC},{GPIOD,GPIO_Pin_2,RCC_APB2Periph_GPIOD}}, //TX => PC12,RX => PD2
	};
	static usartId_t usart5 = {UART5,usart5_Pin,&usart5_Clk,UART5_IRQn}; 
	static uint8_t TXbuf5[Txsize_5];
	static uint8_t RXbuf5[Rxsize_5];
	static usart_t Udata_5 = {.usart=UART5, .txbuf=TXbuf5, .rxbuf=RXbuf5, .rxlen=Rxsize_5,};
#endif
	
//==================================================================================================================
void Usart_GPIO_Configuration(usartId_t* uId, uint8_t Remap)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(uId->Pin[Remap].Tx.CLK | uId->Pin[Remap].Rx.CLK | RCC_APB2Periph_AFIO,ENABLE);  
	
	gpio.GPIO_Pin =  uId->Pin[Remap].Tx.pin;	
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(uId->Pin[Remap].Tx.port, &gpio);    
  
	gpio.GPIO_Pin = uId->Pin[Remap].Rx.pin;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(uId->Pin[Remap].Rx.port, &gpio);	
}
//==================================================================================================================
void Usart_485_Configuration(Port_t portId,Pin_t pin)
{
	gpio_t config;
	config.port = portId;
	config.pin = pin;
	config.mode = Out_PP;
	config.outLevel = Bit_RESET;
	config.Trigger = None;
	GPIO_Configuration(&config,1);
}	
//==================================================================================================================
void Usart_UsartConfiguration(usartId_t* uId, uint32_t BaudRate)
{
	USART_InitTypeDef parameter;
	
	parameter.USART_BaudRate = BaudRate;
	parameter.USART_WordLength = USART_WordLength_8b;
	parameter.USART_StopBits = USART_StopBits_1;
	parameter.USART_Parity = USART_Parity_No;
	parameter.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    parameter.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(uId->Com, &parameter);	
}
//==================================================================================================================
void Usart_NVIC_Configuration(usartId_t* uId, uint8_t Priority, uint8_t Sub)
{
	NVIC_InitTypeDef nvic; 
	
	nvic.NVIC_IRQChannel = uId->IRQn;	 
	nvic.NVIC_IRQChannelPreemptionPriority = Priority;
	nvic.NVIC_IRQChannelSubPriority = Sub;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}
//==================================================================================================================
void Usart(usartId_t* uId,uint8_t PinRemap,uint32_t BaudRate, uint8_t Priority, uint8_t Sub) 
{  
	uId->Clk->USARTx_ClockCmd(uId->Clk->USARTx_Clk,ENABLE);
	Usart_GPIO_Configuration(uId,PinRemap);
	Usart_UsartConfiguration(uId,BaudRate);	
	Usart_NVIC_Configuration(uId,Priority,Sub); 

    USART_ITConfig(uId->Com, USART_IT_RXNE, ENABLE);	
	USART_Cmd(uId->Com, ENABLE);
	USART_ClearFlag(uId->Com, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);	
}
//==================================================================================================================
void _loadChar(usart_t* Ux,char* txbuf)
{
	uint16_t i=0;
	while(txbuf[i]!='\0'){
		Ux->txbuf[i] = (uint8_t)txbuf[i];
		i++;
	};
	Ux->txbuf[i++] = '\r';
	Ux->txbuf[i++] = '\n';
	Ux->txlen = i;
}
//==================================================================================================================
void _loadData(usart_t* Ux,uint8_t* txbuf,uint16_t bufSize)
{
	Ux->txlen = bufSize;
	for(int i=0;i<bufSize;i++)
	{
		Ux->txbuf[i] = txbuf[i];
	}
}
//==================================================================================================================
void Config_ReceiveCallback(UsartId_t uid,ReceiveCallback call)
{
	usart_t* Ux = Udata[uid];
	if(Ux != 0)
	{
		Ux->callback = call;
	}
}
//==================================================================================================================
#if EN_SEND_TI
void SendTI(UsartId_t uid,uint8_t* txbuf,uint16_t bufSize) 
{	
	usart_t* Ux = Udata[uid];
	if(Ux != 0)
	{
		if(Ux->ptr>0) GPIO_SetBits(Ux->gtr, Ux->ptr);
		_loadData(Ux,txbuf,bufSize);
		USART_ITConfig(Ux->usart, USART_IT_TXE, ENABLE);
	}
}

//==================================================================================================================
void SendTI_n(UsartId_t uid,char* txbuf)
{   
	usart_t* Ux = Udata[uid];
	if(Ux != 0)
	{
		if(Ux->ptr>0) GPIO_SetBits(Ux->gtr, Ux->ptr);
		_loadChar(Ux,txbuf);
		USART_ITConfig(Ux->usart, USART_IT_TXE, ENABLE);	
	}
}
#else
//==================================================================================================================
void _sendNormal(usart_t* Ux)
{
	for(int j=0;j<Ux->txlen;j++){
		USART_SendData(Ux->usart, Ux->txbuf[j]);
		while (USART_GetFlagStatus(Ux->usart, USART_FLAG_TXE) == RESET){};	
	}	
	while (USART_GetFlagStatus(Ux->usart, USART_FLAG_TC) == RESET){}; 
}
//==================================================================================================================
void SendNormal(UsartId_t uid,uint8_t* txbuf,uint16_t bufSize)
{
	usart_t* Ux = Udata[uid];
	if(Ux != 0)
	{
		if(Ux->ptr>0) GPIO_SetBits(Ux->gtr, Ux->ptr);
		_loadData(Ux,txbuf,bufSize);
		_sendNormal(Ux);
		if(Ux->ptr>0) GPIO_ResetBits(Ux->gtr, Ux->ptr);
	}
} 
//==================================================================================================================
void SendNormal_n(UsartId_t uid,char* txbuf)
{   
	usart_t* Ux = Udata[uid];
	if(Ux != 0)
	{
		if(Ux->ptr>0) GPIO_SetBits(Ux->gtr, Ux->ptr);
		_loadChar(Ux,txbuf);		
		_sendNormal(Ux);
		if(Ux->ptr>0) GPIO_ResetBits(Ux->gtr, Ux->ptr);	
	}
}
#endif
//==================================================================================================================
void Usart_handler(usart_t* Ux)
{
	if(USART_GetITStatus(Ux->usart, USART_IT_RXNE) != RESET)  
	{
        USART_ClearITPendingBit(Ux->usart, USART_IT_RXNE);		
		Ux->rxbuf[Ux->rxp] = USART_ReceiveData(Ux->usart);
		if(Ux->rxp < Ux->rxlen) Ux->rxp++;
		Ux->rxtime = 4; //0.4ms
    }
#if EN_SEND_TI
	if (USART_GetITStatus(Ux->usart, USART_IT_TXE) != RESET)
	{		
		USART_SendData(Ux->usart, Ux->txbuf[Ux->txp++]);		  
		if(Ux->txp == Ux->txlen){
			USART_ITConfig(Ux->usart, USART_IT_TXE, DISABLE);
			USART_ITConfig(Ux->usart, USART_IT_TC, ENABLE);
		}			
	}
	else if(USART_GetITStatus(Ux->usart, USART_IT_TC) != RESET)  
	{
		Ux->txp = 0;
        USART_ITConfig(Ux->usart, USART_IT_TC, DISABLE);	
        if(Ux->ptr>0) GPIO_ResetBits(Ux->gtr,Ux->ptr);	        		
	}	
#endif	
}
//==================================================================================================================
void Usart_Dispose(void)
{
#if EN_USART_1
	if(Udata_1.rxtime>0){
		Udata_1.rxtime--;
		if(Udata_1.rxtime==0){
			if(Udata_1.callback != 0) Udata_1.callback(Udata_1.rxbuf,Udata_1.rxp);			
			for(int i=0;i<Udata_1.rxp;i++){
				Udata_1.rxbuf[i] = 0;
			}
			Udata_1.rxp = 0;
		}
	}
#endif	
#if EN_USART_2
	if(Udata_2.rxtime>0){
		Udata_2.rxtime--;
		if(Udata_2.rxtime==0){
			if(Udata_2.callback != 0) Udata_2.callback(Udata_2.rxbuf,Udata_2.rxp);			
			for(int i=0;i<Udata_2.rxp;i++){
				Udata_2.rxbuf[i] = 0;
			}
			Udata_2.rxp = 0;
		}
	}
#endif	
#if EN_USART_3
	if(Udata_3.rxtime>0){
		Udata_3.rxtime--;
		if(Udata_3.rxtime==0){
			if(Udata_3.callback != 0) Udata_3.callback(Udata_3.rxbuf,Udata_3.rxp);			
			for(int i=0;i<Udata_3.rxp;i++){
				Udata_3.rxbuf[i] = 0;
			}
			Udata_3.rxp = 0;
		}
	}
#endif		
#if EN_USART_4
	if(Udata_4.rxtime>0){
		Udata_4.rxtime--;
		if(Udata_4.rxtime==0){
			if(Udata_4.callback != 0) Udata_4.callback(Udata_4.rxbuf,Udata_4.rxp);			
			for(int i=0;i<Udata_4.rxp;i++){
				Udata_4.rxbuf[i] = 0;
			}
			Udata_4.rxp = 0;
		}
	}
#endif	
#if EN_USART_5
	if(Udata_5.rxtime>0){
		Udata_5.rxtime--;
		if(Udata_5.rxtime==0){
			if(Udata_5.callback != 0) Udata_5.callback(Udata_5.rxbuf,Udata_5.rxp);			
			for(int i=0;i<Udata_5.rxp;i++){
				Udata_5.rxbuf[i] = 0;
			}
			Udata_5.rxp = 0;
		}
	}
#endif	
}
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_1 
void usart_init1(void)   
{
	Udata[0] = &Udata_1;
#if EN_485_1	
	Port_t port = (Port_t)Gpio485_1;
	Pin_t pin = (Pin_t)Port485_1;	
	Udata_1.gtr = getGpio(port);
	Udata_1.ptr = getPin(pin);
	Usart_485_Configuration(port,pin);
#endif	
	Usart(usart1,PinRemap_1,BaudRate_1,Priority_1,SubPriority_1); 
}	

void USART1_IRQHandler(void) 
{
	Usart_handler(&Udata_1);
}
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_2 
void usart_init2(void)   
{
	Udata[1] = &Udata_2;
#if EN_485_2
	Port_t port = (Port_t)Gpio485_2;
	Pin_t pin = (Pin_t)Port485_2;	
	Udata_2.gtr = getGpio(port);
	Udata_2.ptr = getPin(pin);
	Usart_485_Configuration(port,pin);
#endif	
	Usart(&usart2,PinRemap_2,BaudRate_2,Priority_2,SubPriority_2); 	
}
void USART2_IRQHandler(void) 
{
	Usart_handler(&Udata_2);
}
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_3 
void usart_init3(void)   
{
	Udata[2] = &Udata_3;
#if EN_485_3
	Port_t port = (Port_t)Gpio485_3;
	Pin_t pin = (Pin_t)Port485_3;	
	Udata_3.gtr = getGpio(port);
	Udata_3.ptr = getPin(pin);
	Usart_485_Configuration(port,pin);
#endif
	Usart(usart3,PinRemap_3,BaudRate_3,Priority_3,SubPriority_3); 
}	
void USART3_IRQHandler(void) 
{
	Usart_handler(&Udata_3);
}
#endif
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_4 
void usart_init4(void)   
{
	Udata[3] = &Udata_4;
#if EN_485_4
	Port_t port = (Port_t)Gpio485_4;
	Pin_t pin = (Pin_t)Port485_4;	
	Udata_4.gtr = getGpio(port);
	Udata_4.ptr = getPin(pin);
	Usart_485_Configuration(port,pin);
#endif
	Usart(usart4,PinRemap_4,BaudRate_4,Priority_4,SubPriority_4); 
}	
void USART4_IRQHandler(void) 
{
	Usart_handler(&Udata_4);
}
#endif	
//------------------------------------------------------------------------------------------------------------------

#if EN_USART_5 
void usart_init5(void)   
{
	Udata[4] = &Udata_5;
#if EN_485_5
	Port_t port = (Port_t)Gpio485_5;
	Pin_t pin = (Pin_t)Port485_5;	
	Udata_5.gtr = getGpio(port);
	Udata_5.ptr = getPin(pin);
	Usart_485_Configuration(port,pin);
#endif
	Usart(usart5,PinRemap_5,BaudRate_5,Priority_5,SubPriority_5); 
}	
void USART5_IRQHandler(void) 
{
	Usart_handler(&Udata_5);
}
#endif	
//------------------------------------------------------------------------------------------------------------------

void Usart_Init(void)
{
#if EN_USART_1   //如果使能了串口1
    usart_init1();
#endif
#if EN_USART_2   //如果使能了串口2
    usart_init2();
#endif	
#if EN_USART_3   //如果使能了串口3
    usart_init3();
#endif	
#if EN_USART_4   //如果使能了串口4
    usart_init4();
#endif	
#if EN_USART_5   //如果使能了串口5
    usart_init5();
#endif	
}	
