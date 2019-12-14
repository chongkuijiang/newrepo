
#include "stm32f10x.h"
#include "usart.h"	
#include "crc.h"
#include <jansson.h>
#include <string.h>
extern uint8_t adress;
extern uint16_t led232,led485;
extern __IO uint16_t ADC_Value[8];


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
#define EN_USART_1   1
#define Bound_1      115200
#define WordLength_1  0x0000
#define StopBits_1    0x0000
#define Parity_1      0x0000
#define FlowContro_l 0x0000
#define Usart_Mode_1  0x000C   
#define EN_NVIC_1     1
#define PreemptionPriority_1  0
#define SubPriority_1  2
#define ReMap_1    0  
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
#define EN_USART_2   0
#define Bound_2      38400
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
#define EN_USART_3   1
#define Bound_3      115200
#define WordLength_3  0x0000
#define StopBits_3    0x0000
#define Parity_3      0x0000
#define FlowContro_3 0x0000
#define Usart_Mode_3  0x000C   
#define EN_NVIC_3     1
#define PreemptionPriority_3  0
#define SubPriority_3  1
#define ReMap_3    1  
//-------------------------------------------------------------------------------------------------------------------------

// end of configuration section 
//---------------------------------------------------------------------------------------- 
#if EN_USART_1   //如果使能了串口1
  uint8_t Rxbuf1[200],pr1,rxtim1;
  uint8_t Txbuf1[200],pt1,txlen1;

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
#if EN_NVIC_1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
}
//-------------------------------------------------------------------

 

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		Rxbuf1[pr1++] =USART_ReceiveData(USART1);
        rxtim1 = 3;
	    led232 = 1;
    } 
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{		
		    USART_SendData(USART1, Txbuf1[pt1++]);		  
			if(pt1 == txlen1){
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
			}			
	}
	else if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)  
	{
		pt1 = 0;
        USART_ITConfig(USART1, USART_IT_TC, DISABLE);		
	}		
} 
#endif	

//---------------------------------------------------------------------------------------- 
#if EN_USART_2   //如果使能了串口1
static uint8_t Rxbuf2[0x0a];
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

}
//----------------------------------------------------------------------
void USART2_IRQHandler(void)                	//串口1中断服务程序
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
	{
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	//	Rxbuf2[Msg[Lcdshow].temp++] = USART_ReceiveData(USART2);
	//	staskDelay(Lcdshow,3,Rxbuf2);
     } 

} 
#endif	
//---------------------------------------------------------------------------------------- 
#if EN_USART_3   //如果使能了串口1
  uint8_t Rxbuf3[200],pr3,rxtim3;
  uint8_t Txbuf3[200],pt3,txlen3;
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

}
//----------------------------------------------------------------------
void USART3_IRQHandler(void)                	//串口1中断服务程序
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		Rxbuf3[pr3++] =USART_ReceiveData(USART3);
        rxtim3 = 3;
		led485 = 1;
    } 
	if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{		
		    USART_SendData(USART3, Txbuf3[pt3++]);		  
			if(pt3 == txlen3){
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			    USART_ITConfig(USART3, USART_IT_TC, ENABLE);
			}			
	}
	else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)  
	{
		pt3 = 0;
        USART_ITConfig(USART3, USART_IT_TC, DISABLE);	
        GPIO_ResetBits(GPIOC,GPIO_Pin_12);		
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

//int fputc(int ch, FILE* stream)
//{
//    USART_SendData(USART1, (unsigned char) ch);
//    while (!(USART1->SR & USART_FLAG_TXE));
//    return ch;
//}

void sendn(USART_TypeDef* mUsart,uint8_t* txbuf,uint8_t* txlen,char* buf){   
	uint16_t i=0;
	while(buf[i]!='\0'){
		txbuf[i] = (uint8_t)buf[i];
		i++;
	};
	txbuf[i++] = '\r';
	txbuf[i++] = '\n';
	*txlen = i;
	
	USART_ITConfig(mUsart, USART_IT_TXE, ENABLE);
}
void U1sendn(char* buf){
	sendn(USART1,Txbuf1,&txlen1,buf);
}
void U3sendn(char* buf){	
	GPIO_SetBits(GPIOC,GPIO_Pin_12);
	sendn(USART3,Txbuf3,&txlen3,buf);	
}
//---------------------------------------------------------
void send(USART_TypeDef* mUsart,uint8_t* txbuf,uint8_t* txlen,uint8_t* buf,uint8_t buflen){   
	for(int i=0;i<buflen;i++){
		txbuf[i] = buf[i];
	}
	*txlen = buflen;
	USART_ITConfig(mUsart, USART_IT_TXE, ENABLE);
}

void U1send(uint8_t* buf,uint8_t buflen){
	send(USART1,Txbuf1,&txlen1,buf,buflen);
}
void U3send(uint8_t* buf,uint8_t buflen){
	GPIO_SetBits(GPIOC,GPIO_Pin_12);
	send(USART3,Txbuf3,&txlen3,buf,buflen);	
}

char* ch[8] = {"ch1","ch2","ch3","ch4","ch5","ch6","ch7","ch8"};

void json(USART_TypeDef* mUsart,uint16_t start,uint16_t len){
	 json_t *root;
	
    char *out = "";
    char* str;
	if(start >8) start = 8;	
	uint16_t num = 9 - start;
	if(len > num) len = num;
	start -= 1;
	switch(len){
		case 8:
			str = "{si,si,si,si,si,si,si,si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1],ch[start+2],
	                     ADC_Value[start+2],ch[start+3],ADC_Value[start+3],ch[start+4], ADC_Value[start+4],
                		ch[start+5],ADC_Value[start+5],ch[start+6],ADC_Value[start+6],ch[start+7],ADC_Value[start+7]);
		    break;
		case 7:
			str = "{si,si,si,si,si,si,si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1],ch[start+2],
	                     ADC_Value[start+2],ch[start+3],ADC_Value[start+3],ch[start+4], ADC_Value[start+4],
                		ch[start+5],ADC_Value[start+5],ch[start+6],ADC_Value[start+6]);
		    break;
		case 6:
			str = "{si,si,si,si,si,si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1],ch[start+2],
	                     ADC_Value[start+2],ch[start+3],ADC_Value[start+3],ch[start+4], ADC_Value[start+4],
                		ch[start+5],ADC_Value[start+5]);
		    break;
		case 5:
			str = "{si,si,si,si,si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1],ch[start+2],
	                     ADC_Value[start+2],ch[start+3],ADC_Value[start+3],ch[start+4], ADC_Value[start+4]);
		    break;
		case 4:
			str = "{si,si,si,si,si}";
		   root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1],ch[start+2],
	                     ADC_Value[start+2],ch[start+3],ADC_Value[start+3]);
		    break;
		case 3:
			str = "{si,si,si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1],ch[start+2],
	                     ADC_Value[start+2]);
		    break;
		case 2:
			str = "{si,si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start], ch[start+1],ADC_Value[start+1]);
		    break;
		case 1:
			str = "{si,si}";
		    root = json_pack(str,"adr",adress,ch[start], ADC_Value[start]);		    
		    break;
	};
	out = json_dumps(root, JSON_SORT_KEYS|JSON_REAL_PRECISION(8));
	if(mUsart == USART1) U1sendn(out); 
	else if(mUsart == USART3) U3sendn(out); 
	json_decref(root);
    free(out);  
}

