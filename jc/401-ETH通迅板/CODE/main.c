
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"	
#include "24cxx.h" 
#include "json.h"
#include <String.h>
#include <stdlib.h>

#include "cJSON.h"


//-----------------------------------------------------------------------
__IO uint16_t delay,send,dealySet,isRx,Csytim;
__IO uint8_t isCsy,isMs;

Alarm_t alarmTab[100];

Rx_t Rx[2];
Rx3_t Rx3;


int main(void)
{       
    NVIC_Configuration();   
	initSystick(); 
	uart_init(); 
    initGPIO();     
    State.Numb = 12;   
    Sets.stby_m = 1;
    Sets.stby_s = 14;  
    Sets.stbySW = 1;
    Sets.syncSW = 1;     
    
	IWDG_Config(IWDG_Prescaler_256 ,700); //Tout = prv/40 * rlv (s)	
    while(1)
	{ 
         if(isMs==1){
             isMs = 0;
            alarmExec(); 
         }             
		 if(Rx[0].Rxfig ==1) {			 
		     Rx[0].Rxfig =0; 
			 SerialReceive_Callback(&Rx[0]); continue;
		 }
         if(Rx[1].Rxfig ==1) {
		     Rx[1].Rxfig =0; 
		     SerialReceive_Callback(&Rx[1]); continue;
		 }
		 if(Rx3.Rxfig ==1) {
		     Rx3.Rxfig =0; 
		     Serial_3_Receive(); continue;
		 } 
		 if(dealySet >= 20000){ 
			  if((State.isStby == 0)&&(State.stDelay == 0)) 
				  syncExec();	//20s以后
			  else dealySet = 200;
		 }
		 if(send==0)
			 sendquse();
         if(delay==0)
			 onAlarm();		
         
         IWDG_ReloadCounter();		  
       		
	}
	
}

//-----------------------------------------------------------------------	
void sendCMD(uint8_t address)
{
	 uint8_t Txbuf[5];int i;
	CRC16_t crc;
	if(address <= NUM/2) GPIO_SetBits(GPIOA, GPIO_Pin_10);
	else GPIO_SetBits(GPIOA, GPIO_Pin_11);
    for(i=0;i<6;i++){
		Txbuf[i]=0;
	}
	Txbuf[0] = address;
	Txbuf[1] = 0xaa;	 //查询
	crc = CreatCRC16(Txbuf, 2);
	Txbuf[2] = crc.Bit[1];
	Txbuf[3] = crc.Bit[0];
	if(address <= NUM/2){
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
    	
    if(address <= NUM/2) GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_11);	 	
}
//-----------------------------------------------------------------------	
void sendData(uint8_t address,uint16_t data)
{
	uint8_t Txbuf[7];int i;
	CRC16_t crc;
	if(address <= NUM/2) GPIO_SetBits(GPIOA, GPIO_Pin_10);
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
	if(address <= NUM/2){	
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
	
	if(address <= NUM/2) GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_11);	 
}
//-----------------------------------------------------------------------	
void writeData(uint8_t address,uint8_t reg,uint16_t data)
{
	uint8_t Txbuf[8],i;
	CRC16_t crc;
	if(address <= NUM/2) GPIO_SetBits(GPIOA, GPIO_Pin_10);
	else GPIO_SetBits(GPIOA, GPIO_Pin_11);
	Txbuf[0] = address;
	Txbuf[1] = 0xab;  //命令
	Txbuf[2] = reg;
	Txbuf[3] = data>>8;	
	Txbuf[4] = data;	
	crc = CreatCRC16(Txbuf, 5);
	Txbuf[5] = crc.Bit[1];
	Txbuf[6] = crc.Bit[0];
	if(address <= NUM/2){	
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
	
	if(address <= NUM/2) GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_11);	 
}

