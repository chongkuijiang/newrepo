#ifndef _EXIT_C
#define _EXIT_C

#include "stm32f10x_lib.h"
#include "Exit.h"



void EXTI_init(void)
{

  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_DeInit();//������EXTI�Ĵ�������Ϊȱʡֵ 
  EXTI_StructInit(&EXTI_InitStructure);//��EXTI_InitStruct�е�ÿһ��������ȱʡֵ���� 
  
  //////////////////////    PC6 �ⲿ�ж�device0   /////////////////////////////
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
  EXTI_ClearITPendingBit(EXTI_Line6);//���EXTI��·����λ
  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����ش����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Generate software interrupt: simulate a falling edge applied on EXTI line 6 */
  //EXTI_GenerateSWInterrupt(EXTI_Line6);
   
   //////////////////////    PC7 �ⲿ�ж�device0   ////////////////////////////// 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
  EXTI_ClearITPendingBit(EXTI_Line7);//���EXTI��·����λ
  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����ش����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Generate software interrupt: simulate a falling edge applied on EXTI line 7 */
  //EXTI_GenerateSWInterrupt(EXTI_Line7);
  
  ////////////////////    PC8 �ⲿ�ж�device0   ////////////////////////////// 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
  EXTI_ClearITPendingBit(EXTI_Line8);//���EXTI��·����λ
  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Generate software interrupt: simulate a falling edge applied on EXTI line 8 */
  //EXTI_GenerateSWInterrupt(EXTI_Line8);
//  
//   //////////////////////    PC9 �ⲿ�ж�device0   ////////////////////////////// 
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
//  EXTI_ClearITPendingBit(EXTI_Line9);//���EXTI��·����λ
//  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
//  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش����ж�
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  /* Generate software interrupt: simulate a falling edge applied on EXTI line 9 */
//  EXTI_GenerateSWInterrupt(EXTI_Line9);

}



#endif /* _EXIT_C */