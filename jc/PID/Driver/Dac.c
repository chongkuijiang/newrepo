/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DAC_C
#define _DAC_C
#include "stm32f10x_lib.h"
#include "Dac.h"

void DAC_configration(void)
{
  DAC_InitTypeDef            DAC_InitStructure;

    /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;    //软件触发
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //使能DAC通道缓存
 
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
 

 /* Enable DAC Channel: Once the DAC channel1 is enabled, PA.04 is 
  automatically connected to the DAC converter. */
      
  DAC_Cmd(DAC_Channel_1, ENABLE);
  Dac_Out(596);
}

//40-->0.8v 237-->4.6v 
//596-->0.8v 3426-->4.6v 
void Dac_Out(unsigned int data)
{
  //float value=(float)data*5/5.5;
  //data=(u8)(value);
  //DAC_SetChannel1Data(DAC_Align_8b_R,data);
  DAC_SetChannel1Data(DAC_Align_12b_R,data);
  DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}


#endif /* _Dac_C */