#ifndef _MODBUS_H
#define _MODBUS_H

#define FOOT1_ADR                   0 //����1 ��̤1 ֻ�� ������
#define FOOT2_ADR                   1 //����2 ��̤2 ֻ�� ������
#define Y1_ADR                      8 //Y1          ��д/������  
#define Y2_ADR                      9 //Y2          ��д/������ 
#define Y3_ADR                     10 //Y3          ��д/������ 
#define Y10_ADR                    11 //Y10         ��д/������ 
#define Y11_ADR                    12 //Y11         ��д/������ 
#define Y12_ADR                    13 //Y12         ��д/������ 
#define Y14_ADR                    14 //Y14         ��д/������ 
#define Y16_ADR                    15 //Y16         ��д/������ 
#define Y17_ADR                    16 //Y17         ��д/������ 
#define Y0_ADR                     21 //Y0          ��д/���� 
#define Y13_ADR                    22 //Y13         ��д/����
#define Y15_ADR                    23 //Y15         ��д/���� 
#define READ_CURRENTTEMP_ADR       24 //ʵʱ�¶�ֵ  ��д/����


#define TEMPCONTROL_ADR            31 //�¿ؿ���   ��д/������
#define PIDSET_ADR                 32 //PID������  ��д/������
#define TEST_ACTION                33 //���Զ�����ʼ���
#define AUTO_CUT                   34 //�Զ�������ʼ���
#define MOTOR_CLICK                35 //����㶯

#define SET_TEMP_ADR               51 //�趨�¶�ֵ  ��д/����
#define T0_ADR                     52 //��ǹͬ����ʱ������ͬ����ʱ����д/����
#define T1_ADR                     53//��ǹͬ����ʱ������ֹͣ��ʱ����д/����
#define T2_ADR                     54//��β���ȼ�ʱ��������ֹ��ʱ����д/����
#define T3_ADR                     55 //�ʹ�ʱ���ʱ����β�����ʱ����д/����
#define T4_ADR                     56 //�ʹ�����ʱ���ʱ���ʹ�������ʱ����д/����
#define T5_ADR                     57 //΢��ʱ�䣨����΢����ʱ����д/����

#define SET_POSITIVESPEED_ADR      58 //������ת(����)
#define SET_SENDWITHSPEED_ADR      59 //�ʹ��ٶ�(����)
#define SET_MIRCORBACKSPEED_ADR    60 //΢���ٶ�(����)
#define CUT_EN                     61 //����ʧ��
#define MOTOR_CLICKSPEED           62 //����㶯�ٶ�
#define T6_ADR                     63 //�ʹ���ʱ
#define PIDKC_ADR                  71 //KC
#define PIDTC_ADR                  73 //TC



extern unsigned char Modbus_Rxflag;
extern unsigned short memory[200];
void uart_crc(unsigned char* data , unsigned char length);
void uart1_dispose(void);



#endif /* _MODBUS_H */