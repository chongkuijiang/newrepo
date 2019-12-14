#define USER_Globals
#include <stm32f10x_lib.h>

void CheckD1(void)
{
    u16 i=0;
	s32 dataBuff32s=0;
	s16 data[3]={0,0,0};
    if((CHdata1.UpOrDownToSV_Flag>=3)||(CHdata1.DivNumPV>0))//Save data is 21,average data is 20
	{
		for(i=CHdata1.DivNumPV;i>0;i--) CHdata1.TemBakPV[i]=CHdata1.TemBakPV[i-1];
		if(CHdata1.UpOrDownToSV_Flag>=3) 
		{
			if(CHdata1.DivNumPV<10) CHdata1.DivNumPV++;
			dataBuff32s=CHdata1.DataSet[Tem_SV]*10;
			data[0]=CHdata1.DataGet[Tem_sensor]-dataBuff32s;
			if(CHdata1.DivNumPV==10) 
			{
				if(data[0]>50) {data[2]=data[0]-50;data[0]=50;}
				else if(data[0]<-50) {data[2]=data[0]+50;data[0]=-50;}
				else data[2]=0;
                if(data[0]>=9) {data[1]=data[0]-9;data[0]=9;}
                else if(data[0]<=-9) {data[1]=data[0]+9;data[0]=-9;}
                else data[1]=0;
				CHdata1.TemBakPV[0]=dataBuff32s+data[0]/3+data[1]/2+data[2];
            }
			else CHdata1.TemBakPV[0]=data[0]/2+dataBuff32s;
			dataBuff32s=0;
			for(i=0;i<CHdata1.DivNumPV;i++) dataBuff32s+=CHdata1.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata1.DivNumPV;
		}
		else if(CHdata1.DivNumPV>0)
		{
			CHdata1.TemBakPV[0]=CHdata1.DataGet[Tem_sensor];
			dataBuff32s=0;
			for(i=0;i<CHdata1.DivNumPV;i++) dataBuff32s+=CHdata1.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata1.DivNumPV;
			CHdata1.DivNumPV--;
		}
		if(CHdata1.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata1.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				if(CHdata1.ErrorCode<ALARM_H) CHdata1.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata1.ErrorCode==TC_OP) CHdata1.ErrorCode=CHdata1.RunningMode;
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				if(CHdata1.ErrorCode<TC_OP) CHdata1.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata1.ErrorCode==TC_OP) CHdata1.ErrorCode=CHdata1.RunningMode;
		}
		if(dataBuff32s>0) CHdata1.DataGet[Tem_PV]=dataBuff32s;
		else CHdata1.DataGet[Tem_PV]=0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckD2(void)
{
    u16 i=0;
	s32 dataBuff32s=0;
	s16 data[3]={0,0,0};
    if((CHdata2.UpOrDownToSV_Flag>=3)||(CHdata2.DivNumPV>0))//Save data is 21,average data is 20
	{
		for(i=CHdata2.DivNumPV;i>0;i--) CHdata2.TemBakPV[i]=CHdata2.TemBakPV[i-1];
		if(CHdata2.UpOrDownToSV_Flag>=3) 
		{
			if(CHdata2.DivNumPV<10) CHdata2.DivNumPV++;
			dataBuff32s=CHdata2.DataSet[Tem_SV]*10;
			data[0]=CHdata2.DataGet[Tem_sensor]-dataBuff32s;
			if(CHdata2.DivNumPV==10) 
			{
				if(data[0]>50) {data[2]=data[0]-50;data[0]=50;}
				else if(data[0]<-50) {data[2]=data[0]+50;data[0]=-50;}
				else data[2]=0;
                if(data[0]>=9) {data[1]=data[0]-9;data[0]=9;}
                else if(data[0]<=-9) {data[1]=data[0]+9;data[0]=-9;}
                else data[1]=0;
				CHdata2.TemBakPV[0]=dataBuff32s+data[0]/3+data[1]/2+data[2];
            }
			else CHdata2.TemBakPV[0]=data[0]/2+dataBuff32s;
			dataBuff32s=0;
			for(i=0;i<CHdata2.DivNumPV;i++) dataBuff32s+=CHdata2.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata2.DivNumPV;
		}
		else if(CHdata2.DivNumPV>0)
		{
			CHdata2.TemBakPV[0]=CHdata2.DataGet[Tem_sensor];
			dataBuff32s=0;
			for(i=0;i<CHdata2.DivNumPV;i++) dataBuff32s+=CHdata2.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata2.DivNumPV;
			CHdata2.DivNumPV--;
		}
		if(CHdata2.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata2.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				if(CHdata2.ErrorCode<TC_OP) CHdata2.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata2.ErrorCode==TC_OP) CHdata2.ErrorCode=CHdata2.RunningMode;
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				if(CHdata2.ErrorCode<TC_OP) CHdata2.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata2.ErrorCode==TC_OP) CHdata2.ErrorCode=CHdata2.RunningMode;
		}
		if(dataBuff32s>0) CHdata2.DataGet[Tem_PV]=dataBuff32s;
		else CHdata2.DataGet[Tem_PV]=0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckD3(void)
{
    u16 i=0;
	s32 dataBuff32s=0;
	s16 data[3]={0,0,0};
    if((CHdata3.UpOrDownToSV_Flag>=3)||(CHdata3.DivNumPV>0))//Save data is 21,average data is 20
	{
		for(i=CHdata3.DivNumPV;i>0;i--) CHdata3.TemBakPV[i]=CHdata3.TemBakPV[i-1];
		if(CHdata3.UpOrDownToSV_Flag>=3) 
		{
			if(CHdata3.DivNumPV<10) CHdata3.DivNumPV++;
			dataBuff32s=CHdata3.DataSet[Tem_SV]*10;
			data[0]=CHdata3.DataGet[Tem_sensor]-dataBuff32s;
			if(CHdata3.DivNumPV==10) 
			{
				if(data[0]>50) {data[2]=data[0]-50;data[0]=50;}
				else if(data[0]<-50) {data[2]=data[0]+50;data[0]=-50;}
				else data[2]=0;
                if(data[0]>=9) {data[1]=data[0]-9;data[0]=9;}
                else if(data[0]<=-9) {data[1]=data[0]+9;data[0]=-9;}
                else data[1]=0;
				CHdata3.TemBakPV[0]=dataBuff32s+data[0]/3+data[1]/2+data[2];
            }
			else CHdata3.TemBakPV[0]=data[0]/2+dataBuff32s;
			dataBuff32s=0;
			for(i=0;i<CHdata3.DivNumPV;i++) dataBuff32s+=CHdata3.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata3.DivNumPV;
		}
		else if(CHdata3.DivNumPV>0)
		{
			CHdata3.TemBakPV[0]=CHdata3.DataGet[Tem_sensor];
			dataBuff32s=0;
			for(i=0;i<CHdata3.DivNumPV;i++) dataBuff32s+=CHdata3.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata3.DivNumPV;
			CHdata3.DivNumPV--;
		}
		if(CHdata3.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata3.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				if(CHdata3.ErrorCode<TC_OP) CHdata3.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata3.ErrorCode==TC_OP) CHdata3.ErrorCode=CHdata3.RunningMode;
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				if(CHdata3.ErrorCode<TC_OP) CHdata3.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata3.ErrorCode==TC_OP) CHdata3.ErrorCode=CHdata3.RunningMode;
		}
		if(dataBuff32s>0) CHdata3.DataGet[Tem_PV]=dataBuff32s;
		else CHdata3.DataGet[Tem_PV]=0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckD4(void)
{
    u16 i=0;
	s32 dataBuff32s=0;
	s16 data[3]={0,0,0};
    if((CHdata4.UpOrDownToSV_Flag>=3)||(CHdata4.DivNumPV>0))//Save data is 21,average data is 20
	{
		for(i=CHdata4.DivNumPV;i>0;i--) CHdata4.TemBakPV[i]=CHdata4.TemBakPV[i-1];
		if(CHdata4.UpOrDownToSV_Flag>=3) 
		{
			if(CHdata4.DivNumPV<10) CHdata4.DivNumPV++;
            dataBuff32s=CHdata4.DataSet[Tem_SV]*10;
			data[0]=CHdata4.DataGet[Tem_sensor]-dataBuff32s;
			if(CHdata4.DivNumPV==10) 
			{
				if(data[0]>50) {data[2]=data[0]-50;data[0]=50;}
				else if(data[0]<-50) {data[2]=data[0]+50;data[0]=-50;}
				else data[2]=0;
                if(data[0]>=9) {data[1]=data[0]-9;data[0]=9;}
                else if(data[0]<=-9) {data[1]=data[0]+9;data[0]=-9;}
                else data[1]=0;
				CHdata4.TemBakPV[0]=dataBuff32s+data[0]/3+data[1]/2+data[2];
            }
			else CHdata4.TemBakPV[0]=data[0]/2+dataBuff32s;
			dataBuff32s=0;
			for(i=0;i<CHdata4.DivNumPV;i++) dataBuff32s+=CHdata4.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata4.DivNumPV;
		}
		else if(CHdata4.DivNumPV>0)
		{
			CHdata4.TemBakPV[0]=CHdata4.DataGet[Tem_sensor];
			dataBuff32s=0;
			for(i=0;i<CHdata4.DivNumPV;i++) dataBuff32s+=CHdata4.TemBakPV[i];
			dataBuff32s=dataBuff32s/CHdata4.DivNumPV;
			CHdata4.DivNumPV--;
		}
		if(CHdata4.DataSet[C_F]) dataBuff32s=9*dataBuff32s/5+320;
		if(CHdata4.DataSet[C_F]==0)
		{
			if(dataBuff32s>5000) 
			{
				dataBuff32s=0;
				if(CHdata4.ErrorCode<TC_OP) CHdata4.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata4.ErrorCode==TC_OP) CHdata4.ErrorCode=CHdata4.RunningMode;
		}
		else 
		{
			if(dataBuff32s>9320) 
			{
				dataBuff32s=0;
				if(CHdata4.ErrorCode<TC_OP) CHdata4.ErrorCode=TC_OP;//TC open
			}
			else if(CHdata4.ErrorCode==TC_OP) CHdata4.ErrorCode=CHdata4.RunningMode;
		}
		if(dataBuff32s>0) CHdata4.DataGet[Tem_PV]=dataBuff32s;
		else CHdata4.DataGet[Tem_PV]=0;
    }
}
void RCC_Configuration(void)
{
  RCC_DeInit();											
  RCC_HSEConfig(RCC_HSE_ON);							 
  if(RCC_WaitForHSEStartUp() == SUCCESS)				
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);					 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);					
    RCC_PCLK2Config(RCC_HCLK_Div16);						
    RCC_PCLK1Config(RCC_HCLK_Div16);						
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4); 
    RCC_PLLCmd(ENABLE);									
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);			
    while(RCC_GetSYSCLKSource()!= 0x08);				
  }
  else while(1);	
	RCC_LSEConfig(RCC_LSE_OFF);
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef portSet;
  portSet.GPIO_Speed=GPIO_Speed_2MHz;
	portSet.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	portSet.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_10;
	portSet.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_9;
  portSet.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  portSet.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11;
	portSet.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	portSet.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_15;
  portSet.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_0;
	portSet.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	portSet.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&portSet);
  portSet.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  portSet.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&portSet);
	portSet.GPIO_Pin=GPIO_Pin_2;
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&portSet);
  GPIO_SetBits(GPIOA,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}
