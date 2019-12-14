#ifndef _ADC_C
#define _ADC_C

#include "iostm32f10xxE.h"
#include "stm32f10x_lib.h"
#include "Adc.h"
unsigned short sys_analog[10]; //存放ADC1-4 && ADC10-11转换的值

/*******************************************************************************
* Function Name  : void AD_Configration(void)                  
* Description    : ADC的设置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AD1_Configration(void)       
{
  ADC_InitTypeDef ADC_InitStructure; //定义一个ADC结构体
  ADC_DeInit(ADC1);       //将ADC设置为缺省值
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    //ADC1 工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;          //ADC工作在多通道模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //模数转换连续
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //定义ad转换启动方式，转换由软件启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               //adc数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 8;                              //顺序进行adc转换的通道的数目
  ADC_Init(ADC1, &ADC_InitStructure);                                  //初始化设置ADC
    //6路模拟输入
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5); //ADC1    
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 2, ADC_SampleTime_239Cycles5); //ADC1    
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3, ADC_SampleTime_239Cycles5); //ADC1    
   
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 4, ADC_SampleTime_239Cycles5); //ADC1    
   
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 5, ADC_SampleTime_239Cycles5); //ADC1     
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 6, ADC_SampleTime_239Cycles5); //ADC1   
 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 7, ADC_SampleTime_239Cycles5); //ADC1   
 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_239Cycles5); //ADC1    
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 9, ADC_SampleTime_239Cycles5); //ADC1   
 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 10, ADC_SampleTime_239Cycles5); //ADC1    
  
  
  ADC_DMACmd(ADC1,ENABLE);                     //使能指定的ADC的DMA请求                       
  ADC_Cmd(ADC1,ENABLE);                        //使能ADC1  
  
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换
}


void DMA1_Configration(void)              
{
    DMA_InitTypeDef DMA_InitStructure;
    
    DMA_DeInit(DMA1_Channel1);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)(&ADC1_DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)(&sys_analog);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*******************************************************************************
* Function Name  : void AD_slove(void)                 
* Description    : ADC的值处理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short AD_slove(void)  
{
  unsigned short paixu[10],y;
  unsigned char i,j;
  unsigned short AD1;
  
  for(i=0;i<10;i++)          //给排序数组赋值
  {
      paixu[i]=sys_analog[i];   //+8*(band-1)
  } 
  //冒泡法排序
  for(i=0;i<9;i++)
     {
       for(j=i+1;j<10;j++)
          {
             if(paixu[i]<paixu[j])
                 {                     //交换数据
                      y=paixu[j];
                      paixu[j]=paixu[i];
                      paixu[i]=y;
 
                 }   
           }
     }
//求和
     y=0;
  for(i=1;i<9;i++)
     {
       y+= paixu[i];
     }
  
  AD1=y/8;  //求平均值
  
  return AD1;
}

unsigned char Get_Power(unsigned short Ad)  
{ 
  double temp;
  unsigned char vlotage;
  temp=((double)Ad)/4095*3.3;//计算电压值      
  vlotage=(unsigned char)(temp*25);             //计算实电压值 0.1V
  
  return vlotage;
}

#endif /* _ADC_C */