
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"	
#include "24cxx.h" 
#include <String.h>
#include <stdlib.h>


#include "GUI.h"

#define  NUM 50
#define  LANG  1    //1-cn 0-en
//-----------------------------------------------------------------------
__IO uint16_t delay,send,dealySet,isRx,Csytim;
__IO uint8_t isCsy;

Alarm_t alarmTab[129];
data_t  DataTab[129];   
Rx_t Rx[3];
Stby_t Stby;
uint8_t mode,seg;

const char* errMeg_cn[] = {
	[NoError] = "                      ",
	[ALARM_H] = "#    æ¸©åº¦è¶…å‡ºä¸Šé™!",
	[ALARM_L] = "#    æ¸©åº¦è¶…å‡ºä¸‹é™!",
	[TC_OP]   = "#    çƒ­ç”µè€¦æ–­è·¯!",
	[F1_OP]   = "#    ä¿é™©ä¸1æ–­è·¯!",
	[F2_OP]   = "#    ä¿é™©ä¸2æ–­è·¯!",
	[HT_OP]   = "#    å‘çƒ­åœˆå¼€è·¯!",
	[TC_ST]   = "#    çƒ­ç”µè€¦çŸ­è·¯!",
	[TC_RE]   = "#    æ„Ÿæ¸©çº¿æŽ¥å!",
	[HT_ST]   = "#    å‘çƒ­åœˆçŸ­è·¯!",
	[TR_ST]   = "#    å¯æŽ§ç¡…å¯èƒ½å‡»ç©¿!",
	[HC_RE]   = "#    å¯èƒ½æŽ¥çº¿é”™è¯¯!",
	[OFLI]    = "#    ç¦»çº¿æ— é€šè®¯!",
};

const char* errMeg_en[] = {
	[NoError] = "                       ",
	[ALARM_H] = "#  High limit alarm",
	[ALARM_L] = "#  Low limit alarm",
	[TC_OP]   = "#  Sensor open circuit",
	[F1_OP]   = "#  Fuse1 disconnection",
	[F2_OP]   = "#  Fuse2 disconnection",
	[HT_OP]   = "#  Heater open circuit",
	[TC_ST]   = "#  Sensor short circuit",
	[TC_RE]   = "#  Sensor reverse",
	[HT_ST]   = "#  Heater short circuit",
	[TR_ST]   = "#  Triac short circuit",
	[HC_RE]   = "#  Wiring error",
	[OFLI]    = "#  Off line",
};
const char** errMeg;
uint8_t errArry[4];

//-----------------------------------------------------------------------	
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFont20;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontfont14;

void disVal(uint16_t sv,int y){
	GUI_GotoXY(150,y); 
	GUI_DispDec(sv, 3);
}

void disDelay(){		
	GUI_GotoXY(140,83);
	GUI_DispDec(Stby.stby_m,2);
	GUI_DispString("m");
	GUI_DispDec(Stby.stby_s,2);
	GUI_DispString("s");
}
//-----------------------------------------------------------------------
void disUlt(int y){
	GUI_GotoXY(203,y); 
	if(!Stby.cf) {
		GUI_DispString("â„ƒ");
	}else {
		GUI_DispString("â„‰");	
	}	
}
//-----------------------------------------------------------------------
void disAL(int row,int id,const char* code){
	switch(row)
	{
		case 1: 
			GUI_DispStringAt(code,5,215);
			GUI_GotoX(18);
		    GUI_DispDecMin(id);
		    break;
		case 2: 
			GUI_DispStringAt(code,5,240);
			GUI_GotoX(18);
		    GUI_DispDecMin(id);
		    break;
		case 3: 
			GUI_DispStringAt(code,5,265);
			GUI_GotoX(18);
		    GUI_DispDecMin(id);
		    break;
		case 4: 
			GUI_DispStringAt(code,5,290);
			GUI_GotoX(18);
		    GUI_DispDecMin(id);
		    break;		
	}		
}

//-----------------------------------------------------------------------
void disSw(uint16_t sw,int y){
	GUI_GotoXY(145,y); 
	if(!Stby.cn_en){
		if(sw) GUI_DispString("å¼€å¯");
		else GUI_DispString("å…³é—­");
	}else{
		if(sw) GUI_DispString("ON");
		else GUI_DispString("OFF");
	}
}
void disSeg(uint8_t seg_num){
	switch(seg_num)
	{
		case 0: disVal(Stby.workDsp,3);		                     		        
                break;			
		case 1: disUlt(3);disUlt(43);
		        if(Stby.chC == 1){
					if(!Stby.cf) {
						Stby.workDsp = Stby.workSV;
						Stby.stbyDsp = Stby.stbySV;
					}else {
						Stby.workDsp = 9*Stby.workSV/5+32;
						Stby.stbyDsp = 9*Stby.stbySV/5+32;		
					}
					GUI_SetColor(GUI_YELLOW);
					disVal(Stby.workDsp,3);
					disVal(Stby.stbyDsp,43);
				}									
		        break;
		case 2: disVal(Stby.stbyDsp,43);	        
				break;			
		case 3: GUI_GotoXY(140,83);
				GUI_DispDec(Stby.stby_m,2);		        
				break;			
		case 4: GUI_GotoXY(197,83);
				GUI_DispDec(Stby.stby_s,2);		        
				break;
		case 5: disSw(Stby.enable,123);
				break;
		case 6: disSw(Stby.SyncEnable,163);			
				break;
	}
}

