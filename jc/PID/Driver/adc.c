#ifndef _ADC_C
#define _ADC_C

#include "iostm32f10xxE.h"
#include "stm32f10x_lib.h"
#include "Adc.h"
unsigned short sys_analog[10]; //���ADC1-4 && ADC10-11ת����ֵ

/*******************************************************************************
* Function Name  : void AD_Configration(void)                  
* Description    : ADC������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AD1_Configration(void)       
{
  ADC_InitTypeDef ADC_InitStructure; //����һ��ADC�ṹ��
  ADC_DeInit(ADC1);       //��ADC����Ϊȱʡֵ
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    //ADC1 �����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;          //ADC�����ڶ�ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    //ģ��ת������
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //����adת��������ʽ��ת�����������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               //adc�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 8;                              //˳�����adcת����ͨ������Ŀ
  ADC_Init(ADC1, &ADC_InitStructure);                                  //��ʼ������ADC
    //6·ģ������
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
  
  
  ADC_DMACmd(ADC1,ENABLE);                     //ʹ��ָ����ADC��DMA����                       
  ADC_Cmd(ADC1,ENABLE);                        //ʹ��ADC1  
  
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��
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
* Description    : ADC��ֵ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short AD_slove(void)  
{
  unsigned short paixu[10],y;
  unsigned char i,j;
  unsigned short AD1;
  
  for(i=0;i<10;i++)          //���������鸳ֵ
  {
      paixu[i]=sys_analog[i];   //+8*(band-1)
  } 
  //ð�ݷ�����
  for(i=0;i<9;i++)
     {
       for(j=i+1;j<10;j++)
          {
             if(paixu[i]<paixu[j])
                 {                     //��������
                      y=paixu[j];
                      paixu[j]=paixu[i];
                      paixu[i]=y;
 
                 }   
           }
     }
//���
     y=0;
  for(i=1;i<9;i++)
     {
       y+= paixu[i];
     }
  
  AD1=y/8;  //��ƽ��ֵ
  
  return AD1;
}

unsigned char Get_Power(unsigned short Ad)  
{ 
  double temp;
  unsigned char vlotage;
  temp=((double)Ad)/4095*3.3;//�����ѹֵ      
  vlotage=(unsigned char)(temp*25);             //����ʵ��ѹֵ 0.1V
  
  return vlotage;
}

#endif /* _ADC_C */