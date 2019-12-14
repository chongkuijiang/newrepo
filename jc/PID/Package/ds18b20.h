#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <stm32f10x_lib.h>

#ifndef _DS18B20_LOCAL
#define DS18B20_EXTERN	extern
#else
#define DS18B20_EXTERN
#endif

#define DS18B20_BIT	        GPIO_Pin_1
#define DS18B20_PORT		GPIOA

#define READ_DS18B20()		GPIO_ReadInputDataBit(GPIOA, DS18B20_BIT)

#define	DIR_1WIRE_IN()		{DS18B20_PORT->CRL &= 0xFFFFFF0F;GPIOA->CRL |= 0x00000040;}
#define	DIR_1WIRE_OUT()		{DS18B20_PORT->CRL &= 0xFFFFFF0F;GPIOA->CRL |= 0x00000030;}
#define CLR_OP_1WIRE()		GPIO_ResetBits(DS18B20_PORT, DS18B20_BIT)
#define SET_OP_1WIRE()		GPIO_SetBits(DS18B20_PORT, DS18B20_BIT)
#define CHECK_IP_1WIRE()	GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_BIT)

extern unsigned char Ds18b20_readflag;
extern unsigned char Ds18b20_readcnt;

DS18B20_EXTERN void DS18B20_init(void);
DS18B20_EXTERN void DS18B20_Convert(void);
float DS18B20_Read(void);
DS18B20_EXTERN void init_1820();
DS18B20_EXTERN void delay_nus(u16 n);
float DS18B20_ReadTemperature(void);
void ds1820_delay_long(void);
#endif