void disData()
{
	static uint8_t seg_lst;
	if(mode == 0){
		GUI_SetColor(GUI_YELLOW);
		disSeg(seg_lst);
        if(Stby.chSV == 1){
			Stby.chSV = 0;
			if(!Stby.cf) Stby.workSV = Stby.workDsp;
		    else Stby.workSV = (Stby.workDsp-32)*5/9;   
			AT24CXX_WriteOneByte(1,Stby.workSV>>8);
			AT24CXX_WriteOneByte(2,Stby.workSV);
			for(int i=1;i<=NUM;i++){ 
				alarmTab[i].writeId = 3;  //¹¤×÷
				alarmTab[i].writeData = Stby.workSV*10;
				alarmTab[i].isWrie = 1;	
                alarmTab[i].wNum = 5;				
			}	
		}
		if(Stby.chC == 1){
			Stby.chC = 0;
		    AT24CXX_WriteOneByte(8,Stby.cf);
			for(int i=1;i<=NUM;i++){ 
				alarmTab[i].writeId = 5;
				alarmTab[i].writeData = Stby.cf;
				alarmTab[i].isWrie = 1;
				alarmTab[i].wNum = 5;
			}	
		}
		if(Stby.chST == 1){
			Stby.chST = 0;
			if(!Stby.cf) Stby.stbySV = Stby.stbyDsp;
			else Stby.stbySV = (Stby.stbyDsp-32)*5/9; 
			AT24CXX_WriteOneByte(3,Stby.stbySV>>8);
			AT24CXX_WriteOneByte(4,Stby.stbySV);
		}		
		if(Stby.chM == 1){
			Stby.chM = 0;
			AT24CXX_WriteOneByte(6,Stby.stby_m);
		}
		if(Stby.chS == 1){
			Stby.chS = 0;
			AT24CXX_WriteOneByte(7,Stby.stby_s);
		}
		if(Stby.chE == 1){
			Stby.chE = 0;
			AT24CXX_WriteOneByte(9,Stby.enable);
		}
		if(Stby.chSyn == 1){
			Stby.chSyn = 0;
			AT24CXX_WriteOneByte(10,Stby.SyncEnable);
		}
	}
	else if(mode == 1){
		GUI_SetColor(GUI_YELLOW);
		disSeg(seg_lst);
		GUI_SetColor(GUI_GREEN);
		disSeg(seg);
	}
    else if(mode == 2){
		GUI_SetColor(GUI_ORANGE);
		disSeg(seg);
	}
	seg_lst = seg;
}
void disSig()
{
//	GUI_SetFont(&GUI_FontFont20);
//	GUI_SetColor(GUI_GREEN);
	if(!Stby.cn_en){
	//	GUI_DispStringAt("å‡†",25,220);
	//	GUI_DispStringAt("å¤‡",25,260);
		
		GUI_SetFont(&GUI_Fontfont14);
		GUI_SetColor(GUI_YELLOW);
		GUI_DispStringAt("å¾…æœºè§¦å‘ä¿¡å·",75,215);
		GUI_DispStringAt("å¼€æœºè§¦å‘ä¿¡å·",75,240);
		GUI_DispStringAt("æ¸©åº¦åˆ°è¾¾è¾“å‡º",75,265);
		GUI_DispStringAt("æ•…éšœä¿¡å·è¾“å‡º",75,290);
	}else{
	//	GUI_DispStringAt("Ready",3,240);
		
		GUI_SetFont(&GUI_Fontfont14);
		GUI_SetColor(GUI_YELLOW);
		GUI_DispStringAt("Stby Signal",88,215);
		GUI_DispStringAt("Boot Signal",88,240);
		GUI_DispStringAt("Temp Arrive",88,265);
		GUI_DispStringAt("Alarm Output",88,290);
	}
}
//-----------------------------------------------------------------------
void DrawArcScale(void) 
{
	uint16_t xofs = 10, yofs = 3, hofs = 40,height;
	
    GUI_SetBkColor(GUI_BLACK);   
	GUI_Clear();
	GUI_SetColor(GUI_WHITE);  //BGR
	GUI_SetFont(&GUI_FontFont20);
	GUI_UC_SetEncodeUTF8();
	
	height = yofs;
	if(!Stby.cn_en){
		GUI_DispStringAt("æ¸©åº¦è®¾å®š",xofs,height);	
		height += hofs;
		GUI_DispStringAt("å¾…æœºæ¸©åº¦",xofs,height);    
		height += hofs;
		GUI_DispStringAt("å¾…æœºå»¶æ—¶",xofs,height);	
		height += hofs;
		GUI_DispStringAt("è‡ªåŠ¨å¾…æœº",xofs,height);   
		height += hofs; 
		GUI_DispStringAt("å‡æ¸©åŒæ­¥",xofs,height);	
	}else{
		GUI_DispStringAt("WorkTemp",0,height);	
		height += hofs;
		GUI_DispStringAt("StByTemp",0,height);    
		height += hofs;
		GUI_DispStringAt("StByDelay",0,height);	
		height += hofs;
		GUI_DispStringAt("AutoStBy",0,height);   
		height += hofs; 
		GUI_DispStringAt("SyncHeat",0,height);	
	}
	GUI_SetColor(GUI_YELLOW);	
    if(!Stby.cf) {
		Stby.workDsp = Stby.workSV;
		Stby.stbyDsp = Stby.stbySV;
	}else {
		Stby.workDsp = 9*Stby.workSV/5+32;
		Stby.stbyDsp = 9*Stby.stbySV/5+32;		
	}
	disUlt(3);disUlt(43);
	disVal(Stby.workDsp,3);
	disVal(Stby.stbyDsp,43);
	
    disDelay();

	disSw(Stby.enable,123);
	disSw(Stby.SyncEnable,163);
	
	GUI_SetColor(GUI_ORANGE); 
	GUI_DrawRect(2, 202, 238, 317);
	//
	disSig();

}

