/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PROCESS_H
#define _PROCESS_H

#define L_DETECTION_FLAG   0x02   //��ż��
#define R_DETECTION_FLAG   0x01   //�ҽż��

#define MOTOR_POSITIVE   0x00   //����
#define MOTOR_NEGATIVE   0x01   //��ת

#define L_DETECTION  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) //��������ж�
#define R_DETECTION  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) //�ҽ������ж�

#define GAS_1 GPIO_SetBits(GPIOD,GPIO_Pin_2)  //����λ ����
#define GAS_0 GPIO_ResetBits(GPIOD,GPIO_Pin_2)  //����λ  ��ѹ
#define READ_GAS GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_2)  //��ȡ��ǰ�������

#define GUN_0 GPIO_SetBits(GPIOC,GPIO_Pin_10)  //������λ    �ϰ�
#define GUN_1 GPIO_ResetBits(GPIOC,GPIO_Pin_10)  //������λ      �°�

#define SCISSORS_0 GPIO_SetBits(GPIOC,GPIO_Pin_11)  //������λ
#define SCISSORS_1 GPIO_ResetBits(GPIOC,GPIO_Pin_11)  //������λ

#define PRESSURE_0 GPIO_SetBits(GPIOC,GPIO_Pin_12)  //ѹ����λ ����
#define PRESSURE_1 GPIO_ResetBits(GPIOC,GPIO_Pin_12)  //ѹ����λ  ��ѹ

#define READ_MOTORDIRECTOE GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)  //��ȡ��ǰ�������

#define MOTORDIRECTOE_0 GPIO_SetBits(GPIOA,GPIO_Pin_7) //�������
#define MOTORDIRECTOE_1 GPIO_ResetBits(GPIOA,GPIO_Pin_7)   //�������

#define MOTORRUN_0 GPIO_ResetBits(GPIOA,GPIO_Pin_6)    //���ֹͣ
#define MOTORRUN_1 GPIO_SetBits(GPIOA,GPIO_Pin_6)    //�������

#define AIR_0 GPIO_SetBits(GPIOA,GPIO_Pin_5)  //��ǹ�ر�
#define AIR_1 GPIO_ResetBits(GPIOA,GPIO_Pin_5)   //��ǹ����

//���߼�����
#define L0R0    0x00
//�ҽſ����߼�
#define L0R1     0x01   //�ҽŲ������̧��  �ҽ��߼���ʼ
#define R_L0R1   0x11   //�ҽŲ������̧��  ���ǰ��  
#define R_L1R1   0x13   //�ҽŲ�����Ų���  ���ǰ�� 

#define R_L1R0   0x12   //�ҽ�̧����Ų���  ���ֹͣ
#define R_L0R0   0x10   //�ҽ�̧�����̧��  ���ֹͣ  ���߼�
//����ſ����߼�
#define L1R0     0x02   //��Ų����ҽ�̧��   ������߼���ʼ
#define L_L1R0   0x22   //��Ų����ҽ�̧��   ���� 
#define L_L0R0   0x20   //���̧���ҽ�̧��   ����  ���߼�
//�ȷ�����߼�
#define L_L1R1   0x23   //��Ų����ҽŲ���  �ȷ��߼���ʼ
#define L_L0R1   0x21   //���̧���ҽŲ���  �ȷ��߼���ʼ

#define LR_L1R1  0x33   //��Ų����ҽŲ���  �ȷ⿪ʼ
#define LR_L0R1  0x31   //���̧���ҽŲ���  �ȷ���β

#define LR_L1R0  0x32   //��Ų����ҽ�̧��  �ȷ���ͣ
#define LR_L0R0  0x30   //���̧���ҽ�̧��  �ȷ�������߼�


#define LRE_L0R1 0x41  //�ȷ���β����
#define LRE_L1R0 0x42  //�ȷ���β�����ٴ���ѹ
#define LRE_L0R0 0x40  //�ȷ���β�������� ���߼�
//�Ƿ��߼�
#define L1R1     0x03   //���ҽŲ���

extern unsigned char Pedal;  //̤��ֵ
extern unsigned char Pedal_old;  //ǰ̤��ֵ
extern unsigned char Delay_flag; //������ʱ��ʶ
extern unsigned int Delay_Cnt;   //������ʱ����
extern unsigned char HotBlowerContinue_falg;//�ȷ������ʶ
extern unsigned char PidSet_flag;   //PID������ʼ����ʶ
extern unsigned char Tempcontrol_flag;   //�¿س�ʼ����ʶ

extern int Environment_temp;

//extern unsigned short test;

void Parameter(void); //����
void HotblowerProcess(void);
void Hotblowerend_Test(void);  // ��β����
void MotorControl(unsigned char director,unsigned short speed); //�������
void SingeControl(unsigned short addr); //���
void HotblowerTest(void);
void delay_ms(unsigned short delay_ms);
#endif /* _PROCESS_H */