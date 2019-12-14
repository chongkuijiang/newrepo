/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PROCESS_H
#define _PROCESS_H

#define L_DETECTION_FLAG   0x02   //左脚检测
#define R_DETECTION_FLAG   0x01   //右脚检测

#define MOTOR_POSITIVE   0x00   //正传
#define MOTOR_NEGATIVE   0x01   //反转

#define L_DETECTION  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) //左脚输入判断
#define R_DETECTION  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) //右脚输入判断

#define GAS_1 GPIO_SetBits(GPIOD,GPIO_Pin_2)  //气复位 上升
#define GAS_0 GPIO_ResetBits(GPIOD,GPIO_Pin_2)  //气置位  下压
#define READ_GAS GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_2)  //获取当前电机方向

#define GUN_0 GPIO_SetBits(GPIOC,GPIO_Pin_10)  //摆抢复位    上摆
#define GUN_1 GPIO_ResetBits(GPIOC,GPIO_Pin_10)  //摆抢置位      下摆

#define SCISSORS_0 GPIO_SetBits(GPIOC,GPIO_Pin_11)  //剪刀复位
#define SCISSORS_1 GPIO_ResetBits(GPIOC,GPIO_Pin_11)  //剪刀置位

#define PRESSURE_0 GPIO_SetBits(GPIOC,GPIO_Pin_12)  //压柱复位 上升
#define PRESSURE_1 GPIO_ResetBits(GPIOC,GPIO_Pin_12)  //压柱置位  下压

#define READ_MOTORDIRECTOE GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)  //获取当前电机方向

#define MOTORDIRECTOE_0 GPIO_SetBits(GPIOA,GPIO_Pin_7) //电机正向
#define MOTORDIRECTOE_1 GPIO_ResetBits(GPIOA,GPIO_Pin_7)   //电机反向

#define MOTORRUN_0 GPIO_ResetBits(GPIOA,GPIO_Pin_6)    //电机停止
#define MOTORRUN_1 GPIO_SetBits(GPIOA,GPIO_Pin_6)    //电机运行

#define AIR_0 GPIO_SetBits(GPIOA,GPIO_Pin_5)  //风枪关闭
#define AIR_1 GPIO_ResetBits(GPIOA,GPIO_Pin_5)   //风枪开启

//无逻辑控制
#define L0R0    0x00
//右脚控制逻辑
#define L0R1     0x01   //右脚踩下左脚抬起  右脚逻辑开始
#define R_L0R1   0x11   //右脚踩下左脚抬起  电机前进  
#define R_L1R1   0x13   //右脚踩下左脚踩下  电机前进 

#define R_L1R0   0x12   //右脚抬起左脚踩下  电机停止
#define R_L0R0   0x10   //右脚抬起左脚抬起  电机停止  无逻辑
//单左脚控制逻辑
#define L1R0     0x02   //左脚踩下右脚抬起   单左脚逻辑开始
#define L_L1R0   0x22   //左脚踩下右脚抬起   下柱 
#define L_L0R0   0x20   //左脚抬起右脚抬起   上柱  无逻辑
//热封控制逻辑
#define L_L1R1   0x23   //左脚踩下右脚踩下  热封逻辑开始
#define L_L0R1   0x21   //左脚抬起右脚踩下  热封逻辑开始

#define LR_L1R1  0x33   //左脚踩下右脚踩下  热封开始
#define LR_L0R1  0x31   //左脚抬起右脚踩下  热封收尾

#define LR_L1R0  0x32   //左脚踩下右脚抬起  热封暂停
#define LR_L0R0  0x30   //左脚抬起右脚抬起  热封结束无逻辑


#define LRE_L0R1 0x41  //热封收尾动作
#define LRE_L1R0 0x42  //热封收尾动作再次下压
#define LRE_L0R0 0x40  //热封收尾动作结束 无逻辑
//非法逻辑
#define L1R1     0x03   //左右脚踩下

extern unsigned char Pedal;  //踏板值
extern unsigned char Pedal_old;  //前踏板值
extern unsigned char Delay_flag; //工序延时标识
extern unsigned int Delay_Cnt;   //工序延时计数
extern unsigned char HotBlowerContinue_falg;//热封继续标识
extern unsigned char PidSet_flag;   //PID参数初始化标识
extern unsigned char Tempcontrol_flag;   //温控初始化标识

extern int Environment_temp;

//extern unsigned short test;

void Parameter(void); //参数
void HotblowerProcess(void);
void Hotblowerend_Test(void);  // 收尾动作
void MotorControl(unsigned char director,unsigned short speed); //电机运行
void SingeControl(unsigned short addr); //电控
void HotblowerTest(void);
void delay_ms(unsigned short delay_ms);
#endif /* _PROCESS_H */