void sing()
{
	static uint16_t lsatdel;
	GUI_SetFont(&GUI_FontFont20);
	if(Stby.stbyDelay>0){
		GUI_SetColor(GUI_YELLOW);
		GUI_GotoXY(140,83);
		GUI_DispDec(Stby.stbyDelay/60,2);	
		GUI_GotoXY(197,83);
		GUI_DispDec(Stby.stbyDelay%60,2);
	}else if(lsatdel>0){
		GUI_SetColor(GUI_YELLOW);
		disDelay();
	}
	lsatdel = Stby.stbyDelay;	
	GUI_SetColor(GUI_GREEN);
	switch(Stby.workMode)
	{		
		case 0: 
			if(!Stby.cn_en){
				GUI_DispStringAt("å‡†",25,220);
				GUI_DispStringAt("å¤‡",25,260); 
			}else{
				GUI_DispStringAt("Ready",3,240);
			}
				break;
		case 1: 
			if(!Stby.cn_en){
				GUI_DispStringAt("è‡ª",25,220);
				GUI_DispStringAt("åŠ¨",25,260); 
			}else{
				GUI_DispStringAt("Auto",3,240);
			}
			break;
		case 2: 
			if(!Stby.cn_en){
				GUI_DispStringAt("å¾…",25,220);
				GUI_DispStringAt("æœº",25,260); 
			}else{
				GUI_DispStringAt("Stby",3,240);
			}
			break;
		case 3: 
			if(!Stby.cn_en){
				GUI_DispStringAt("å…³",25,220);
				GUI_DispStringAt("æœº",25,260); 
			}else{
				GUI_DispStringAt("Shut",3,240);
			}
			break;		
	}				
	
	if(!Stby.sigStby) {
		GUI_SetColor(GUI_BLACK); 
		GUI_FillCircle(220, 224, 9);
	}else {
		GUI_SetColor(GUI_GREEN);
		GUI_FillCircle(220, 224, 9);
	}
	
	if(!Stby.sigpower) {
		GUI_SetColor(GUI_BLACK); 
		GUI_FillCircle(220, 249, 9);
	}else {
		GUI_SetColor(GUI_GREEN);
		GUI_FillCircle(220, 249, 9);
	}
	
	if(!Stby.outArrive) {
		GUI_SetColor(GUI_BLACK); 
		GUI_FillCircle(220, 274, 9);
	}else {
		GUI_SetColor(GUI_GREEN);
		GUI_FillCircle(220, 274, 9);
	}
	
	if(!Stby.outAlarm) {
		GUI_SetColor(GUI_BLACK); 
		GUI_FillCircle(220, 299, 9);
	}else {
		GUI_SetColor(GUI_GREEN);
		GUI_FillCircle(220, 299, 9);
	}
	
	
	
	
	
}
void addErrCode(uint8_t id)
{
	int i=0;
	for(i=0;i<4;i++){
		if(errArry[i] == id) break;
	}
	if(i==4){
		for(i=0;i<4;i++){
			if(errArry[i] == 0){
				errArry[i] = id;
				break;
			}
		}
	}
	
}

