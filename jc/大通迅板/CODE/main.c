


#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"	
#include <String.h>
#include <stdlib.h>

#define  NUM 60
//-----------------------------------------------------------------------
__IO uint16_t delay,send,dealySet,isRx,Csytim;
__IO uint8_t isRxmain,isRxhmi,isCsy;

Alarm_t alarmTab[129];
data_t  DataTab[129];   




//-----------------------------------------------------------------------	

int main(void)
{
   NVIC_Configuration();	
	initSystick(); 
	uart_init();
    initGPIO();
	IWDG_Config(IWDG_Prescaler_64 ,100); //Tout = prv/40 * rlv (s)
    while(1)
	{   
		  if(isRxmain ==1) {isRxmain =0; SerialReceive_Callback(); continue;}
		//  if(isRxhmi ==1) {isRxhmi =0; Serial1_Receive(); continue;}	
		  if(dealySet >= 20000) 
			  syncExec();	//20s以后
		  if(send==0)
			 sendquse(); 
        if(delay==0)
			 onAlarm();		
        IWDG_ReloadCounter();		  
	}
	
}

//-----------------------------------------------------------------------	
static uint8_t comChang;
void sendCMD(uint8_t address)
{
	 uint8_t Txbuf[5];int i;
	CRC16_t crc;
	if(comChang==0) GPIO_SetBits(GPIOA, GPIO_Pin_10);
	else GPIO_SetBits(GPIOA, GPIO_Pin_11);
    for(i=0;i<6;i++){
		Txbuf[i]=0;
	}
	Txbuf[0] = address;
	Txbuf[1] = 0xaa;	 //查询
	crc = CreatCRC16(Txbuf, 2);
	Txbuf[2] = crc.Bit[1];
	Txbuf[3] = crc.Bit[0];
	if(comChang==0){
		for(i=0;i<4;i++){
			USART_SendData(USART2, Txbuf[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
		 }	
         while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);			
	 }
	else{
		for(i=0;i<4;i++){
			USART_SendData(USART1, Txbuf[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
		 }	
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	 }
    	
    if(comChang==0) GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_11);	 	
}
//-----------------------------------------------------------------------	
void sendData(uint8_t address,uint16_t data)
{
	uint8_t Txbuf[7];int i;
	CRC16_t crc;
	if(comChang==0) GPIO_SetBits(GPIOA, GPIO_Pin_10);
	else GPIO_SetBits(GPIOA, GPIO_Pin_11);
	
    for(i=0;i<6;i++){
		Txbuf[i]=0;
	}
	Txbuf[0] = address;
	Txbuf[1] = 0x5c;  //设定值
	Txbuf[2] = data>>8;	
	Txbuf[3] = data;	
	crc = CreatCRC16(Txbuf, 4);
	Txbuf[4] = crc.Bit[1];
	Txbuf[5] = crc.Bit[0];
	if(comChang==0){	
		for(i=0;i<6;i++){
			USART_SendData(USART2, Txbuf[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
		 }
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);			
	 }
	else{
		for(i=0;i<6;i++){
			USART_SendData(USART1, Txbuf[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
		 }
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);			
	 }
	
	if(comChang==0) GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_11);	 
}
//-----------------------------------------------------------------------	
void writeData(uint8_t address,uint8_t reg,uint16_t data)
{
	uint8_t Txbuf[8],i;
	CRC16_t crc;
	if(comChang==0) GPIO_SetBits(GPIOA, GPIO_Pin_10);
	else GPIO_SetBits(GPIOA, GPIO_Pin_11);
	Txbuf[0] = address;
	Txbuf[1] = 0xab;  //命令
	Txbuf[2] = reg;
	Txbuf[3] = data>>8;	
	Txbuf[4] = data;	
	crc = CreatCRC16(Txbuf, 5);
	Txbuf[5] = crc.Bit[1];
	Txbuf[6] = crc.Bit[0];
	if(comChang==0){	
		for(i=0;i<7;i++){
			USART_SendData(USART2, Txbuf[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
		 }
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);			
	 }
	else{
		for(i=0;i<7;i++){
			USART_SendData(USART1, Txbuf[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
		 }
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);			
	 }
	
	if(comChang==0) GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_11);	 
}
//----------------------------------------------------------------------		
//void sendHTMI(uint8_t address,uint8_t index)
//{
//	static uint8_t Txbuf[17],i;
//	CRC16_t crc; 
//	Txbuf[0] = address;
//	Txbuf[1] = 0x5a;
//	Txbuf[2] = DataTab[index].code;	  //code
//	Txbuf[3] = DataTab[index].PV >>8;  //PV
//	Txbuf[4] = DataTab[index].PV;
//	Txbuf[5] = DataTab[index].SV >>8;  //SV
//	Txbuf[6] = DataTab[index].SV;
//	Txbuf[7] = DataTab[index].on_off;    //ON_OFF
//	Txbuf[8] = DataTab[index].cf;        //C_F
//	Txbuf[9] = DataTab[index].Iout>>8;   //Iout
//	Txbuf[10] = DataTab[index].Iout;
//	Txbuf[11] = DataTab[index].Oout>>8;   //Oout  
//	Txbuf[12] = DataTab[index].Oout;
//	Txbuf[13] = DataTab[index].Pout>>8;   //Pout  
//	Txbuf[14] = DataTab[index].Pout;	
//	crc = CreatCRC16(Txbuf, 15);
//	Txbuf[15] = crc.Bit[1];
//	Txbuf[16] = crc.Bit[0];
//   for(i=0;i<17;i++){
//	   USART_SendData(USART1, Txbuf[i]);
//	   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
//    }	
//	
//}
//-----------------------------------------------------------------------
//Txbuf[0] = address;
//	Txbuf[1] = 0x5a;
//	Txbuf[2] = SYS.ErrorCode;	         //code
//	Txbuf[3] = SYS.DataGet[Tem_PV]>>8;  //PV
//	Txbuf[4] = SYS.DataGet[Tem_PV];
//	Txbuf[5] = SYS.SV_Dis>>8;           //SV
//	Txbuf[6] = SYS.SV_Dis;
//	Txbuf[7] = disp;                    //ON_OFF
//	Txbuf[8] = SYS.DataSet[C_F];        //C_F
//	Txbuf[9] = SYS.DataGet[Current]>>8; //Iout
//	Txbuf[10] = SYS.DataGet[Current];
//	Txbuf[11] = SYS.DataGet[Rate]>>8;   //Oout  
//	Txbuf[12] = SYS.DataGet[Rate];
//	Txbuf[13] = power>>8;                //Pout  
//	Txbuf[14] = power;	
//	crc = CreatCRC16Value(Txbuf, 15);
//	Txbuf[15] = crc.Bit[1];
//	Txbuf[16] = crc.Bit[0];
//static uint8_t isRx;
//---------------------------------------------------------------------------
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{		
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();	
}
void SerialReceive_Callback(void)
{
	CRC16_t crc;
	int i;	
	uint8_t* rxbuf = Rxbuf2;
	comP = 0;    	
	
	if(rxbuf[1]==0x5a) //adr,5a,state,pv,sv,crc
	{
		crc = CreatCRC16(rxbuf, 15);
		if((rxbuf[15] == crc.Bit[1])&&(rxbuf[16] == crc.Bit[0]))
		{								
			alarmTab[rxbuf[0]].code = rxbuf[2];				
			alarmTab[rxbuf[0]].timeOut = 2000; //2s
			alarmTab[rxbuf[0]].msg =0;				
			alarmTab[rxbuf[0]].PV = (rxbuf[3]<<8) + rxbuf[4];
			alarmTab[rxbuf[0]].SV = (rxbuf[5]<<8) + rxbuf[6];
			DataTab[rxbuf[0]].code = alarmTab[rxbuf[0]].code; 
			DataTab[rxbuf[0]].PV = alarmTab[rxbuf[0]].PV;
			DataTab[rxbuf[0]].SV = alarmTab[rxbuf[0]].SV;
			DataTab[rxbuf[0]].on_off = rxbuf[7];
			DataTab[rxbuf[0]].cf = rxbuf[8];
			DataTab[rxbuf[0]].Iout = (rxbuf[9]<<8) + rxbuf[10];
			DataTab[rxbuf[0]].Oout = (rxbuf[11]<<8) + rxbuf[12];
			DataTab[rxbuf[0]].Pout = (rxbuf[13]<<8) + rxbuf[14];
			DataTab[rxbuf[0]].updata = 1;
		   isRx = 4000; //4s
		}
	}
	else if(rxbuf[1]==0x5b) //adr,5b,
	{
		crc = CreatCRC16(rxbuf, 3);
		if((rxbuf[3] == crc.Bit[1])&&(rxbuf[4] == crc.Bit[0]))
		{								
				 alarmTab[rxbuf[0]].code = 0;				
			alarmTab[rxbuf[0]].timeOut = 0; //3s
			alarmTab[rxbuf[0]].msg = 0;
			alarmTab[rxbuf[0]].PV = DataTab[rxbuf[0]].SV;
		}
	}
   for(i=0;i<20;i++) rxbuf[i] = 0;
	
}
//void Serial1_Receive(void)
//{
//	CRC16_t crc;
//	int i;	
//	uint8_t* rxbuf = Rxbuf1;
//	P1 = 0;  
	
//	if(rxbuf[1]==0xab) 
//	{
//		crc = CreatCRC16(rxbuf, 5);
//		if((rxbuf[5] == crc.Bit[1])&&(rxbuf[6] == crc.Bit[0]))
//		{								
//			alarmTab[rxbuf[0]].writeId =rxbuf[2];	 
//			alarmTab[rxbuf[0]].writeData = (rxbuf[3]<<8) + rxbuf[4];
//			alarmTab[rxbuf[0]].isWrie =1;
//		}
//	}
//	
//	for(i=0;i<10;i++) rxbuf[i] = 0;
//}
//----------------------------------------------------------------------
void initGPIO(void)
{
	GPIO_InitTypeDef portSet;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	portSet.GPIO_Speed=GPIO_Speed_2MHz; 
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	
	portSet.GPIO_Pin=GPIO_Pin_11;//红灯
	portSet.GPIO_Pin|=GPIO_Pin_2;//温度到达
	GPIO_Init(GPIOB,&portSet);	
	
	portSet.GPIO_Pin=GPIO_Pin_12;//绿灯
	portSet.GPIO_Pin|=GPIO_Pin_10;//485-2
	portSet.GPIO_Pin|=GPIO_Pin_11;//485-1
	GPIO_Init(GPIOA,&portSet);
	
	portSet.GPIO_Mode=GPIO_Mode_IPU;
	portSet.GPIO_Pin=GPIO_Pin_5;//一键同步	
	GPIO_Init(GPIOB,&portSet);
	
	portSet.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	portSet.GPIO_Pin =GPIO_Pin_10;//开关机
	GPIO_Init(GPIOB,&portSet);
		
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
}
//----------------------------------------------------------------------
void onAlarm(void)
{
	static uint8_t bit,rxbit;	
	uint8_t Error,tem_fish;
	
	Error = 0;	
    for(int i=1;i<=NUM;i++){
		if(alarmTab[i].code != 0) Error = 1;
		if(alarmTab[i].msg == 1)  Error = 1;
	}
    if(Error){
		 if(bit) GPIO_SetBits(GPIOB, GPIO_Pin_11); else GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		 bit = ~bit;	
	}
	else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);	 //red
		if(isRx==0) {
			if(rxbit) GPIO_SetBits(GPIOA, GPIO_Pin_12); else GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			rxbit = ~rxbit;	
			
		//	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
		}			
		else GPIO_SetBits(GPIOA,GPIO_Pin_12); //green
	}
	
	tem_fish = 1;  //温度到达
	for(int i=1;i<=NUM;i++){ 
		if(abs(alarmTab[i].SV-alarmTab[i].PV)>100){
			tem_fish = 0;
		}
	}
	if(tem_fish) GPIO_SetBits(GPIOB, GPIO_Pin_2); else GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		
	
	delay = 4000;//300ms
}

//-----------------------------------------------------------------------
uint8_t sendCmd(uint8_t adr,uint16_t data)
{	
	alarmTab[adr].isSV =1;
	alarmTab[adr].SVdata = data;
	if(alarmTab[adr].isfish ==1){
		alarmTab[adr].isSV = 0;
		alarmTab[adr].isfish = 0;
		return 1;
	}
	else
		return 0;	
}
//-----------------------------------------------------------------------
void getTempSV(uint16_t i)
{ 

   if(alarmTab[i].PV+500 < alarmTab[i].SV){
		alarmTab[i].tempSV = alarmTab[i].PV+500;
	}	
	else if(alarmTab[i].PV+400 < alarmTab[i].SV){
		alarmTab[i].tempSV = alarmTab[i].PV+400;
	}
   else if(alarmTab[i].PV+300 < alarmTab[i].SV){
		alarmTab[i].tempSV = alarmTab[i].PV+300;
	}	
   else if(alarmTab[i].PV+200 < alarmTab[i].SV){
		alarmTab[i].tempSV = alarmTab[i].PV+200;
	}
   else if(alarmTab[i].PV < alarmTab[i].SV){		
		alarmTab[i].tempSV = alarmTab[i].SV;			
	}		
}
//-----------------------------------------------------------------------
void syncExec(void)
{
	uint16_t i ,fish,gpio;
	static uint16_t minSV,EN=1;
	static uint8_t step,sendStep;
	
   gpio = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);	
	if((!EN)&&(gpio)) {if(step != 0){step =3; sendStep = 1;}}
	else if((EN)&&(!gpio)) {step =0; sendStep = 1;isCsy =0;Csytim =0;}
	EN = gpio;
	if(step == 0)
	{
		if((!EN)&&(!gpio)){
			for(i=1;i<=NUM;i++)
			{
				alarmTab[i].tempSV = 0;
				if((alarmTab[i].code ==0)&&(alarmTab[i].msg == 0)){
					getTempSV(i);				
				}											
			}
			minSV = 0xffff;
			for(i=1;i<=NUM;i++)
			{
				if((alarmTab[i].tempSV > 0)&&(alarmTab[i].tempSV < minSV))
					minSV = alarmTab[i].tempSV;
			}
			if(minSV < 0xffff) {step = 1;sendStep = 1;}
		}
	}
	else if(step == 1)
	{
		if(alarmTab[sendStep].tempSV >0){
		   if(sendCmd(sendStep,minSV)){
			//	if(++alarmTab[sendStep].reSend==2)
			//	{
				//   alarmTab[sendStep].reSend =0; 
				   sendStep++;
			//	}
			}
		}
		else 
			sendStep++;		
		if(sendStep > NUM){
			sendStep = 1;
			step = 2;
	   }
	}
	else if(step == 2)
	{
		fish = 1;
		for(i=1;i<=NUM;i++)
		{	
         if((alarmTab[i].code ==0)&&(alarmTab[i].msg == 0)){			
				if((alarmTab[i].tempSV >0)&&(alarmTab[i].PV < minSV))
					fish = 0;	
			}				
		}
		
		if(fish) step = 0;
		else {
			isCsy =1;
			if(Csytim >= 4000) {isCsy =0;Csytim =0;step = 1;}
		}
	}
	else if(step == 3)
	{
		if(alarmTab[i].PV < alarmTab[i].SV){
			if(sendCmd(sendStep,alarmTab[sendStep].SV)){
				if(++alarmTab[sendStep].reSend==2)
				{
					alarmTab[sendStep].reSend =0; 
					sendStep++;
				}
			}	
		}
		else
			sendStep++;		
		if(sendStep > NUM){
			sendStep = 1;
			step = 0;
	   }
	}
}

//----------------------------------------------------------------------
void alarmExec(void)
{  
	static uint8_t num;	
	if(++num >10)
	{
		num = 0;
		for(int i=1;i<=NUM;i++){ //1-109
			if(alarmTab[i].timeOut >0){
				alarmTab[i].timeOut--;
				if(alarmTab[i].timeOut ==0) alarmTab[i].msg =1;
			}			
		}
		
		 if(dealySet < 20001) //20s
			 dealySet++;
		 if(isCsy==1){if(Csytim < 5000) Csytim++;} //4s   		 
		
		if(isRx >0) isRx--;
	}			
}



void readSignal()
{
	static int upnum,downnum;
	static uint8_t signal,swlast;

	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_SET)
	{
		downnum =0;
		if(signal==0){
			if(++upnum == 8) signal = 1;
		}
	}
	else 
	{
		upnum = 0;
		if(signal==1){
			if(++downnum == 8) signal = 0;
		}
	}
	
    if((!swlast)&&(signal)){ //on
		for(int i=1;i<=NUM;i++){ 
			alarmTab[i].writeId = 4;
			alarmTab[i].writeData = 1;
			alarmTab[i].isWrie = 1;
		}		
	}
	else if((swlast)&&(!signal)){ //off
	    for(int i=1;i<=NUM;i++){ 
			alarmTab[i].writeId = 4;
			alarmTab[i].writeData = 0;
			alarmTab[i].isWrie = 1;
		}
	}
	swlast = signal;
	

}
//--------------------------------------------------------------------------
void sendquse(void)  
{
	static uint16_t i;
	
	if(++i > NUM) i =1; //127

	if(i <= NUM/2) comChang =0;
	else
		comChang =1;
   
    readSignal();
	if(alarmTab[i].isSV ==1){   //setSV
		sendData(i,alarmTab[i].SVdata);
		alarmTab[i].isfish =1;
    } 
    else if(alarmTab[i].isWrie == 1){
		writeData(i,alarmTab[i].writeId,alarmTab[i].writeData);
		alarmTab[i].isWrie = 0;
		alarmTab[i].writeId = 0;
		alarmTab[i].writeData = 0;
	}
	else{
		sendCMD(i);   //cmd
	}	
	
	
	send = 22; //2.2ms
}
/*
adr cmd dat crc 
*/













