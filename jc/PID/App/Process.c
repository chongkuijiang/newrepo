/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PROCESS_C
#define _PROCESS_C

#include "stm32f10x_lib.h"
#include "Modbus.h"
#include "Pid.h"
#include "Flash.h"
#include "Process.h"
#include "ds18b20.h"
#include "Max6675.h"
#include "Readtemp.h"
unsigned char Pedal=0;      //踏板值
unsigned char Pedal_old=0;  //前踏板值
unsigned char Delay_flag=0; //工序延时标识
unsigned int  Delay_Cnt=0;   //工序延时计数
unsigned char HotBlowerContinue_falg=0;//热封继续标识


unsigned char PidSet_flag=0;   //PID参数初始化标识
unsigned char Tempcontrol_flag=0;   //温控初始化标识


int Environment_temp;

//unsigned short test=0;
void delay_ms(unsigned short delay_ms)
{
  Delay_Cnt=0;
  Delay_flag=1;
  while(Delay_Cnt<delay_ms)
  {
   if(Modbus_Rxflag)  //串口接收到数据
    {
      uart1_dispose();
      Modbus_Rxflag=0;
    }
   //if(memory[TEMPCONTROL_ADR]&&Tempcontrol_flag) //温度控制
   //  pid_pro(); 
  }
  Delay_Cnt=0;
  Delay_flag=0;
}


void Parameter(void) //参数
{
// KC=12.12;
// memory[PIDKC_ADR]=*((unsigned short *)(&KC));
// memory[PIDKC_ADR+1]=*((unsigned short *)(&KC)+1);
// KC=*((float *)(&memory[PIDKC_ADR]));
// TC=122345;
// memory[PIDTC_ADR]=*((unsigned short *)(&TC));
// memory[PIDTC_ADR+1]=*((unsigned short *)(&TC)+1);
// TC=*((int *)(&memory[PIDTC_ADR]));
  
 for(unsigned char i=0;i<200;i++)
   memory[i]=0;
 flash_load(&memory[TEMPCONTROL_ADR-1]);//
 
 if(memory[SET_TEMP_ADR]>9000)
   memory[SET_TEMP_ADR]=9000;
 
 SV_value=((float)memory[SET_TEMP_ADR])/10; //获取设定温度
 
 DS18B20_Convert(); 
 Environment_temp = (int)DS18B20_Read();
 
// PV_value = read_max6675_temper();          //获取当前温度
 //memory[READ_CURRENTTEMP_ADR]=(unsigned short)(PV_value*10);
 
 if(memory[CUT_EN]!=0xFF00)
   memory[CUT_EN]=0;
 if(memory[TEMPCONTROL_ADR]!=0xFF00)
   memory[TEMPCONTROL_ADR]=0;
 if(memory[PIDSET_ADR]!=0xFF00)
   memory[PIDSET_ADR]=0;
 if(memory[TEST_ACTION]!=0xFF00)
   memory[TEST_ACTION]=0;
 if(memory[MOTOR_CLICK]!=0xFF00)
   memory[MOTOR_CLICK]=0;
 if(memory[AUTO_CUT]!=0xFF00)
   memory[AUTO_CUT]=0;
 MotorControl(MOTOR_POSITIVE,0);//电机停止
 GAS_1;
}
// director 0：正向 1：反向  speed：速度值 0-100
void MotorControl(unsigned char director,unsigned short speed) //电机运行
{
  if(speed)  //有速度
  {
    if(READ_MOTORDIRECTOE &&(director==0)) //当前电机反向，要求正向
    {
      TIM2->CCR4 =0;
      MOTORRUN_0;   //电机停止
      MOTORDIRECTOE_1; //换正向
    }
    else if((READ_MOTORDIRECTOE==0) && director) //当前电机正向，要求反向
    {
      TIM2->CCR4 =0;
      MOTORRUN_0;   //电机停止
      MOTORDIRECTOE_0; //换反向
    }
    for(unsigned char i=0;i<50;i++) 
      max6675_delay(5000);
    
    TIM2->CCR4 =speed*4;
    MOTORRUN_1;   //电机运行
  }
  else
  {
    TIM2->CCR4 =0;
    MOTORRUN_0;   //电机停止
//    if(director) 
      MOTORDIRECTOE_0;//反向
//    else
     // MOTORDIRECTOE_1;//正向
  }
}

