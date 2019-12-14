
#include "bsb_gpio.h"

typedef struct
{
	GPIO_TypeDef* port;
	uint32_t clk;
}PortConfig_t;

typedef struct
{
	uint16_t pin;
	uint32_t line;	
	uint8_t  IRQn;
}PinConfig_t;


static ExitCallback Callback;
static PortConfig_t portConfig[] = {
	{GPIOA,RCC_APB2Periph_GPIOA},{GPIOB,RCC_APB2Periph_GPIOB},{GPIOC,RCC_APB2Periph_GPIOC},
	{GPIOD,RCC_APB2Periph_GPIOD},{GPIOE,RCC_APB2Periph_GPIOE},{GPIOF,RCC_APB2Periph_GPIOF},{GPIOG,RCC_APB2Periph_GPIOG}};

static PinConfig_t pinConfig[] = {
	{GPIO_Pin_0,EXTI_Line0,EXTI0_IRQn},{GPIO_Pin_1,EXTI_Line1,EXTI1_IRQn},{GPIO_Pin_2,EXTI_Line2,EXTI2_IRQn},{GPIO_Pin_3,EXTI_Line3,EXTI3_IRQn},{GPIO_Pin_4,EXTI_Line4,EXTI4_IRQn},
    {GPIO_Pin_5,EXTI_Line5,EXTI9_5_IRQn},{GPIO_Pin_6,EXTI_Line6,EXTI9_5_IRQn},{GPIO_Pin_7,EXTI_Line7,EXTI9_5_IRQn},{GPIO_Pin_8,EXTI_Line8,EXTI9_5_IRQn},{GPIO_Pin_9,EXTI_Line9,EXTI9_5_IRQn},
    {GPIO_Pin_10,EXTI_Line10,EXTI15_10_IRQn},{GPIO_Pin_11,EXTI_Line11,EXTI15_10_IRQn},{GPIO_Pin_12,EXTI_Line12,EXTI15_10_IRQn},{GPIO_Pin_13,EXTI_Line13,EXTI15_10_IRQn},
    {GPIO_Pin_14,EXTI_Line14,EXTI15_10_IRQn},{GPIO_Pin_15,EXTI_Line15,EXTI15_10_IRQn}};

//==================================================================================================================
GPIO_TypeDef* getGpio(Port_t port) 
{
	return portConfig[port].port;
}
//==================================================================================================================
uint16_t getPin(Pin_t pin)
{
	return pinConfig[pin].pin;
}
//==================================================================================================================
void setHi(Port_t port,Pin_t pin)
{
	portConfig[port].port->BSRR = pinConfig[pin].pin;
}
//==================================================================================================================
void setLo(Port_t port,Pin_t pin)
{
	portConfig[port].port->BRR = pinConfig[pin].pin;
}
//==================================================================================================================
void setToggle(Port_t port,Pin_t pin)
{
	portConfig[port].port->ODR ^= pinConfig[pin].pin;
}
//==================================================================================================================
void GPIO_Configuration(gpio_t* config,int count)				
{
	GPIO_InitTypeDef portSet;
	EXTI_InitTypeDef ExtiSet;
	NVIC_InitTypeDef nvic;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	for(int i=0;i<count;i++)
	{
		RCC_APB2PeriphClockCmd(portConfig[config[i].port].clk,ENABLE);  
		portSet.GPIO_Pin = pinConfig[config[i].pin].pin;
		portSet.GPIO_Mode = (GPIOMode_TypeDef)config[i].mode;
		if((config[i].mode & 0x10) != 0x00) portSet.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(portConfig[config[i].port].port, &portSet); 
		
		if((config[i].mode & 0x10) != 0x00){  //out
			if(config[i].outLevel!=Bit_RESET) setHi(config[i].port,config[i].pin);
			else setLo(config[i].port,config[i].pin);
		}
        else if(config[i].Trigger != None)
		{
			nvic.NVIC_IRQChannel = pinConfig[config[i].pin].IRQn;
			nvic.NVIC_IRQChannelPreemptionPriority = 0;
			nvic.NVIC_IRQChannelSubPriority = 0;
			nvic.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&nvic);		
			GPIO_EXTILineConfig(config[i].port, config[i].pin); 
			ExtiSet.EXTI_Line = pinConfig[config[i].pin].line;
			ExtiSet.EXTI_Mode = EXTI_Mode_Interrupt;
			ExtiSet.EXTI_Trigger = (EXTITrigger_TypeDef)config[i].Trigger;
			ExtiSet.EXTI_LineCmd = ENABLE;
			EXTI_Init(&ExtiSet); 
		}
       		
	}
	
}
//==================================================================================================================

void Config_EXTICallback(ExitCallback call)
{
	Callback = call;
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
	{
		if(Callback != 0) Callback(Pin0);
		EXTI_ClearITPendingBit(EXTI_Line0);     
	}  
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) 
	{
		if(Callback != 0) Callback(Pin1);
		EXTI_ClearITPendingBit(EXTI_Line1);     
	} 
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) 
	{
		if(Callback != 0) Callback(Pin2);
		EXTI_ClearITPendingBit(EXTI_Line2);     
	} 
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) 
	{
		if(Callback != 0) Callback(Pin3);
		EXTI_ClearITPendingBit(EXTI_Line3);     
	} 
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
	{
		if(Callback != 0) Callback(Pin4);
		EXTI_ClearITPendingBit(EXTI_Line4);     
	} 
}

void EXTI9_5_IRQHandler(void)
{
	for(int i=5;i<=9;i++)
	{
		if(EXTI_GetITStatus(pinConfig[i].line) != RESET) 
		{
			if(Callback != 0) Callback((Pin_t)i);
			EXTI_ClearITPendingBit(pinConfig[i].line);     
		}  
	}
}

void EXTI15_10_IRQHandler(void)
{
	for(int i=10;i<=15;i++)
	{
		if(EXTI_GetITStatus(pinConfig[i].line) != RESET) 
		{
			if(Callback != 0) Callback((Pin_t)i);
			EXTI_ClearITPendingBit(pinConfig[i].line);     
		}  
	}
}


