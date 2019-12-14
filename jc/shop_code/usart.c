#include "stm32f10x.h"
#include "usart.h"



// Use Configuration Wizard in Context Menu 

//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口1    
//     <o10> 引脚       <0=> 无映射 TX_PA9,RX_PA10  <1=> 重映射 TX_PB6,RX_PB7 
//     <o1> 波特率      <9600=> 9600    <19200=> 19200
//                      <38400=> 38400  <57600=> 57600  <115200=> 115200
//     <o2> 数据格式    <0x0000=> USART_WordLength_8b    <0x1000=> USART_WordLength_9b
//     <o3> 停止位    <0x0000=> USART_StopBits_1    <0x1000=> USART_StopBits_0_5
//                    <0x2000=> USART_StopBits_2    <0x3000=> USART_StopBits_1_5
//     <o4> 奇偶校验    <0x0000=> USART_Parity_No    <0x0400=> USART_Parity_Even   <0x0600=> USART_Parity_Odd
// 
//     <o5> 硬件数据流控制  <0x0000=> USART_HardwareFlowControl_None    <0x0100=> USART_HardwareFlowControl_RTS  
//                         <0x0200=> USART_HardwareFlowControl_CTS    <0x0300=> USART_HardwareFlowControl_RTS_CTS
//     <o6> 收发模式    <0x0004=> USART_Mode_Rx    <0x0008=> USART_Mode_Tx   <0x000C=> USART_Mode_Rx | USART_Mode_Tx
//     <e7> 使能接收中断
//        <o8> 抢占优先级  <0x0-0x0F>
//        <o9> 子优先级  <0x0-0x0F>
//     </e>
// </e>
#define EN_USART_1   0
#define Bound_1      115200
#define WordLength_1  0x0000
#define StopBits_1    0x0000
#define Parity_1      0x0000
#define FlowContro_l 0x0000
#define Usart_Mode_1  0x000C   
#define EN_NVIC_1     1
#define PreemptionPriority_1  0
#define SubPriority_1  2
#define ReMap_1    1  
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口2    
//     <o10> 引脚       <0=> 无映射 TX_PA2,RX_PA3  <1=> 重映射 TX_PD5,RX_PD6 
//     <o1> 波特率      <9600=> 9600    <19200=> 19200
//                      <38400=> 38400  <57600=> 57600  <115200=> 115200
//     <o2> 数据格式    <0x0000=> USART_WordLength_8b    <0x1000=> USART_WordLength_9b
//     <o3> 停止位    <0x0000=> USART_StopBits_1    <0x1000=> USART_StopBits_0_5
//                    <0x2000=> USART_StopBits_2    <0x3000=> USART_StopBits_1_5
//     <o4> 奇偶校验    <0x0000=> USART_Parity_No    <0x0400=> USART_Parity_Even   <0x0600=> USART_Parity_Odd
// 
//     <o5> 硬件数据流控制  <0x0000=> USART_HardwareFlowControl_None    <0x0100=> USART_HardwareFlowControl_RTS  
//                         <0x0200=> USART_HardwareFlowControl_CTS    <0x0300=> USART_HardwareFlowControl_RTS_CTS
//     <o6> 收发模式    <0x0004=> USART_Mode_Rx    <0x0008=> USART_Mode_Tx   <0x000C=> USART_Mode_Rx | USART_Mode_Tx
//     <e7> 使能接收中断
//        <o8> 抢占优先级  <0x0-0x0F>
//        <o9> 子优先级  <0x0-0x0F>
//     </e>
// </e>
#define EN_USART_2   1
#define Bound_2      115200
#define WordLength_2  0x0000
#define StopBits_2    0x0000
#define Parity_2      0x0000
#define FlowContro_2 0x0000
#define Usart_Mode_2  0x000C   
#define EN_NVIC_2     1
#define PreemptionPriority_2  1
#define SubPriority_2  2
#define ReMap_2    0  
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口3    
//     <o10> 引脚       <0=> 无映射 TX_PB10,RX_PB11  <1=> 部分映射 TX_PC10,RX_PC11  <2=> 完全映射 TX_PD8,RX_PD9 
//     <o1> 波特率      <9600=> 9600    <19200=> 19200
//                      <38400=> 38400  <57600=> 57600  <115200=> 115200
//     <o2> 数据格式    <0x0000=> USART_WordLength_8b    <0x1000=> USART_WordLength_9b
//     <o3> 停止位    <0x0000=> USART_StopBits_1    <0x1000=> USART_StopBits_0_5
//                    <0x2000=> USART_StopBits_2    <0x3000=> USART_StopBits_1_5
//     <o4> 奇偶校验    <0x0000=> USART_Parity_No    <0x0400=> USART_Parity_Even   <0x0600=> USART_Parity_Odd
// 
//     <o5> 硬件数据流控制  <0x0000=> USART_HardwareFlowControl_None    <0x0100=> USART_HardwareFlowControl_RTS  
//                         <0x0200=> USART_HardwareFlowControl_CTS    <0x0300=> USART_HardwareFlowControl_RTS_CTS
//     <o6> 收发模式    <0x0004=> USART_Mode_Rx    <0x0008=> USART_Mode_Tx   <0x000C=> USART_Mode_Rx | USART_Mode_Tx
//     <e7> 使能接收中断
//        <o8> 抢占优先级  <0x0-0x0F>
//        <o9> 子优先级  <0x0-0x0F>
//     </e>
// </e>
#define EN_USART_3   0
#define Bound_3      38400
#define WordLength_3  0x0000
#define StopBits_3    0x0000
#define Parity_3      0x0000
#define FlowContro_3 0x0000
#define Usart_Mode_3  0x000C   
#define EN_NVIC_3     1
#define PreemptionPriority_3  1
#define SubPriority_3  2
#define ReMap_3    1  
//-------------------------------------------------------------------------------------------------------------------------

