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
//        TIM_TimeBaseStructure.TIM_Period = 1000 ;//1000������10ms 
//        TIM_TimeBaseStructure.TIM_Prescaler = 0x2d0;//0.01ms
//        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  
//		
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //PWMģʽ1
//        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //����ͨ����Ч  PA8 
//        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;  
//        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //��ʼ�ߵ�ƽ
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
  TIM_TimeBaseStructure.TIM_Period = 1000;//���ü��������С��ÿ��200�����Ͳ���һ�������¼�
  TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //Ԥ��Ƶϵ��Ϊ7200-1������������ʱ��Ϊ72MHz/7200 = 10kHz 0.1ms
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
  
  //TIM5ͨ����ʼ��
  //�������жϱ�־
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
  //��ֹARRԤװ�ػ�����
  //TIM_ARRPreloadConfig(TIM5, DISABLE);  //Ԥװ�ؼĴ��������ݱ��������͵�Ӱ�ӼĴ��� 
  //����TIM5���ж�
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);    
}
////�������
void PWM_TIM2_Configuration(void)
{
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
       TIM_OCInitTypeDef  TIM_OCInitStructure;
 
        /* Time base configuration */
//       TIM_TimeBaseStructure.TIM_Period = 20000;//0-20000����20000�����Զ���װ�ؼĴ������ڵ�ֵ  ����PWM����������=20ms 
//       TIM_TimeBaseStructure.TIM_Prescaler =0x48;//   72M/72=0.001M,  ʱ���׼Ϊ0.001ms
//       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_Period = 1000 ;//1000������10ms 
        TIM_TimeBaseStructure.TIM_Prescaler = 0x2d0;//0.01ms
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	  
	   //TIM2ͨ����ʼ��
       TIM_OCStructInit(& TIM_OCInitStructure);                               //Ĭ�ϲ���
       TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //����״̬
       TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //�趨Ϊ�������ҪPWM�������Ҫ���д���
       TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;            //�ߵ�ƽ
       TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
       
      
       TIM_OCInitStructure.TIM_Pulse = 0;                                 //ռ�ճ���
       TIM_OC4Init(TIM2, &TIM_OCInitStructure);                             //ͨ����ʼ��
     
       TIM_Cmd(TIM2, ENABLE);                                              //����TIM2
}
//
//�������
//void PWM_TIM2_Configuration(void)
//{
//       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//       TIM_OCInitTypeDef  TIM_OCInitStructure;
// 
//        /* Time base configuration */
//       TIM_TimeBaseStructure.TIM_Period = 20000;//0-20000����20000�����Զ���װ�ؼĴ������ڵ�ֵ  ����PWM����������=20ms 
//       TIM_TimeBaseStructure.TIM_Prescaler =0x48;//   72M/72=0.001M,  ʱ���׼Ϊ0.001ms
//       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//       TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
//	  
//	   //TIM2ͨ����ʼ��
//       TIM_OCStructInit(& TIM_OCInitStructure);                               //Ĭ�ϲ���
//       TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                     //����״̬
//       TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;         //�趨Ϊ�������ҪPWM�������Ҫ���д���
//       TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;            //�͵�ƽ
//       TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//       
// 
//       TIM_OCInitStructure.TIM_Pulse = 500;                                 //ռ�ճ���
//       TIM_OC2Init(TIM2, &TIM_OCInitStructure);                             //ͨ����ʼ��
//       
//       TIM_OCInitStructure.TIM_Pulse = 500;                                 //ռ�ճ���
//       TIM_OC3Init(TIM2, &TIM_OCInitStructure);                             //ͨ����ʼ��
//   
//       TIM_Cmd(TIM2, ENABLE);                                              //����TIM2
//  
//}

#endif /* _TIME_C */