//---------------------------------------------------------------------------
void IWDG_Config(uint8_t prv ,uint16_t rlv)
{		
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( prv );
	IWDG_SetReload( rlv );
	IWDG_ReloadCounter();
	IWDG_Enable();	
}
void SendRequest_U3(char* buf)
{
	int i;
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	i = 0;	
	while(buf[i]!='\0'){
		USART_SendData(USART3, buf[i++]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);      
	}
    USART_SendData(USART3, '\r');
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){};
        
    USART_SendData(USART3, '\n');
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);	
}
void SerialReceive_Callback(Rx_t* rx)
{
	CRC16_t crc;
	int i,adr=0;	
	uint8_t* rxbuf = rx->Rxbuf;
	rx->Rxp = 0;    	
	
	if(rxbuf[1]==0x5a) 
	{
		crc = CreatCRC16(rxbuf, 15);
		if((rxbuf[15] == crc.Bit[1])&&(rxbuf[16] == crc.Bit[0]))
		{	
            adr = rxbuf[0] - 1;							
			status[adr].code = alarmTab[adr].code = rxbuf[2];            
			alarmTab[adr].timeOut = 2000; //2s
			alarmTab[adr].msg =0;				
			status[adr].PV = alarmTab[adr].PV = (rxbuf[3]<<8) + rxbuf[4];
			status[adr].SV = alarmTab[adr].SV = (rxbuf[5]<<8) + rxbuf[6];
            status[adr].jk = rxbuf[7];
            Sets.cf = rxbuf[8];
            status[adr].I = (rxbuf[9]<<8) + rxbuf[10];
            status[adr].out = (rxbuf[11]<<8) + rxbuf[12];
            status[adr].power = (rxbuf[13]<<8) + rxbuf[14];

		    isRx = 4000; //4s
		}
	}
	else if(rxbuf[1]==0x5b) 
	{
		crc = CreatCRC16(rxbuf, 15);
		if((rxbuf[15] == crc.Bit[1])&&(rxbuf[16] == crc.Bit[0]))
		{	
            adr = rxbuf[0] - 1;	            
			Sets.pwm  = rxbuf[2];
            Sets.stSV = (rxbuf[3]<<8) + rxbuf[4];   
            status[adr].mode = rxbuf[5];
            status[adr].enable = rxbuf[6];
            status[adr].I_h = (rxbuf[7]<<8) + rxbuf[8]; 
            status[adr].al_h = (rxbuf[9]<<8) + rxbuf[10]; 
            status[adr].al_l = (rxbuf[11]<<8) + rxbuf[12];
            status[adr].version = rxbuf[13]; 
            status[adr].check = rxbuf[14];            
		}
	}		
	
   for(i=0;i<20;i++) rxbuf[i] = 0;
	
}
void Serial_3_Receive()
{
	Rx3.Rxp = 0;    
	recv(Rx3.Rxbuf);	
	for(int i=0;i<RXLEN;i++) Rx3.Rxbuf[i] = 0;
}
//----------------------------------------------------------------------
void initGPIO(void)
{
	GPIO_InitTypeDef portSet;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	portSet.GPIO_Speed=GPIO_Speed_10MHz; 
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	
	portSet.GPIO_Pin=GPIO_Pin_14;   //红灯
	portSet.GPIO_Pin|=GPIO_Pin_2;   //温度到达
	portSet.GPIO_Pin|=GPIO_Pin_15;  //运行/故障
	GPIO_Init(GPIOB,&portSet);	
	
	portSet.GPIO_Pin=GPIO_Pin_12;   //绿灯
	portSet.GPIO_Pin|=GPIO_Pin_9;   //485-3
	portSet.GPIO_Pin|=GPIO_Pin_10;  //485-2
	portSet.GPIO_Pin|=GPIO_Pin_11;  //485-1
	GPIO_Init(GPIOA,&portSet);
	
	portSet.GPIO_Pin=GPIO_Pin_3;  //wp
    portSet.GPIO_Pin|=GPIO_Pin_6;   //yello
	GPIO_Init(GPIOC,&portSet);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	
	portSet.GPIO_Mode=GPIO_Mode_IPU;
	portSet.GPIO_Pin=GPIO_Pin_8;    //SET
	portSet.GPIO_Pin|=GPIO_Pin_9;   //INC
	portSet.GPIO_Pin|=GPIO_Pin_10;  //MODE/return
	portSet.GPIO_Pin|=GPIO_Pin_11;  //DEC	
	GPIO_Init(GPIOC,&portSet);
	
	portSet.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	portSet.GPIO_Pin=GPIO_Pin_12;   //自动待机
	portSet.GPIO_Pin|=GPIO_Pin_13;  //开关机
	GPIO_Init(GPIOB,&portSet);
		
	GPIO_ResetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
}
//----------------------------------------------------------------------
void onAlarm(void)
{
	static uint8_t bit,rxbit;	
	uint8_t Error,tem_fish,run;
	
	Error = 0;	
    for(int i=0;i<NUM;i++){
		if(alarmTab[i].code != 0) Error = 1;
		if(alarmTab[i].msg == 1)  Error = 1;
	}
 
    if(Error){
		 if(bit) GPIO_SetBits(GPIOB, GPIO_Pin_14); else GPIO_ResetBits(GPIOB, GPIO_Pin_14); //red
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12); //green
         GPIO_ResetBits(GPIOC,GPIO_Pin_6);  //yello
		 GPIO_SetBits(GPIOB, GPIO_Pin_15); //err-out2
		 bit = ~bit;
         State.sigAlarm = 1;		
	}
	else {                      
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	 //red
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);  //err-out2
		State.sigAlarm = 0;	
		if(isRx==0) {
			if(rxbit) GPIO_SetBits(GPIOA, GPIO_Pin_12); else GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			rxbit = ~rxbit;	
            GPIO_ResetBits(GPIOC,GPIO_Pin_6);  //yello
		}			
		else {
            run = 0;
            for(int i=0;i<NUM;i++){
                if(status[i].run && status[i].enable) run = 1;
            }
            if(run == 0) {
                GPIO_SetBits(GPIOC,GPIO_Pin_6);  //yello
                GPIO_ResetBits(GPIOA,GPIO_Pin_12); //green
            }
            else {
                GPIO_SetBits(GPIOA,GPIO_Pin_12); //green
                GPIO_ResetBits(GPIOC,GPIO_Pin_6);  //yello
            }
		}
	}
		
	tem_fish = 1;  //温度到达
	for(int i=0;i<NUM;i++){ 
        if(status[i].run && status[i].enable){
            if(abs(alarmTab[i].SV-alarmTab[i].PV)>100){
                tem_fish = 0;
            }
        }
	}
	if(isRx){
		if(tem_fish) {
			GPIO_SetBits(GPIOB, GPIO_Pin_2); 
            State.temp_ok = 1;
		}else {
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			State.temp_ok = 0;
		}
	}
			
	
	delay = 5000;//300ms
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
	uint16_t i=0 ,fish;
	static uint16_t minSV,EN=1;
	static uint8_t step,sendStep;

	if((EN)&&(!Sets.syncSW)) {if(step != 0){step =3; sendStep = 0;}}
	else if((!EN)&&(Sets.syncSW)) {step =0; sendStep = 0;isCsy =0;Csytim =0;}
	EN = Sets.syncSW;
	if(step == 0)
	{
		if((EN)&&(Sets.syncSW)){
			for(i=0;i<NUM;i++)
			{
				alarmTab[i].tempSV = 0;
                if(status[i].run && status[i].enable){
                    if((alarmTab[i].code ==0)&&(alarmTab[i].msg == 0)){
                        getTempSV(i);				
                    }	
                }
														
			}
			minSV = 0xffff;
			for(i=0;i<NUM;i++)
			{
                if(status[i].run && status[i].enable){
                    if((alarmTab[i].tempSV > 0)&&(alarmTab[i].tempSV < minSV))
					   minSV = alarmTab[i].tempSV;
                }				
			}
			if(minSV < 0xffff) {step = 1;sendStep = 0;}
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
		if(sendStep >= NUM){
			sendStep = 0;
			step = 2;
	    }
	}
	else if(step == 2)
	{
		fish = 1;
		for(i=0;i<NUM;i++)
		{
            if(status[i].run && status[i].enable){
                if((alarmTab[i].code ==0)&&(alarmTab[i].msg == 0)){			
                    if((alarmTab[i].tempSV >0)&&(alarmTab[i].PV < minSV))
                        fish = 0;	
                }	
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
        if(status[sendStep].run && status[sendStep].enable){
            if(alarmTab[sendStep].PV < alarmTab[sendStep].SV){
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
        }
        else
            sendStep++;	
    	
		if(sendStep >= NUM){
			sendStep = 0;
			step = 0;
	   }
	}
}
//-----------------------------------------------------------------------
void PoewrSignal()
{
	static int upnum,downnum;
	static uint8_t signal,swlast;

	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_SET)
	{
		downnum =0;
		if(signal==0){
			if(++upnum == 16) signal = 1;
		}
	}
	else 
	{
		upnum = 0;
		if(signal==1){
			if(++downnum == 16) signal = 0;
		}
	}
	State.sigpower = signal;
    if((!swlast)&&(signal)){ //on
		for(int i=0;i<NUM;i++){ 
            if(status[i].run && status[i].enable){            
                alarmTab[i].writeId = 7;
                alarmTab[i].writeData = 0; //work
                alarmTab[i].isWrie = 1;
                alarmTab[i].wNum = 5;
            }
		}		
	}
	else if((swlast)&&(!signal)){ //off
	    for(int i=0;i<NUM;i++){ 
			if(status[i].run && status[i].enable){            
                alarmTab[i].writeId = 7;
                alarmTab[i].writeData = 3;  //0ff
                alarmTab[i].isWrie = 1;
                alarmTab[i].wNum = 5;
            }
		}
	}
	swlast = signal;	
}
//-----------------------------------------------------------------------
void StbySignal() //ms
{
	static int upnum,downnum;
	static uint8_t signal,swlast;
	static uint16_t numS;

	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET)
	{
		downnum =0;
		if(signal==0){
			if(++upnum == 16) signal = 1;
		}
	}
	else 
	{
		upnum = 0;
		if(signal==1){
			if(++downnum == 16) signal = 0;
		}
	}
	State.sigStby = signal;
    if((!swlast)&&(signal)){ //work
		State.stDelay = 0;
		if(State.isStby){
			State.isStby = 0;
			for(int i=0;i<NUM;i++){ 
                 if(status[i].run && status[i].enable){
                    alarmTab[i].writeId = 7;  //mode
                    alarmTab[i].writeData = 0; //work
                    alarmTab[i].isWrie = 1; 
                    alarmTab[i].wNum = 3;	
                 }                     
			}		
		}
	}
	else if((swlast)&&(!signal)){ //stby
		if(State.temp_ok){
		    State.stDelay =  Sets.stby_m*60 + Sets.stby_s;   
		}			
	}
	swlast = signal;	 
	
	if(++numS > 1000){ //1s  
		numS = 0;
		if(State.stDelay > 0){
			State.stDelay--;
			if(State.stDelay == 0){
                State.isStby = 1;
				for(int i=0;i<NUM;i++){
					alarmTab[i].isSV = 0; //关闭同步
				}
				for(int i=0;i<NUM;i++){ 
                    if(status[i].run && status[i].enable){					
                        alarmTab[i].writeId = 7;  //mode
                        alarmTab[i].writeData = 1; //stby
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;
                    }                        
				}	
			}
		}
	}
}


//-----------------------------------------------------------------------
void alarmExec(void)
{  
	static uint8_t num;	
	if(++num >10)
	{
		num = 0; 
		for(int i=0;i<NUM;i++){ //1-109
			if(alarmTab[i].timeOut >0){
				alarmTab[i].timeOut--;
				if(alarmTab[i].timeOut ==0) {
					alarmTab[i].msg =1;
					alarmTab[i].code = OFLI;
				}
			}			
		}	 
	}
    
    if(num == 5)
    {
        PoewrSignal();
		

		if(Sets.stbySW) {
			StbySignal();
		}
		else{
			State.stDelay = 0;
			State.isStby = 0;
		}
				
		
		 if(dealySet < 20005) //20s
			 dealySet++;
		 if(isCsy==1){if(Csytim < 5000) Csytim++;} //4s   		 
		
		if(isRx >0) isRx--;
    }	
}

//--------------------------------------------------------------------------
void sendquse(void)  
{
	static uint16_t max = NUM/2, i = 0, j = NUM/2;
	if(++i >= max) i = 0; 
	if(++j > NUM) j = max; 	 

//---------------------------------------	
	
    if(alarmTab[i].isWrie == 1){  //on/off
		writeData(i+1,alarmTab[i].writeId,alarmTab[i].writeData);		
		alarmTab[i].writeId = 0;
		alarmTab[i].writeData = 0;
		if(alarmTab[i].wNum > 0) alarmTab[i].wNum--;			
        else alarmTab[i].isWrie = 0;
	}
	else if(alarmTab[i].isSV ==1){   //setSV
		sendData(i+1,alarmTab[i].SVdata);
		alarmTab[i].isfish =1;
    } 
	else{
		sendCMD(i+1);   //cmd
	}	
//----------------------------------------	
	if(State.Numb >= max){
		if(alarmTab[j].isWrie == 1){  //on/off
			writeData(j+1,alarmTab[j].writeId,alarmTab[j].writeData);
			alarmTab[j].writeId = 0;
			alarmTab[j].writeData = 0;
			if(alarmTab[j].wNum > 0) alarmTab[j].wNum--;			
			else alarmTab[j].isWrie = 0;
		}
		else if(alarmTab[j].isSV ==1){   //setSV
			sendData(j+1,alarmTab[j].SVdata);
			alarmTab[j].isfish =1;
		} 
		else{
			sendCMD(j+1);   //cmd
		}	
	}

    
	send = 25; //2.2ms
}
















