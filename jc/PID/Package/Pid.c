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

float  Proportion  = 0.0;           //  比例常数 Proportional Const
float  Integral    = 0.0;           //  积分常数 Integral Const        
float  Derivative  = 0.0;           //  微分常数 Derivative Const

float  Error       = 0.0;           //  e(k)
float  LastError   = 0.0;           //  e(k-1)
float  PrevError   = 0.0;           //  e(k-2)
float  SumError    = 0.0;           //  Sums of Errors

//pid
float           SV_value           = 50.0; //设定温度值
float           PV_value           = 0.0;  //用于参与计算的当前温度值
float           KV_value           = 0.0;  //温度差值

//float           PV_Oldvalue           = 0.0;  //用于参与计算的当前温度值
int  pid_result = 0;                //DAC输出

float max_min_temp  = 0.0 ;  //最大最小温度临时存放
unsigned char max_min_cnt=0; //最大最小温度寻找计数
unsigned char max_flag=0;    //寻找最大温度标志
unsigned char min_flag=0;    //寻找最小温度标志
unsigned char Mutation_cnt=0; //突变值判断计数

float T_Hight = 0.0;
float T_LOW   = 100.0;           //温度
unsigned long TIME_Hight = 0;
unsigned long TIME_LOW   = 0;	//具体的秒
unsigned long TIME_Hight_LOW = 0;//最大最小温度时间临时存放

float  KC = 1.0;  //临界比例系数  初始默认的值
unsigned int TC = 40;   //振荡周期      初始默认的值

//float pid_self_calc_buffer[4]={0.0,0.0,0.0,0.0};

//0 算出 临界增益 KC 及 振荡周期 TC
  // KC = (4*d)/(3.14*A)  ---> d = 5(输出幅值) ; A = 记录的温度最高值与最低值的差值的0.5倍 即：(T_Hight - T_LOW)*0.5
  // KC = (4*5)/(3.14*((T_Hight - T_LOW)*0.5)) = 40/3.14/(T_Hight - T_LOW) =  12.7/(T_Hight - T_LOW)
  // KC = 12.7/(T_Hight - T_LOW)
  // TC = 2 * (TIME_Hight - TIME_LOW) ---> 2 * ( 高点温度对应时间 - 低点温度对应时间 )
  // TC = 2 * (TIME_Hight - TIME_LOW)
  //1 算出 具体的比例系数 积分秒数 微分秒数
  //Proportion = 0.6*KC
  //I_value    = 0.5*TC
  //D_value    = 0.125*TC
  //2 算出具体的 比例带 积分系数 微分系数 
  //P_value     = (1/Proportion)*100
  //Integral	  = Proportion/I_value = (0.6*KC)/(0.5*TC)
  //Derivative  = Proportion*D_value = (0.6*KC)*(0.125*TC)  
  //3显示用的3个变量的值
  //P_value     = (1/Proportion)*100  百分比
  //I_value     = 0.5*TC				秒 
  //D_value     = 0.125*TC			秒 
  //4pid计算用的3个变量的值
  //Proportion  = 0.6*KC
  //Integral	  = Proportion/I_value = (0.6*KC)/(0.5*TC)
  //Derivative  = Proportion*D_value = (0.6*KC)*(0.125*TC)  
  
  //KC = 21.4;//test 
  //TC = 471;//test 