//------------------------------------------------------
void SendRequest(USART_TypeDef* mUsart,uint8_t* txbuf,uint8_t* txlen,uint8_t adr,uint8_t command,__IO uint16_t *reg,uint16_t bitNum){
	int i,j;
	uint16_8_t crc;
	if(mUsart == USART3) GPIO_SetBits(GPIOC,GPIO_Pin_12);
	txbuf[0] = adr;
	txbuf[1] = command;
	txbuf[2] = bitNum;
	for(i=0,j=0;i<bitNum;i+=2,j++){
		txbuf[i+3] = reg[j]>>8;
		txbuf[i+4] = reg[j];
	}
	crc = CreatCRC16Value(txbuf, bitNum+3);
	txbuf[bitNum+3] = crc.Bit[1];
	txbuf[bitNum+4] = crc.Bit[0];
	*txlen = bitNum+5;	
	USART_ITConfig(mUsart, USART_IT_TXE, ENABLE);	    	
}



//-------------------------------------------------------------------------
void proc(USART_TypeDef* mUsart,uint8_t* txbuf,uint8_t* txlen,uint8_t* rxbuf, uint8_t* pr){		
	 	json_t* injson;
	char* str = (char*)rxbuf;
	if(strncmp(str,"{\"adr\":",7)==0){
	  	json_error_t *error;				
		injson = json_loads(str,JSON_REJECT_DUPLICATES,error);		
        uint16_t adr,start,len;	
        int isjson = json_unpack(injson, "{s:i,s:i,s:i}", "adr", &adr, "ch", &start, "num", &len);		
	    if(isjson == 0){			
			if(adr == adress) json(mUsart,start,len);
		}
		json_decref(injson);
	}		
	else if(rxbuf[0] == adress){		
		uint16_8_t crc = CreatCRC16Value(rxbuf, *pr-2);
		if((rxbuf[*pr-2] == crc.Bit[1])&&(rxbuf[*pr-1] == crc.Bit[0])){
			if(rxbuf[1] == 0x04){
				__IO uint16_t* Pr = ADC_Value + ((rxbuf[2]<<8) + rxbuf[3]);
				uint8_t num = (rxbuf[4]<<8) + rxbuf[5];
				SendRequest(mUsart,txbuf,txlen,rxbuf[0],rxbuf[1],Pr,num*2);
			}
		}
	}

	for(int i=0;i<*pr;i++){
		rxbuf[i] = 0;
	}
	*pr = 0;				
}

void usertProc(void){
	if(rxtim1 > 0){
		rxtim1--;
		if(rxtim1 == 0){
			proc(USART1,Txbuf1,&txlen1,Rxbuf1,&pr1);	
		}
	}
	if(rxtim3 > 0){
		rxtim3--;
		if(rxtim3 == 0){
			proc(USART3,Txbuf3,&txlen3,Rxbuf3,&pr3);
		}
	}	
}




