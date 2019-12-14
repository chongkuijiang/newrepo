#ifndef _SYSINIT_C
#define _SYSINIT_C
#include "stm32f10x_lib.h"
#include "SysInit.h"
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void sys_config(void)
{
    
   RCC_DeInit();  //将外设RCC寄存器重设为缺省值
   
   RCC_HSEConfig(RCC_HSE_ON);  //外部高速晶振
   if(RCC_WaitForHSEStartUp() == SUCCESS)   //HSE晶振起振
   {
     //FLASH时序控制
     //推荐值：SYSCLK=0~24MHz  Latency=0
     //SYSCLK=24~48MHz  Latency=1
     //SYSCLK=48~72MHz  Latency=2
      FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //预取指缓存使能
      FLASH_SetLatency(FLASH_Latency_2);
      RCC_HCLKConfig(RCC_SYSCLK_Div1);   //AHB(HCLK)时钟=系统时钟
      RCC_PCLK2Config(RCC_HCLK_Div1);    //高速APB(APB2)时钟= HCLK/1 
      RCC_PCLK1Config(RCC_HCLK_Div2);    //低速APB(APB1)时钟= HCLK/2 
      RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);  //PLLCLK= 8MHz * 9=72MHz   设置PLL时钟源及倍频系数
      RCC_PLLCmd(ENABLE);  //使能PLL
      while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  //等待PLL初始化成功
      
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    //选择PLL作为系统时钟
      
      while(RCC_GetSYSCLKSource() != 0x08);    //等待PLL成功用作于系统时钟的时钟源
   }
   else  // 如果晶振没有正常起振则输出错误
   {
   }
  
  //使能或者失能 APB2 外设时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
//  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.

*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                                                                              
 //USART1_TX 
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  //USART1_RX 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  //ADC  PA1
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //OUT： Y1：PA5 Y2：PA6 Y3：PA7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  //OUT： Y10：PC0 Y11：PC1 Y12：PC2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  //OUT： Y17：PD2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //PWM0:Y0 电机机控制
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;  //推挽输出 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  //EXIT ：X0:PC6 X1:PC7 //IN2:PC8 IN3:PC9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;// | GPIO_Pin_9 ;//PC6，PC7,PC8，PC9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //SPI ：PB12:NSS  PB13:SCLK    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
   //SPI ：PB14:MISO   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //DAC ：PA4  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4  ;//PA4
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQChannel; //定时器
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //从占
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
//  
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQChannel ; //外部中断线15-10中断
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //从占
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQChannel; //外部中断线15-10中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //从占
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configures SysTick
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//SysTick设置
void SysTick_Config(void)
{
    /* Disable SysTick Counter */
    SysTick_CounterCmd(SysTick_Counter_Disable);
  
    /* Disable the SysTick Interrupt */
    SysTick_ITConfig(DISABLE);
  
    /* Configure HCLK clock as SysTick clock source */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

    /* SysTick interrupt each 1000 Hz with HCLK equal to 72MHz */
    SysTick_SetReload(9000);
    
    /* Enable the SysTick Interrupt */
    SysTick_ITConfig(ENABLE);

    /* Enable the SysTick Counter */
    SysTick_CounterCmd(SysTick_Counter_Enable);
}


#endif /* _SYSINIT_C */