void Hotblowerend_Test(void)  // 收尾动作
{
  SCISSORS_1; //剪刀置位
  delay_ms(memory[T2_ADR]); //封尾计时
  
  SCISSORS_0; //剪刀复位
  delay_ms(memory[T6_ADR]); //送带延时计时 
  MotorControl(MOTOR_POSITIVE,memory[SET_SENDWITHSPEED_ADR]);//电机正转
  delay_ms(memory[T4_ADR]); //送带长度计时
  MotorControl(MOTOR_POSITIVE,0);//电机停止 
  
}

void HotblowerTest(void)
{
  PRESSURE_1; // 下压
  delay_ms(1000);
  for(unsigned char i=0;i<5;i++)
  {
    GUN_1;   //下摆抢
    delay_ms(memory[T0_ADR]); //进枪计时
    MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//电机正转
    delay_ms(1000);
    GUN_0;   //上摆抢
    delay_ms(memory[T1_ADR]); //退枪计时
    MotorControl(MOTOR_NEGATIVE,memory[SET_MIRCORBACKSPEED_ADR]);//电机反转
    delay_ms(memory[T5_ADR]); //微退计时
    MotorControl(MOTOR_POSITIVE,0);//电机停止
    delay_ms(1000);
    if(memory[TEST_ACTION]==0)  //
      break;
  }
  
  GUN_1;   //下摆抢
  delay_ms(memory[T0_ADR]); //进枪计时
  MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//电机正转
  delay_ms(1000);
  if(memory[CUT_EN]==0)  //剪刀使能
  {
    SCISSORS_1; //剪刀置位
    delay_ms(memory[T2_ADR]); //封尾计时
  }
  GUN_0;   //上摆抢
  PRESSURE_0; //上柱
  TIM2->CCR4 =0;//电机停止 
  if(memory[CUT_EN]==0)  //剪刀使能
  {
    delay_ms(memory[T3_ADR]); //送带计时
    SCISSORS_0; //剪刀复位
    delay_ms(memory[T6_ADR]); //送带延时计时
    MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//电机正转
    delay_ms(memory[T4_ADR]); //送带长度计时
    MotorControl(MOTOR_POSITIVE,0);//电机停止 
  }
  delay_ms(1000);
}

