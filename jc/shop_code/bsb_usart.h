
#ifndef __BSB_USART_H
#define __BSB_USART_H

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif


// Use Configuration Wizard in Context Menu 
//     <o0> 发送方式       <0=> 等侍方式  <1=> 中断方式
#define EN_SEND_TI   1	 
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口1   
//     <o1> 引脚       <0=> 无映射 TX_PA9,RX_PA10  <1=> 重映射 TX_PB6,RX_PB7 
//     <o2> 波特率     <9600=> 9600  <19200=> 19200  <38400=> 38400  <57600=> 57600   <115200=> 115200                     
//     <o3> 发送缓冲区大小 <0x0-0xFFF>
//     <o4> 接收缓冲区大小 <0x0-0xFFF>
//     <e5> 使能485  
//         <o6> 端口   <0=> GPIOA  <1=> GPIOB  <2=> GPIOC  <3=> GPIOD  <4=> GPIOE  <5=> GPIOF  <6=> GPIOG
//         <o7> 引脚   <0=> GPIO_Pin_0  <1=> GPIO_Pin_1  <2=> GPIO_Pin_2  <3=> GPIO_Pin_3  <4=> GPIO_Pin_4  <5=> GPIO_Pin_5  <6=> GPIO_Pin_6  <7=> GPIO_Pin_7  
//                     <8=> GPIO_Pin_8  <9=> GPIO_Pin_9  <10=> GPIO_Pin_10  <11=> GPIO_Pin_11  <12=> GPIO_Pin_12  <13=> GPIO_Pin_13  <14=> GPIO_Pin_14  <15=> GPIO_Pin_15  
//     </e>
//     <o8> 抢占优先级  <0x0-0x0F>
//     <o9> 子优先级    <0x0-0x0F>
// </e>
#define EN_USART_1   0
#define PinRemap_1   1
#define BaudRate_1   115200
#define Txsize_1     120
#define Rxsize_1     20
#define EN_485_1     1
#define Gpio485_1    0
#define Port485_1    11
#define Priority_1   1
#define SubPriority_1  1
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口2   
//     <o1> 引脚       <0=> 无映射 TX_PA2,RX_PA3  <1=> 重映射 TX_PD5,RX_PD6 
//     <o2> 波特率      <9600=> 9600  <19200=> 19200  <38400=> 38400  <57600=> 57600   <115200=> 115200         
//     <o3> 发送缓冲区大小 <0x0-0xFFF>
//     <o4> 接收缓冲区大小 <0x0-0xFFF>
//     <e5> 使能485  
//         <o6> 端口   <0=> GPIOA  <1=> GPIOB  <2=> GPIOC  <3=> GPIOD  <4=> GPIOE  <5=> GPIOF  <6=> GPIOG
//         <o7> 引脚   <0=> GPIO_Pin_0  <1=> GPIO_Pin_1  <2=> GPIO_Pin_2  <3=> GPIO_Pin_3  <4=> GPIO_Pin_4  <5=> GPIO_Pin_5  <6=> GPIO_Pin_6  <7=> GPIO_Pin_7  
//                     <8=> GPIO_Pin_8  <9=> GPIO_Pin_9  <10=> GPIO_Pin_10  <11=> GPIO_Pin_11  <12=> GPIO_Pin_12  <13=> GPIO_Pin_13  <14=> GPIO_Pin_14  <15=> GPIO_Pin_15  
//     </e>
//     <o8> 抢占优先级  <0x0-0x0F>
//     <o9> 子优先级    <0x0-0x0F>
// </e>
#define EN_USART_2   1
#define PinRemap_2   0
#define BaudRate_2   115200
#define Txsize_2     20
#define Rxsize_2     20
#define EN_485_2     1
#define Gpio485_2    0
#define Port485_2    10
#define Priority_2   1
#define SubPriority_2  2
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口3   
//     <o1> 引脚       <0=> 无映射 TX_PB10,RX_PB11  <1=> 部分映射 TX_PC10,RX_PC11  <2=> 完全映射 TX_PD8,RX_PD9 
//     <o2> 波特率     <9600=> 9600  <19200=> 19200  <38400=> 38400  <57600=> 57600   <115200=> 115200         
//     <o3> 发送缓冲区大小 <0x0-0xFFF>
//     <o4> 接收缓冲区大小 <0x0-0xFFF>
//     <e5> 使能485  
//         <o6> 端口   <0=> GPIOA  <1=> GPIOB  <2=> GPIOC  <3=> GPIOD  <4=> GPIOE  <5=> GPIOF  <6=> GPIOG
//         <o7> 引脚   <0=> GPIO_Pin_0  <1=> GPIO_Pin_1  <2=> GPIO_Pin_2  <3=> GPIO_Pin_3  <4=> GPIO_Pin_4  <5=> GPIO_Pin_5  <6=> GPIO_Pin_6  <7=> GPIO_Pin_7  
//                     <8=> GPIO_Pin_8  <9=> GPIO_Pin_9  <10=> GPIO_Pin_10  <11=> GPIO_Pin_11  <12=> GPIO_Pin_12  <13=> GPIO_Pin_13  <14=> GPIO_Pin_14  <15=> GPIO_Pin_15  
//     </e>
//     <o8> 抢占优先级  <0x0-0x0F>
//     <o9> 子优先级    <0x0-0x0F>
// </e>
#define EN_USART_3   0
#define PinRemap_3   1
#define BaudRate_3   9600
#define Txsize_3     20
#define Rxsize_3     20
#define EN_485_3     1
#define Gpio485_3    2
#define Port485_3    13
#define Priority_3   1
#define SubPriority_3  2
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口4   
//     <o1> 引脚       <0=> 无映射 TX_PC10,RX_PC11  
//     <o2> 波特率     <9600=> 9600  <19200=> 19200  <38400=> 38400  <57600=> 57600   <115200=> 115200         
//     <o3> 发送缓冲区大小 <0x0-0xFFF>
//     <o4> 接收缓冲区大小 <0x0-0xFFF>
//     <e5> 使能485  
//         <o6> 端口   <0=> GPIOA  <1=> GPIOB  <2=> GPIOC  <3=> GPIOD  <4=> GPIOE  <5=> GPIOF  <6=> GPIOG
//         <o7> 引脚   <0=> GPIO_Pin_0  <1=> GPIO_Pin_1  <2=> GPIO_Pin_2  <3=> GPIO_Pin_3  <4=> GPIO_Pin_4  <5=> GPIO_Pin_5  <6=> GPIO_Pin_6  <7=> GPIO_Pin_7  
//                     <8=> GPIO_Pin_8  <9=> GPIO_Pin_9  <10=> GPIO_Pin_10  <11=> GPIO_Pin_11  <12=> GPIO_Pin_12  <13=> GPIO_Pin_13  <14=> GPIO_Pin_14  <15=> GPIO_Pin_15  
//     </e>
//     <o8> 抢占优先级  <0x0-0x0F>
//     <o9> 子优先级    <0x0-0x0F>
// </e>
#define EN_USART_4   0
#define PinRemap_4   0
#define BaudRate_4   9600
#define Txsize_4     20
#define Rxsize_4     20
#define EN_485_4     1
#define Gpio485_4    2
#define Port485_4    13
#define Priority_4   1
#define SubPriority_4  2
//----------------------------------------------------------------------------------------------------------------------
// <e0> 使能串口5   
//     <o1> 引脚       <0=> 无映射 TX_PC12 ,RX_D2
//     <o2> 波特率     <9600=> 9600  <19200=> 19200  <38400=> 38400  <57600=> 57600   <115200=> 115200         
//     <o3> 发送缓冲区大小 <0x0-0xFFF>
//     <o4> 接收缓冲区大小 <0x0-0xFFF>
//     <e5> 使能485  
//         <o6> 端口   <0=> GPIOA  <1=> GPIOB  <2=> GPIOC  <3=> GPIOD  <4=> GPIOE  <5=> GPIOF  <6=> GPIOG
//         <o7> 引脚   <0=> GPIO_Pin_0  <1=> GPIO_Pin_1  <2=> GPIO_Pin_2  <3=> GPIO_Pin_3  <4=> GPIO_Pin_4  <5=> GPIO_Pin_5  <6=> GPIO_Pin_6  <7=> GPIO_Pin_7  
//                     <8=> GPIO_Pin_8  <9=> GPIO_Pin_9  <10=> GPIO_Pin_10  <11=> GPIO_Pin_11  <12=> GPIO_Pin_12  <13=> GPIO_Pin_13  <14=> GPIO_Pin_14  <15=> GPIO_Pin_15  
//     </e>
//     <o8> 抢占优先级  <0x0-0x0F>
//     <o9> 子优先级    <0x0-0x0F>
// </e>
#define EN_USART_5   0
#define PinRemap_5   0
#define BaudRate_5   9600
#define Txsize_5     20
#define Rxsize_5     20
#define EN_485_5     1
#define Gpio485_5    2
#define Port485_5    13
#define Priority_5   1
#define SubPriority_5  2
//-------------------------------------------------------------------------------------------------------------------------
// end of configuration section 

typedef void (*ReceiveCallback)(uint8_t* rxbuf,uint16_t Size);

	 
typedef enum
{
	U1=0,U2,U3,U4,U5
}UsartId_t;


void Usart_Init(void);
void Usart_Dispose(void);

#if EN_SEND_TI
void SendTI(UsartId_t uid,uint8_t* txbuf,uint16_t bufSize);
void SendTI_n(UsartId_t uid,char* txbuf);

#else
void SendNormal(UsartId_t uid,uint8_t* txbuf,uint16_t bufSize);
void SendNormal_n(UsartId_t uid,char* txbuf);

#endif

void Config_ReceiveCallback(UsartId_t uid,ReceiveCallback call);




#ifdef __cplusplus
}
#endif

#endif


