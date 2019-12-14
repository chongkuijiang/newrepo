#ifndef _PID_C
#define _PID_C
#include "stm32f10x_lib.h"
#include "Dac.h"
#include "Max6675.h"
#include "Modbus.h"
#include "Pid.h"
#include "Flash.h"
#include "Fuzzy.h"
#include "Process.h"

volatile u8  once_add_1_flag     = 0;
volatile u8  Pid_Setok = 0;

unsigned short Sec_Cnt=0;
unsigned char zero_across_counter = 0;
unsigned long pid_self_time_cnt = 0;
unsigned char hot_counter    = 0;
unsigned char cool_counter    = 0;

float  Proportion  = 0.0;           //  �������� Proportional Const
float  Integral    = 0.0;           //  ���ֳ��� Integral Const        
float  Derivative  = 0.0;           //  ΢�ֳ��� Derivative Const

float  Error       = 0.0;           //  e(k)
float  LastError   = 0.0;           //  e(k-1)
float  PrevError   = 0.0;           //  e(k-2)
float  SumError    = 0.0;           //  Sums of Errors

//pid
float           SV_value           = 50.0; //�趨�¶�ֵ
float           PV_value           = 0.0;  //���ڲ������ĵ�ǰ�¶�ֵ
float           KV_value           = 0.0;  //�¶Ȳ�ֵ

//float           PV_Oldvalue           = 0.0;  //���ڲ������ĵ�ǰ�¶�ֵ
int  pid_result = 0;                //DAC���

float max_min_temp  = 0.0 ;  //�����С�¶���ʱ���
unsigned char max_min_cnt=0; //�����С�¶�Ѱ�Ҽ���
unsigned char max_flag=0;    //Ѱ������¶ȱ�־
unsigned char min_flag=0;    //Ѱ����С�¶ȱ�־
unsigned char Mutation_cnt=0; //ͻ��ֵ�жϼ���

float T_Hight = 0.0;
float T_LOW   = 100.0;           //�¶�
unsigned long TIME_Hight = 0;
unsigned long TIME_LOW   = 0;	//�������
unsigned long TIME_Hight_LOW = 0;//�����С�¶�ʱ����ʱ���

float  KC = 1.0;  //�ٽ����ϵ��  ��ʼĬ�ϵ�ֵ
unsigned int TC = 40;   //������      ��ʼĬ�ϵ�ֵ

//float pid_self_calc_buffer[4]={0.0,0.0,0.0,0.0};

//0 ��� �ٽ����� KC �� ������ TC
  // KC = (4*d)/(3.14*A)  ---> d = 5(�����ֵ) ; A = ��¼���¶����ֵ�����ֵ�Ĳ�ֵ��0.5�� ����(T_Hight - T_LOW)*0.5
  // KC = (4*5)/(3.14*((T_Hight - T_LOW)*0.5)) = 40/3.14/(T_Hight - T_LOW) =  12.7/(T_Hight - T_LOW)
  // KC = 12.7/(T_Hight - T_LOW)
  // TC = 2 * (TIME_Hight - TIME_LOW) ---> 2 * ( �ߵ��¶ȶ�Ӧʱ�� - �͵��¶ȶ�Ӧʱ�� )
  // TC = 2 * (TIME_Hight - TIME_LOW)
  //1 ��� ����ı���ϵ�� �������� ΢������
  //Proportion = 0.6*KC
  //I_value    = 0.5*TC
  //D_value    = 0.125*TC
  //2 �������� ������ ����ϵ�� ΢��ϵ�� 
  //P_value     = (1/Proportion)*100
  //Integral	  = Proportion/I_value = (0.6*KC)/(0.5*TC)
  //Derivative  = Proportion*D_value = (0.6*KC)*(0.125*TC)  
  //3��ʾ�õ�3��������ֵ
  //P_value     = (1/Proportion)*100  �ٷֱ�
  //I_value     = 0.5*TC				�� 
  //D_value     = 0.125*TC			�� 
  //4pid�����õ�3��������ֵ
  //Proportion  = 0.6*KC
  //Integral	  = Proportion/I_value = (0.6*KC)/(0.5*TC)
  //Derivative  = Proportion*D_value = (0.6*KC)*(0.125*TC)  
  
  //KC = 21.4;//test 
  //TC = 471;//test 
