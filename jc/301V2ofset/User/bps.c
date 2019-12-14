#include "stm32f10x.h"
#include "userApp.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define AD_BUF_Address     ((uint32_t)&SYS.AD_Value)



//#define VECT_TAB_OFFSET  0x0
//void SystemInit (void)
//{
//  RCC_DeInit();		
//  RCC_HSEConfig(RCC_HSE_ON);	
//  if(RCC_WaitForHSEStartUp() == SUCCESS)	
//  {
//    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//    FLASH_SetLatency(FLASH_Latency_2);
//    RCC_HCLKConfig(RCC_SYSCLK_Div1);	//48M
//    RCC_PCLK2Config(RCC_HCLK_Div16);	//3M
//    RCC_PCLK1Config(RCC_HCLK_Div16);	//3M
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4); //48M
//    RCC_PLLCmd(ENABLE);							
//	  RCC_ADCCLKConfig(RCC_PCLK2_Div4); //0.75M
//    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//    while(RCC_GetSYSCLKSource()!= 0x08);			
//  }
//  else while(1);
//  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;	
//}
/*
void SystemInit (void)
{
  RCC_DeInit();		
	RCC_HSICmd(ENABLE);

    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);	
    RCC_PCLK2Config(RCC_HCLK_Div16);	
    RCC_PCLK1Config(RCC_HCLK_Div16);	
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12); //48M
    RCC_PLLCmd(ENABLE);							
	  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource()!= 0x08);	
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;	
}
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef portSet;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	portSet.GPIO_Speed=GPIO_Speed_2MHz; 
	portSet.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10;
	portSet.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&portSet);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	portSet.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	portSet.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	portSet.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&portSet);

	portSet.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_12;
	portSet.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_8;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_9;
	portSet.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&portSet);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	portSet.GPIO_Pin=GPIO_Pin_All;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&portSet);
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_Write(GPIOC,0x00);
	
}
void Timer2_Config(void)
{
	NVIC_InitTypeDef nvicInit;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseStructure.TIM_Period=20000;   //1M/20000 = 50Hz
	TIM_TimeBaseStructure.TIM_Prescaler=6-1;  //6M/6 = 1M
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2,DISABLE);
	TIM2->CR1|=0x0c;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	nvicInit.NVIC_IRQChannel=TIM2_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicInit);
}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_init;
	NVIC_InitTypeDef nvicInit;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	EXTI_ClearITPendingBit(EXTI_Line6);	
	EXTI_init.EXTI_Line=EXTI_Line6;
	EXTI_init.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_init.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_init.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_init);	 
    EXTI->EMR &= ~EXTI_Line6;
	
	nvicInit.NVIC_IRQChannel=EXTI9_5_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=1;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicInit);
}
void ADC_Configuration(void)
{
		ADC_InitTypeDef ADC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 3; 
    ADC_Init(ADC1, &ADC_InitStructure);
  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_71Cycles5);
       
    ADC_DMACmd(ADC1, ENABLE); 
	ADC_Cmd(ADC1, ENABLE);      
    ADC_ResetCalibration(ADC1); 
    while(ADC_GetResetCalibrationStatus(ADC1)); 
    ADC_StartCalibration(ADC1); 
    while(ADC_GetCalibrationStatus(ADC1));
	
	
}
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef nvicInit;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = AD_BUF_Address;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
    DMA_InitStructure.DMA_BufferSize = 3;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    nvicInit.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority=1;
	nvicInit.NVIC_IRQChannelSubPriority=1;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&nvicInit);
    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void BSP_Init(void)
{
	SystemCoreClockUpdate();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_Configuration();
	Timer2_Config();
	EXTI_Configuration();
	DMA_Configuration();
	ADC_Configuration();
	SysTick_Config(SystemCoreClock/10000);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
