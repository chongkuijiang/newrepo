#include <stm32f10x_lib.h>
#include <stdint.h>

//--------------------------------------------------
void Hchang_1(uint8_t dir)
{
	if(dir == 0){
	    GPIO_ResetBits(GPIOE,GPIO_Pin_2);
		OSTimeDly(800); //800ms
	}
	else if(dir == 1){
	   GPIO_SetBits(GPIOE,GPIO_Pin_2);	
       OSTimeDly(800);
	}	
}
//--------------------------------------------------
void Hchang_2(uint8_t dir)
{
	if(dir == 0){
	    GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		OSTimeDly(800); //800ms
	}
	else if(dir == 1){
	   GPIO_SetBits(GPIOE,GPIO_Pin_3);	
       OSTimeDly(800);
	}	
}
//--------------------------------------------------
void Hchang_3(uint8_t dir)
{
	if(dir == 0){
	    GPIO_ResetBits(GPIOE,GPIO_Pin_4);
		OSTimeDly(800); //800ms
	}
	else if(dir == 1){
	   GPIO_SetBits(GPIOE,GPIO_Pin_4);	
       OSTimeDly(800);
	}	
}
//--------------------------------------------------
void Hchang_4(uint8_t dir)
{
	if(dir == 0){
	    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		OSTimeDly(800); //800ms
	}
	else if(dir == 1){
	   GPIO_SetBits(GPIOE,GPIO_Pin_5);	
       OSTimeDly(800);
	}	
}

//------------------------------------------------
uint16_t CurrentAdc_1(uint16_t tim)
{
	uint16_t temp; 
	u8 ADnum=7;
	
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	temp = SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum];	
	return temp;	
}
//------------------------------------------------
uint16_t CurrentAdc_2(uint16_t tim)
{
	uint16_t temp; 
	u8 ADnum=6;
	
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	temp = SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum];	
	return temp;	
}
//------------------------------------------------
uint16_t CurrentAdc_3(uint16_t tim)
{
	uint16_t temp; 
	u8 ADnum=5;
	
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	temp = SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum];	
	return temp;	
}
//------------------------------------------------
uint16_t CurrentAdc_4(uint16_t tim)
{
	uint16_t temp; 
	u8 ADnum=4;
	
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	temp = SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum];	
	return temp;	
}

//------------------------------------------------
void errCode_1(uint16_t code)
{
	CHdata1.ErrorCode = code;
	if(code>0){
		checkError1 = 1;
		Relay(1,OFF);
	}
	else checkError1 = 0;
}
//------------------------------------------------
void errCode_2(uint16_t code)
{
	CHdata2.ErrorCode = code;
	if(code>0){
		checkError2 = 1;
		Relay(2,OFF);
	}
	else checkError2 = 0;
}
//------------------------------------------------
void errCode_3(uint16_t code)
{
	CHdata3.ErrorCode = code;
	if(code>0){
		checkError3 = 1;
		Relay(3,OFF);
	}
	else checkError3 = 0;
}
//------------------------------------------------
void errCode_4(uint16_t code)
{
	CHdata4.ErrorCode = code;
	if(code>0){
		checkError4 = 1;
		Relay(4,OFF);
	}
	else checkError4 = 0;
}



//------------------------------------------------
void check_HT_1(void)
{
	uint16_t errCode=0,Ctemp=0;
	
	startDelay1 = 1;
	CHdata1.DataGet[Rate] = 5;
	while(startDelay1 >0){}	
	CHdata1.DataGet[Rate] = 0;	
	OSTimeDly(1);	
	
	Ctemp=CurrentAdc_1(22);	//20	
    if(Ctemp > 15)//8-13 
	{ 
		errCode = HC_RE;
		errCode_1(errCode); 
	}	 	
}
//------------------------------------------------
void check_HT_2(void)
{
	uint16_t errCode=0,Ctemp=0;
	
	startDelay2 = 1;
	CHdata2.DataGet[Rate] = 5;
	while(startDelay2 >0){}	
	CHdata2.DataGet[Rate] = 0;	
	OSTimeDly(1);	
	
	Ctemp=CurrentAdc_2(22);	//20	
    if(Ctemp > 15)//8-13 
	{ 
		errCode = HC_RE;
		errCode_2(errCode); 
	}	 	
}
//------------------------------------------------
void check_HT_3(void)
{
	uint16_t errCode=0,Ctemp=0;
	
	startDelay3 = 1;
	CHdata3.DataGet[Rate] = 5;
	while(startDelay3 >0){}	
	CHdata3.DataGet[Rate] = 0;	
	OSTimeDly(1);	
	
	Ctemp=CurrentAdc_3(22);	//20	
    if(Ctemp > 15)//8-13 
	{ 
		errCode = HC_RE;
		errCode_3(errCode); 
	}	 	
}
//------------------------------------------------
void check_HT_4(void)
{
	uint16_t errCode=0,Ctemp=0;
	
	startDelay4 = 1;
	CHdata4.DataGet[Rate] = 5;
	while(startDelay4 >0){}	
	CHdata4.DataGet[Rate] = 0;	
	OSTimeDly(1);	
	
	Ctemp=CurrentAdc_4(22);	//20	
    if(Ctemp > 15)//8-13 
	{ 
		errCode = HC_RE;
		errCode_4(errCode); 
	}	 	
}

