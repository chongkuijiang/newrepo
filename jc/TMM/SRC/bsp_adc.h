#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

// 转换通道个数
#define    macNOFCHANEL				8
#define    AVGNUMBER            500

extern __IO uint16_t ADC_ConvertedValue[macNOFCHANEL];
extern __IO uint16_t ADC_Value[macNOFCHANEL];
extern __IO uint32_t ADC_SumValue[macNOFCHANEL];

/**************************函数声明********************************/
void ADCx_Init(void);
//void AdcProc(void);

#endif 

