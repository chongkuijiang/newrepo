#include "stm32f10x.h"
#include "userApp.h"
#include "TableLED.txt"
#include <stdlib.h>

//-------------------------------------------------
void Delay(UINT num)
{
	UCHAR i;
	while(num--)
	{
		for(i=0;i<255;i++);
	}
}
//--------------------------------------------------
void Hchang(uint8_t dir)
{
	if(dir == 0){
	    GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		stTime = 8000;  //5ms
	    while(stTime >0){}
	}
	else if(dir == 1){
	   GPIO_SetBits(GPIOB,GPIO_Pin_12);	
       stTime = 8000;  //1000ms
	   while(stTime >0){}		
	}
	
}
//------------------------------------------------
uint16_t CurrentAdc(uint16_t tim)
{
	uint16_t temp;
	stTime = tim;  
	SYS.AD_AddValue[2] = 0;
	SYS.AD_num2 = 0;
	while(stTime >0){}
	temp = SYS.AD_AddValue[2]/SYS.AD_num2;	
	return temp;	
}
//--------------------------------------------------
//extern const UINT ascii[128];
//void Delay(UINT num);
void Buzzer(BOOL command);
extern BOOL disUpFlag;
void errCodeDisp(uint16_t code,uint16_t num)
{
	switch (code)
	{
		case F1_OP: LED[0]=ascii['F'];
					LED[1]=ascii['U'];
					LED[2]=ascii['-'];
					LED[3]=ascii['1'];
			        break;
		case F2_OP: LED[0]=ascii['F'];
					LED[1]=ascii['U'];
					LED[2]=ascii['-'];
					LED[3]=ascii['2'];
			        break;
		
		case TC_RE: LED[0]=ascii['T'];
					LED[1]=ascii['C'];
					LED[2]=ascii['R'];
					LED[3]=ascii['E'];
			        break;
		case TC_OP: LED[0]=ascii['T'];
					LED[1]=ascii['C'];
					LED[2]=ascii['O'];
					LED[3]=ascii['P'];
					break;
		case TC_ST: LED[0]=ascii['T'];
					LED[1]=ascii['C'];
					LED[2]=ascii['S'];
					LED[3]=ascii['T'];
					break;
			
		case HC_RE: LED[0]=ascii['H'];
					LED[1]=ascii['C'];
					LED[2]=ascii['R'];
					LED[3]=ascii['E'];
			        break;		
		case TR_ST: LED[0]=ascii['T'];
					LED[1]=ascii['R'];
					LED[2]=ascii['S'];
					LED[3]=ascii['T'];
					break;
					
        case HT_ST: LED[0]=ascii['H'];
					LED[1]=ascii['T'];
					LED[2]=ascii['S'];
					LED[3]=ascii['T'];
					break;	
        case HT_OP: LED[0]=ascii['H'];
					LED[1]=ascii['T'];
					LED[2]=ascii['O'];
					LED[3]=ascii['P'];
					break;	
		case TEST:  LED[0]=ascii['C'];
					LED[1]=ascii['H'];
					LED[2]=ascii['K'];
					LED[3]=ascii['-'];
					disUpFlag=1;
					break;				
		default:
			break;
	}
	if(num == 0)
	{
		LED[4]=ascii['-'];
		LED[5]=ascii['-'];
		LED[6]=ascii['-'];
		LED[7]=ascii['-'];
	}
	else
	{
		LED[4]=ascii[(num/1000%10)+0x30];
		LED[5]=ascii[(num/100%10)+0x30];
		LED[6]=ascii[(num/10%10)+0x30];
		LED[7]=ascii[(num%10)+0x30];		
	}
	if(code != TEST){
		SYS.DataGet[Rate] = 0;
		disUpFlag=1;
		Buzzer(ON);
		while(1);
	}
}	
//------------------------------------------------
void check_HT(void)
{
	uint16_t errCode=NoError,Ctemp=0;
	
	startDelay = 1;
	SYS.DataGet[Rate] = 5;
	trun = 0;
	while(startDelay >0){}			
	while(trun >0){}	
    SYS.DataGet[Rate] = 0;		
	Ctemp=CurrentAdc(22);	//20	
    if(Ctemp > 50)//60 //6,7,6,9,5,
	{ //66,67,64,83,66,75,73
		errCode = HC_RE;
		errCodeDisp(errCode,0);
	}	
  	
}
//------------------------------------------------