// end of configuration section 
//---------------------------------------------------------------------------------------- 
uint8_t Rxbuf2[30];
uint16_t comRx,comP;
uint8_t* Txbuf1;
  uint16_t pt1,txlen1;
  
GPIO_TypeDef* Gtr2;
uint16_t Ptr2;

#if EN_USART_1   //如果使能了串口1
// uint8_t Rxbuf1[10];
// uint16_t Rx1,P1;

void uart1_init(void){
    
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
# if !ReMap_1	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	 //USART1_TX   PB.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART1_RX	  PB.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
#endif
   //Usart1 NVIC 配置
#if EN_NVIC_1
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority_1 ;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority_1;		
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	  NVIC_Init(&NVIC_InitStructure);		  

      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	
#endif  
   //USART 初始化设置

	 USART_InitStructure.USART_BaudRate = Bound_1;  
	 USART_InitStructure.USART_WordLength = WordLength_1;  
	 USART_InitStructure.USART_StopBits = StopBits_1;
	 USART_InitStructure.USART_Parity = Parity_1;
	 USART_InitStructure.USART_HardwareFlowControl = FlowContro_l;
	 USART_InitStructure.USART_Mode = Usart_Mode_1;	

    USART_Init(USART1, &USART_InitStructure);     
    USART_Cmd(USART1, ENABLE);                   
    USART_ClearFlag(USART1,USART_FLAG_TC);
}
//-------------------------------------------------------------------

 

void USART1_IRQHandler(void)                	//串口1中断服务程序
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
/*		Rxbuf1[P1] = USART_ReceiveData(USART1);
		if(P1 < 9) P1++;
		Rx1 = 3; //0.5ms*/
		Rxbuf2[comP] = USART_ReceiveData(USART1);
		if(comP < 29) comP++;
		comRx = 3; //0.5ms
    } 
	
} 
#endif	

//---------------------------------------------------------------------------------------- 
#if EN_USART_2   //如果使能了串口1



