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
unsigned char Pedal=0;      //̤��ֵ
unsigned char Pedal_old=0;  //ǰ̤��ֵ
unsigned char Delay_flag=0; //������ʱ��ʶ
unsigned int  Delay_Cnt=0;   //������ʱ����
unsigned char HotBlowerContinue_falg=0;//�ȷ������ʶ


unsigned char PidSet_flag=0;   //PID������ʼ����ʶ
unsigned char Tempcontrol_flag=0;   //�¿س�ʼ����ʶ


int Environment_temp;

//unsigned short test=0;
void delay_ms(unsigned short delay_ms)
{
  Delay_Cnt=0;
  Delay_flag=1;
  while(Delay_Cnt<delay_ms)
  {
   if(Modbus_Rxflag)  //���ڽ��յ�����
    {
      uart1_dispose();
      Modbus_Rxflag=0;
    }
   //if(memory[TEMPCONTROL_ADR]&&Tempcontrol_flag) //�¶ȿ���
   //  pid_pro(); 
  }
  Delay_Cnt=0;
  Delay_flag=0;
}


void Parameter(void) //����
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
 
 SV_value=((float)memory[SET_TEMP_ADR])/10; //��ȡ�趨�¶�
 
 DS18B20_Convert(); 
 Environment_temp = (int)DS18B20_Read();
 
// PV_value = read_max6675_temper();          //��ȡ��ǰ�¶�
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
 MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
 GAS_1;
}
// director 0������ 1������  speed���ٶ�ֵ 0-100
void MotorControl(unsigned char director,unsigned short speed) //�������
{
  if(speed)  //���ٶ�
  {
    if(READ_MOTORDIRECTOE &&(director==0)) //��ǰ�������Ҫ������
    {
      TIM2->CCR4 =0;
      MOTORRUN_0;   //���ֹͣ
      MOTORDIRECTOE_1; //������
    }
    else if((READ_MOTORDIRECTOE==0) && director) //��ǰ�������Ҫ����
    {
      TIM2->CCR4 =0;
      MOTORRUN_0;   //���ֹͣ
      MOTORDIRECTOE_0; //������
    }
    for(unsigned char i=0;i<50;i++) 
      max6675_delay(5000);
    
    TIM2->CCR4 =speed*4;
    MOTORRUN_1;   //�������
  }
  else
  {
    TIM2->CCR4 =0;
    MOTORRUN_0;   //���ֹͣ
//    if(director) 
      MOTORDIRECTOE_0;//����
//    else
     // MOTORDIRECTOE_1;//����
  }
}

void Hotblowerend_Test(void)  // ��β����
{
  SCISSORS_1; //������λ
  delay_ms(memory[T2_ADR]); //��β��ʱ
  
  SCISSORS_0; //������λ
  delay_ms(memory[T6_ADR]); //�ʹ���ʱ��ʱ 
  MotorControl(MOTOR_POSITIVE,memory[SET_SENDWITHSPEED_ADR]);//�����ת
  delay_ms(memory[T4_ADR]); //�ʹ����ȼ�ʱ
  MotorControl(MOTOR_POSITIVE,0);//���ֹͣ 
  
}

void HotblowerTest(void)
{
  PRESSURE_1; // ��ѹ
  delay_ms(1000);
  for(unsigned char i=0;i<5;i++)
  {
    GUN_1;   //�°���
    delay_ms(memory[T0_ADR]); //��ǹ��ʱ
    MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//�����ת
    delay_ms(1000);
    GUN_0;   //�ϰ���
    delay_ms(memory[T1_ADR]); //��ǹ��ʱ
    MotorControl(MOTOR_NEGATIVE,memory[SET_MIRCORBACKSPEED_ADR]);//�����ת
    delay_ms(memory[T5_ADR]); //΢�˼�ʱ
    MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
    delay_ms(1000);
    if(memory[TEST_ACTION]==0)  //
      break;
  }
  
  GUN_1;   //�°���
  delay_ms(memory[T0_ADR]); //��ǹ��ʱ
  MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//�����ת
  delay_ms(1000);
  if(memory[CUT_EN]==0)  //����ʹ��
  {
    SCISSORS_1; //������λ
    delay_ms(memory[T2_ADR]); //��β��ʱ
  }
  GUN_0;   //�ϰ���
  PRESSURE_0; //����
  TIM2->CCR4 =0;//���ֹͣ 
  if(memory[CUT_EN]==0)  //����ʹ��
  {
    delay_ms(memory[T3_ADR]); //�ʹ���ʱ
    SCISSORS_0; //������λ
    delay_ms(memory[T6_ADR]); //�ʹ���ʱ��ʱ
    MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//�����ת
    delay_ms(memory[T4_ADR]); //�ʹ����ȼ�ʱ
    MotorControl(MOTOR_POSITIVE,0);//���ֹͣ 
  }
  delay_ms(1000);
}

