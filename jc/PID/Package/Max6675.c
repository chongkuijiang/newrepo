/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAX6675_C
#define _MAX6675_C
#include "stm32f10x_lib.h"
#include "Max6675.h"

int k_cut_off_flag = 0;//断k偶标志
void max6675_delay(u16 n)
{
  while (n--)
  {
    asm("nop"); 
    asm("nop"); 
  }
}

float read_max6675_temper(void)// 利用max6675读k探头的温度 返回最终温度的1倍
{
  
  u8 i=0;   
  u16 dat_temp=0;    
  float dat_6675=0.0;   

  cs1_0;
  max6675_delay(10);
  sck_0;
  for(i=0;i<16;i++)   
  {      		 
    sck_1;
    max6675_delay(10);      
    dat_temp = dat_temp<<1;    
    if(so_status)
    {
      dat_temp |= 0x01;   
    }
    sck_0;
    max6675_delay(10);		
  }   
  cs1_1;
  max6675_delay(10);
  sck_0; 
  k_cut_off_flag = (dat_temp>>2)&0x0001; //断偶标志 1的话，断开
  dat_temp = dat_temp<<1;   
  dat_temp = dat_temp>>4;//舍弃最高位及最低的3位 留下中间的12位 这12位中的后2位代表小数位
  
  //dat_6675 = (float)((((float)(dat_temp & 0x0003)*0.25) +  (float)(dat_temp>>2))*1.0);//最终温度扩大1倍
  dat_6675 = ((float)dat_temp) * 0.25;
  
  return dat_6675;  
}

  


#endif /* _MAX6675_H */