void uart2_init(void){
    
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
# if !ReMap_2	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	
     //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
#else
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
     //USART2_TX   PD.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOD, &GPIO_InitStructure);
   
    //USART2_RX	  PD.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
#endif
   //Usart1 NVIC 配置
#if EN_NVIC_2
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority_2 ;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority_2;		
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	  NVIC_Init(&NVIC_InitStructure);	
	  
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
#endif  
   //USART 初始化设置

	  USART_InitStructure.USART_BaudRate = Bound_2;  
	  USART_InitStructure.USART_WordLength = WordLength_2;  
	  USART_InitStructure.USART_StopBits = StopBits_2;
	  USART_InitStructure.USART_Parity = Parity_2;
	  USART_InitStructure.USART_HardwareFlowControl = FlowContro_2;
	  USART_InitStructure.USART_Mode = Usart_Mode_2;	

    USART_Init(USART2, &USART_InitStructure);     
    USART_Cmd(USART2, ENABLE);                   
    USART_ClearFlag(USART2,USART_FLAG_TC|USART_IT_RXNE|USART_FLAG_TXE);
}
//----------------------------------------------------------------------
void tx485_handler(Rx485_t* Ux)
{
	if (USART_GetITStatus(Ux->usart, USART_IT_TXE) != RESET)
	{		
		    USART_SendData(Ux->usart, Ux->txbuf[Ux->txp++]);		  
			if(Ux->txp == Ux->txlen){
				USART_ITConfig(Ux->usart, USART_IT_TXE, DISABLE);
			    USART_ITConfig(Ux->usart, USART_IT_TC, ENABLE);
			}			
	}
	else if(USART_GetITStatus(Ux->usart, USART_IT_TC) != RESET)  
	{
		Ux->txp = 0;
        USART_ITConfig(Ux->usart, USART_IT_TC, DISABLE);	
        GPIO_ResetBits(Ux->gtr,Ux->ptr);	        		
	}		
}



//void USART2_IRQHandler(void)                	//串口1中断服务程序
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
//	{
//        USART_ClearITPendingBit(USART2, USART_IT_RXNE);		
//		Rxbuf2[comP] = USART_ReceiveData(USART2);
//		if(comP < 29) comP++;
//		comRx = 3; //0.5ms
//     }
//    tx485_handler(&Usarts[1]);	

//} 
#endif	
//---------------------------------------------------------------------------------------- 
#if EN_USART_3   //如果使能了串口1
static uint8_t Rxbuf3[0x0a];
void uart3_init(void){
    
     GPIO_InitTypeDef GPIO_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef  NVIC_InitStructure;
# if (!ReMap_3)	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);		
     //USART1_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART1_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
#elif (ReMap_3 == 1)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
     //USART1_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    //USART1_RX	  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
#elif (ReMap_3 == 2)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
     //USART1_TX   PD.8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOD, &GPIO_InitStructure);
   
    //USART1_RX	  PD.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  	
#endif
   //Usart1 NVIC 配置
#if EN_NVIC_3
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority_3 ;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority_3;		
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	  NVIC_Init(&NVIC_InitStructure);	
	  
	  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
#endif  
   //USART 初始化设置

	  USART_InitStructure.USART_BaudRate = Bound_3;  
	  USART_InitStructure.USART_WordLength = WordLength_3;  
	  USART_InitStructure.USART_StopBits = StopBits_3;
	  USART_InitStructure.USART_Parity = Parity_3;
	  USART_InitStructure.USART_HardwareFlowControl = FlowContro_3;
	  USART_InitStructure.USART_Mode = Usart_Mode_3;	

    USART_Init(USART3, &USART_InitStructure);     
    USART_Cmd(USART3, ENABLE);                   
    USART_ClearFlag(USART3,USART_FLAG_TC);
}
//----------------------------------------------------------------------
void USART3_IRQHandler(void)                	//串口1中断服务程序
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
	   
     } 

} 
#endif	
//-------------------------------------------------------------------
void uart_init(void)
{
#if EN_USART_1   //如果使能了串口1
    uart1_init();
#endif
#if EN_USART_2   //如果使能了串口2
    uart2_init();
#endif	
#if EN_USART_3   //如果使能了串口3
    uart3_init();
#endif	
}