//�ȷ��������
void HotblowerProcess(void)
{
  //���߼���ʼ
  if(Pedal==L0R1)      //�ҽ��߼���ʼ
    Pedal=R_L0R1;
  else if(Pedal==L1R0)//������߼���ʼ
    Pedal=L_L1R0;
  else if(Pedal==L_L1R1) //�ȷ��߼���ʼ
    Pedal=LR_L1R1;
  else if((Pedal==LR_L0R1)&&(Pedal_old==LR_L1R1))//�ȷ���β�߼���ʼ
    Pedal=LRE_L0R1;
  
  //else if(Pedal==L_L0R1) //�ȷ��߼���ʼ
   // Pedal=LR_L0R1;
  
  switch(Pedal)
  {
    //�ҽ��߼�
    {
    case R_L0R1:  //���ǰ��
    case R_L1R1:  //���ǰ��
      {
        Pedal_old=Pedal;  //��״̬������״̬
        MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);
        break;
      }
    case R_L1R0:  //���ֹͣ
      {
        Pedal_old=Pedal;  //��״̬������״̬
        MotorControl(MOTOR_POSITIVE,0);
        break;
      }
    case R_L0R0:  //���ֹͣ ���߼�
      {
        Pedal_old=Pedal;  //��״̬������״̬
        MotorControl(MOTOR_POSITIVE,0);
        Pedal=L0R0;
        Pedal_old=Pedal;
        break;
      }
    }
    //����ſ����߼�  
    {
    case L_L1R0:  // ��ѹ
      {
        Pedal_old=Pedal;  //��״̬������״̬
        PRESSURE_1;
        break;
      }
    case L_L0R0:  // ���� ���߼�
      {
        Pedal_old=Pedal;  //��״̬������״̬
        PRESSURE_0;
        Pedal=L0R0;
        Pedal_old=Pedal;
        break;
      }
    }
    //�ȷ�����߼�
    {
    case LR_L1R1:  // �ȷ⿪ʼ
      //case LR_L0R1:
      {
        Pedal_old=Pedal;  //��״̬������״̬
        GUN_1;   //�°���
        delay_ms(memory[T0_ADR]); //��ǹ��ʱ
        MotorControl(MOTOR_POSITIVE,memory[SET_POSITIVESPEED_ADR]);//�����ת
        break;
      }
    case LR_L1R0: //�ȷ���ͣ
      {
        Pedal_old=Pedal;  //��״̬������״̬
        GUN_0;   //�ϰ���
        delay_ms(memory[T1_ADR]); //��ǹ��ʱ
        MotorControl(MOTOR_NEGATIVE,memory[SET_MIRCORBACKSPEED_ADR]);//�����ת
        delay_ms(memory[T5_ADR]); //΢�˼�ʱ
        MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
        break;
      } 
    case LR_L0R0: //�ȷ����    ���߼�
      {
        Pedal_old=Pedal;  //��״̬������״̬
        PRESSURE_0; //����
        GUN_0;   //�ϰ���
        MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
        Pedal=L0R0;
        Pedal_old=Pedal;
        break;
      } 
    }
    {
    case LRE_L0R1:  //�ȷ���β 
      {
        Pedal_old=Pedal;  //��״̬������״̬
        if(memory[CUT_EN]==0)  //����ʹ��
        {
          SCISSORS_1; //������λ
          delay_ms(memory[T2_ADR]); //��β��ʱ
        }
        GUN_0;   //�ϰ���
        PRESSURE_0; //����
        //MotorControl(MOTOR_POSITIVE,0);//���ֹͣ 
         if(memory[CUT_EN]==0)  //����ʹ��
        {
          TIM2->CCR4 =0;
          delay_ms(memory[T3_ADR]); //�ʹ���ʱ
          SCISSORS_0; //������λ
          delay_ms(memory[T6_ADR]); //�ʹ���ʱ��ʱ 
          MotorControl(MOTOR_POSITIVE,memory[SET_SENDWITHSPEED_ADR]);//�����ת
          delay_ms(memory[T4_ADR]); //�ʹ����ȼ�ʱ
        } 
          MotorControl(MOTOR_POSITIVE,0);//���ֹͣ 
        
        break;
      }
    case LRE_L0R0: //�ȷ���β����  ���߼�
      {
        Pedal_old=Pedal;  //��״̬������״̬
        PRESSURE_0; //����
        GUN_0;   //�ϰ���
        MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
        Pedal=L0R0;
        Pedal_old=Pedal;
        HotBlowerContinue_falg=0;
        break;
      }
    }
    default:
      Pedal_old=Pedal;  //��״̬������״̬
      break;
  }
}

void SingeControl(unsigned short addr) //���
{
  if(MOTOR_CLICK==addr) 
  {
    if(memory[MOTOR_CLICK])  //����㶯
      MotorControl(MOTOR_NEGATIVE,memory[MOTOR_CLICKSPEED]);
    else
       MotorControl(MOTOR_POSITIVE,0);//���ֹͣ
  }
  if(Y1_ADR==addr) 
  {
    if(memory[Y1_ADR])  //�������
      MOTORDIRECTOE_1;
    else
      MOTORDIRECTOE_0;
  }
  
  if(Y2_ADR==addr)
  {
    if(memory[Y2_ADR]) //�������
      MOTORRUN_1;   
    else
      MOTORRUN_0; 
  }
  
  if(Y3_ADR==addr)
  {
    if(memory[Y3_ADR]) //��
      AIR_1;    
    else
      AIR_0;  
  }
  
  if(Y10_ADR==addr)
  {
    if(memory[Y10_ADR])//ǹ
      GUN_1;       
    else
      GUN_0;   
  }
  
  if(Y11_ADR==addr)
  {
    if(memory[Y11_ADR])//����
      SCISSORS_1;       
    else
      SCISSORS_0;
  }
  
  if(Y12_ADR==addr)
  {
    if(memory[Y12_ADR])//��
      PRESSURE_1;       
    else
      PRESSURE_0;
  }
  
}
#endif /* _PROCESS_C */