#include "Example.h"
void RCC_Configuration(void)
{
  RCC_DeInit();		
  RCC_HSEConfig(RCC_HSE_ON);	
  if(RCC_WaitForHSEStartUp() == SUCCESS)	
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);	
    RCC_PCLK2Config(RCC_HCLK_Div16);	
    RCC_PCLK1Config(RCC_HCLK_Div16);	
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4); 
    RCC_PLLCmd(ENABLE);							
	  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource()!= 0x08);			
  }
  else while(1);
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef portSet;
	portSet.GPIO_Speed=GPIO_Speed_2MHz; 
	portSet.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10;
	portSet.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&portSet);

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
	
	portSet.GPIO_Pin=GPIO_Pin_All;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&portSet);
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_Write(GPIOC,0x80ff);
	
}
void Timer2_Config(void)
{
	NVIC_InitTypeDef nvicInit;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	nvicInit.NVIC_IRQChannel=TIM2_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	TIM_TimeBaseStructure.TIM_Period=20000;
	TIM_TimeBaseStructure.TIM_Prescaler=6-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	NVIC_Init(&nvicInit);
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2,DISABLE);
	TIM2->CR1|=0x0c;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

void SysTick_Config(void)
{
    SysTick_SetReload(600);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_init;
	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel=EXTI9_5_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=1;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicInit);

	EXTI_ClearITPendingBit(EXTI_Line6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	EXTI_init.EXTI_Line=EXTI_Line6;
	EXTI_init.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_init.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_init.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_init);	 
  EXTI->EMR &= ~EXTI_Line6;
}
void ADC_Configuration(void)
{
		ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 3; 
    ADC_Init(ADC1, &ADC_InitStructure);
  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_71Cycles5);//301
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_71Cycles5);
   
    ADC_Cmd(ADC1, ENABLE); 
    ADC_DMACmd(ADC1, ENABLE);   
    ADC_ResetCalibration(ADC1); 
    while(ADC_GetResetCalibrationStatus(ADC1)); 
    ADC_StartCalibration(ADC1); 
    while(ADC_GetCalibrationStatus(ADC1));
}
void DMA_Configuration(u32 AD_Value)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef nvicInit;
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)0x4001244C;
    DMA_InitStructure.DMA_MemoryBaseAddr = AD_Value;
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
    
    nvicInit.NVIC_IRQChannel=DMA1_Channel1_IRQChannel;
		nvicInit.NVIC_IRQChannelPreemptionPriority=1;
		nvicInit.NVIC_IRQChannelSubPriority=1;
		nvicInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&nvicInit);
    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}


void StartUp103x(u32 addNum)
{
	RCC_Configuration();
	RCC_LSEConfig(RCC_LSE_OFF);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL,ENABLE);
	GPIO_Configuration();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	Timer2_Config();
	EXTI_Configuration();
	SysTick_Config();
	DMA_Configuration(addNum);
	ADC_Configuration();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