void USART_Configuration(void)
{	
	NVIC_InitTypeDef nvicInit;
	USART_InitTypeDef USART_init;
	nvicInit.NVIC_IRQChannel=USART1_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	USART_init.USART_BaudRate=38400;
	USART_init.USART_WordLength=USART_WordLength_8b;
	USART_init.USART_StopBits=USART_StopBits_1;
	USART_init.USART_Parity=USART_Parity_No;
	USART_init.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	NVIC_Init(&nvicInit);
	USART_Init(USART1,&USART_init);
	USART_Cmd(USART1,ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
void Timer1_Config(void)
{
	NVIC_InitTypeDef nvicInit;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	nvicInit.NVIC_IRQChannel=TIM1_UP_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&nvicInit);
	TIM_TimeBaseStructure.TIM_Period=20000;
    TIM_TimeBaseStructure.TIM_Prescaler=6-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM1,DISABLE);
	TIM1->CR1|=0x0c;
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
}
void KeyTurnS(UCHAR i)
{
    if(i) KeUr|=0x0008;
		else KeUr&=0xfff7;
}
void KeyTurnX(UCHAR i)
{
    if(i) KeUr|=0x0004;
		else KeUr&=0xfffB;
}
void Timer2_Config(void)
{
	NVIC_InitTypeDef nvicInit;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	nvicInit.NVIC_IRQChannel=TIM2_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	TIM_TimeBaseStructure.TIM_Period=20000;
	TIM_TimeBaseStructure.TIM_Prescaler=6-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	NVIC_Init(&nvicInit);
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2,DISABLE);
	TIM2->CR1|=0x0c;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}
