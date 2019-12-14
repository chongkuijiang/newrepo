#ifndef __SYS_H
#define __SYS_H	


#include "stm32f10x.h"

// Use Configuration Wizard in Context Menu 


//  <o0> �ж����ȼ����飺 <0x700=> 0 bits-pre-emption,4 bits-subpriority  <0x600=> 1 bits-pre-emption,3 bits-subpriority
//     <0x500=> 2 bits-pre-emption,2 bits-subpriority  <0x400=> 3 bits-pre-emption,1 bits-subpriority  
//     <0x300=> 4 bits-pre-emption,0 bits-subpriority

#define  NVIC_Group    0x500  

//-------------------------------------------------------------
// <o0> ÿ��ʱ��������10-10000�� <10-10000>

#define   TICKS_SEC     10000

// end of configuration section 
//********************************************************************************  

#define CLI()      __set_PRIMASK(1)		/* �ر����ж� */  
#define SEI() __set_PRIMASK(0)				/* �������ж� */ 


void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_Group);	//����NVIC�жϷ���

}
//---------------------------------------------------
void initSystick(void)
{
    RCC_ClocksTypeDef RCC_ClockFreq;
    
    RCC_GetClocksFreq(&RCC_ClockFreq); 	  //��ȡRCC����Ϣ
    
    SysTick_Config(RCC_ClockFreq.HCLK_Frequency/TICKS_SEC); 	  //SYSTICK��Ƶ�Ƿ����
    
}

#endif

