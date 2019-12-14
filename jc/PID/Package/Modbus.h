#ifndef _MODBUS_H
#define _MODBUS_H

#define FOOT1_ADR                   0 //输入1 脚踏1 只读 开关量
#define FOOT2_ADR                   1 //输入2 脚踏2 只读 开关量
#define Y1_ADR                      8 //Y1          读写/开关量  
#define Y2_ADR                      9 //Y2          读写/开关量 
#define Y3_ADR                     10 //Y3          读写/开关量 
#define Y10_ADR                    11 //Y10         读写/开关量 
#define Y11_ADR                    12 //Y11         读写/开关量 
#define Y12_ADR                    13 //Y12         读写/开关量 
#define Y14_ADR                    14 //Y14         读写/开关量 
#define Y16_ADR                    15 //Y16         读写/开关量 
#define Y17_ADR                    16 //Y17         读写/开关量 
#define Y0_ADR                     21 //Y0          读写/整形 
#define Y13_ADR                    22 //Y13         读写/整形
#define Y15_ADR                    23 //Y15         读写/整形 
#define READ_CURRENTTEMP_ADR       24 //实时温度值  读写/整形


#define TEMPCONTROL_ADR            31 //温控开关   读写/开关量
#define PIDSET_ADR                 32 //PID自整定  读写/开关量
#define TEST_ACTION                33 //测试动作起始标记
#define AUTO_CUT                   34 //自动剪带起始标记
#define MOTOR_CLICK                35 //电机点动

#define SET_TEMP_ADR               51 //设定温度值  读写/整形
#define T0_ADR                     52 //进枪同步计时（滚轮同步延时）读写/整形
#define T1_ADR                     53//退枪同步计时（滚轮停止延时）读写/整形
#define T2_ADR                     54//封尾长度计时（剪刀起止延时）读写/整形
#define T3_ADR                     55 //送带时间计时（带尾完成延时）读写/整形
#define T4_ADR                     56 //送带长度时间计时（送带长短延时）读写/整形
#define T5_ADR                     57 //微退时间（滚轮微退延时）读写/整形

#define SET_POSITIVESPEED_ADR      58 //滚轮正转(设置)
#define SET_SENDWITHSPEED_ADR      59 //送带速度(设置)
#define SET_MIRCORBACKSPEED_ADR    60 //微退速度(设置)
#define CUT_EN                     61 //剪刀失能
#define MOTOR_CLICKSPEED           62 //电机点动速度
#define T6_ADR                     63 //送带延时
#define PIDKC_ADR                  71 //KC
#define PIDTC_ADR                  73 //TC



extern unsigned char Modbus_Rxflag;
extern unsigned short memory[200];
void uart_crc(unsigned char* data , unsigned char length);
void uart1_dispose(void);



#endif /* _MODBUS_H */