void Timer3_Config(void)
{
	NVIC_InitTypeDef nvicInit;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	nvicInit.NVIC_IRQChannel=TIM3_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	TIM_TimeBaseStructure.TIM_Period=20000;
	TIM_TimeBaseStructure.TIM_Prescaler=6-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	NVIC_Init(&nvicInit);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM3,DISABLE);
	TIM3->CR1|=0x0c;
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
}
void CCdataValue(unsigned char *data, unsigned int data1)
{
  	unsigned char i ;      /* will index into CRC lookup table */
	SYS1.Cata1=0xff;   /* High byte of CRC initialized */
  	SYS1.Cata2=0xff;   /* Low byte of CRC initialized */
  	while (data1--)               /* pass through message buffer */
  	{
		i = SYS1.Cata1 ^ *data++;    /* calculate the CRC */
     	SYS1.Cata1 = SYS1.Cata2 ^ asci1[i];
     	SYS1.Cata2 = asci2[i];
  	}
}
void Tim_data(UCHAR i)
{
    SYS1.CNt1=i; 
    SYS1.Cata3=SYS1.Cata7;
    OSTimeDly(5);  
    USART_SendData(USART1, *SYS1.Cata3++);
		KeyTurnS(20);
}
/////////////////////////////////////////////////////////////////////////////////////////
void Timcomm(u16 *data1,UCHAR data2,UCHAR data3,UINT data4,UCHAR data5)
{
    u8 n=0;
	SYS1.Cata7[0]=data2;
	SYS1.Cata7[1]=data3;
    if(data3==6)
    {
        SYS1.Cata7[2]=0x00;
        SYS1.Cata7[3]=data4;
		data4++;
        SYS1.Cata7[4]=(u8)(data1[data4]>>8);
        SYS1.Cata7[5]=(u8)data1[data4];
        CCdataValue(&SYS1.Cata7[0],6);
        SYS1.Cata7[6]=SYS1.Cata1;
        SYS1.Cata7[7]=SYS1.Cata2;
        Tim_data(8);
    }
    else
    {
        SYS1.Cata7[2]=data5<<1;	
        while(data5--)
        {
            SYS1.Cata7[3+n++]=(u8)(data1[data4]>>8);
            SYS1.Cata7[3+n++]=(u8)data1[data4++];
        }
	    CCdataValue(&SYS1.Cata7[0],3+SYS1.Cata7[2]);
	    SYS1.Cata7[3+n++]=SYS1.Cata1;
	    SYS1.Cata7[3+n]=SYS1.Cata2;
	    Tim_data(SYS1.Cata7[2]+5);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer4_Config(void)
{
	NVIC_InitTypeDef nvicInit;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	nvicInit.NVIC_IRQChannel=TIM4_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	TIM_TimeBaseStructure.TIM_Period=20000;
	TIM_TimeBaseStructure.TIM_Prescaler=6-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	NVIC_Init(&nvicInit);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM4,DISABLE);
	TIM4->CR1|=0x0c;
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    
}
void SysTick_Config(void)
{
    SysTick_SetReload(4800);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_init;
	NVIC_InitTypeDef nvicInit;
	EXTI_ClearITPendingBit(EXTI_Line0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	EXTI_init.EXTI_Line=EXTI_Line0;
	EXTI_init.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_init.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_init.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_init);
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	nvicInit.NVIC_IRQChannelPreemptionPriority=0;
	nvicInit.NVIC_IRQChannelSubPriority=1;
	nvicInit.NVIC_IRQChannel=EXTI0_IRQChannel;
	NVIC_Init(&nvicInit);	
}
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 9;   
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);  
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_71Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_71Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_71Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_71Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_71Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10,9, ADC_SampleTime_71Cycles5);
    ADC_Cmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}
void SysTick_Handler(void)
{
  OSIntNesting++;
	if(SYS.PWM_GetPeriod<5000) SYS.PWM_GetPeriod++;
  if(SYS1.CNt3<20000)  SYS1.CNt3++;
	OSTimeTick();
  OSIntExit();
}
void USART1_IRQHandler(void)
{
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
    #endif
	u8 data;
    OS_ENTER_CRITICAL();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{
        data=USART_ReceiveData(USART1);
        if(SYS1.CNt3>5) {SYS1.Cata6[0]=data;SYS1.CNt2=0;}
        else 
        {
            if(SYS1.CNt2<10) SYS1.CNt2++;
            if(SYS1.CNt2<8) SYS1.Cata6[SYS1.CNt2]=data;
        }
        SYS1.CNt3=0;
        if((SYS1.CNt2==7)&&((SYS1.Cata6[0]==SYS.ID)||(SYS1.Cata6[0]==30))&&(SYS1.Cata6[2]==0))
        {   
            DisLED(1); SYS1.CNt2=0;
            OSSemPost(SS1[0]);
        }
    }          
  	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  	{   
		if(SYS1.CNt1--> 0 ) USART_SendData(USART1, *SYS1.Cata3++);
		else KeyTurnS(0);
  	}
    OS_EXIT_CRITICAL();
}
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef nvicInit;
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&SYS.AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
    DMA_InitStructure.DMA_BufferSize =9;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    nvicInit.NVIC_IRQChannel=DMA1_Channel1_IRQChannel;
	nvicInit.NVIC_IRQChannelPreemptionPriority=1;
	nvicInit.NVIC_IRQChannelSubPriority=1;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&nvicInit); 
    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}