void pid_pro(void)//pid 控制输出 ppppppppppppppppppppppppppppp
{
  int temp_pid;
  Error = SV_value - PV_value;                 // 当前偏差 e(k)
  if(( Error < max_value_error  ) && ( Error > (min_value_error)  ))//只有在一定的温差范围内才pid计算
  {   
//    if(Fuzzy_flag)
//      Control_PID(SV_value,PV_value,&Proportion,&Integral,&Derivative); //模糊控制
    //位置式PID
    //U(K)=Kp*e(k)+ki*Sum(e)+kd*(e(k)-e(k-1))
    SumError += Error;
    temp_pid  =  (int)(((Proportion * Error) + (Integral * SumError) + (Derivative * (Error - LastError)))*100);    
    /*
    //增量式PID
    //U(K)=DU(K)+U(k-1)
    //DU(K)=Kp*(e(k)-e(k-1))+ki*e(k)+kd*((e(k)-e(k-1))-(e(k-1)-e(k-2)))
    //DU(K)=Kp*(e(k)-e(k-1))+ki*e(k)+kd*(e(k)-2*e(k-1)+e(k-2))
    temp_pid=pid_result+(int)((Proportion*(Error-LastError)) + (Integral*Error ) + (Derivative*(Error-2*LastError+PrevError))); */  
    pid_result=temp_pid;    //上次temp_pid
    PrevError = LastError; //e(k-2)
    LastError = Error;     //e(k-1)
    
    //temp_pid  =  (int)(temp_pid * 0.5) ;//输出比例控制
  }
  else//只有开关作用
  {
    if( Error >= max_value_error )//远大于当前温度，加热
    {
      temp_pid = 1000;
      //temp_pid = 80;
    }
    else if( Error <= (min_value_error) )//远小于当前温度，不加热
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
  //Dac_Out(40 + (u8)(temp_pid * 1.97) );//根据上一步的结果控制输出 
  Dac_Out(596 + (u8)(temp_pid * 2.83));
}


//PID自整定
void Pid_Sinceset(void)
{
  if(pid_self_time_cnt > 300000) // 如果总的自整定时间大于了5MIN，则说明整定失败
  {
    pid_self_time_cnt = 0;			
    KC = 1.0;//临界比例系数  初始默认的值
    TC = 40; //振荡周期    初始默认的值 
    memory[PIDSET_ADR]=0;     //关闭PID自整定
    Pid_Setok=0;   //PID自整定失败
  }
  //基本on/off控制
  if( SV_value >= PV_value )//启动加热
  {
    hot_counter++;
    cool_counter=0;
    if(hot_counter > D_Error)//连续3次都是一样的结果 说明确定 SV_value >= PV_value
    {
      hot_counter=0;
      Dac_Out(3426);//全速加热
      if(once_add_1_flag == 0)
      {
        once_add_1_flag = 1;
        zero_across_counter++;//到达设置温度计数
        T_LOW=PV_value;  //获取最小温度初始值
        max_min_temp=T_LOW; //获取最小温度参考值
        TIME_LOW=pid_self_time_cnt;//获取最小温度的时间
        //PV_Oldvalue=PV_value; //记录上次温度为现在温度  
        max_min_cnt=0;  //清连续计数
        Mutation_cnt=0; //清突变计数
        min_flag=1;  //开始寻找最低温度
        max_flag=0;  //强制停止寻找最高温度
      }
    }
  }
  else//当前温度 大于 设定温度 停止加热
  { 
    cool_counter++;
    hot_counter=0;
    if(cool_counter > D_Error)
    {
      cool_counter=0;
      Dac_Out(596);//不加热
      if(once_add_1_flag == 1)
      {
        once_add_1_flag = 0;
        zero_across_counter++;//到达设置温度计数
        T_Hight=PV_value;  //获取最大温度初始值
        max_min_temp=T_Hight;//获取最大温度参考值
        TIME_Hight=pid_self_time_cnt;//获取最大温度的时间
        //PV_Oldvalue=PV_value; //记录上次温度为现在温度  
        max_min_cnt=0;  //清连续计数
        Mutation_cnt=0; //清突变计数
        max_flag=1;  //开始寻找最高温度
        min_flag=0;  //强制停止寻找最低温度
      } 
    }
    //第3、4阶段 寻找最高最低温度  以及出现的时间
    if((zero_across_counter == 3 ) || (zero_across_counter == 4 ))
    {
      if(max_flag)  //找最高温度和最高温度的时间
      {
        if(PV_value>max_min_temp) //当前温度大于上次最大温度
        {
          if(PV_value<=(max_min_temp+max_min_error)) //当前温度小于上次最大温度+max_min_error
          {
            max_min_cnt=0;          //清连续计数
            Mutation_cnt=0; //清突变计数
            max_min_temp=PV_value; //记录临时温度
            TIME_Hight_LOW=pid_self_time_cnt;//获取最大温度的时间
          }
          else
          {
            Mutation_cnt++; //突变计数开启
            if(Mutation_cnt>D_Mutation) //突变计数达到某个值 判定为正常值
            {
              max_min_cnt=0;   //清连续计数
              Mutation_cnt=0; //清突变计数
              max_min_temp=PV_value; //记录临时最大温度
              TIME_Hight_LOW=pid_self_time_cnt;//获取临时最大温度的时间
            }
          }
        }
        else if(PV_value<max_min_temp)//当前温度小于上次临时最大温度
        {
          max_min_cnt++;
          if(max_min_cnt>D_MaxMin)  //连续D_MaxMin次低于临时最大温度
          {
            max_flag=0;                //停止寻找最高温度
            max_min_cnt=0;  //清连续计数
            T_Hight=max_min_temp;      //获取最大温度
            TIME_Hight=TIME_Hight_LOW; //获取最大温度的时间
          }   
        }
      }
      else if(min_flag)  //找最低温度和最低温度的时间
      {
        if(PV_value<max_min_temp) //当前温度小于上次最小温度
        {
          if(PV_value>=(max_min_temp-max_min_error)) //当前温度大于上次最小温度-max_min_error
          {
            max_min_cnt=0;          //清连续计数
            Mutation_cnt=0; //清突变计数
            max_min_temp=PV_value; //记录临时最小温度
            TIME_Hight_LOW=pid_self_time_cnt;//获取临时最小温度的时间
          }
          else
          {
            Mutation_cnt++; //突变计数开启
            if(Mutation_cnt>D_Mutation) //突变计数达到某个值 判定为正常值
            {
              max_min_cnt=0;          //清连续计数
              Mutation_cnt=0; //清突变计数
              max_min_temp=PV_value; //记录临时最小温度
              TIME_Hight_LOW=pid_self_time_cnt;//获取临时最小温度的时间
            }
          }
        }
        else if(PV_value>max_min_temp) //当前温度大于上次临时最小温度
        {
          max_min_cnt++;
          if(max_min_cnt>D_MaxMin)  //连续D_MaxMin次大于临时最小温度
          {
            max_flag=0;                //停止寻找最高温度
            max_min_cnt=0;  //清连续计数
            T_LOW=max_min_temp;      //获取最小温度
            TIME_LOW=TIME_Hight_LOW; //获取最小温度的时间
          }   
        }
      } 
    }
    else if(zero_across_counter == 5 )//4次过0 则说明出现了振荡 整定成功
    {
      zero_across_counter = 0;				
      //计算 T_Hight T_LOW TIME_Hight TIME_LOW 这4个值 
      //根据以上4个值  KC 与 TC 的值便会计算出来    
      KC = (8*D/(Pi*(T_Hight - T_LOW)));
      TC = (unsigned int)((2.0 * (TIME_Hight - TIME_LOW))/1000);//如果记录了 最低温度 与 最高温度对应的时间 那么沿用这个公式：TC = 2 * (TIME_Hight - TIME_LOW);
      memory[PIDSET_ADR]=0;     //关闭PID自整定
      Pid_Setok=1;   //PID自整定成功
    }
    //PV_Oldvalue=PV_value; //记录上次温度 
  }
}

void PidParameter_Sinceset(void)//PID自整定参数
{
  //记录此刻的状态 即设定温度是否 高于或等于 当前温度 
  //PV_value = read_max6675_temper();
  if( SV_value >= PV_value )//设定温度 高于 或者 等于  当前温度  启动加热
  {
    once_add_1_flag = 0;
  }
  else//设定温度 低于 当前温度
  {
    once_add_1_flag = 1;
  } 
  zero_across_counter = 0;
  pid_self_time_cnt = 0;      

  TIME_Hight = 0;
  TIME_LOW   = 0;	//具体的0.2s 
}

void PidParameter_pro(void)//PID参数
{
  
 KC=*((float *)(&memory[PIDKC_ADR]));
 TC=*((int *)(&memory[PIDTC_ADR]));
  if(KC > 1666.0 )
  {
    KC = 1666.0;//对应 比例带为 0.1%
  }
  else if(KC < 0.5 )
  {
    KC = 0.5;//对应 比例带为 200.0%
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
  Fuzzy_flag=0; //关闭模糊
}


void Preheat(void)// 预热
{
  unsigned short preheat_cnt=0;
  unsigned int  preheat_out=596;
  while(preheat_cnt<240) //2分
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