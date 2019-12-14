
#define _DS18B20_LOCAL
#include "ds18b20.h"
unsigned char Ds18b20_readflag=0;
unsigned char Ds18b20_readcnt=59;
/*******************************************************************************
*******************************************************************************/
void DS18B20_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	// PC9-DQ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  DS18B20_BIT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

//延时程序： 

/*----------------------------------------------------------------------- 
 
-----------------------------------------------------------------------*/ 
/*
	使用systick，reload值是9000，即1us为9个计数值
*/
void delay_nus(u16 n)       //N us延时函数 
{ 
        u16 start;
        u16 total;
	u16 count;
	u16 diff=0;
	
	start = (u16)SysTick->VAL;
	total = 9*n;
	
	while(diff < total)
	{
		count = (u16)SysTick->VAL;
		if(start < count)
			diff = 9000 - (count - start);
		else
			diff = start - count;
	} 
}
   


void ds1820_delay_long(void)
{
  unsigned short n;
  n= 2900;
  while(n) {n--;}
}

//单总线上的所有处理均从初始化开始
void init_1820()  
{  
    DIR_1WIRE_OUT();
    SET_OP_1WIRE();   
    CLR_OP_1WIRE();  
    delay_nus(600);    //480us以上  
    SET_OP_1WIRE();  
    DIR_1WIRE_IN();  
    delay_nus(30);     //15~60us  
    while(CHECK_IP_1WIRE());  
    DIR_1WIRE_OUT();  
    SET_OP_1WIRE();  
    delay_nus(160);   //60~240us  
}
/*
指令                    代码
Read ROM(读ROM)         [33H]
Match ROM(匹配ROM)      [55H]
Skip ROM(跳过ROM]       [CCH]
Search ROM(搜索ROM)     [F0H]
Alarm search(告警搜索)  [ECH]

指令                             代码
Write Scratchpad(写暂存存储器) 	[4EH]
Read Scratchpad(读暂存存储器) 	[BEH]
Copy Scratchpad(复制暂存存储器) [48H]
Convert Temperature(温度变换) 	[44H]
Recall EPROM(重新调出)          [B8H]
Read Power supply(读电源)       [B4H]
*/  
void write_1820(u8 x)  
{     
    u16 i;  
    
	for(i=0; i<8; i++)  
    {   
        DIR_1WIRE_OUT();
       	CLR_OP_1WIRE();
       	if(x & 0x01)    //写数据了，先写低位的！ 
       		SET_OP_1WIRE();  
       	else  
       		CLR_OP_1WIRE();  
		x >>= 1;
       	delay_nus(30);   //15~60us  
       	SET_OP_1WIRE();  
    }  
    SET_OP_1WIRE();  
}  

u8 read_1820(void)  
{      
	u16 i;
	u8 x = 0;
	
	
    for(i=0; i<8; i++)  
    {       DIR_1WIRE_OUT();
	    CLR_OP_1WIRE();  
	    x = x >> 1;
	  //  SET_OP_1WIRE();  
	    DIR_1WIRE_IN();
	    //读数据,从低位开始  
	    if(CHECK_IP_1WIRE()==1)  
	    	x |= 0x80;
	    delay_nus(100); //60~120us      
	    DIR_1WIRE_OUT();  
    }
    return (x);  
}   

/*
	返回值是温度
	格式s,s,s,s, s,b6,b5,b4, b3,b2,b1,b0, b-1,b-2,b-3,b-4
	转换为十进制
	温度 T = (int)val*10/16;
	放大了10倍，个位是小数部分。
	

 */
void DS18B20_Convert(void) //温度转换
{
     init_1820();        //复位18b20  
     write_1820(0xcc);   // 发出转换命令  
     write_1820(0x44); 
}

float DS18B20_Read(void)     //读取温度值 
{  
     u16  data,dat1,dat2;
     float res; 
     init_1820();  
     write_1820(0xcc);  //发出读命令  
     write_1820(0xbe);  

     dat1=read_1820();  //读数据  
     dat2=read_1820(); 
     dat2=(dat2<<8);
     data= dat1|dat2; 
     
     res=(data*1/ 16);
     return res;
}   