void HT_test(void)
{
	uint16_t temp;
	uint16_t errCode=NoError;
	startDelay = 1;
	SYS.DataGet[Rate] = 20;
	trun = 0;
	while(startDelay >0){}			
	while(trun >0){}
    SYS.DataGet[Rate] = 0;		
	temp=CurrentAdc(25);		
    if(temp > 320)//320 //电热线电流大 
	{ //331,333,335,313,313,321,336,357
		errCode = HT_ST;
		errCodeDisp(errCode,0);
	}	
  	else if(temp == 0) //电热线断
	{
		errCode = HT_OP;
		errCodeDisp(errCode,0);
	}	
	stTime = 500;  //50ms
	while(stTime >0){}
}
//------------------------------------------------
int16_t temAdc(uint16_t tim)
{
	stTime = tim;  
	SYS.AD_AddValue[0] = 0;
	SYS.AD_num0 = 0;
	while(stTime >0){}
	return(SYS.AD_AddValue[0]/SYS.AD_num0);	
}

//------------------------------------------------


void stTest(void)
{
	uint16_t temAd,temAd_1,AD;
    uint16_t errCode=NoError;
	SYS.DataGet[Rate] = 0;
	Hchang(0);
	
	if(GPIOB->IDR & GPIO_Pin_1) //保险丝断
	{
		stTime = 120;
		while(stTime >0){
			if(!(GPIOB->IDR & GPIO_Pin_1)){
			   errCode = F1_OP;
			   errCodeDisp(errCode,0);
		    }
		}
	}
	else
	{
		errCode = F1_OP;
		errCodeDisp(errCode,0);
	}
//--------	
	if(GPIOB->IDR & GPIO_Pin_2) //保险丝断
	{
		stTime = 120;
		while(stTime >0){
			if(!(GPIOB->IDR & GPIO_Pin_2)){
			   errCode = F2_OP;
			   errCodeDisp(errCode,0);
		    }
		}
	}
	else
	{
		errCode = F2_OP;
		errCodeDisp(errCode,0);
	}
//--------	
	AD = CurrentAdc(400);		
    if(AD > 20)   //可控硅短路
	{
		errCode = TR_ST;
		errCodeDisp(errCode,0);
	}	
//--------	
	if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
	{
		Delay(100);
		if(GPIOA->IDR & GPIO_Pin_12){
			errCode = TC_RE;
			errCodeDisp(errCode,0);
		}
	}
//-------	
	temAd_1 = temAdc(4000);
	Hchang(1);
	temAd = temAdc(4000);
	AD = temAd - temAd_1;		
	AD = (AD*3300)/4095;	
	Hchang(0);	
	if((temAd_1>4000) || (temAd>4000)) //感温线断
	{
		errCode = TC_OP;
		errCodeDisp(errCode,0);
	}
//--------	
//加热圈>11.7R 	热电耦<15R
    if(AD>70)  //加热圈与感温线接反69
	{ //69,69,69,69,69,69
	    if(AD<=95)//89,89,90,88,88,89,89,89,
		{
			check_HT();
			check_HT();
		}
		else //>95
		{
			errCode = HC_RE;
			errCodeDisp(errCode,0);
		}
	}
	
//----------		
	HT_test(); //加热圈测式
	HT_test();
/*	
	SYS.DataGet[Rate] = 150;		
	testTime = 10; //10s
	stTime = 10*10000; 
	errCode = TEST;
	while(stTime >0){
		if(stTime%10000 == 0){
			if(disp ==0){
				testTime--;
				if(testTime ==2) SYS.DataGet[Rate] = 0;
				errCodeDisp(errCode,testTime);
				disp = 1;
			}
		}else disp = 0;
	}		
	SYS.DataGet[Rate] = 0;	
//	if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
//	{
//		errCode = TC_RE;
//		errCodeDisp(errCode,0);
//	}
    if(AD==0)  //感温线短路
	{
		if(temAdc(1000)==0){
			errCode = TC_ST;
			errCodeDisp(errCode,0);
		}
	}
*/	
}
//----------------------------------------------------------------------------
//uint16_t converCurrent(uint16_t x)
//{
//	uint16_t y;
//	static uint16_t i;
//	static struct
//	{
//		uint16_t x;
//		uint16_t y;
//	}tab[20]={
//		{81,1000},{157,203},{225,290},{288,374},{350,451},{411,533},{475,610},{537,688},{600,766},{665,846},
//		{728,926},{792,999},{858,1085},{926,1164},{995,1252},{1070,1337},{1150,1427},{1230,1525},{1330,1647},{1580,1916},	
//	}; 
//	y = 0;
//    if(x < tab[0].x)
//		return x*1.27;
//	else if(x > tab[19].x)
//		return x*1.27;
//	while(1)
//	{
//		if(x < tab[i].x) 
//		{
//			i--;
//			if(x > tab[i].x)			
//			break;
//		}
//        else if(x > tab[i].x)
//		{
//			i++;
//			if(x < tab[i].x) {
//				i--;
//				break;
//			}
//		}
//        else 
//		{
//			y = 0;
//			break;
//		}					
//	}
//	if(y == 1)		
//	   return tab[i].y + ((x-tab[i].x) / (tab[i+1].x-tab[i].x)) * (tab[i+1].y-tab[i].y);
//	else
//	   return tab[i].y;	
//}
/*//延时450ms-280ms
//短路
//0-
温度：--
//0--4.7,1-31.4  =31.4  //实温
//0-3.7, 1-38.7  =35    //实温反向
//0--30.7,1--6   =0     //实温反向加热  
//0--30,1-11.4   =11.4
//0--22,1-15.2   =15.2
//0--13.7,1-25.6 =25.6
//0-66.8,1-102   =35.2 //实温加热  
//0-60,1-90.7    =30.7
//0-33.5,1-67    =33.5
//0--2.6,1-33.5  =33.5

//0--0.3,1-85    =85    //电阻13.6R //0-15R
//0-0,   1-301   =301   //电阻500R

//tcre = -340mv
u*u/r=3000=16
//  70< x <110
*/
/*
1.4A -> 249mv   //1400/250=5.6 //1400/5.6=250 //15000/5.6=2678mv
1.4A -> 250mv
1.4A -> 247mv   //1400/247=5.66
0.4A -> 42.9mv  //400/42.9=9.3
0.6A -> 55mv    //600/55=10.9
0.4A -> 51.7Mv  //400/51.7=7.7
0.6A ->57.8mv  //600/57.8=10.38
0.6->57.2
1.4->254   //1400/254=5.5
1.2->127   //1200/127=9.44
1.3->204   //1300/204=6.37
/////////
1.49 ->252 //5.9
1.32->184  //7.17
0.52->87.2  //5.96
0.16->31.1  //5.14
0.2->33.9 //5.8

*/
//-----------------------------------------------
static uint8_t comAddrss,tempId;