void disErrMsg()  
{
	const char* err;
	uint8_t isErr,i,j;
	static uint8_t lastErr;  
	isErr = 0;
	for(i=0;i<4;i++){
		if(errArry[i]>0) {
			if(alarmTab[errArry[i]].code == 0){
				errArry[i] = 0;
			}
			else {
				isErr++;
			}
		}
	}

	if(isErr>0)  //æœ‰æ•…éšœ
	{
		if(isErr != lastErr){
			GUI_ClearRect(4, 204, 236, 315);
            GUI_SetFont(&GUI_Fontfont14);			
	        GUI_SetColor(GUI_RED); 
			for(i=0,j=1;i<4;i++){
				if(errArry[i]>0) {	
					err = errMeg[alarmTab[errArry[i]].code];
					disAL(j++,errArry[i],err);		
				}
			}
			GUI_SetFont(&GUI_FontFont20);
			GUI_Exec();
		}
	}
	else  //æ²¡æ•…éšœ
	{
		if(lastErr>0) {
			GUI_ClearRect(4, 204, 236, 315);
			disSig();
			GUI_Exec();
		}
		else{
			sing();					
			GUI_Exec();
		}		
	}
		
	lastErr = isErr;
			
}	
void readData()
{
	u16 h,l;
	h = AT24CXX_ReadOneByte(0);		   
	if(h == 0X5a){
		h = AT24CXX_ReadOneByte(1);		
		l = AT24CXX_ReadOneByte(2);
		Stby.workSV = (h<<8) + l;
		
		h = AT24CXX_ReadOneByte(3);		
		l = AT24CXX_ReadOneByte(4);
		Stby.stbySV = (h<<8) + l;
		
		Stby.stby_m = AT24CXX_ReadOneByte(6);
		Stby.stby_s = AT24CXX_ReadOneByte(7);
		
		Stby.cf = AT24CXX_ReadOneByte(8);
		Stby.enable = AT24CXX_ReadOneByte(9);
		Stby.SyncEnable = AT24CXX_ReadOneByte(10);
		
	}else{
		Stby.workSV = 200;
		Stby.stbySV = 100;
		Stby.stby_m = 13;
		Stby.stby_s = 0;
		Stby.enable = 1;      //¹Ø×Ô¶¯´ý»ú
		Stby.SyncEnable = 1;  //¿ªÒ»¼üÍ¬²½
		
		AT24CXX_WriteOneByte(0,0x5a);
		AT24CXX_WriteOneByte(1,Stby.workSV>>8);
		AT24CXX_WriteOneByte(2,Stby.workSV);
		
		AT24CXX_WriteOneByte(3,Stby.stbySV>>8);
		AT24CXX_WriteOneByte(4,Stby.stbySV);
		
		AT24CXX_WriteOneByte(6,Stby.stby_m);
		AT24CXX_WriteOneByte(7,Stby.stby_s);
		
		AT24CXX_WriteOneByte(8,Stby.cf);
		AT24CXX_WriteOneByte(9,Stby.enable);
		AT24CXX_WriteOneByte(10,Stby.SyncEnable);
	}
}

