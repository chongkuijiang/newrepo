
#ifndef __BSB_GPIO_H
#define __BSB_GPIO_H

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define  GPIO_SIZE(a)     (sizeof(a)/sizeof(a[0]))	 
	 
typedef enum
{
	AIN = GPIO_Mode_AIN,
	IN_FLOATING = GPIO_Mode_IN_FLOATING,
	IPD = GPIO_Mode_IPD,
	IPU = GPIO_Mode_IPU,
	Out_OD = GPIO_Mode_Out_OD,
	Out_PP = GPIO_Mode_Out_PP,
	AF_OD = GPIO_Mode_AF_OD,
	AF_PP = GPIO_Mode_AF_PP,
}GpioMode_t;

typedef enum
{
	None = 0,
    Rising = EXTI_Trigger_Rising,
    Falling = EXTI_Trigger_Falling,  
    Rising_Falling = EXTI_Trigger_Rising_Falling
}trigger_t;


//-------------------------------------------------------------------------

typedef enum
{
	PA=0,PB,PC,PD,PE,PF,PG,
}Port_t;

typedef enum
{
	Pin0=0,Pin1,Pin2,Pin3,Pin4,Pin5,Pin6,Pin7,Pin8,Pin9,Pin10,Pin11,Pin12,Pin13,Pin14,Pin15,
}Pin_t;
	 
typedef struct //port,pin,mode,outLevel,Trigger
{
	Port_t port;
	Pin_t  pin;
	GpioMode_t mode;
	BitAction  outLevel;
	trigger_t  Trigger;
}gpio_t;	



typedef void (*ExitCallback)(Pin_t pin);
//---------------------------------------------------------- 
void GPIO_Configuration(gpio_t* config,int count);
void Config_EXTICallback(ExitCallback call);

//----------------------------------------------------------
GPIO_TypeDef* getGpio(Port_t port);
uint16_t getPin(Pin_t pin);
void setHi(Port_t port,Pin_t pin);
void setLo(Port_t port,Pin_t pin);
void setToggle(Port_t port,Pin_t pin);

#ifdef __cplusplus
}
#endif

#endif


