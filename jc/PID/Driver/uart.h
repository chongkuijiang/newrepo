#ifndef _UART_H
#define _UART_H




extern unsigned char uart1_rxbuf[256];			//�������ݰ�����
extern unsigned char uart1_txbuf[256];			//�������ݰ�����
extern unsigned char uart1_rxptr;				//�������ݰ���ǰָ��
extern unsigned char uart1_txptr;				//�������ݰ���ǰָ��
extern unsigned char uart1_txall;  			//�������ݰ��ܳ���
extern unsigned char uart1_time;				//�������ݰ��ָ��ʱ����
extern unsigned char uart1_rxflag;
void USART1_Configuration(void);
#endif /* _UART_C */