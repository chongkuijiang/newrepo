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
extern BOOL disUpFlag;
void errCodeDisp(uint16_t code)
{
	SYS.ErrorCode = code;	
	if(code > 0){
		SYS.DataGet[Rate] = 0;
		disUpFlag=1;
		replan = 1;
		while(replan);
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
    if(Ctemp > 15)//8-13 
	{ 
		errCode = HC_RE;
		errCodeDisp(errCode);
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
    if(temp > 88)//320 //电热线电流大 
	{ 
		errCode = HT_ST;
		errCodeDisp(errCode);
	}	
  	else if(temp < 4) //电热线断
	{
		errCode = HT_OP;
		errCodeDisp(errCode);
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
	replan = 0;
	SYS.DataGet[Rate] = 0;
	Hchang(0);
	
	if(GPIOB->IDR & GPIO_Pin_1) //保险丝断
	{
		stTime = 120;
		while(stTime >0){
			if(!(GPIOB->IDR & GPIO_Pin_1)){
			   errCode = F1_OP;
			   errCodeDisp(errCode);
		    }
		}
	}
	else
	{
		errCode = F1_OP;
		errCodeDisp(errCode);
	}
//--------	
	if(GPIOB->IDR & GPIO_Pin_2) //保险丝断
	{
		stTime = 120;
		while(stTime >0){
			if(!(GPIOB->IDR & GPIO_Pin_2)){
			   errCode = F2_OP;
			   errCodeDisp(errCode);
		    }
		}
	}
	else
	{
		errCode = F2_OP;
		errCodeDisp(errCode);
	}
//--------	
	AD = CurrentAdc(400);		
    if(AD > 20)   //可控硅短路
	{
		errCode = TR_ST;
		errCodeDisp(errCode);
	}	
//--------	
	if(GPIOA->IDR & GPIO_Pin_12) //感温线接反
	{
		Delay(100);
		if(GPIOA->IDR & GPIO_Pin_12){
			errCode = TC_RE;
			errCodeDisp(errCode);
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
		errCodeDisp(errCode);
	}
//--------	
	
//加热圈>11.7R 	热电耦<15R
    if(AD>70)  //加热圈与感温线接反69
    { //69,69,69,69,69,69
        //18R: 111,112,111
        //22.5R: 117,116,
        if(AD<=116)//113
        {
            check_HT();
        }
        else //>95
        {
            errCode = HC_RE;
            errCodeDisp(errCode);
        }
    }
    
//----------	
    check_HT();


}

//-----------------------------------------------------------------------	
		
void sendData(uint8_t address)
{
	uint16_8_t crc;	
    uint16_t temp;
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
    if(SYS.DataSet[C_F]){
        temp = 9 * SYS.DataGet[Tem_PV]/5 + 320;
    }else{
        temp = SYS.DataGet[Tem_PV];
    }
	Txbuf[0] = address;
	Txbuf[1] = 0x5a;
	Txbuf[2] = SYS.ErrorCode;	         //code
	Txbuf[3] = temp>>8;  //PV
	Txbuf[4] = temp;
	Txbuf[5] = SYS.SV_Dis>>8;           //SV
	Txbuf[6] = SYS.SV_Dis;
	Txbuf[7] = SYS.DataSet[In];          //J_K
	Txbuf[8] = SYS.DataSet[C_F];        //C_F
	Txbuf[9] = SYS.DataGet[Current]>>8; //Iout
	Txbuf[10] = SYS.DataGet[Current];
	Txbuf[11] = SYS.DataGet[Rate]>>8;   //Oout  
	Txbuf[12] = SYS.DataGet[Rate];
	Txbuf[13] = power>>8;                //Pout  
	Txbuf[14] = power;	

	crc = CreatCRC16Value(Txbuf, 15);
	Txbuf[15] = crc.Bit[1];
	Txbuf[16] = crc.Bit[0];
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
}
void sendSetData(uint8_t address)
{
	uint16_8_t crc;	
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	Txbuf[0] = address;
	Txbuf[1] = 0x5b;
	Txbuf[2] = SYS.DataSet[HSCI];     //ssr_pwm  
	Txbuf[3] = SYS.stSV_Dis>>8;       //SV_stby
	Txbuf[4] = SYS.stSV_Dis;   
	Txbuf[5] = SYS.RunningMode;       //run=0_std=1_m=2
	Txbuf[6] = SYS.DataSet[ON_OFF];       //enable
	Txbuf[7] = SYS.DataSet[HC_H]>>8;  //SYS.DataSet[HC_H]
	Txbuf[8] = SYS.DataSet[HC_H];       
	Txbuf[9] = SYS.DataSet[AL_H]>>8;  //SYS.DataSet[AL_H]
	Txbuf[10] = SYS.DataSet[AL_H];
	Txbuf[11] = SYS.DataSet[AL_L]>>8; //SYS.DataSet[AL_L] 
	Txbuf[12] = SYS.DataSet[AL_L];
	Txbuf[13] = version;              //version  
    if(checkOff == 0){
        check = 0;
    }
    else{
       if(replan) check = 3;   //hcre
       else if(check != 1) check = 2; //ok            
    }
	Txbuf[14] = check;	              //开机自检结果
    
	crc = CreatCRC16Value(Txbuf, 15);
	Txbuf[15] = crc.Bit[1];
	Txbuf[16] = crc.Bit[0];
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
}
void sendto()
{
    static uint8_t tim;
    if(tim) {
        tim = 0;
        sendData(comAddrss);
    }
    else {
        tim = 1;
        sendSetData(comAddrss);
    }
}
void readvalue()
{
    uint16_8_t crc;	
    u16 voltage = SYS.TemVoltage/10;
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	Txbuf[0] = comAddrss;
	Txbuf[1] = Rxbuf1[2];
	Txbuf[2] = Rxbuf1[3]; //4
    Txbuf[3] = voltage>>8;       
    Txbuf[4] = voltage;    
    Txbuf[5] = SYS.DataGet[Tem_PV]>>8;       
    Txbuf[6] = SYS.DataGet[Tem_PV];      
    Txbuf[7] = SYS.DataSet[ET_Z]>>8;       
    Txbuf[8] = SYS.DataSet[ET_Z];    
    Txbuf[9] = SYS.DataSet[HT_Z]>>8;       
    Txbuf[10] = SYS.DataSet[HT_Z];     
    Txbuf[11] = SYS.DataSet[HT_A]>>8;       
    Txbuf[12] = SYS.DataSet[HT_A]; 
    Txbuf[13] = SYS.DataGet[Env_sensor]>>8;       
    Txbuf[14] = SYS.DataGet[Env_sensor];  
    crc = CreatCRC16Value(Txbuf, 15);
	Txbuf[15] = crc.Bit[1];
	Txbuf[16] = crc.Bit[0];
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
//-------------------------------------------------------------------
void Rxcom(void)
{
	uint16_8_t crc;
	uint16_t temp=0,i;
	
	
    Rxbuf1[0] = 0;	
	if(Rxbuf1[1]==comAddrss){
        if(Rxbuf1[2]==0x5c) //设定值
		{
			crc = CreatCRC16Value(&Rxbuf1[1], 4);
			if((Rxbuf1[5] == crc.Bit[1])&&(Rxbuf1[6] == crc.Bit[0]))
			{
				temp = (Rxbuf1[3]<<8) + Rxbuf1[4];	
				if(SYS.DataSet[C_F]==0){
					if(temp >=400 && temp <= 4500){
                        
                        
						SYS.DataSet[Tem_SV] = temp;
					}
				}else{
					if(temp >=1040 && temp <= 8420){
						SYS.DataSet[Tem_SV] = (temp-320)*5/9; 
					}
				}
                
				sendto();
			}
		}
		else if(Rxbuf1[2]==0xaa)  //查询
		{
			crc = CreatCRC16Value(&Rxbuf1[1], 2);
			if((Rxbuf1[3] == crc.Bit[1])&&(Rxbuf1[4] == crc.Bit[0]))
			{				
				sendto();					 
			}
		}
		else if(Rxbuf1[2]==0xab) //命令
		{
			crc = CreatCRC16Value(&Rxbuf1[1], 5);
			if((Rxbuf1[6] == crc.Bit[1])&&(Rxbuf1[7] == crc.Bit[0]))
			{
                    temp = (Rxbuf1[4]<<8) +  Rxbuf1[5];
					switch (Rxbuf1[3])
					{
						case 3:						
								 if(SYS.DataSet[C_F]==0){
									if(temp >=400 && temp <= 4500){
										SYS.SV_Dis =  temp;  
                                        SYS.DataSet[Tem_SV] = SYS.DataSet[SV_WK] = SYS.SV_Dis;                                                                        
										SYS.WriteState=SV_WK;  //sv
									}
								 }else{
									if(temp >=1040 && temp <= 8420){
										SYS.SV_Dis =  temp;
                                        SYS.DataSet[Tem_SV] = SYS.DataSet[SV_WK] =(SYS.SV_Dis-320)*5/9; 
										SYS.WriteState=SV_WK;  //sv
									}
								 }							             
							     break;
                        case 4:  
								 if(temp == 0 || temp == 1){
							         SYS.DataSet[ON_OFF] = temp;	//SYS.DataSet[ON_OFF]
                                     SYS.WriteState=ON_OFF;
								 }									 
							     break;          						
						case 5:  
							     if(temp == 0 || temp == 1){
									 SYS.DataSet[C_F] = temp; //cf
                                     SYS.WriteState=C_F;
                                     
                                    if(SYS.DataSet[C_F]==0){
                                       SYS.SV_Dis = SYS.DataSet[Tem_SV];
                                       SYS.stSV_Dis = SYS.DataSet[STSV];
                                    }else{
                                       SYS.SV_Dis = SYS.DataSet[Tem_SV]*9/5+320; 
                                       SYS.stSV_Dis = SYS.DataSet[STSV]*9/5+320;                                         
									}
								 }
							     break;
						case 6:					
								 if(SYS.DataSet[C_F]==0){
									if(temp >=400 && temp <= 4500){
										SYS.stSV_Dis = temp;        //SV_stby
                                        SYS.DataSet[STSV]  = SYS.stSV_Dis;
                                        SYS.WriteState = STSV;                                        
									}
								 }else{
									if(temp >=1040 && temp <= 8420){
                                        SYS.stSV_Dis = temp;        //SV_stby
										SYS.DataSet[STSV] = (SYS.stSV_Dis-320)*5/9; 
                                        SYS.WriteState=STSV;
									}
								 }							             
							     break;		 
						case 7:       //run=0_std=1_m=2                	                                
                                 if(temp == 0){
                                     SYS.RunningMode = 0;
                                     SYS.DataSet[Tem_SV] = SYS.DataSet[SV_WK];
                                 }
                                 else if(temp == 1){
                                     SYS.RunningMode = 1;
                                     SYS.DataSet[Tem_SV] = SYS.DataSet[STSV];
                                 }
                                 else if(temp == 3){
                                     SYS.RunningMode = 3;
                                 }
                                 else if(Rxbuf1[4] ==2 && Rxbuf1[5] < 100){
                                     SYS.RunningMode = 2;
                                     SYS.ManualRate = Rxbuf1[5]*10;
                                 }      								 
							     break;
                        case 8:  	
								 if(temp == 0 || temp == 1){
							        SYS.DataSet[HSCI] = temp;	//SYS.DataSet[HSCI]
                                    SYS.WriteState=HSCI;
								 }									 
							     break;           
                        case 9:			 		
                                 if(temp >0 && temp <= 160){
                                     SYS.DataSet[HC_H] = temp;  //SYS.DataSet[HC_H]
                                     SYS.WriteState=HC_H;
                                 }                                     
                                 break;                       
                        case 10:			 		
                                 if(temp > 40 && temp <= 500){
                                     SYS.DataSet[AL_H] = temp;  //SYS.DataSet[AL_H]
                                     SYS.WriteState=AL_H; 
                                 }                                     
                                 break;    
                        case 11:			 		
                                 if(temp > 40 && temp <= 500){
                                     SYS.DataSet[AL_L] = temp;  //SYS.DataSet[AL_L]
                                     SYS.WriteState=AL_L;
                                 }                                     
                                 break;     
                        case 12:  	
								 if(temp == 1){
							         replan = 0;    //clear
                                     check = 1;  //ignore
								 }									 
							     break;
                        case 13:  	
								 if(temp == 0 || temp == 1){
							         SYS.DataSet[In] = temp;    //j_k
                                     SYS.WriteState=In;
								 }									 
							     break;                         
                                 
						default:
							break;
					}
                    sendto();						
			}
		}
        else if(Rxbuf1[2]==0xca) //校准
		{
			crc = CreatCRC16Value(&Rxbuf1[1], 5);
			if((Rxbuf1[6] == crc.Bit[1])&&(Rxbuf1[7] == crc.Bit[0]))
            {
                temp = (Rxbuf1[4]<<8) +  Rxbuf1[5];
                switch (Rxbuf1[3])
                {
                    case 1: 
                        SYS.DataSet[ET_Z] = temp;
                        SYS.WriteState=ET_Z;
                        break;
                    case 2: 
                        SYS.DataSet[HT_Z] = temp;
                        SYS.WriteState=HT_Z;
                        break;
                    case 3: 
                        SYS.DataSet[HT_A] = temp;
                        SYS.WriteState=HT_A;
                        break;
                    case 4:
                        readvalue();
                        break;
                    
                    
                }
            }
        }
  } 
  
  for(i=0;i<10;i++) Rxbuf1[i] = 0;
	
}
//---------------------------------------------------------------------------
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{		
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();	
}
