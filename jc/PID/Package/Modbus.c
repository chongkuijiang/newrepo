#ifndef _MODBUS_C
#define _MODBUS_C
#include "stm32f10x_lib.h"
#include "uart.h"
#include "Flash.h"
#include "Pid.h"
#include "Process.h"
#include "Modbus.h"

/*************Both Uart Used Variable Define *********************/
unsigned char crc1,crc2;
unsigned char crc3,crc4;
unsigned short memory[200];
unsigned char  Modbus_Rxflag=0;
/****************************************************************************	F
* 名称：uart_crc()
* 功能：串口数据 CRC16 运算程序
* 入口参数：无
* 出口参数：无
****************************************************************************/
void uart_crc(unsigned char* data , unsigned char length)
{
  unsigned short reg_crc;
  unsigned short s_crcchk;
  s_crcchk = 0;
  reg_crc = 0xffff;
  while(length--)
  {
    reg_crc ^= *data++;
    for(s_crcchk = 0; s_crcchk < 8; s_crcchk ++)
    {
      if(reg_crc & 0x01)
      {
        reg_crc = (reg_crc >> 1) ^ 0xa001;
      }
      else
      {
        reg_crc = reg_crc >> 1;
      }
    }
  }
  crc1 = reg_crc & 0x00ff;
  crc2 = reg_crc >> 8;
}