void pid_pro(void)//pid ������� ppppppppppppppppppppppppppppp
{
  int temp_pid;
  Error = SV_value - PV_value;                 // ��ǰƫ�� e(k)
  if(( Error < max_value_error  ) && ( Error > (min_value_error)  ))//ֻ����һ�����²Χ�ڲ�pid����
  {   
//    if(Fuzzy_flag)
//      Control_PID(SV_value,PV_value,&Proportion,&Integral,&Derivative); //ģ������
    //λ��ʽPID
    //U(K)=Kp*e(k)+ki*Sum(e)+kd*(e(k)-e(k-1))
    SumError += Error;
    temp_pid  =  (int)(((Proportion * Error) + (Integral * SumError) + (Derivative * (Error - LastError)))*100);    
    /*
    //����ʽPID
    //U(K)=DU(K)+U(k-1)
    //DU(K)=Kp*(e(k)-e(k-1))+ki*e(k)+kd*((e(k)-e(k-1))-(e(k-1)-e(k-2)))
    //DU(K)=Kp*(e(k)-e(k-1))+ki*e(k)+kd*(e(k)-2*e(k-1)+e(k-2))
    temp_pid=pid_result+(int)((Proportion*(Error-LastError)) + (Integral*Error ) + (Derivative*(Error-2*LastError+PrevError))); */  
    pid_result=temp_pid;    //�ϴ�temp_pid
    PrevError = LastError; //e(k-2)
    LastError = Error;     //e(k-1)
    
    //temp_pid  =  (int)(temp_pid * 0.5) ;//�����������
  }
  else//ֻ�п�������
  {
    if( Error >= max_value_error )//Զ���ڵ�ǰ�¶ȣ�����
    {
      temp_pid = 1000;
      //temp_pid = 80;
    }
    else if( Error <= (min_value_error) )//ԶС�ڵ�ǰ�¶ȣ�������
    {
      temp_pid = 0;
    }
  }
  if( temp_pid < 0 )
  {
    temp_pid = 0;
  }
  else if( temp_pid >1000 )
  {
    temp_pid = 1000;
  } 
  //Dac_Out(40 + (u8)(temp_pid * 1.97) );//������һ���Ľ��������� 
  Dac_Out(596 + (u8)(temp_pid * 2.83));
}


