#ifndef __SYS_H
#define __SYS_H	


#include "stm32f10x.h"

// Use Configuration Wizard in Context Menu 


//  <o0> 中断优先级分组： <0x700=> 0 bits-pre-emption,4 bits-subpriority  <0x600=> 1 bits-pre-emption,3 bits-subpriority
//     <0x500=> 2 bits-pre-emption,2 bits-subpriority  <0x400=> 3 bits-pre-emption,1 bits-subpriority  
//     <0x300=> 4 bits-pre-emption,0 bits-subpriority

#define  NVIC_Group    0x500  

//-------------------------------------------------------------
// <o0> 每秒时间粒度数10-10000： <10-10000>

#define   TICKS_SEC     10000

// end of configuration section 
//********************************************************************************  

#define CLI()      __set_PRIMASK(1)		/* 关闭总中断 */  
#define SEI() __set_PRIMASK(0)				/* 开放总中断 */ 


void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_Group);	//设置NVIC中断分组

}
//---------------------------------------------------
void initSystick(void)
{
    RCC_ClocksTypeDef RCC_ClockFreq;
    
    RCC_GetClocksFreq(&RCC_ClockFreq); 	  //获取RCC的信息
    
    SysTick_Config(RCC_ClockFreq.HCLK_Frequency/TICKS_SEC); 	  //SYSTICK分频是否完成
    
}

#endif

