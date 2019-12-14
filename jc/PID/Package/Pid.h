#ifndef _PID_H
#define _PID_H

#define     max_value_error               50.0
#define     min_value_error              -50.0//��������
#define     max_min_error                 5   //5���϶�

#define     D_Mutation                    10  //1S  10*00ms
#define     D_Error                       10  //1S  10*00ms
#define     D_MaxMin                      10  //1S  20*00ms
#define     Pi                            3.15159 
#define     D                             5 

extern volatile u8  once_add_1_flag;
extern volatile u8  Pid_Setok;

extern unsigned short Sec_Cnt;
extern unsigned char zero_across_counter;
extern unsigned long pid_self_time_cnt;
extern unsigned char hot_counter;
extern unsigned char cool_counter;

extern float  Proportion; //  �������� Proportional Const
extern float  Integral;   //  ���ֳ��� Integral Const        
extern float  Derivative; //  ΢�ֳ��� Derivative Const

extern float  Error;      //  e(k)
extern float  LastError;  //  e(k-1)
extern float  PrevError;  //  e(k-2)
extern float  SumError;   //  Sums of Errors

//pid
extern float  SV_value;   //�趨�¶�ֵ
extern float  PV_value;   //���ڲ������ĵ�ǰ�¶�ֵ
extern float  KV_value;   //�¶Ȳ�ֵ
extern int  pid_result;   //DAC���

extern float max_min_temp ;         //�����С�¶���ʱ���
extern unsigned char max_min_cnt;   //�����С�¶�Ѱ�Ҽ���
extern unsigned char max_flag;      //Ѱ������¶ȱ�־
extern unsigned char min_flag;      //Ѱ����С�¶ȱ�־

extern float T_Hight;
extern float T_LOW;       //�¶�

extern unsigned long TIME_Hight;
extern unsigned long TIME_LOW;	//�������
extern unsigned long TIME_Hight_LOW;//�����С�¶�ʱ����ʱ���

extern float  KC ;        //�ٽ����ϵ��  ��ʼĬ�ϵ�ֵ
extern unsigned int TC;   //������      ��ʼĬ�ϵ�ֵ

void pid_pro(void);
void Pid_Sinceset(void);
void PidParameter_Sinceset(void);//PID����������
void PidParameter_pro(void);//PID����
void Preheat(void);// Ԥ��
#endif /* _PID_H */