#include <stdint.h>
#include "cJSON.h"



typedef struct{
    uint16_t PV;
    uint16_t I;
    uint16_t out;
    uint8_t code; 
    uint8_t mode;      
    uint8_t enable;   //enable
    uint8_t jk;   
    uint8_t check;
    
    uint16_t SV;
       
    uint16_t I_h;
    uint16_t al_h;
    uint16_t al_l; 
    uint16_t power;    
    uint8_t  version;
    uint8_t run;

}Status_t;

typedef struct{   
	uint16_t SV;
	uint8_t  enable;   //enable
	uint8_t  jk; 
	uint8_t  mode;     //run-mode
	uint16_t  I_h;      //hc-h
	uint16_t  al_h;      //al-h
	uint16_t  al_l;      //al-l
	
}Control_t;


typedef struct{    
    uint8_t  temp_ok;  //温度到达
    uint8_t  sigpower;  //开机信号  
    uint8_t  sigStby;   //侍机信号
    uint8_t  sigAlarm;  //报警信号
    uint8_t  ch;
    
    uint8_t  isStby;   //侍机状态   
    uint16_t stDelay;  //侍机延时
    
	uint8_t Numb;      //
}M_Sta_t;

typedef struct{
	uint8_t  stbySW;    //
	uint8_t  syncSW;    //    
    uint8_t  stby_m;
    uint8_t  stby_s;
    uint8_t  cf;    
    uint8_t  pwm;
    uint16_t stSV;
    
    uint8_t  on_off;   //tur-on	总开关   
}M_Set_t;

//---------------------------------------------
extern Status_t   status[100];
extern Control_t  control[100];

extern M_Sta_t    State;
extern M_Set_t    Sets;




void jsonPrint();
void jsonPrintSetting();
void recv(const char *input);