#define BUSZ  ((GPIOB->IDR&(1<<6))&&(GPIOB->IDR&(1<<7)))

void Txcom(uint16_t rad)	
{	
	uint16_8_t crc;
	int i;
	static uint16_t comBusz;
	static uint8_t Txbuf[6],state;
	
	if(state ==0){
		if(BUSZ)
		{
			srand(rad); 
			comBusz = 500+(int)(2000.0*rand()/(RAND_MAX+1.0)); //50-250ms
			tempId = (comBusz-499)%255;
			state = 1;
		} //500-2500
	}
	else if(state ==1){
		if(comBusz >0){
			comBusz--;
			if(!BUSZ) state = 0;
			if(comBusz ==0)	state = 2;				
		}
	}
	else if(state ==2){
		if(BUSZ)
		{
			Txbuf[0] = comAddrss;
			Txbuf[1] = 0xaa;
			
			if(comAddrss)				
			Txbuf[2] = SYS.ErrorCode;
			else
			Txbuf[2] = tempId;	
			
			crc = CreatCRC16Value(Txbuf, 3);
			Txbuf[3] = crc.Bit[1];
			Txbuf[4] = crc.Bit[0];					
			for(i=0;i<5;i++){
			   USART_SendData(USART2, Txbuf[i]);
			   while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
		   }
		   comTx = 8000;				
		}
		state = 0;
	}				
}
//-------------------------------------------------------------------
void Rxcom(void)
{
	uint16_8_t crc;
	int i;	
	
    Rxbuf1[0] = 0;		
	if(comAddrss ==0)
	{
		if((Rxbuf1[1]==tempId) && (Rxbuf1[2]==0xab))
		{
			crc = CreatCRC16Value(&Rxbuf1[1], 2);
			if((Rxbuf1[3] == crc.Bit[1])&&(Rxbuf1[4] == crc.Bit[0]))
			{
				comAddrss = Rxbuf1[1];
			}
		}
	}
	for(i=0;i<8;i++) Rxbuf1[i] = 0;
	
}

//void Rxcom(void)
//{
//	uint16_8_t crc;
//	int i;
//	

//    Rxbuf1[0] = 0;	
//	if((Rxbuf1[1]==SYS.DataSet[ID]) && (Rxbuf1[2]==0xaa))
//	{
//		crc = CreatCRC16Value(&Rxbuf1[1], 2);
//		if((Rxbuf1[3] == crc.Bit[1])&&(Rxbuf1[4] == crc.Bit[0]))
//		{
//			Txbuf[0] = comAddrss;//SYS.DataSet[ID];
//			Txbuf[1] = 0xaa;
//			Txbuf[2] = SYS.ErrorCode;
//			crc = CreatCRC16Value(Txbuf, 3);
//			Txbuf[3] = crc.Bit[1];
//			Txbuf[4] = crc.Bit[0];
//			for(i=0;i<5;i++){
//			   USART_SendData(USART1, Txbuf[i]);
//			   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
//		   }
//		}
//	}
//	for(i=0;i<8;i++) Rxbuf1[i] = 0;
//	
//}
