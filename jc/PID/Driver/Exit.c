#ifndef _EXIT_C
#define _EXIT_C

#include "stm32f10x_lib.h"
#include "Exit.h"



void EXTI_init(void)
{

  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_DeInit();//将外设EXTI寄存器重设为缺省值 
  EXTI_StructInit(&EXTI_InitStructure);//把EXTI_InitStruct中的每一个参数按缺省值填入 
  
  //////////////////////    PC6 外部中断device0   /////////////////////////////
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
  EXTI_ClearITPendingBit(EXTI_Line6);//清除EXTI线路挂起位
  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上升沿触发中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Generate software interrupt: simulate a falling edge applied on EXTI line 6 */
  //EXTI_GenerateSWInterrupt(EXTI_Line6);
   
   //////////////////////    PC7 外部中断device0   ////////////////////////////// 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
  EXTI_ClearITPendingBit(EXTI_Line7);//清除EXTI线路挂起位
  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上升沿触发中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Generate software interrupt: simulate a falling edge applied on EXTI line 7 */
  //EXTI_GenerateSWInterrupt(EXTI_Line7);
  
  ////////////////////    PC8 外部中断device0   ////////////////////////////// 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
  EXTI_ClearITPendingBit(EXTI_Line8);//清除EXTI线路挂起位
  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Generate software interrupt: simulate a falling edge applied on EXTI line 8 */
  //EXTI_GenerateSWInterrupt(EXTI_Line8);
//  
//   //////////////////////    PC9 外部中断device0   ////////////////////////////// 
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
//  EXTI_ClearITPendingBit(EXTI_Line9);//清除EXTI线路挂起位
//  /* Configure EXTI Line3 to generate an interrupt on falling edge */  
//  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发中断
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  /* Generate software interrupt: simulate a falling edge applied on EXTI line 9 */
//  EXTI_GenerateSWInterrupt(EXTI_Line9);

}



#endif /* _EXIT_C */