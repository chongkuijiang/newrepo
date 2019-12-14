/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_C
#define _UART_C
#include "stm32f10x_lib.h"
//#include <stdio.h> 
#include "uart.h"


/*************** Uart 1 Used Variable Define *********************/
unsigned char uart1_rxbuf[256];			//接受数据包缓冲
unsigned char uart1_txbuf[256];			//发送数据包缓冲
unsigned char uart1_rxptr=0;				//接受数据包当前指针
unsigned char uart1_txptr=0;				//发送数据包当前指针
unsigned char uart1_txall=0;  			//发送数据包总长度
unsigned char uart1_time=0;				//接受数据包分割计时参数
unsigned char uart1_rxflag=0;

/*******************************************************************************
* Function Name  : USART1_Configuration
* Description    : Configures the uart1 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
 
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);  
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
  
}

/****************************************************************/
//int fputc(int ch, FILE * f)
//{
//   USART_SendData(USART1, (u8) ch);
//    /* Loop until the end of transmission */
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//    {
//    }
//    return ch;
//}

#endif /* _UART_C */