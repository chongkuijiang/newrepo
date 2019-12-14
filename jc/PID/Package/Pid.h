#ifndef _PID_H
#define _PID_H

#define     max_value_error               50.0
#define     min_value_error              -50.0//控温区域
#define     max_min_error                 5   //5摄氏度

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

extern float  Proportion; //  比例常数 Proportional Const
extern float  Integral;   //  积分常数 Integral Const        
extern float  Derivative; //  微分常数 Derivative Const

extern float  Error;      //  e(k)
extern float  LastError;  //  e(k-1)
extern float  PrevError;  //  e(k-2)
extern float  SumError;   //  Sums of Errors

//pid
extern float  SV_value;   //设定温度值
extern float  PV_value;   //用于参与计算的当前温度值
extern float  KV_value;   //温度差值
extern int  pid_result;   //DAC输出

extern float max_min_temp ;         //最大最小温度临时存放
extern unsigned char max_min_cnt;   //最大最小温度寻找计数
extern unsigned char max_flag;      //寻找最大温度标志
extern unsigned char min_flag;      //寻找最小温度标志

extern float T_Hight;
extern float T_LOW;       //温度

extern unsigned long TIME_Hight;
extern unsigned long TIME_LOW;	//具体的秒
extern unsigned long TIME_Hight_LOW;//最大最小温度时间临时存放

extern float  KC ;        //临界比例系数  初始默认的值
extern unsigned int TC;   //振荡周期      初始默认的值

void pid_pro(void);
void Pid_Sinceset(void);
void PidParameter_Sinceset(void);//PID自整定参数
void PidParameter_pro(void);//PID参数
void Preheat(void);// 预热
#endif /* _PID_H */