//热封控制流程
void HotblowerProcess(void)
{
  //各逻辑开始
  if(Pedal==L0R1)      //右脚逻辑开始
    Pedal=R_L0R1;
  else if(Pedal==L1R0)//单左脚逻辑开始
    Pedal=L_L1R0;
  else if(Pedal==L_L1R1) //热封逻辑开始
    Pedal=LR_L1R1;
  else if((Pedal==LR_L0R1)&&(Pedal_old==LR_L1R1))//热封收尾逻辑开始
    Pedal=LRE_L0R1;
  
  //else if(Pedal==L_L0R1) //热封逻辑开始
   // Pedal=LR_L0R1;
  
  switch(Pedal)
  {
    //右脚逻辑
    {
    case R_L0R1:  //电机前进
    case R_L1R1:  //电机前进
      {
        Pedal_old=Pedal;  //新状态付给老状态
        MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);
        break;
      }
    case R_L1R0:  //电机停止
      {
        Pedal_old=Pedal;  //新状态付给老状态
        MotorControl(MOTOR_POSITIVE,0);
        break;
      }
    case R_L0R0:  //电机停止 无逻辑
      {
        Pedal_old=Pedal;  //新状态付给老状态
        MotorControl(MOTOR_POSITIVE,0);
        Pedal=L0R0;
        Pedal_old=Pedal;
        break;
      }
    }
    //单左脚控制逻辑  
    {
    case L_L1R0:  // 下压
      {
        Pedal_old=Pedal;  //新状态付给老状态
        PRESSURE_1;
        break;
      }
    case L_L0R0:  // 上升 无逻辑
      {
        Pedal_old=Pedal;  //新状态付给老状态
        PRESSURE_0;
        Pedal=L0R0;
        Pedal_old=Pedal;
        break;
      }
    }
    //热封控制逻辑
    {
    case LR_L1R1:  // 热封开始
      //case LR_L0R1:
      {
        Pedal_old=Pedal;  //新状态付给老状态
        GUN_1;   //下摆抢
        delay_ms(memory[T0_ADR]); //进枪计时
        MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//电机正转
        break;
      }
    case LR_L1R0: //热封暂停
      {
        Pedal_old=Pedal;  //新状态付给老状态
        GUN_0;   //上摆抢
        delay_ms(memory[T1_ADR]); //退枪计时
        MotorControl(MOTOR_NEGATIVE,memory[SET_MIRCORBACKSPEED_ADR]);//电机反转
        delay_ms(memory[T5_ADR]); //微退计时
        MotorControl(MOTOR_POSITIVE,0);//电机停止
        break;
      } 
    case LR_L0R0: //热封结束    无逻辑
      {
        Pedal_old=Pedal;  //新状态付给老状态
        PRESSURE_0; //上柱
        GUN_0;   //上摆抢
        MotorControl(MOTOR_POSITIVE,0);//电机停止
        Pedal=L0R0;
        Pedal_old=Pedal;
        break;
      } 
    }
    {
    case LRE_L0R1:  //热封收尾 
      {
        Pedal_old=Pedal;  //新状态付给老状态
        if(memory[CUT_EN]==0)  //剪刀使能
        {
          SCISSORS_1; //剪刀置位
          delay_ms(memory[T2_ADR]); //封尾计时
        }
        GUN_0;   //上摆抢
        PRESSURE_0; //上柱
        //MotorControl(MOTOR_POSITIVE,0);//电机停止 
         if(memory[CUT_EN]==0)  //剪刀使能
        {
          TIM2->CCR4 =0;
          delay_ms(memory[T3_ADR]); //送带计时
          SCISSORS_0; //剪刀复位
          delay_ms(memory[T6_ADR]); //送带延时计时 
          MotorControl(MOTOR_POSITIVE,memory[SET_SENDWITHSPEED_ADR]);//电机正转
          delay_ms(memory[T4_ADR]); //送带长度计时
        } 
          MotorControl(MOTOR_POSITIVE,0);//电机停止 
        
        break;
      }
    case LRE_L0R0: //热封收尾结束  无逻辑
      {
        Pedal_old=Pedal;  //新状态付给老状态
        PRESSURE_0; //上柱
        GUN_0;   //上摆抢
        MotorControl(MOTOR_POSITIVE,0);//电机停止
        Pedal=L0R0;
        Pedal_old=Pedal;
        HotBlowerContinue_falg=0;
        break;
      }
    }
    default:
      Pedal_old=Pedal;  //新状态付给老状态
      break;
  }
}

void SingeControl(unsigned short addr) //电控
{
  if(MOTOR_CLICK==addr) 
  {
    if(memory[MOTOR_CLICK])  //电机点动
      MotorControl(MOTOR_NEGATIVE,memory[MOTOR_CLICKSPEED]);
    else
       MotorControl(MOTOR_POSITIVE,0);//电机停止
  }
  if(Y1_ADR==addr) 
  {
    if(memory[Y1_ADR])  //电机方向
      MOTORDIRECTOE_1;
    else
      MOTORDIRECTOE_0;
  }
  
  if(Y2_ADR==addr)
  {
    if(memory[Y2_ADR]) //电机运行
      MOTORRUN_1;   
    else
      MOTORRUN_0; 
  }
  
  if(Y3_ADR==addr)
  {
    if(memory[Y3_ADR]) //风
      AIR_1;    
    else
      AIR_0;  
  }
  
  if(Y10_ADR==addr)
  {
    if(memory[Y10_ADR])//枪
      GUN_1;       
    else
      GUN_0;   
  }
  
  if(Y11_ADR==addr)
  {
    if(memory[Y11_ADR])//剪刀
      SCISSORS_1;       
    else
      SCISSORS_0;
  }
  
  if(Y12_ADR==addr)
  {
    if(memory[Y12_ADR])//柱
      PRESSURE_1;       
    else
      PRESSURE_0;
  }
  
}
#endif /* _PROCESS_C */