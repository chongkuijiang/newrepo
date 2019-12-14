/* Includes XXXX.H------------------------------------------------------------------*/
//#include "iostm32f10xxE.h"
#include "stm32f10x_lib.h"
#include "SysInit.h"
#include "Exit.h"
#include "uart.h"
#include "Adc.h"
#include "Dac.h"
#include "Flash.h"
#include "Time.h"
#include "Max6675.h"
#include "Pid.h"
#include "Modbus.h"
#include "Readtemp.h"
#include "Process.h"
#include "ds18b20.h"
#include "Fuzzy.h"
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None   
* Return         : None
*******************************************************************************/

int main(void)
{
#ifdef DEBUG
  debug();
#endif
  sys_config();     //系统时钟配置    
  //改变指定管脚的映射  GPIO_Remap_SWJ_JTAGDisable  JTAG-DP 失能 + SW-DP使能
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
  //I/O口初始化 
  GPIO_Configuration();
  
  GUN_0;  //baiqiang
  AIR_0;  //关闭风
  MOTORRUN_0; //电机停止
  PRESSURE_0; //压柱复位
  SCISSORS_0;//剪刀复位
  
  //外部中断初始化
  NVIC_Configuration();  
  //系统时钟 1 ms 定时
  SysTick_Config(); 
  
  EXTI_init();           //外部中断
  USART1_Configuration();//串口通讯
  
  AD1_Configration();
  DMA1_Configration();
  
  DAC_configration();    //DAC输出
  PWM_TIM2_Configuration();//PWM输出
  TIM5_Configuration();   //100MS定时 PID
  MotorControl(MOTOR_POSITIVE,0);//电机停止
  
  Parameter();  //参数
  
//  while(1)
//  {
//    //Dac_Out(Pid_Setok) ;
//    if(time0_temp2==0)
//    {
//      time0_temp2++; 
//      DS18B20_Convert(); 
//      Environment_temp = (int)DS18B20_Read();
//    }
//    PV_value=(float)read_temper();
//  }

//  while(1)//主循环系统
//  { 
//    if(Modbus_Rxflag)  //串口接收到数据
//    {
//      uart1_dispose();
//      Modbus_Rxflag=0;
//    }
//    if(Pedal_old!=Pedal) //有新状态
//    {
//      HotblowerProcess();//工序控制  
//      Pedal_old=Pedal;  //新状态付给老状态
//    }
//  }
  while(1)//主循环系统
  {  
//    Dac_Out(TC) ;
    if(Ds18b20_readflag)  //获取室温
    {
      Ds18b20_readflag=0;
      DS18B20_Convert(); 
      Environment_temp = (int)DS18B20_Read();
    }
    if(Modbus_Rxflag)  //串口接收到数据
    {
      uart1_dispose();
      Modbus_Rxflag=0;
    }
    if(memory[PIDSET_ADR] && PidSet_flag) //PID自整定开始
    {
     // Pid_Sinceset();  //参数出事化
    }
    else if(memory[PIDSET_ADR] && (PidSet_flag==0)) //PID自整定参数初始化
    {
      Preheat();// 预热
      PidSet_flag=1;
      PidParameter_Sinceset();
      AIR_1;
    }
    else if((memory[PIDSET_ADR]==0) && PidSet_flag)//PID自整定结束
    {
      Dac_Out(596);
      PidSet_flag=0; 
      if(Pid_Setok)
      {
        Pid_Setok=0;
        memory[PIDKC_ADR]=*((unsigned short *)(&KC));
        memory[PIDKC_ADR+1]=*((unsigned short *)(&KC)+1); // 存储KC
        memory[PIDTC_ADR]=*((unsigned short *)(&TC));
        memory[PIDTC_ADR+1]=*((unsigned short *)(&TC)+1); // 存储TC 
        flash_save(&memory[TEMPCONTROL_ADR-1]); //存储参数
        
        memory[TEMPCONTROL_ADR]=0xFF00;//  开启PID温控
        Tempcontrol_flag=1;
        PidParameter_pro(); //参数出事化
      }
    }
    else 
    {
      if(memory[TEMPCONTROL_ADR]&&(Tempcontrol_flag==0)) //温度控制
      {
        Preheat();// 预热
        Tempcontrol_flag=1;
        PidParameter_pro(); //参数出事化
        AIR_1;
      }
      else if((memory[TEMPCONTROL_ADR]==0)&& Tempcontrol_flag)
      {
        Dac_Out(596);
        Fuzzy_flag=0; //关闭模糊
        Tempcontrol_flag=0;
      }
      else if((PV_value<100)&&((memory[TEMPCONTROL_ADR]==0)&& (Tempcontrol_flag==0))) //温度小于100度
      {
        AIR_0; 
      }

      if(memory[TEST_ACTION])  //自检标识
      {
        HotblowerTest();
        if(memory[AUTO_CUT]) //收尾
          memory[AUTO_CUT]=0;
      } 
      else
      {
        if(memory[AUTO_CUT]) //收尾
        {
          if(memory[CUT_EN]==0)
            Hotblowerend_Test();
          memory[AUTO_CUT]=0;
        }
        else if(Pedal_old!=Pedal) //有新状态
        {
          HotblowerProcess();//工序控制 
        }
      }
    }
  }
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMFANS *********************************
*****END OF FILE****/
