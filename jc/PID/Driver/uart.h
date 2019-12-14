#ifndef _UART_H
#define _UART_H




extern unsigned char uart1_rxbuf[256];			//接受数据包缓冲
extern unsigned char uart1_txbuf[256];			//发送数据包缓冲
extern unsigned char uart1_rxptr;				//接受数据包当前指针
extern unsigned char uart1_txptr;				//发送数据包当前指针
extern unsigned char uart1_txall;  			//发送数据包总长度
extern unsigned char uart1_time;				//接受数据包分割计时参数
extern unsigned char uart1_rxflag;
void USART1_Configuration(void);
#endif /* _UART_C */