//------------------------------------------------
int16_t temAdc_1(uint16_t tim)
{
	u8 ADnum=0;
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	return(SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum]);	
}
//------------------------------------------------
int16_t temAdc_2(uint16_t tim)
{
	u8 ADnum=1;
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	return(SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum]);	
}
//------------------------------------------------
int16_t temAdc_3(uint16_t tim)
{
	u8 ADnum=2;
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	return(SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum]);	
}
//------------------------------------------------
int16_t temAdc_4(uint16_t tim)
{
	u8 ADnum=3;
	SYS.AD_AddValue[ADnum] = 0;
	SYS.AD_num[ADnum] = 0;
	OSTimeDly(tim);
	return(SYS.AD_AddValue[ADnum]/SYS.AD_num[ADnum]);	
}

//------------------------------------------------
void stTest_1(void)
{
	uint16_t temAd,temAd_1,AD;
    uint16_t errCode=0;
	static uint16_t ready=0;
	if(checkOff==0){
		ready = 0;
	}
    else if(ready==0)
	{
		ready = 1;
		CHdata1.DataGet[Rate] = 0;
		checkError1 = 0;
		Hchang_1(0);
		
		if(GPIOB->IDR & GPIO_Pin_1) //保险丝断
		{
			OSTimeDly(12);
			if(!(GPIOB->IDR & GPIO_Pin_1)){
			   errCode = Fu_OP;
			   errCode_1(errCode);
			}

		}
		else
		{
			errCode = Fu_OP;
			errCode_1(errCode);
		}
	//--------	
		AD = CurrentAdc_1(400);		
		if(AD > 20)   //可控硅短路
		{
			errCode = TR_ST;
			errCode_1(errCode);
		}	
	//--------	
		if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
		{
			OSTimeDly(12);
			if(GPIOA->IDR & GPIO_Pin_12){
				errCode = TC_RE;
				errCode_1(errCode);
			}
		}
	//-------	
		temAd_1 = temAdc_1(4000);
		Hchang_1(1);
		temAd = temAdc_1(4000);
		AD = temAd - temAd_1;		
		AD = (AD*3300)/4095;	
		Hchang_1(0);	
		if((temAd_1>4000) || (temAd>4000)) //感温线断
		{
			errCode = TC_OP;
			errCode_1(errCode);
		}
	//加热圈>11.7R 	热电耦<15R
		if(AD>70)  //加热圈与感温线接反69
		{ //69,69,69,69,69,69
			//18R: 111,112,111
			//22.5R: 117,116,
			if(AD<=116)//113
			{
				check_HT_1();
			}
			else 
			{
				errCode = HC_RE;
				errCode_1(errCode);
			}
		}		
	//----------	
		check_HT_1();

	}
}
//------------------------------------------------
void stTest_2(void)
{
	uint16_t temAd,temAd_1,AD;
    uint16_t errCode=0;
    static uint16_t ready=0;
	if(checkOff==0){
		ready = 0;
	}
    else if(ready==0)
	{
		ready = 1;
		CHdata2.DataGet[Rate] = 0;
		checkError2 = 0;
		Hchang_2(0);
		
		if(GPIOB->IDR & GPIO_Pin_1) //保险丝断
		{
			OSTimeDly(12);
			if(!(GPIOB->IDR & GPIO_Pin_1)){
			   errCode = Fu_OP;
			   errCode_2(errCode);
			}

		}
		else
		{
			errCode = Fu_OP;
			errCode_2(errCode);
		}
	//--------	
		AD = CurrentAdc_2(400);		
		if(AD > 20)   //可控硅短路
		{
			errCode = TR_ST;
			errCode_2(errCode);
		}	
	//--------	
		if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
		{
			OSTimeDly(12);
			if(GPIOA->IDR & GPIO_Pin_12){
				errCode = TC_RE;
				errCode_2(errCode);
			}
		}
	//-------	
		temAd_1 = temAdc_2(4000);
		Hchang_2(1);
		temAd = temAdc_2(4000);
		AD = temAd - temAd_1;		
		AD = (AD*3300)/4095;	
		Hchang_2(0);	
		if((temAd_1>4000) || (temAd>4000)) //感温线断
		{
			errCode = TC_OP;
			errCode_2(errCode);
		}
	//加热圈>11.7R 	热电耦<15R
		if(AD>70)  //加热圈与感温线接反69
		{ //69,69,69,69,69,69
			//18R: 111,112,111
			//22.5R: 117,116,
			if(AD<=116)//113
			{
				check_HT_2();
			}
			else 
			{
				errCode = HC_RE;
				errCode_2(errCode);
			}
		}		
	//----------	
		check_HT_2();
	}
}
//------------------------------------------------
void stTest_3(void)
{
	uint16_t temAd,temAd_1,AD;
    uint16_t errCode=0;
    static uint16_t ready=0;
	if(checkOff==0){
		ready = 0;
	}
    else if(ready==0)
	{
		ready = 1;
		CHdata3.DataGet[Rate] = 0;
		checkError3 = 0;
		Hchang_3(0);
		
		if(GPIOB->IDR & GPIO_Pin_1) //保险丝断
		{
			OSTimeDly(12);
			if(!(GPIOB->IDR & GPIO_Pin_1)){
			   errCode = Fu_OP;
			   errCode_3(errCode);
			}

		}
		else
		{
			errCode = Fu_OP;
			errCode_3(errCode);
		}
	//--------	
		AD = CurrentAdc_3(400);		
		if(AD > 20)   //可控硅短路
		{
			errCode = TR_ST;
			errCode_3(errCode);
		}	
	//--------	
		if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
		{
			OSTimeDly(12);
			if(GPIOA->IDR & GPIO_Pin_12){
				errCode = TC_RE;
				errCode_3(errCode);
			}
		}
	//-------	
		temAd_1 = temAdc_3(4000);
		Hchang_3(1);
		temAd = temAdc_3(4000);
		AD = temAd - temAd_1;		
		AD = (AD*3300)/4095;	
		Hchang_3(0);	
		if((temAd_1>4000) || (temAd>4000)) //感温线断
		{
			errCode = TC_OP;
			errCode_3(errCode);
		}
//加热圈>11.7R 	热电耦<15R
		if(AD>70)  //加热圈与感温线接反69
		{ //69,69,69,69,69,69
			//18R: 111,112,111
			//22.5R: 117,116,
			if(AD<=116)//113
			{
				check_HT_3();
			}
			else 
			{
				errCode = HC_RE;
				errCode_3(errCode);
			}
		}		
	//----------	
		check_HT_3();
	}
}
//------------------------------------------------
void stTest_4(void)
{
	uint16_t temAd,temAd_1,AD;
    uint16_t errCode=0;
    static uint16_t ready=0;
	if(checkOff==0){
		ready = 0;
	}
    else if(ready==0)
	{
		ready = 1;
		CHdata4.DataGet[Rate] = 0;
		checkError4 = 0;
		Hchang_4(0);
		
		if(GPIOB->IDR & GPIO_Pin_1) //保险丝断
		{
			OSTimeDly(12);
			if(!(GPIOB->IDR & GPIO_Pin_1)){
			   errCode = Fu_OP;
			   errCode_4(errCode);
			}

		}
		else
		{
			errCode = Fu_OP;
			errCode_4(errCode);
		}
	//--------	
		AD = CurrentAdc_4(400);		
		if(AD > 20)   //可控硅短路
		{
			errCode = TR_ST;
			errCode_4(errCode);
		}	
	//--------	
		if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
		{
			OSTimeDly(12);
			if(GPIOA->IDR & GPIO_Pin_12){
				errCode = TC_RE;
				errCode_4(errCode);
			}
		}
	//-------	
		temAd_1 = temAdc_4(4000);
		Hchang_4(1);
		temAd = temAdc_4(4000);
		AD = temAd - temAd_1;		
		AD = (AD*3300)/4095;	
		Hchang_4(0);	
		if((temAd_1>4000) || (temAd>4000)) //感温线断
		{
			errCode = TC_OP;
			errCode_4(errCode);
		}
//加热圈>11.7R 	热电耦<15R
		if(AD>70)  //加热圈与感温线接反69
		{ //69,69,69,69,69,69
			//18R: 111,112,111
			//22.5R: 117,116,
			if(AD<=116)//113
			{
				check_HT_4();
			}
			else 
			{
				errCode = HC_RE;
				errCode_4(errCode);
			}
		}		
	//----------	
		check_HT_4();
	}
}


