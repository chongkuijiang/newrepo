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
  sys_config();     //ϵͳʱ������    
  //�ı�ָ���ܽŵ�ӳ��  GPIO_Remap_SWJ_JTAGDisable  JTAG-DP ʧ�� + SW-DPʹ��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
  //I/O�ڳ�ʼ�� 
  GPIO_Configuration();
  
  GUN_0;  //baiqiang
  AIR_0;  //�رշ�
  MOTORRUN_0; //���ֹͣ
  PRESSURE_0; //ѹ����λ
  SCISSORS_0;//������λ
  
  //�ⲿ�жϳ�ʼ��
  NVIC_Configuration();  
  //ϵͳʱ�� 1 ms ��ʱ
  SysTick_Config(); 
  
  EXTI_init();           //�ⲿ�ж�
  USART1_Configuration();//����ͨѶ
  
  AD1_Configration();
  DMA1_Configration();
  
  DAC_configration();    //DAC���
  PWM_TIM2_Configuration();//PWM���
  TIM5_Configuration();   //100MS��ʱ PID
  MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
  
  Parameter();  //����
  
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

//  while(1)//��ѭ��ϵͳ
//  { 
//    if(Modbus_Rxflag)  //���ڽ��յ�����
//    {
//      uart1_dispose();
//      Modbus_Rxflag=0;
//    }
//    if(Pedal_old!=Pedal) //����״̬
//    {
//      HotblowerProcess();//�������  
//      Pedal_old=Pedal;  //��״̬������״̬
//    }
//  }
  while(1)//��ѭ��ϵͳ
  {  
//    Dac_Out(TC) ;
    if(Ds18b20_readflag)  //��ȡ����
    {
      Ds18b20_readflag=0;
      DS18B20_Convert(); 
      Environment_temp = (int)DS18B20_Read();
    }
    if(Modbus_Rxflag)  //���ڽ��յ�����
    {
      uart1_dispose();
      Modbus_Rxflag=0;
    }
    if(memory[PIDSET_ADR] && PidSet_flag) //PID��������ʼ
    {
     // Pid_Sinceset();  //�������»�
    }
    else if(memory[PIDSET_ADR] && (PidSet_flag==0)) //PID������������ʼ��
    {
      Preheat();// Ԥ��
      PidSet_flag=1;
      PidParameter_Sinceset();
      AIR_1;
    }
    else if((memory[PIDSET_ADR]==0) && PidSet_flag)//PID����������
    {
      Dac_Out(596);
      PidSet_flag=0; 
      if(Pid_Setok)
      {
        Pid_Setok=0;
        memory[PIDKC_ADR]=*((unsigned short *)(&KC));
        memory[PIDKC_ADR+1]=*((unsigned short *)(&KC)+1); // �洢KC
        memory[PIDTC_ADR]=*((unsigned short *)(&TC));
        memory[PIDTC_ADR+1]=*((unsigned short *)(&TC)+1); // �洢TC 
        flash_save(&memory[TEMPCONTROL_ADR-1]); //�洢����
        
        memory[TEMPCONTROL_ADR]=0xFF00;//  ����PID�¿�
        Tempcontrol_flag=1;
        PidParameter_pro(); //�������»�
      }
    }
    else 
    {
      if(memory[TEMPCONTROL_ADR]&&(Tempcontrol_flag==0)) //�¶ȿ���
      {
        Preheat();// Ԥ��
        Tempcontrol_flag=1;
        PidParameter_pro(); //�������»�
        AIR_1;
      }
      else if((memory[TEMPCONTROL_ADR]==0)&& Tempcontrol_flag)
      {
        Dac_Out(596);
        Fuzzy_flag=0; //�ر�ģ��
        Tempcontrol_flag=0;
      }
      else if((PV_value<100)&&((memory[TEMPCONTROL_ADR]==0)&& (Tempcontrol_flag==0))) //�¶�С��100��
      {
        AIR_0; 
      }

      if(memory[TEST_ACTION])  //�Լ��ʶ
      {
        HotblowerTest();
        if(memory[AUTO_CUT]) //��β
          memory[AUTO_CUT]=0;
      } 
      else
      {
        if(memory[AUTO_CUT]) //��β
        {
          if(memory[CUT_EN]==0)
            Hotblowerend_Test();
          memory[AUTO_CUT]=0;
        }
        else if(Pedal_old!=Pedal) //����״̬
        {
          HotblowerProcess();//������� 
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