//PID������
void Pid_Sinceset(void)
{
  if(pid_self_time_cnt > 300000) // ����ܵ�������ʱ�������5MIN����˵������ʧ��
  {
    pid_self_time_cnt = 0;			
    KC = 1.0;//�ٽ����ϵ��  ��ʼĬ�ϵ�ֵ
    TC = 40; //������    ��ʼĬ�ϵ�ֵ 
    memory[PIDSET_ADR]=0;     //�ر�PID������
    Pid_Setok=0;   //PID������ʧ��
  }
  //����on/off����
  if( SV_value >= PV_value )//��������
  {
    hot_counter++;
    cool_counter=0;
    if(hot_counter > D_Error)//����3�ζ���һ���Ľ�� ˵��ȷ�� SV_value >= PV_value
    {
      hot_counter=0;
      Dac_Out(3426);//ȫ�ټ���
      if(once_add_1_flag == 0)
      {
        once_add_1_flag = 1;
        zero_across_counter++;//���������¶ȼ���
        T_LOW=PV_value;  //��ȡ��С�¶ȳ�ʼֵ
        max_min_temp=T_LOW; //��ȡ��С�¶Ȳο�ֵ
        TIME_LOW=pid_self_time_cnt;//��ȡ��С�¶ȵ�ʱ��
        //PV_Oldvalue=PV_value; //��¼�ϴ��¶�Ϊ�����¶�  
        max_min_cnt=0;  //����������
        Mutation_cnt=0; //��ͻ�����
        min_flag=1;  //��ʼѰ������¶�
        max_flag=0;  //ǿ��ֹͣѰ������¶�
      }
    }
  }
  else//��ǰ�¶� ���� �趨�¶� ֹͣ����
  { 
    cool_counter++;
    hot_counter=0;
    if(cool_counter > D_Error)
    {
      cool_counter=0;
      Dac_Out(596);//������
      if(once_add_1_flag == 1)
      {
        once_add_1_flag = 0;
        zero_across_counter++;//���������¶ȼ���
        T_Hight=PV_value;  //��ȡ����¶ȳ�ʼֵ
        max_min_temp=T_Hight;//��ȡ����¶Ȳο�ֵ
        TIME_Hight=pid_self_time_cnt;//��ȡ����¶ȵ�ʱ��
        //PV_Oldvalue=PV_value; //��¼�ϴ��¶�Ϊ�����¶�  
        max_min_cnt=0;  //����������
        Mutation_cnt=0; //��ͻ�����
        max_flag=1;  //��ʼѰ������¶�
        min_flag=0;  //ǿ��ֹͣѰ������¶�
      } 
    }
    //��3��4�׶� Ѱ���������¶�  �Լ����ֵ�ʱ��
    if((zero_across_counter == 3 ) || (zero_across_counter == 4 ))
    {
      if(max_flag)  //������¶Ⱥ�����¶ȵ�ʱ��
      {
        if(PV_value>max_min_temp) //��ǰ�¶ȴ����ϴ�����¶�
        {
          if(PV_value<=(max_min_temp+max_min_error)) //��ǰ�¶�С���ϴ�����¶�+max_min_error
          {
            max_min_cnt=0;          //����������
            Mutation_cnt=0; //��ͻ�����
            max_min_temp=PV_value; //��¼��ʱ�¶�
            TIME_Hight_LOW=pid_self_time_cnt;//��ȡ����¶ȵ�ʱ��
          }
          else
          {
            Mutation_cnt++; //ͻ���������
            if(Mutation_cnt>D_Mutation) //ͻ������ﵽĳ��ֵ �ж�Ϊ����ֵ
            {
              max_min_cnt=0;   //����������
              Mutation_cnt=0; //��ͻ�����
              max_min_temp=PV_value; //��¼��ʱ����¶�
              TIME_Hight_LOW=pid_self_time_cnt;//��ȡ��ʱ����¶ȵ�ʱ��
            }
          }
        }
        else if(PV_value<max_min_temp)//��ǰ�¶�С���ϴ���ʱ����¶�
        {
          max_min_cnt++;
          if(max_min_cnt>D_MaxMin)  //����D_MaxMin�ε�����ʱ����¶�
          {
            max_flag=0;                //ֹͣѰ������¶�
            max_min_cnt=0;  //����������
            T_Hight=max_min_temp;      //��ȡ����¶�
            TIME_Hight=TIME_Hight_LOW; //��ȡ����¶ȵ�ʱ��
          }   
        }
      }
      else if(min_flag)  //������¶Ⱥ�����¶ȵ�ʱ��
      {
        if(PV_value<max_min_temp) //��ǰ�¶�С���ϴ���С�¶�
        {
          if(PV_value>=(max_min_temp-max_min_error)) //��ǰ�¶ȴ����ϴ���С�¶�-max_min_error
          {
            max_min_cnt=0;          //����������
            Mutation_cnt=0; //��ͻ�����
            max_min_temp=PV_value; //��¼��ʱ��С�¶�
            TIME_Hight_LOW=pid_self_time_cnt;//��ȡ��ʱ��С�¶ȵ�ʱ��
          }
          else
          {
            Mutation_cnt++; //ͻ���������
            if(Mutation_cnt>D_Mutation) //ͻ������ﵽĳ��ֵ �ж�Ϊ����ֵ
            {
              max_min_cnt=0;          //����������
              Mutation_cnt=0; //��ͻ�����
              max_min_temp=PV_value; //��¼��ʱ��С�¶�
              TIME_Hight_LOW=pid_self_time_cnt;//��ȡ��ʱ��С�¶ȵ�ʱ��
            }
          }
        }
        else if(PV_value>max_min_temp) //��ǰ�¶ȴ����ϴ���ʱ��С�¶�
        {
          max_min_cnt++;
          if(max_min_cnt>D_MaxMin)  //����D_MaxMin�δ�����ʱ��С�¶�
          {
            max_flag=0;                //ֹͣѰ������¶�
            max_min_cnt=0;  //����������
            T_LOW=max_min_temp;      //��ȡ��С�¶�
            TIME_LOW=TIME_Hight_LOW; //��ȡ��С�¶ȵ�ʱ��
          }   
        }
      } 
    }
    else if(zero_across_counter == 5 )//4�ι�0 ��˵���������� �����ɹ�
    {
      zero_across_counter = 0;				
      //���� T_Hight T_LOW TIME_Hight TIME_LOW ��4��ֵ 
      //��������4��ֵ  KC �� TC ��ֵ���������    
      KC = (8*D/(Pi*(T_Hight - T_LOW)));
      TC = (unsigned int)((2.0 * (TIME_Hight - TIME_LOW))/1000);//�����¼�� ����¶� �� ����¶ȶ�Ӧ��ʱ�� ��ô���������ʽ��TC = 2 * (TIME_Hight - TIME_LOW);
      memory[PIDSET_ADR]=0;     //�ر�PID������
      Pid_Setok=1;   //PID�������ɹ�
    }
    //PV_Oldvalue=PV_value; //��¼�ϴ��¶� 
  }
}

