/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIME_C
#define _TIME_C

#include "stm32f10x_lib.h"
#include "Time.h"
//
//void PWM_TIM1_Configuration(void)
//{
//       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//       TIM_OCInitTypeDef  TIM_OCInitStructure;
// 
//        /* Time base configuration */
//       /* Time base configuration */
//        TIM_TimeBaseStructure.TIM_Period = 1000 ;//1000个，即10ms 
//        TIM_TimeBaseStructure.TIM_Prescaler = 0x2d0;//0.01ms
//        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  
//		
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWM模式1
//        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //正向通道有效  PA8 
//        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;  
//        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //初始高电平
//        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
//        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
//              
//        TIM_OCInitStructure.TIM_Pulse = 500;;
//        TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//        
//        TIM_OCInitStructure.TIM_Pulse =500;;
//        TIM_OC4Init(TIM1, &TIM_OCInitStructure);	
//        // TIM1 counter enable 
//        TIM_Cmd(TIM1, ENABLE);
//        // Main Output Enable 
//        TIM_CtrlPWMOutputs(TIM1, ENABLE);
//	
//}

void TIM5_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000;//设置计数溢出大小，每计200个数就产生一个更新事件
  TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //预分频系数为7200-1，这样计数器时钟为72MHz/7200 = 10kHz 0.1ms
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
  
  //TIM5通道初始化
  //清除溢出中断标志
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
  //禁止ARR预装载缓冲器
  //TIM_ARRPreloadConfig(TIM5, DISABLE);  //预装载寄存器的内容被立即传送到影子寄存器 
  //开启TIM5的中断
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);    
}
////电机控制
void PWM_TIM2_Configuration(void)
{
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
       TIM_OCInitTypeDef  TIM_OCInitStructure;
 
        /* Time base configuration */
//       TIM_TimeBaseStructure.TIM_Period = 20000;//0-20000，即20000个，自动重装载寄存器周期的值  即是PWM方波的周期=20ms 
//       TIM_TimeBaseStructure.TIM_Prescaler =0x48;//   72M/72=0.001M,  时间基准为0.001ms
//       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_Period = 1000 ;//1000个，即10ms 
        TIM_TimeBaseStructure.TIM_Prescaler = 0x2d0;//0.01ms
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	  
	   //TIM2通道初始化
       TIM_OCStructInit(& TIM_OCInitStructure);                               //默认参数
       TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //工作状态
       TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //设定为输出，需要PWM输出才需要这行代码
       TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;            //高电平
       TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
       
      
       TIM_OCInitStructure.TIM_Pulse = 0;                                 //占空长度
       TIM_OC4Init(TIM2, &TIM_OCInitStructure);                             //通道初始化
     
       TIM_Cmd(TIM2, ENABLE);                                              //启动TIM2
}
//
//舵机控制
//void PWM_TIM2_Configuration(void)
//{
//       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//       TIM_OCInitTypeDef  TIM_OCInitStructure;
// 
//        /* Time base configuration */
//       TIM_TimeBaseStructure.TIM_Period = 20000;//0-20000，即20000个，自动重装载寄存器周期的值  即是PWM方波的周期=20ms 
//       TIM_TimeBaseStructure.TIM_Prescaler =0x48;//   72M/72=0.001M,  时间基准为0.001ms
//       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//       TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
//	  
//	   //TIM2通道初始化
//       TIM_OCStructInit(& TIM_OCInitStructure);                               //默认参数
//       TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //工作状态
//       TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //设定为输出，需要PWM输出才需要这行代码
//       TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;            //低电平
//       TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//       
// 
//       TIM_OCInitStructure.TIM_Pulse = 500;                                 //占空长度
//       TIM_OC2Init(TIM2, &TIM_OCInitStructure);                             //通道初始化
//       
//       TIM_OCInitStructure.TIM_Pulse = 500;                                 //占空长度
//       TIM_OC3Init(TIM2, &TIM_OCInitStructure);                             //通道初始化
//   
//       TIM_Cmd(TIM2, ENABLE);                                              //启动TIM2
//  
//}

#endif /* _TIME_C */