int main(void)
{
    NVIC_Configuration();  
	initSystick(); 
	uart_init(); 
    initGPIO();
	AT24CXX_Init();	
	readData();
	GUI_Init();
	Stby.workMode = 0;
	Stby.cn_en = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
	if(!Stby.cn_en) {
		errMeg = errMeg_cn;
	}else{
		errMeg = errMeg_en;
	}
	DrawArcScale();	
	IWDG_Config(IWDG_Prescaler_256 ,700); //Tout = prv/40 * rlv (s)	
    while(1)
	{  
		 if(Rx[0].Rxfig ==1) {			 
		     Rx[0].Rxfig =0; 
			 SerialReceive_Callback(&Rx[0]); continue;
		 }
         if(Rx[1].Rxfig ==1) {
		     Rx[1].Rxfig =0; 
		     SerialReceive_Callback(&Rx[1]); continue;
		 }
		 if(Rx[2].Rxfig ==1) {
		     Rx[2].Rxfig =0; 
		     Serial_3_Receive(&Rx[2]); continue;
		 } 
		 if(dealySet >= 20000){ 
			  if((Stby.isStby == 0)&&(Stby.stbyDelay == 0)) 
				  syncExec();	//20sÒÔºó
			  else dealySet = 200;
		 }
		 if(send==0)
			 sendquse();
         if(delay==0)
			 onAlarm();		
         IWDG_ReloadCounter();		  
       // GUI_Exec();		
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
	Txbuf[1] = 0xaa;	 //²éÑ¯
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
	Txbuf[1] = 0x5c;  //Éè¶¨Öµ
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
	Txbuf[1] = 0xab;  //ÃüÁî
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
void SendRequest_U3(uint16_t adr,uint8_t command,uint16_t *reg,uint16_t num)
{
	int i,j;
	CRC16_t crc;
	static uint8_t TxBuf[0xc5+6];
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	TxBuf[0] = adr;
	TxBuf[1] = command;
	TxBuf[2] = num;
	for(i=0,j=0;i<num;i+=2,j++){
		TxBuf[i+3] = reg[j]>>8;
		TxBuf[i+4] = reg[j];
	}
	crc = CreatCRC16(TxBuf, num+3);
	TxBuf[num+3] = crc.Bit[1];
	TxBuf[num+4] = crc.Bit[0];
	
	for(i=0;i<num+5;i++){
		USART_SendData(USART3, TxBuf[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
	}
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);	
}
void SerialReceive_Callback(Rx_t* rx)
{
	CRC16_t crc;
	int i;	
	uint8_t* rxbuf = rx->Rxbuf;
	rx->Rxp = 0;    	
	
	if(rxbuf[1]==0x5a) //adr,5a,state,pv,sv,crc
	{
		crc = CreatCRC16(rxbuf, 15);
		if((rxbuf[15] == crc.Bit[1])&&(rxbuf[16] == crc.Bit[0]))
		{								
			alarmTab[rxbuf[0]].code = rxbuf[2];				
			alarmTab[rxbuf[0]].timeOut = 3000; //2s
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
			if(rxbuf[2] >0 ) addErrCode(rxbuf[0]);		
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
void Serial_3_Receive(Rx_t* rx)
{
	CRC16_t crc;
	uint16_t * pSysPart;
	int i;	
	uint8_t* rxbuf = rx->Rxbuf;
	rx->Rxp = 0;    
	if(rxbuf[1] == 0x03){
		crc = CreatCRC16(rxbuf, 6);
		if((rxbuf[6] == crc.Bit[1])&&(rxbuf[7] == crc.Bit[0]))
		{
		    pSysPart = (uint16_t *)&DataTab[rxbuf[0]] + ((rxbuf[2]<<8) + rxbuf[3]);
	        SendRequest_U3(rxbuf[0],rxbuf[1],pSysPart,rxbuf[5]*2);	
		}
	}
	else if(rxbuf[1] == 0x06){
		crc = CreatCRC16(rxbuf, 6);
		if((rxbuf[6] == crc.Bit[1])&&(rxbuf[7] == crc.Bit[0]))
		{
			uint16_t temp;
			pSysPart = (uint16_t *)&DataTab[rxbuf[0]] + ((rxbuf[2]<<8) + rxbuf[3]);	
			if(pSysPart == &DataTab[rxbuf[0]].SV){
				temp = (rxbuf[4]<<8) + rxbuf[5];
				if(temp != DataTab[rxbuf[0]].SV){
					DataTab[rxbuf[0]].SV = temp;
					alarmTab[rxbuf[0]].writeId = 3;  //¹¤×÷
					alarmTab[rxbuf[0]].writeData = temp;
					alarmTab[rxbuf[0]].isWrie = 1;	
					alarmTab[rxbuf[0]].wNum = 2;
				}
				
			}
			else if(pSysPart == &DataTab[rxbuf[0]].on_off){
				temp = (rxbuf[4]<<8) + rxbuf[5];
				if(temp != DataTab[rxbuf[0]].on_off){
				    DataTab[rxbuf[0]].SV = temp; 
					alarmTab[rxbuf[0]].writeId = 4;     //on_off
					alarmTab[rxbuf[0]].writeData = temp;
					alarmTab[rxbuf[0]].isWrie = 1;
					alarmTab[rxbuf[0]].wNum = 2;
				}
			}
			else if(pSysPart == &DataTab[rxbuf[0]].cf){
				temp = (rxbuf[4]<<8) + rxbuf[5];
				if(temp != DataTab[rxbuf[0]].cf){
					DataTab[rxbuf[0]].cf = temp;
				    alarmTab[rxbuf[0]].writeId = 5;    //cf
					alarmTab[rxbuf[0]].writeData = temp;
					alarmTab[rxbuf[0]].isWrie = 1;
					alarmTab[rxbuf[0]].wNum = 2;
				}
			}
		}
	}
	
	for(i=0;i<20;i++) rxbuf[i] = 0;
}
//----------------------------------------------------------------------
void initGPIO(void)
{
	GPIO_InitTypeDef portSet;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	portSet.GPIO_Speed=GPIO_Speed_10MHz; 
	portSet.GPIO_Mode=GPIO_Mode_Out_PP;
	
	portSet.GPIO_Pin=GPIO_Pin_14;   //ºìµÆ
	portSet.GPIO_Pin|=GPIO_Pin_2;   //ÎÂ¶Èµ½´ï
	portSet.GPIO_Pin|=GPIO_Pin_15;  //ÔËÐÐ/¹ÊÕÏ
	GPIO_Init(GPIOB,&portSet);	
	
	portSet.GPIO_Pin=GPIO_Pin_12;   //ÂÌµÆ
	portSet.GPIO_Pin|=GPIO_Pin_9;   //485-3
	portSet.GPIO_Pin|=GPIO_Pin_10;  //485-2
	portSet.GPIO_Pin|=GPIO_Pin_11;  //485-1
	GPIO_Init(GPIOA,&portSet);
	
	portSet.GPIO_Pin=GPIO_Pin_3;  //wp
	GPIO_Init(GPIOC,&portSet);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	
	portSet.GPIO_Mode=GPIO_Mode_IPU;
	portSet.GPIO_Pin=GPIO_Pin_8;    //SET
	portSet.GPIO_Pin|=GPIO_Pin_9;   //INC
	portSet.GPIO_Pin|=GPIO_Pin_10;  //MODE/return
	portSet.GPIO_Pin|=GPIO_Pin_11;  //DEC	
	GPIO_Init(GPIOC,&portSet);
	
	portSet.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	portSet.GPIO_Pin=GPIO_Pin_12;   //×Ô¶¯´ý»ú
	portSet.GPIO_Pin|=GPIO_Pin_13;  //¿ª¹Ø»ú
    portSet.GPIO_Pin|=GPIO_Pin_5;  //ÓïÑÔÇÐ»»
	GPIO_Init(GPIOB,&portSet);
		
	GPIO_ResetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
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
	if(isRx==0) Stby.workMode = 0; 
    if(Error){
		 if(bit) GPIO_SetBits(GPIOB, GPIO_Pin_14); else GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		 GPIO_SetBits(GPIOB, GPIO_Pin_15); //err-out2
		 bit = ~bit;
         Stby.outAlarm = 1;		
	}
	else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	 //red
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);  //err-out2
		Stby.outAlarm = 0;	
		if(isRx==0) {
			if(rxbit) GPIO_SetBits(GPIOA, GPIO_Pin_12); else GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			rxbit = ~rxbit;	
		}			
		else {
			GPIO_SetBits(GPIOA,GPIO_Pin_12); //green
            if(Stby.workMode == 0) Stby.workMode = 1;			
		}
	}
		
	
	tem_fish = 1;  //ÎÂ¶Èµ½´ï
	for(int i=1;i<=NUM;i++){ 
		if(abs(alarmTab[i].SV-alarmTab[i].PV)>100){
			tem_fish = 0;
		}
	}
	if(isRx){
		if(tem_fish) {
			GPIO_SetBits(GPIOB, GPIO_Pin_2); 
			Stby.outArrive = 1;
		}else {
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			Stby.outArrive = 0;
		}
	}
			
		disErrMsg();
	
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
	uint16_t i=0 ,fish;
	static uint16_t minSV,EN=1;
	static uint8_t step,sendStep;

	if((EN)&&(!Stby.SyncEnable)) {if(step != 0){step =3; sendStep = 1;}}
	else if((!EN)&&(Stby.SyncEnable)) {step =0; sendStep = 1;isCsy =0;Csytim =0;}
	EN = Stby.SyncEnable;
	if(step == 0)
	{
		if((EN)&&(Stby.SyncEnable)){
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
	Stby.sigpower = signal;
    if((!swlast)&&(signal)){ //on
		Stby.workMode = 1; //auto
		for(int i=1;i<=NUM;i++){ 
			alarmTab[i].writeId = 4;
			alarmTab[i].writeData = 1;
			alarmTab[i].isWrie = 1;
			alarmTab[i].wNum = 5;
		}		
	}
	else if((swlast)&&(!signal)){ //off
		Stby.workMode = 3;   //off
	    for(int i=1;i<=NUM;i++){ 
			alarmTab[i].writeId = 4;
			alarmTab[i].writeData = 0;
			alarmTab[i].isWrie = 1;
			alarmTab[i].wNum = 5;
		}
	}
	swlast = signal;	
}
//-----------------------------------------------------------------------
void StbySignal() //ms
{
	static int upnum,downnum;
	static uint8_t signal,swlast;
	static uint16_t numS,svUp;

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
	Stby.sigStby = signal;
    if((!swlast)&&(signal)){ //work
		Stby.stbyDelay = 0;
		if(Stby.isStby){
			Stby.isStby = 0;
			Stby.workMode = 1;   //×Ô¶¯
			for(int i=1;i<=NUM;i++){ 
				alarmTab[i].writeId = 7;  //»Ö¸´
				alarmTab[i].writeData = alarmTab[i].stbySV; 
				alarmTab[i].isWrie = 1; 
                alarmTab[i].wNum = 2;				
			}
          //  svUp = 300;	//5m		
		}
	}
	else if((swlast)&&(!signal)){ //stby
		if(Stby.outArrive){
		    Stby.stbyDelay =  Stby.stby_m*60 + Stby.stby_s;   
		}			
	}
	swlast = signal;	 
	
	if(++numS > 1000){ //1s  
		numS = 0;
		if(svUp>0) svUp--;
		if(Stby.stbyDelay > 0){
			Stby.stbyDelay--;
			if(Stby.stbyDelay == 0){
				Stby.workMode = 2;   //´ý»ú
				Stby.isStby = 1;
				for(int i=1;i<=NUM;i++){
					alarmTab[i].isSV = 0; //¹Ø±ÕÍ¬²½
				}
			//	if(svUp == 0){
					for(int i=1;i<=NUM;i++){
						if(alarmTab[i].SV != alarmTab[i].stbySV){
							alarmTab[i].stbySV = alarmTab[i].SV;
						}
					}
			// 	}
				for(int i=1;i<=NUM;i++){ 					
					alarmTab[i].writeId = 6;  //¹¤×÷
					alarmTab[i].writeData = Stby.stbySV*10;
					alarmTab[i].isWrie = 1;
                    alarmTab[i].wNum = 2;					
				}	
//				for(int i=1;i<=NUM;i++){    //´ý»ú
//					alarmTab[i].isSV =1;
//					alarmTab[i].SVdata = Stby.stbySV*10;
//					alarmTab[i].isfish = 0; 
//				}
			}
		}
	}
}
//-----------------------------------------------------------------------
#define RETURN_KEY GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define SET_KEY    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)
#define DEC_KEY    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define ADD_KEY    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)
void add()
{
	switch(seg)
	{
		case 0: 
			if(!Stby.cf) {
				if(Stby.workDsp<450) {
					Stby.workDsp++;
					Stby.chSV = 1;
				}
			}
			else {
				if(Stby.workDsp<842) {
					Stby.workDsp++;
					Stby.chSV = 1;
				}
			}			
			break;
		case 1:
			if(Stby.cf) {
				Stby.cf = 0; 
                Stby.chC = 1;				
			}				
		break;
		case 2: 
			if(!Stby.cf) {
				if(Stby.stbyDsp<450) {
					Stby.stbyDsp++;
					Stby.chST = 1;
				}
			}
			else {
				if(Stby.stbyDsp<842) {
					Stby.stbyDsp++;
					Stby.chST = 1;
				}
			}
			break; 
		case 3:
			if(Stby.stby_m<59) {
				Stby.stby_m++; 
				Stby.chM = 1;
			}
		    break;    
		case 4:
			if(Stby.stby_s<59) {
				Stby.stby_s++; 
				Stby.chS = 1;
			}
			break;
		case 5: if(Stby.enable == 0){
				Stby.enable = 1; 
			    Stby.chE = 1;
			}
		    break;
		case 6: if(Stby.SyncEnable == 0){
				Stby.SyncEnable = 1; 
			    Stby.chSyn = 1;
			}
			break;
	}					
    disData();		
}
void dec()
{
	switch(seg)
	{
		case 0: 
			if(!Stby.cf) {
				if(Stby.workDsp>40) {
					Stby.workDsp--;
					Stby.chSV = 1;
				}
			}
			else {						
				if(Stby.workDsp>104) {
					Stby.workDsp--;
					Stby.chSV = 1;
				}
			}
			break;
		case 1:
			 if(Stby.cf==0){
				 Stby.cf = 1;
				 Stby.chC = 1;	
			 }				 
		    break;
		case 2: 
			if(!Stby.cf) {
				if(Stby.stbyDsp>40) {
					Stby.stbyDsp--;
					Stby.chST = 1;
				}
			}
			else {
				if(Stby.stbyDsp>104) {
					Stby.stbyDsp--;
					Stby.chST = 1;
				}
			}
			break; 
		case 3:
			if(Stby.stby_m>0) {
				Stby.stby_m--; 
				Stby.chM = 1;
			}
			break;    
		case 4:
			if(Stby.stby_s>0) {
				Stby.stby_s--; 
				Stby.chS = 1;
			}
			break;
		case 5:  if(Stby.enable){
				Stby.enable = 0; 
			    Stby.chE = 1;
			}
		    break;
		case 6: if(Stby.SyncEnable){
			    Stby.SyncEnable = 0; 
			    Stby.chSyn = 1;
			}
			break;
	}	
    disData();		
}
void keyset()
{
	static uint16_t Ns,Nr,Na,Nn,_Na,_Nn;
	static uint16_t Num_na,Num_nn;
	
	if(SET_KEY == Bit_SET) {
		if(Ns<2000) Ns++; 
		if(Ns == 1000){
			if(mode==0) {
				mode=1;	
                GUI_SetFont(&GUI_FontFont20);				
			    disData();
			}
		}
		else if(Ns == 50 && mode == 1){
			mode=2;				
			disData();
		}
	}else Ns = 0;
	
	if(RETURN_KEY == Bit_SET) {
		if(Nr<200) Nr++; 
		if(Nr == 50){
			if(mode>0) {
				mode--;
				disData();
			}
		}
	}else Nr = 0;
	
	if(ADD_KEY == Bit_SET) {
		if(Na<2000) Na++; 		
	}else Na = 0;
	
	if(DEC_KEY == Bit_SET) {
		if(Nn < 2000) Nn++; 		
	}else Nn = 0;
	
	if(Na>1000){
		if(++Num_na > 100){
			Num_na = 0;
			if(mode == 2){
				dec();	
			}      
		}
	}
	else if(_Na>50 && Na==0){
		if(mode == 1){
			if(seg < 6) seg++;
			else seg = 0;
			disData();	
		}
		else if(mode == 2){
			dec();	
		}
        Num_na = 0;		
	}
	
	if(Nn>1000){
		if(++Num_nn > 100){
			Num_nn = 0;
			if(mode == 2){
				add();	
			}      
		}
	}
	else if(_Nn>50 && Nn==0){
		if(mode == 1){
			if(seg > 0) seg--;
			else seg = 6;
			disData();	
		}
		else if(mode == 2){
			add();	
		}
        Num_nn = 0;		
	}
	
	_Na = Na;
	_Nn = Nn;
}
//-----------------------------------------------------------------------
void alarmExec(void)
{  
	static uint8_t num;	
	if(++num >10)
	{
		num = 0; 
		for(int i=1;i<=NUM;i++){ //1-109
			if(alarmTab[i].timeOut >0){
				alarmTab[i].timeOut--;
				if(alarmTab[i].timeOut ==0) {
					alarmTab[i].msg =1;
					alarmTab[i].code = OFLI;
					addErrCode(i);	
				}
			}			
		}
		PoewrSignal();
		

		if(Stby.enable) {
			StbySignal();
		}
		else{
			Stby.stbyDelay = 0;
			Stby.isStby = 0;
		}
				
		
		 if(dealySet < 20005) //20s
			 dealySet++;
		 if(isCsy==1){if(Csytim < 5000) Csytim++;} //4s   		 
		
		if(isRx >0) isRx--;
		 
		 keyset();
	}
	
}




//--------------------------------------------------------------------------
void sendquse(void)  
{
	static uint16_t max = NUM/2, i = 0, j = NUM/2;
	
	if(++i > max) i = 1; 
	if(++j > NUM) j = max; 	 

//---------------------------------------	
	
    if(alarmTab[i].isWrie == 1){  //on/off
		writeData(i,alarmTab[i].writeId,alarmTab[i].writeData);		
		alarmTab[i].writeId = 0;
		alarmTab[i].writeData = 0;
		if(alarmTab[i].wNum > 0) alarmTab[i].wNum--;			
        else alarmTab[i].isWrie = 0;
	}
	else if(alarmTab[i].isSV ==1){   //setSV
		sendData(i,alarmTab[i].SVdata);
		alarmTab[i].isfish =1;
    } 
	else{
		sendCMD(i);   //cmd
	}	
//----------------------------------------	
	
    if(alarmTab[j].isWrie == 1){  //on/off
		writeData(j,alarmTab[j].writeId,alarmTab[j].writeData);
		alarmTab[j].writeId = 0;
		alarmTab[j].writeData = 0;
		if(alarmTab[j].wNum > 0) alarmTab[j].wNum--;			
        else alarmTab[j].isWrie = 0;
	}
	else if(alarmTab[j].isSV ==1){   //setSV
		sendData(j,alarmTab[j].SVdata);
		alarmTab[j].isfish =1;
    } 
	else{
		sendCMD(j);   //cmd
	}	
	
	send = 23; //2.2ms
}
/*
adr cmd dat crc 
*/