void bps_init(void)
{
  GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	RCC_Configuration();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL,ENABLE);
	GPIO_Configuration();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
  Timer1_Config();
	Timer2_Config();
  Timer3_Config();
  Timer4_Config();
}
//---------------------------------------------------------------------------
#define IWDG_WriteAccess_Enable     ((u16)0x5555)
#define KR_KEY_Reload    ((u16)0xAAAA)
#define KR_KEY_Enable    ((u16)0xCCCC)
#define IWDG_Prescaler_64           ((u8)0x04)
void IWDG_Config(u8 prv ,u16 rlv)
{		
	IWDG->KR = IWDG_WriteAccess_Enable;
//	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG->PR = prv;
//	IWDG_SetPrescaler( prv );
	IWDG->RLR = rlv;
//	IWDG_SetReload( rlv );
	IWDG->KR = KR_KEY_Reload;
//	IWDG_ReloadCounter();
	IWDG->KR = KR_KEY_Enable;
//	IWDG_Enable();	
}

int main(void)
{
	init:  
	bps_init();	
  //DisLED(3);
	OSInit();
	TaskCreateDisplay();
	OSStart();
	while(1){
		goto init;
	}
}
void TaskStartDisplay(void *yydata)
{
    s16 Env[10]={0,0,0,0,0,0,0,0,0,0};
		u8 i;
	yydata=yydata;
	Delay(30000);
	LoadData();
    
	          
    EXTI_Configuration();
    DMA_Configuration();
	ADC_Configuration();
		SysTick_Config();
    USART_Configuration(); 
		for(i=0;i<9;i++){
		   SYS.AD_AddValue[i]=0;
			 SYS.AD_num[i]=0;
		}
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		  GetCommID();
		SS1[0]=OSSemCreate(1);
	SS1[1]=OSSemCreate(0);
	SS1[2]=OSSemCreate(0);
	SS1[3]=OSSemCreate(0);
	SS1[4]=OSSemCreate(0);
	  TaskCreatePID1();
    TaskCreatePID2();
    TaskCreatePID3();
    TaskCreatePID4(); 
    TaskCreateSaveData();	
		
	IWDG_Config(IWDG_Prescaler_64 ,850); //1200
	while(1)
	{
        GetEnvTem(&Env[0]);
				CheckD1();
        CheckD2();
        CheckD3();
        CheckD4();
        if (GPIOB->IDR & GPIO_Pin_11) {if(SYS.PWM_GetPeriod1<50) SYS.PWM_GetPeriod1++;}
        else SYS.PWM_GetPeriod1=0;
        if (GPIOB->IDR & GPIO_Pin_10) {if(SYS.PWM_GetPeriod2<50) SYS.PWM_GetPeriod2++;}
        else SYS.PWM_GetPeriod2=0;
        if (GPIOB->IDR & GPIO_Pin_2) {if(SYS.PWM_GetPeriod3<50) SYS.PWM_GetPeriod3++;}
        else SYS.PWM_GetPeriod3=0;
        if (GPIOB->IDR & GPIO_Pin_1) {if(SYS.PWM_GetPeriod4<50) SYS.PWM_GetPeriod4++;}
        else SYS.PWM_GetPeriod4=0;
        Reg3Data[2]=CHdata1.DataGet[Tem_PV];
        Reg3Data[3]=CHdata2.DataGet[Tem_PV];
        Reg3Data[4]=CHdata3.DataGet[Tem_PV];
        Reg3Data[5]=CHdata4.DataGet[Tem_PV];
        Reg3Data[10]=CHdata1.DataGet[Rate];
        Reg3Data[11]=CHdata2.DataGet[Rate];
        Reg3Data[12]=CHdata3.DataGet[Rate];
        Reg3Data[13]=CHdata4.DataGet[Rate];	
        Reg3Data[14]=(s32)Reg4Data[91]*CHdata1.DataGet[Current]/100;	
        Reg3Data[15]=(s32)Reg4Data[91]*CHdata2.DataGet[Current]/100;	
        Reg3Data[16]=(s32)Reg4Data[91]*CHdata3.DataGet[Current]/100;	
        Reg3Data[17]=(s32)Reg4Data[91]*CHdata4.DataGet[Current]/100; 
      
            if(CHdata1.DataSet[OnOff]==0) Reg3Data[22]=STOP;
            else if(CHdata1.RunningMode==0) Reg3Data[22]=READY;
            else if(CHdata1.ErrorCode>=ALARM_H) Reg3Data[22]=CHdata1.ErrorCode;
            else if(CHdata1.RunningMode==1)
            {
                if(CHdata1.AutoFlag==1) Reg3Data[22]=SOFTSTARTUP;
                else if((CHdata1.AutoFlag==2)||(CHdata1.AutoFlag==4)) Reg3Data[22]=AUTO;
                else if(CHdata1.AutoFlag==3) Reg3Data[22]=AUTOTURN;
            }
            else if(CHdata1.RunningMode==2) Reg3Data[22]=STANDBY;
            else if(CHdata1.RunningMode==3) Reg3Data[22]=MANUAL;
            else Reg3Data[22]=STOP;
       
            if(CHdata2.DataSet[OnOff]==0) Reg3Data[23]=STOP;
            else if(CHdata2.RunningMode==0) Reg3Data[23]=READY;
            else if(CHdata2.ErrorCode>=ALARM_H) Reg3Data[23]=CHdata2.ErrorCode;
            else if(CHdata2.RunningMode==1)
            {
                if(CHdata2.AutoFlag==1) Reg3Data[23]=SOFTSTARTUP;
                else if((CHdata2.AutoFlag==2)||(CHdata2.AutoFlag==4)) Reg3Data[23]=AUTO;
                else if(CHdata2.AutoFlag==3) Reg3Data[23]=AUTOTURN;
            }
            else if(CHdata2.RunningMode==2) Reg3Data[23]=STANDBY;
            else if(CHdata2.RunningMode==3) Reg3Data[23]=MANUAL;
       
            if(CHdata3.DataSet[OnOff]==0) Reg3Data[24]=STOP;
            else if(CHdata3.RunningMode==0) Reg3Data[24]=READY;
            else if(CHdata3.ErrorCode>=ALARM_H) Reg3Data[24]=CHdata3.ErrorCode;
            else if(CHdata3.RunningMode==1)
            {
                if(CHdata3.AutoFlag==1) Reg3Data[24]=SOFTSTARTUP;
                else if((CHdata3.AutoFlag==2)||(CHdata3.AutoFlag==4)) Reg3Data[24]=AUTO;
                else if(CHdata3.AutoFlag==3) Reg3Data[24]=AUTOTURN;
            }
            else if(CHdata3.RunningMode==2) Reg3Data[24]=STANDBY;
            else if(CHdata3.RunningMode==3) Reg3Data[24]=MANUAL;
 
            if(CHdata4.DataSet[OnOff]==0) Reg3Data[25]=STOP;
            else if(CHdata4.RunningMode==0) Reg3Data[25]=READY;
            else if(CHdata4.ErrorCode>=ALARM_H) Reg3Data[25]=CHdata4.ErrorCode;
            else if(CHdata4.RunningMode==1)
            {
                if(CHdata4.AutoFlag==1) Reg3Data[25]=SOFTSTARTUP;
                else if((CHdata4.AutoFlag==2)||(CHdata4.AutoFlag==4)) Reg3Data[25]=AUTO;
                else if(CHdata4.AutoFlag==3) Reg3Data[25]=AUTOTURN;
            }
            else if(CHdata4.RunningMode==2) Reg3Data[25]=STANDBY;
            else if(CHdata4.RunningMode==3) Reg3Data[25]=MANUAL;
        Reg4Data[90]=SYS.PWM_Period;
        DisLED(3);
						
						IWDG->KR = KR_KEY_Reload;
		  OSTimeDly(5000);
						
	}
}
void SafeData(u8 *data)
{
#if OS_CRITICAL_METHOD == 3
OS_CPU_SR  cpu_sr;
#endif
u16 dataBuff=0,dataBak=0;
OS_ENTER_CRITICAL();
dataBuff=data[4];
dataBuff=(dataBuff<<8)+data[5];
dataBak=Reg4Data[data[3]];
Reg4Data[data[3]]=dataBuff;
switch(data[3])
{
case 2: CHdata1.SV_Dis=dataBuff;if(CHdata1.DataSet[C_F]) dataBuff=(dataBuff-32)*5/9;
        if(CHdata1.RunningMode==2) dataBuff=dataBuff*100/CHdata1.DataSet[ST_R];
		CHdata1.UpOrDownToSV_Flag=0;
        CHdata1.DataSet[Tem_SV]=dataBuff;CommIIC(&CHdata1.DataSet[Tem_SV],2,Tem_SV<<1,0);        
        CHdata1.TemSV=GetTemSV(CHdata1.DataSet[Tem_SV]);break;
case 3: CHdata2.SV_Dis=dataBuff;if(CHdata2.DataSet[C_F]) dataBuff=(dataBuff-32)*5/9;
        if(CHdata2.RunningMode==2) dataBuff=dataBuff*100/CHdata2.DataSet[ST_R];
		CHdata2.UpOrDownToSV_Flag=0;
        CHdata2.DataSet[Tem_SV]=dataBuff;CommIIC(&CHdata2.DataSet[Tem_SV],2,64+(Tem_SV<<1),0);
        CHdata2.TemSV=GetTemSV(CHdata2.DataSet[Tem_SV]);break;
case 4: CHdata3.SV_Dis=dataBuff;if(CHdata3.DataSet[C_F]) dataBuff=(dataBuff-32)*5/9;
        if(CHdata3.RunningMode==2) dataBuff=dataBuff*100/CHdata3.DataSet[ST_R];
		CHdata3.UpOrDownToSV_Flag=0;
        CHdata3.DataSet[Tem_SV]=dataBuff;CommIIC(&CHdata3.DataSet[Tem_SV],2,128+(Tem_SV<<1),0);
        CHdata3.TemSV=GetTemSV(CHdata3.DataSet[Tem_SV]);break;
case 5: CHdata4.SV_Dis=dataBuff;if(CHdata4.DataSet[C_F]) dataBuff=(dataBuff-32)*5/9;
        if(CHdata4.RunningMode==2) dataBuff=dataBuff*100/CHdata4.DataSet[ST_R];
		CHdata4.UpOrDownToSV_Flag=0;
        CHdata4.DataSet[Tem_SV]=dataBuff;CommIIC(&CHdata4.DataSet[Tem_SV],2,192+(Tem_SV<<1),0);
        CHdata4.TemSV=GetTemSV(CHdata4.DataSet[Tem_SV]);break;
case 6: dataBuff&=0x01;
		if((dataBuff==1)&&(data[0]==30)) 
		{
			if(CHdata1.TcopFlag==0) CHdata1.DataSet[OnOff]=dataBuff;
			else CHdata1.DataSet[OnOff]=dataBak;
		}
		else CHdata1.DataSet[OnOff]=dataBuff; 
		
		Reg4Data[data[3]]=CHdata1.DataSet[OnOff]; //Reg4Data[6]=CHdata1.DataSet[OnOff];
		CommIIC(&CHdata1.DataSet[OnOff],2,OnOff<<1,0); break;             
case 7: dataBuff&=0x01;
		if((dataBuff==1)&&(data[0]==30)) 
		{
			if(CHdata2.TcopFlag==0) CHdata2.DataSet[OnOff]=dataBuff;
			else CHdata2.DataSet[OnOff]=dataBak;
		}
		else CHdata2.DataSet[OnOff]=dataBuff; 
		Reg4Data[data[3]]=CHdata2.DataSet[OnOff];
		CommIIC(&CHdata2.DataSet[OnOff],2,64+(OnOff<<1),0); break;                
case 8: dataBuff&=0x01;
		if((dataBuff==1)&&(data[0]==30)) 
		{
			if(CHdata3.TcopFlag==0) CHdata3.DataSet[OnOff]=dataBuff;
			else CHdata3.DataSet[OnOff]=dataBak;
		}
		else CHdata3.DataSet[OnOff]=dataBuff;
		Reg4Data[data[3]]=CHdata3.DataSet[OnOff];
		CommIIC(&CHdata3.DataSet[OnOff],2,128+(OnOff<<1),0); break;              
case 9: 
		dataBuff&=0x01;
		if((dataBuff==1)&&(data[0]==30)) 
		{
			if(CHdata4.TcopFlag==0) CHdata4.DataSet[OnOff]=dataBuff;
			else CHdata4.DataSet[OnOff]=dataBak;
		}
		else CHdata4.DataSet[OnOff]=dataBuff;
		Reg4Data[data[3]]=CHdata4.DataSet[OnOff];
		CommIIC(&CHdata4.DataSet[OnOff],2,192+(OnOff<<1),0); break;
		
case 10: CHdata1.DataSet[In]=dataBuff;CommIIC(&CHdata1.DataSet[In],2,In<<1,0);break;
case 11: CHdata2.DataSet[In]=dataBuff;CommIIC(&CHdata2.DataSet[In],2,64+(In<<1),0);break;
case 12: CHdata3.DataSet[In]=dataBuff;CommIIC(&CHdata3.DataSet[In],2,128+(In<<1),0);break;
case 13: CHdata4.DataSet[In]=dataBuff;CommIIC(&CHdata4.DataSet[In],2,192+(In<<1),0);break;
case 14: CHdata1.DataSet[C_F]=dataBuff;
         if(dataBuff) Reg4Data[2]=CHdata1.SV_Dis=9*CHdata1.DataSet[Tem_SV]/5+32;
         else Reg4Data[2]=CHdata1.SV_Dis=CHdata1.DataSet[Tem_SV];
         CommIIC(&CHdata1.DataSet[C_F],2,C_F<<1,0);break;
case 15: CHdata2.DataSet[C_F]=dataBuff;
         if(dataBuff) Reg4Data[3]=CHdata2.SV_Dis=9*CHdata2.DataSet[Tem_SV]/5+32;
         else Reg4Data[3]=CHdata2.SV_Dis=CHdata2.DataSet[Tem_SV];
         CommIIC(&CHdata2.DataSet[C_F],2,64+(C_F<<1),0);break;
case 16: CHdata3.DataSet[C_F]=dataBuff;
         if(dataBuff) Reg4Data[4]=CHdata3.SV_Dis=9*CHdata3.DataSet[Tem_SV]/5+32;
         else Reg4Data[4]=CHdata3.SV_Dis=CHdata3.DataSet[Tem_SV];
         CommIIC(&CHdata3.DataSet[C_F],2,128+(C_F<<1),0);break;
case 17: CHdata4.DataSet[C_F]=dataBuff;
         if(dataBuff) Reg4Data[5]=CHdata4.SV_Dis=9*CHdata4.DataSet[Tem_SV]/5+32;
         else Reg4Data[5]=CHdata4.SV_Dis=CHdata4.DataSet[Tem_SV];
         CommIIC(&CHdata4.DataSet[C_F],2,192+(C_F<<1),0);break;
case 18: CHdata1.DataSet[HSCI]=dataBuff;CommIIC(&CHdata1.DataSet[HSCI],2,(HSCI<<1),0);break;
case 19: CHdata2.DataSet[HSCI]=dataBuff;CommIIC(&CHdata2.DataSet[HSCI],2,64+(HSCI<<1),0);break;
case 20: CHdata3.DataSet[HSCI]=dataBuff;CommIIC(&CHdata3.DataSet[HSCI],2,128+(HSCI<<1),0);break;
case 21: CHdata4.DataSet[HSCI]=dataBuff;CommIIC(&CHdata4.DataSet[HSCI],2,192+(HSCI<<1),0);break;
case 22: CHdata1.DataSet[SWSF]=dataBuff;CommIIC(&CHdata1.DataSet[SWSF],2,SWSF<<1,0);break;
case 23: CHdata2.DataSet[SWSF]=dataBuff;CommIIC(&CHdata2.DataSet[SWSF],2,64+(SWSF<<1),0);break;
case 24: CHdata3.DataSet[SWSF]=dataBuff;CommIIC(&CHdata3.DataSet[SWSF],2,128+(SWSF<<1),0);break;
case 25: CHdata4.DataSet[SWSF]=dataBuff;CommIIC(&CHdata4.DataSet[SWSF],2,192+(SWSF<<1),0);break;
case 26: if(dataBuff==2)
         {
            if(CHdata1.RunningMode!=2) Reg4Data[2]=CHdata1.SV_Dis=CHdata1.SV_Dis*CHdata1.DataSet[ST_R]/100; 
            CHdata1.StandbyCount=CHdata1.StandbyTime;
         }
         else if(CHdata1.RunningMode==2) Reg4Data[2]=CHdata1.SV_Dis=CHdata1.SV_Dis*100/CHdata1.DataSet[ST_R];
         CHdata1.RunningMode=dataBuff;break;
case 27: if(dataBuff==2)
         {
            if(CHdata2.RunningMode!=2) Reg4Data[3]=CHdata2.SV_Dis=CHdata2.SV_Dis*CHdata2.DataSet[ST_R]/100; 
            CHdata2.StandbyCount=CHdata2.StandbyTime;
         }
         else if(CHdata2.RunningMode==2) Reg4Data[3]=CHdata2.SV_Dis=CHdata2.SV_Dis*100/CHdata2.DataSet[ST_R];
         CHdata2.RunningMode=dataBuff;break;
case 28: if(dataBuff==2)
         {
            if(CHdata3.RunningMode!=2) Reg4Data[4]=CHdata3.SV_Dis=CHdata3.SV_Dis*CHdata3.DataSet[ST_R]/100; 
            CHdata3.StandbyCount=CHdata3.StandbyTime;
         }
         else if(CHdata3.RunningMode==2) Reg4Data[4]=CHdata3.SV_Dis=CHdata3.SV_Dis*100/CHdata3.DataSet[ST_R];
         CHdata3.RunningMode=dataBuff;break;
case 29: if(dataBuff==2)
         {
            if(CHdata4.RunningMode!=2) Reg4Data[5]=CHdata4.SV_Dis=CHdata4.SV_Dis*CHdata4.DataSet[ST_R]/100; 
            CHdata4.StandbyCount=CHdata4.StandbyTime;
         }
         else if(CHdata4.RunningMode==2) Reg4Data[5]=CHdata4.SV_Dis=CHdata4.SV_Dis*100/CHdata4.DataSet[ST_R];
         CHdata4.RunningMode=dataBuff;break;
case 34: CHdata1.DataSet[TURN]=dataBuff;CommIIC(&CHdata1.DataSet[TURN],2,TURN<<1,0);break;
case 35: CHdata2.DataSet[TURN]=dataBuff;CommIIC(&CHdata2.DataSet[TURN],2,64+(TURN<<1),0);break;
case 36: CHdata3.DataSet[TURN]=dataBuff;CommIIC(&CHdata3.DataSet[TURN],2,128+(TURN<<1),0);break;
case 37: CHdata4.DataSet[TURN]=dataBuff;CommIIC(&CHdata4.DataSet[TURN],2,192+(TURN<<1),0);break;
case 38: CHdata1.DataSet[AL_H]=dataBuff;CommIIC(&CHdata1.DataSet[AL_H],2,AL_H<<1,0);break;
case 39: CHdata2.DataSet[AL_H]=dataBuff;CommIIC(&CHdata2.DataSet[AL_H],2,64+(AL_H<<1),0);break;
case 40: CHdata3.DataSet[AL_H]=dataBuff;CommIIC(&CHdata3.DataSet[AL_H],2,128+(AL_H<<1),0);break;
case 41: CHdata4.DataSet[AL_H]=dataBuff;CommIIC(&CHdata4.DataSet[AL_H],2,192+(AL_H<<1),0);break;
case 42: CHdata1.DataSet[AL_L]=dataBuff;CommIIC(&CHdata1.DataSet[AL_L],2,AL_L<<1,0);break;
case 43: CHdata2.DataSet[AL_L]=dataBuff;CommIIC(&CHdata2.DataSet[AL_L],2,64+(AL_L<<1),0);break;
case 44: CHdata3.DataSet[AL_L]=dataBuff;CommIIC(&CHdata3.DataSet[AL_L],2,128+(AL_L<<1),0);break;
case 45: CHdata4.DataSet[AL_L]=dataBuff;CommIIC(&CHdata4.DataSet[AL_L],2,192+(AL_L<<1),0);break;
case 46: CHdata1.DataSet[ST_T]=dataBuff;CHdata1.StandbyTime=CHdata1.DataSet[ST_T]*60;
         CommIIC(&CHdata1.DataSet[ST_T],2,ST_T<<1,0);break;
case 47: CHdata2.DataSet[ST_T]=dataBuff;CHdata2.StandbyTime=CHdata2.DataSet[ST_T]*60;
         CommIIC(&CHdata2.DataSet[ST_T],2,64+(ST_T<<1),0);break;
case 48: CHdata3.DataSet[ST_T]=dataBuff;CHdata3.StandbyTime=CHdata3.DataSet[ST_T]*60;
         CommIIC(&CHdata3.DataSet[ST_T],2,128+(ST_T<<1),0);break;
case 49: CHdata4.DataSet[ST_T]=dataBuff;CHdata4.StandbyTime=CHdata4.DataSet[ST_T]*60;
         CommIIC(&CHdata4.DataSet[ST_T],2,192+(ST_T<<1),0);break;
case 50: CHdata1.DataSet[ST_R]=dataBuff;CommIIC(&CHdata1.DataSet[ST_R],2,ST_R<<1,0);break;
case 51: CHdata2.DataSet[ST_R]=dataBuff;CommIIC(&CHdata2.DataSet[ST_R],2,64+(ST_R<<1),0);break;
case 52: CHdata3.DataSet[ST_R]=dataBuff;CommIIC(&CHdata3.DataSet[ST_R],2,128+(ST_R<<1),0);break;
case 53: CHdata4.DataSet[ST_R]=dataBuff;CommIIC(&CHdata4.DataSet[ST_R],2,192+(ST_R<<1),0);break;
case 54: CHdata1.DataSet[HC_H]=dataBuff;CommIIC(&CHdata1.DataSet[HC_H],2,HC_H<<1,0);break;
case 55: CHdata2.DataSet[HC_H]=dataBuff;CommIIC(&CHdata2.DataSet[HC_H],2,64+(HC_H<<1),0);break;
case 56: CHdata3.DataSet[HC_H]=dataBuff;CommIIC(&CHdata3.DataSet[HC_H],2,128+(HC_H<<1),0);break;
case 57: CHdata4.DataSet[HC_H]=dataBuff;CommIIC(&CHdata4.DataSet[HC_H],2,192+(HC_H<<1),0);break;
case 58: CHdata1.DataSet[HC_L]=dataBuff;CommIIC(&CHdata1.DataSet[HC_L],2,HC_L<<1,0);break;
case 59: CHdata2.DataSet[HC_L]=dataBuff;CommIIC(&CHdata2.DataSet[HC_L],2,64+(HC_L<<1),0);break;
case 60: CHdata3.DataSet[HC_L]=dataBuff;CommIIC(&CHdata3.DataSet[HC_L],2,128+(HC_L<<1),0);break;
case 61: CHdata4.DataSet[HC_L]=dataBuff;CommIIC(&CHdata4.DataSet[HC_L],2,192+(HC_L<<1),0);break;
case 62: CHdata1.DataSet[SO_T]=dataBuff;CommIIC(&CHdata1.DataSet[SO_T],2,SO_T<<1,0);break;
case 63: CHdata2.DataSet[SO_T]=dataBuff;CommIIC(&CHdata2.DataSet[SO_T],2,64+(SO_T<<1),0);break;
case 64: CHdata3.DataSet[SO_T]=dataBuff;CommIIC(&CHdata3.DataSet[SO_T],2,128+(SO_T<<1),0);break;
case 65: CHdata4.DataSet[SO_T]=dataBuff;CommIIC(&CHdata4.DataSet[SO_T],2,192+(SO_T<<1),0);break;
case 66: CHdata1.DataSet[SO_R]=dataBuff;CommIIC(&CHdata1.DataSet[SO_R],2,SO_R<<1,0);break;
case 67: CHdata2.DataSet[SO_R]=dataBuff;CommIIC(&CHdata2.DataSet[SO_R],2,64+(SO_R<<1),0);break;
case 68: CHdata3.DataSet[SO_R]=dataBuff;CommIIC(&CHdata3.DataSet[SO_R],2,128+(SO_R<<1),0);break;
case 69: CHdata4.DataSet[SO_R]=dataBuff;CommIIC(&CHdata4.DataSet[SO_R],2,192+(SO_R<<1),0);break;
case 70: CHdata1.DataSet[P]=dataBuff;CommIIC(&CHdata1.DataSet[P],2,P<<1,0);break;
case 71: CHdata2.DataSet[P]=dataBuff;CommIIC(&CHdata2.DataSet[P],2,64+(P<<1),0);break;
case 72: CHdata3.DataSet[P]=dataBuff;CommIIC(&CHdata3.DataSet[P],2,128+(P<<1),0);break;
case 73: CHdata4.DataSet[P]=dataBuff;CommIIC(&CHdata4.DataSet[P],2,192+(P<<1),0);break;
case 74: CHdata1.DataSet[I]=dataBuff;CommIIC(&CHdata1.DataSet[I],2,I<<1,0);break;
case 75: CHdata2.DataSet[I]=dataBuff;CommIIC(&CHdata2.DataSet[I],2,64+(I<<1),0);break;
case 76: CHdata3.DataSet[I]=dataBuff;CommIIC(&CHdata3.DataSet[I],2,128+(I<<1),0);break;
case 77: CHdata4.DataSet[I]=dataBuff;CommIIC(&CHdata4.DataSet[I],2,192+(I<<1),0);break;
case 78: CHdata1.DataSet[D]=dataBuff;CommIIC(&CHdata1.DataSet[D],2,D<<1,0);break;
case 79: CHdata2.DataSet[D]=dataBuff;CommIIC(&CHdata2.DataSet[D],2,64+(D<<1),0);break;
case 80: CHdata3.DataSet[D]=dataBuff;CommIIC(&CHdata3.DataSet[D],2,128+(D<<1),0);break;
case 81: CHdata4.DataSet[D]=dataBuff;CommIIC(&CHdata4.DataSet[D],2,192+(D<<1),0);break;
case 82: CHdata1.ManualRate=dataBuff;break;
case 83: CHdata2.ManualRate=dataBuff;break;
case 84: CHdata3.ManualRate=dataBuff;break;
case 85: CHdata4.ManualRate=dataBuff;break;
case 86: CHdata1.DataSet[SampFreq]=dataBuff;CommIIC(&CHdata1.DataSet[SampFreq],2,SampFreq<<1,0);break;
case 87: CHdata2.DataSet[SampFreq]=dataBuff;CommIIC(&CHdata2.DataSet[SampFreq],2,64+(SampFreq<<1),0);break;
case 88: CHdata3.DataSet[SampFreq]=dataBuff;CommIIC(&CHdata3.DataSet[SampFreq],2,128+(SampFreq<<1),0);break;
case 89: CHdata4.DataSet[SampFreq]=dataBuff;CommIIC(&CHdata4.DataSet[SampFreq],2,192+(SampFreq<<1),0);break;
case 90: break;
case 91: SYS.PowerValue=dataBuff;CommIIC(&SYS.PowerValue,2,64+(Nop2<<1),0);break;
case 92: break;
case 93: CHdata4.DataSet[Nop2]=dataBuff;CommIIC(&CHdata4.DataSet[Nop2],2,192+(Nop2<<1),0);break;

case 94: CHdata1.DataSet[Man1]=dataBuff;CommIIC(&CHdata1.DataSet[Man1],2,Man1<<1,0);break;
case 95: CHdata2.DataSet[Man1]=dataBuff;CommIIC(&CHdata2.DataSet[Man1],2,64+(Man1<<1),0);break;
case 96: CHdata3.DataSet[Man1]=dataBuff;CommIIC(&CHdata3.DataSet[Man1],2,128+(Man1<<1),0);break;
case 97: CHdata4.DataSet[Man1]=dataBuff;CommIIC(&CHdata4.DataSet[Man1],2,192+(Man1<<1),0);break;

case 110: SYS.DataCalSet[ET_Z]=dataBuff;CommIIC(&SYS.DataCalSet[ET_Z],2,256+(ET_Z<<1),0);break;
case 111: SYS.DataCalSet[HT_Z1]=dataBuff;CommIIC(&SYS.DataCalSet[HT_Z1],2,256+(HT_Z1<<1),0);break;
case 112: SYS.DataCalSet[HT_A1]=dataBuff;CommIIC(&SYS.DataCalSet[HT_A1],2,256+(HT_A1<<1),0);break;
case 113: SYS.DataCalSet[HT_Z2]=dataBuff;CommIIC(&SYS.DataCalSet[HT_Z2],2,256+(HT_Z2<<1),0);break;
case 114: SYS.DataCalSet[HT_A2]=dataBuff;CommIIC(&SYS.DataCalSet[HT_A2],2,256+(HT_A2<<1),0);break;
case 115: SYS.DataCalSet[HT_Z3]=dataBuff;CommIIC(&SYS.DataCalSet[HT_Z3],2,256+(HT_Z3<<1),0);break;
case 116: SYS.DataCalSet[HT_A3]=dataBuff;CommIIC(&SYS.DataCalSet[HT_A3],2,256+(HT_A3<<1),0);break;
case 117: SYS.DataCalSet[HT_Z4]=dataBuff;CommIIC(&SYS.DataCalSet[HT_Z4],2,256+(HT_Z4<<1),0);break;
case 118: SYS.DataCalSet[HT_A4]=dataBuff;CommIIC(&SYS.DataCalSet[HT_A4],2,256+(HT_A4<<1),0);break;       
default: break;
}
OS_EXIT_CRITICAL();
}
void TaskStartSaveData(void *yydata)
{
	#if OS_CRITICAL_METHOD == 3
	OS_CPU_SR  cpu_sr;
	#endif
    u8 i=0,status,dataBuff[8];
	yydata=yydata;
//	OSTimeDly(10000);
    KeyTurnX(20);
    Reg4Data[130]=0x55;
    Reg4Data[131]=0xaf0a;
	
	while(1)
	{   
        OSSemPend(SS1[0],0,&status);
        for(i=0;i<8;i++) 
        {
            dataBuff[i]=SYS1.Cata6[i];
            SYS1.Cata6[i]=0x00; //0xff
        }
        CCdataValue(&dataBuff[0],6);
        if((SYS1.Cata1==dataBuff[6])&&(SYS1.Cata2==dataBuff[7]))
        {
            dataBuff[3]++;
            if(dataBuff[1]==3) 
						{
							if(dataBuff[0]==SYS.ID) Timcomm(Reg4Data,SYS.ID,3,dataBuff[3],dataBuff[5]);
							else if((SYS.ID==0x01)&&(dataBuff[0]==30)) Timcomm(Reg4Data,30,3,dataBuff[3],dataBuff[5]);

						}
            else if(dataBuff[1]==4) {Timcomm(Reg3Data,SYS.ID,4,dataBuff[3],dataBuff[5]);}
            else if(dataBuff[1]==6) 
            {
							if(dataBuff[0]==30) 
							{
								OS_ENTER_CRITICAL();
								for(i=0;i<4;i++) {SafeData(&dataBuff[0]);dataBuff[3]++;}
								OS_EXIT_CRITICAL();
							}
                else if(dataBuff[0]==SYS.ID) {SafeData(&dataBuff[0]);Timcomm(Reg4Data,SYS.ID,6,dataBuff[3]-1,1);}	
            } 
        }
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