/****************************************************************************  
* 名称：uart0_dispose()
* 功能：串口uart1数据处理。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void uart1_dispose(void)
{
  unsigned short address;
  unsigned short length;
  unsigned short counter;
  //
  unsigned short tc;
  
  if((uart1_rxbuf[0] == 0x08) || (uart1_rxbuf[0] == 0x01 )) //通讯地址判断机制——8位模式	 #### 以后用数据表参数代替
  {
    crc3 = uart1_rxbuf[uart1_rxptr - 2];
    crc4 = uart1_rxbuf[uart1_rxptr - 1];
    
    uart_crc(uart1_rxbuf , (uart1_rxptr - 2));
    if((crc3 == crc1)&&(crc4 == crc2))
    {
      //+++++++++++++++++++++++++++++++++++++++++++++++
      switch(uart1_rxbuf[1])
      {
        //+++++++++++++++++++++++++++++++++++++++++++++++
        
        case(0x01):	
          address = uart1_rxbuf[2];
          address = address << 8;
          address |= uart1_rxbuf[3];
          
          length = uart1_rxbuf[4];
          length = length << 8;
          length |= uart1_rxbuf[5];
          
          counter = 2;
          uart1_txbuf[0] = uart1_rxbuf[0];
          uart1_txbuf[1] = uart1_rxbuf[1];
          
          if(address > 200) address = 199;
          if((length+address) > 200) length = 200-address;
          
//          while(length)
//          {
//            counter ++;
//            uart1_txbuf[counter] = memory[address] >> 8;
//            counter ++;
//            uart1_txbuf[counter] = memory[address] & 0x00ff;
//            
//            length --;
//            address ++;
//          }
           counter++;
          for(unsigned short bit=0;bit<length;bit++)
          {
            
             if(((bit%8)==0)&(bit!=0))
              counter ++;  
             if(memory[address+bit]==0xFF00)
               uart1_txbuf[counter] |=(1<<(bit%8));
             else
               uart1_txbuf[counter] &=~(1<<(bit%8));
            
          }
          
          uart1_txbuf[2]= counter -2;
          uart_crc(uart1_txbuf,(counter + 1));
          uart1_txbuf[counter+1] = crc1;
          uart1_txbuf[counter+2] = crc2;
          uart1_txall = counter+2;
          
          uart1_txptr = 0;
          
          //COM_OUT;
          USART1->DR = uart1_txbuf[0];
          break;
        case(0x03):	
          address = uart1_rxbuf[2];
          address = address << 8;
          address |= uart1_rxbuf[3];
          
          length = uart1_rxbuf[4];
          length = length << 8;
          length |= uart1_rxbuf[5];
          
//          counter = 4;
            counter = 2;
          uart1_txbuf[0] = uart1_rxbuf[0];
          uart1_txbuf[1] = uart1_rxbuf[1];
//          uart1_txbuf[2] = uart1_rxbuf[2];
//          uart1_txbuf[3] = uart1_rxbuf[3];
          
           if(address > 200) address = 199;
           if((length+address) > 200) length = 200-address;
          
          while(length)
          {
            counter ++;
            uart1_txbuf[counter] = memory[address] >> 8;
            counter ++;
            uart1_txbuf[counter] = memory[address] & 0x00ff;
            
            length --;
            address ++;
          }
          
//          uart1_txbuf[4]= counter -4;
          uart1_txbuf[2]= counter -2;
          uart_crc(uart1_txbuf,(counter + 1));
          uart1_txbuf[counter+1] = crc1;
          uart1_txbuf[counter+2] = crc2;
          uart1_txall = counter+2;
          
          uart1_txptr = 0;
          
          //COM_OUT;
          USART1->DR = uart1_txbuf[0];
          break;
          
          case(0x05):
            address = uart1_rxbuf[2];
            address = address << 8;
            address |= uart1_rxbuf[3];
            
            counter = 7;
            uart1_txbuf[0] = uart1_rxbuf[0];
            uart1_txbuf[1] = uart1_rxbuf[1];
            uart1_txbuf[2] = uart1_rxbuf[2];
            uart1_txbuf[3] = uart1_rxbuf[3];
            uart1_txbuf[4] = uart1_rxbuf[4];
            uart1_txbuf[5] = uart1_rxbuf[5];
            
            if(address > 200) address = 199;

            tc = uart1_rxbuf[4];
            tc = tc << 8;
            tc |= uart1_rxbuf[5];
            memory[address] = tc;
 
            uart1_txbuf[6] = uart1_rxbuf[6];
            uart1_txbuf[7] = uart1_rxbuf[7];
            uart1_txall = 7;
            
            uart1_txptr = 0;
            if((address>=TEMPCONTROL_ADR)&&(address!=TEST_ACTION)&&(address!=AUTO_CUT)&&(address!=MOTOR_CLICK))
            {
             flash_save(&memory[TEMPCONTROL_ADR-1]); //存储参数
            }
//            flash_save(&memory[TEMPCONTROL_ADR]); //存储参数
            //COM_OUT;
            USART1->DR = uart1_txbuf[0];
            SingeControl(address); //点控
            break;
          //+++++++++++++++++++++++++++++++++++++++++++++++
          case(0x06):
            address = uart1_rxbuf[2];
            address = address << 8;
            address |= uart1_rxbuf[3];
            
            counter = 7;
            uart1_txbuf[0] = uart1_rxbuf[0];
            uart1_txbuf[1] = uart1_rxbuf[1];
            uart1_txbuf[2] = uart1_rxbuf[2];
            uart1_txbuf[3] = uart1_rxbuf[3];
            uart1_txbuf[4] = uart1_rxbuf[4];
            uart1_txbuf[5] = uart1_rxbuf[5];
            
            if(address > 200) address = 199;

            
            tc = uart1_rxbuf[4];
            tc = tc << 8;
            tc |= uart1_rxbuf[5];
            memory[address] = tc;
            
            
            uart1_txbuf[6] = uart1_rxbuf[6];
            uart1_txbuf[7] = uart1_rxbuf[7];
            uart1_txall = 7;
            
            uart1_txptr = 0;
            if((address>=TEMPCONTROL_ADR)&&(address!=TEST_ACTION)&&(address!=AUTO_CUT)&&(address!=MOTOR_CLICK))
            {
             flash_save(&memory[TEMPCONTROL_ADR-1]); //存储参数
             SV_value=((float)memory[SET_TEMP_ADR])/10; //获取设定温度
            }
            else if(address==Y0_ADR)
              TIM2->CCR4 =memory[TEMPCONTROL_ADR]*4;
              
            //COM_OUT;
            USART1->DR = uart1_txbuf[0];
            break;
            //+++++++++++++++++++++++++++++++++++++++++++++++
            case(0x10):
              address = uart1_rxbuf[2];
              address = address << 8;
              address |= uart1_rxbuf[3];
              
              length = uart1_rxbuf[4];
              length = length << 8;
              length |= uart1_rxbuf[5];
              
              counter = 7;
              uart1_txbuf[0] = uart1_rxbuf[0];
              uart1_txbuf[1] = uart1_rxbuf[1];
              uart1_txbuf[2] = uart1_rxbuf[2];
              uart1_txbuf[3] = uart1_rxbuf[3];
              uart1_txbuf[4] = uart1_rxbuf[4];
              uart1_txbuf[5] = uart1_rxbuf[5];
              
              if(address > 200) address = 199;
              if((length+address) > 200) length = 200-address;
              
              
              while(length)
              {
                tc = uart1_rxbuf[counter];
                tc = tc << 8;
                tc |= uart1_rxbuf[counter +1];
                memory[address] = tc;
                
                counter +=2;
                address ++;
                length --;
              }   
 
              uart_crc(uart1_txbuf,6);
              uart1_txbuf[6] = crc1;
              uart1_txbuf[7] = crc2;
              uart1_txall = 7;
              uart1_txptr = 0;
             if((address>=TEMPCONTROL_ADR)&&(address!=TEST_ACTION)&&(address!=AUTO_CUT)&&(address!=MOTOR_CLICK))
               {
                 flash_save(&memory[TEMPCONTROL_ADR-1]); //存储参数
                 SV_value=((float)memory[SET_TEMP_ADR])/10; //获取设定温度
               }
              else if(address==Y0_ADR)
               TIM2->CCR4 =memory[TEMPCONTROL_ADR]*4;
              //COM_OUT;
              USART1->DR = uart1_txbuf[0];
              
              break;                       
      }		 		
    }
  }
  uart1_rxptr = 0;		  //清空uart1的缓冲计数 以及 定时
}



#endif /* _MODBUS_C */