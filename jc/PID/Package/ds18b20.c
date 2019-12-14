
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

//��ʱ���� 

/*----------------------------------------------------------------------- 
 
-----------------------------------------------------------------------*/ 
/*
	ʹ��systick��reloadֵ��9000����1usΪ9������ֵ
*/
void delay_nus(u16 n)       //N us��ʱ���� 
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

//�������ϵ����д�����ӳ�ʼ����ʼ
void init_1820()  
{  
    DIR_1WIRE_OUT();
    SET_OP_1WIRE();   
    CLR_OP_1WIRE();  
    delay_nus(600);    //480us����  
    SET_OP_1WIRE();  
    DIR_1WIRE_IN();  
    delay_nus(30);     //15~60us  
    while(CHECK_IP_1WIRE());  
    DIR_1WIRE_OUT();  
    SET_OP_1WIRE();  
    delay_nus(160);   //60~240us  
}
/*
ָ��                    ����
Read ROM(��ROM)         [33H]
Match ROM(ƥ��ROM)      [55H]
Skip ROM(����ROM]       [CCH]
Search ROM(����ROM)     [F0H]
Alarm search(�澯����)  [ECH]

ָ��                             ����
Write Scratchpad(д�ݴ�洢��) 	[4EH]
Read Scratchpad(���ݴ�洢��) 	[BEH]
Copy Scratchpad(�����ݴ�洢��) [48H]
Convert Temperature(�¶ȱ任) 	[44H]
Recall EPROM(���µ���)          [B8H]
Read Power supply(����Դ)       [B4H]
*/  
void write_1820(u8 x)  
{     
    u16 i;  
    
	for(i=0; i<8; i++)  
    {   
        DIR_1WIRE_OUT();
       	CLR_OP_1WIRE();
       	if(x & 0x01)    //д�����ˣ���д��λ�ģ� 
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
	    //������,�ӵ�λ��ʼ  
	    if(CHECK_IP_1WIRE()==1)  
	    	x |= 0x80;
	    delay_nus(100); //60~120us      
	    DIR_1WIRE_OUT();  
    }
    return (x);  
}   

/*
	����ֵ���¶�
	��ʽs,s,s,s, s,b6,b5,b4, b3,b2,b1,b0, b-1,b-2,b-3,b-4
	ת��Ϊʮ����
	�¶� T = (int)val*10/16;
	�Ŵ���10������λ��С�����֡�
	

 */
void DS18B20_Convert(void) //�¶�ת��
{
     init_1820();        //��λ18b20  
     write_1820(0xcc);   // ����ת������  
     write_1820(0x44); 
}

float DS18B20_Read(void)     //��ȡ�¶�ֵ 
{  
     u16  data,dat1,dat2;
     float res; 
     init_1820();  
     write_1820(0xcc);  //����������  
     write_1820(0xbe);  

     dat1=read_1820();  //������  
     dat2=read_1820(); 
     dat2=(dat2<<8);
     data= dat1|dat2; 
     
     res=(data*1/ 16);
     return res;
}   