void PidParameter_Sinceset(void)//PID����������
{
  //��¼�˿̵�״̬ ���趨�¶��Ƿ� ���ڻ���� ��ǰ�¶� 
  //PV_value = read_max6675_temper();
  if( SV_value >= PV_value )//�趨�¶� ���� ���� ����  ��ǰ�¶�  ��������
  {
    once_add_1_flag = 0;
  }
  else//�趨�¶� ���� ��ǰ�¶�
  {
    once_add_1_flag = 1;
  } 
  zero_across_counter = 0;
  pid_self_time_cnt = 0;      

  TIME_Hight = 0;
  TIME_LOW   = 0;	//�����0.2s 
}

void PidParameter_pro(void)//PID����
{
  
 KC=*((float *)(&memory[PIDKC_ADR]));
 TC=*((int *)(&memory[PIDTC_ADR]));
  if(KC > 1666.0 )
  {
    KC = 1666.0;//��Ӧ ������Ϊ 0.1%
  }
  else if(KC < 0.5 )
  {
    KC = 0.5;//��Ӧ ������Ϊ 200.0%
  }
  if(TC > 7200 )
  {
    TC = 7200;
  }
  else if(TC < 8 )
  {
    TC = 8;
  }   
  Proportion  = 0.6*KC;
  Integral	= (0.6*KC)/(0.5*TC);
  Derivative  = (0.6*KC)*(0.125*TC); 
  Fuzzy_flag=0; //�ر�ģ��
}


void Preheat(void)// Ԥ��
{
  unsigned short preheat_cnt=0;
  unsigned int  preheat_out=596;
  while(preheat_cnt<240) //2��
  {
    preheat_cnt++;
    if(preheat_out<3426)
      preheat_out+=10;
    else
      preheat_out=3426;
     Dac_Out(preheat_out);
    if(PV_value>=(SV_value-50))
      break;
    delay_ms(500); 
  }
}
#endif /* _PID_C */