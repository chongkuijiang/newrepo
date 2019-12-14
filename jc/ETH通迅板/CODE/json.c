#include <json.h>
#include "cJSON.h"
#include "usart.h"

Status_t   status[100];
Control_t  control[100];

M_Sta_t    State;
M_Set_t    Sets;


void SendRequest_U3(char* buf);


void jsonPrint()  
{   
    cJSON* root =  cJSON_CreateObject();
    cJSON_AddItemToObject(root, "del", cJSON_CreateNumber(State.stDelay));
    cJSON_AddItemToObject(root, "ok", cJSON_CreateNumber(State.temp_ok));
    cJSON_AddItemToObject(root, "alarm", cJSON_CreateNumber(State.sigAlarm));
    cJSON_AddItemToObject(root, "sig", cJSON_CreateNumber(State.sigpower));
    cJSON_AddItemToObject(root, "stby", cJSON_CreateNumber(State.sigStby));
    cJSON_AddItemToObject(root, "out_temp", cJSON_CreateNumber(status[State.ch].PV));
    cJSON_AddItemToObject(root, "out_i", cJSON_CreateNumber(status[State.ch].I));
      
    char *s =  cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    SendRequest_U3(s); 
    cJSON_free(s);      
}
void jsonPrintState()
{
    cJSON* root =  cJSON_CreateArray();
    for(int i=0;i<State.Numb;i++){              
        cJSON* data = cJSON_CreateObject(); 
        cJSON_AddItemToObject(data, "PV", cJSON_CreateNumber(status[i].PV));
        cJSON_AddItemToObject(data, "I", cJSON_CreateNumber(status[i].I));
        cJSON_AddItemToObject(data, "out", cJSON_CreateNumber(status[i].out));
        cJSON_AddItemToObject(data, "mod", cJSON_CreateNumber(status[i].mode));
        cJSON_AddItemToObject(data, "cod", cJSON_CreateNumber(status[i].code));       
        cJSON_AddItemToObject(data, "check", cJSON_CreateNumber(status[i].check));
        cJSON_AddItemToArray(root, data);
    } 
    char *s =  cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    SendRequest_U3(s); 
    cJSON_free(s);      
}
void jsonPrintSetting()
{
    cJSON* root =  cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sSV", cJSON_CreateNumber(Sets.stSV));
    cJSON_AddItemToObject(root, "pwm", cJSON_CreateNumber(Sets.pwm));
    cJSON_AddItemToObject(root, "cf",  cJSON_CreateNumber(Sets.cf));      
    cJSON_AddItemToObject(root, "sSW", cJSON_CreateNumber(Sets.stbySW));
    cJSON_AddItemToObject(root, "cSW", cJSON_CreateNumber(Sets.syncSW));
    cJSON_AddItemToObject(root, "del_m", cJSON_CreateNumber(Sets.stby_m));
    cJSON_AddItemToObject(root, "del_s", cJSON_CreateNumber(Sets.stby_s));
        
    cJSON* array =  cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", array);
    for(int i=0;i<State.Numb;i++){              
        cJSON* data = cJSON_CreateObject(); 
        cJSON_AddItemToObject(data, "SV", cJSON_CreateNumber(status[i].SV));
        cJSON_AddItemToObject(data, "mod", cJSON_CreateNumber(status[i].mode));
        cJSON_AddItemToObject(data, "jk", cJSON_CreateNumber(status[i].jk));
        cJSON_AddItemToObject(data, "Ih", cJSON_CreateNumber(status[i].I_h));
        cJSON_AddItemToObject(data, "Ah", cJSON_CreateNumber(status[i].al_h));
        cJSON_AddItemToObject(data, "Al", cJSON_CreateNumber(status[i].al_l));
        cJSON_AddItemToObject(data, "en", cJSON_CreateNumber(status[i].enable));        
//        cJSON_AddItemToObject(data, "pow", cJSON_CreateNumber(status[i].power));
        cJSON_AddItemToObject(data, "ver", cJSON_CreateNumber(status[i].version));
        cJSON_AddItemToArray(array, data);
    }
    
    char *s =  cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    SendRequest_U3(s); 
    cJSON_free(s); 
    
}
//----------------------------------------------------------------------------
void recvStby(cJSON* root)   
{
    uint16_t temp = 0;
    char* str = "{\"answer\":\"ok\"}";
    cJSON* item = cJSON_GetObjectItem(root, "sSV");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp >=400 && temp <= 8420) {                
            for(int i=0;i<State.Numb;i++){                
                if(status[i].enable){            
                    alarmTab[i].writeId = 6; //stsv
                    alarmTab[i].writeData = temp; 
                    alarmTab[i].isWrie = 1;
                    alarmTab[i].wNum = 3;
                }
            }	   
        }
    }
    item = cJSON_GetObjectItem(root, "del_m");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp < 60) Sets.stby_m = temp;
    }
    item = cJSON_GetObjectItem(root, "del_s");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp < 60) Sets.stby_s = temp;
    }
    item = cJSON_GetObjectItem(root, "stby");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp == 0 || temp == 1){
            Sets.stbySW = temp;
        }
    }
    item = cJSON_GetObjectItem(root, "sync");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp == 0 || temp == 1){
            Sets.syncSW = temp;
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvCheck(cJSON* root){
    uint16_t temp=0;    
    char* str = "{\"answer\":\"err\"}";
    cJSON* array = cJSON_GetObjectItem(root, "data"); //check
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                    temp = (uint8_t)item->valueint;
                    if(temp == 1){                                                
                        alarmTab[i].writeId = 12; //clear
                        alarmTab[i].writeData = 1; 
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;	   
                    }     
                }
            }
            str = "{\"answer\":\"ok\"}"; 
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvEN(cJSON* root)   
{
    uint16_t temp=0;    
    char* str = "{\"answer\":\"err\"}";
    cJSON* array = cJSON_GetObjectItem(root, "data"); //enable
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                    temp = (uint8_t)item->valueint;
                    if(temp == 0 || temp == 1){                           
                        control[i].enable = temp;                         
                        alarmTab[i].writeId = 4; //onoff
                        alarmTab[i].writeData = control[i].enable; 
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;	   
                    }     
                }
            }
            str = "{\"answer\":\"ok\"}"; 
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvSW(cJSON* root)   
{
    uint16_t temp=0;       
    char* str = "{\"answer\":\"err\"}";    
    cJSON* array = cJSON_GetObjectItem(root, "data"); //sw
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                    status[i].run =  temp = (uint16_t)item->valueint;
                    alarmTab[i].writeId = 7; //mode
                    if(temp == 0) alarmTab[i].writeData = 3;  //stop
                    else alarmTab[i].writeData = 0;  //run   
                    alarmTab[i].isWrie = 1;
                    alarmTab[i].wNum = 3;
                }
            }
            str = "{\"answer\":\"ok\"}"; 
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvMODE(cJSON* root)   
{
    uint16_t temp=0;  
    uint8_t ch=0,out=0;   
    static uint8_t mode;    
    char* str = "{\"answer\":\"err\"}";  
    cJSON* item = cJSON_GetObjectItem(root, "ch");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp < State.Numb){
            State.ch = ch = temp-1;            
        }
    }
    item = cJSON_GetObjectItem(root, "out");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp < 100){
            out = temp;
        }
    }
    item = cJSON_GetObjectItem(root, "sw_out");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(out >0){
            mode = status[ch].mode;
            if(temp){
                alarmTab[ch].writeId = 7; //mode
                alarmTab[ch].writeData = (2<<8) + out;   
                alarmTab[ch].isWrie = 1;
                alarmTab[ch].wNum = 3;
            }else{
                alarmTab[ch].writeId = 7; //mode
                alarmTab[ch].writeData = 3;  //stop   
                alarmTab[ch].isWrie = 1;
                alarmTab[ch].wNum = 3;
            }
            str = "{\"answer\":\"ok\"}";            
        }
    } 
    SendRequest_U3(str); 
}
//------------------------------
void recvCF(cJSON* root)   
{
    uint16_t temp=0; 
    char* str = "{\"answer\":\"err\"}";    
    cJSON* item = cJSON_GetObjectItem(root, "cf");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp == 0 || temp == 1){
            for(int i=0;i<State.Numb;i++){                     
                alarmTab[i].writeId = 5; //cf
                alarmTab[i].writeData = temp; 
                alarmTab[i].isWrie = 1;
                alarmTab[i].wNum = 3;
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvPWM(cJSON* root)   
{
    uint16_t temp=0;  
    char* str = "{\"answer\":\"err\"}";     
    cJSON* item = cJSON_GetObjectItem(root, "pwm");
    if(cJSON_IsNumber(item)) {
        temp = (uint16_t)item->valueint;
        if(temp == 0 || temp == 1){
            for(int i=0;i<State.Numb;i++){                         
                alarmTab[i].writeId = 8; //pwm-ssr
                alarmTab[i].writeData = temp; 
                alarmTab[i].isWrie = 1;
                alarmTab[i].wNum = 3;	
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str);     
}
//------------------------------
void recvSV(cJSON* root)   
{
    uint16_t temp=0;    
    char* str = "{\"answer\":\"err\"}"; 
    cJSON* array = cJSON_GetObjectItem(root, "data");  //sv
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                    temp = (uint16_t)item->valueint;
                    if(Sets.cf == 0){
                        if(temp >=400 && temp <= 4500){
                            control[i].SV = temp;                         
                            alarmTab[i].writeId = 3; //SV
                            alarmTab[i].writeData = control[i].SV; 
                            alarmTab[i].isWrie = 1;
                            alarmTab[i].wNum = 3;
                        }
                    }else{
                        if(temp >=1040 && temp <= 8420){
                            control[i].SV = temp;                         
                            alarmTab[i].writeId = 3; //SV
                            alarmTab[i].writeData = control[i].SV; 
                            alarmTab[i].isWrie = 1;
                            alarmTab[i].wNum = 3;	
                        }
                    }                    
                }
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvCH(cJSON* root)   
{
    uint16_t temp=0; 
    char* str = "{\"answer\":\"err\"}";      
    cJSON* array = cJSON_GetObjectItem(root, "data");
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                   temp = (uint16_t)item->valueint;
                   if(temp >0 && temp <= 160){
                        control[i].I_h = temp;
                        alarmTab[i].writeId = 9; //HC_H
                        alarmTab[i].writeData = control[i].I_h; 
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;	
                    }
                }
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvAH(cJSON* root)   
{
    uint16_t temp=0; 
    char* str = "{\"answer\":\"err\"}";     
    cJSON* array = cJSON_GetObjectItem(root, "data"); //alarm_h
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                   temp = (uint16_t)item->valueint;
                   if(temp > 40 && temp <= 500){
                        control[i].al_h = temp;
                        alarmTab[i].writeId = 10; //AL_H
                        alarmTab[i].writeData = control[i].al_h; 
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;	
                    }
                }
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str); 
}
//------------------------------
void recvAL(cJSON* root)   
{
    uint16_t temp=0;    
    char* str = "{\"answer\":\"err\"}"; 
    cJSON* array = cJSON_GetObjectItem(root, "data");
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                   temp = (uint16_t)item->valueint;
                   if(temp > 40 && temp <= 500){
                        control[i].al_l = temp;
                        alarmTab[i].writeId = 11; //AL_L
                        alarmTab[i].writeData = control[i].al_l; 
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;
                   }
                }
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str); 
}
void recvJK(cJSON* root)
{
    uint16_t temp=0;    
    char* str = "{\"answer\":\"err\"}"; 
    cJSON* array = cJSON_GetObjectItem(root, "data");
    if(cJSON_IsArray(array))
    {
        int num = cJSON_GetArraySize(array);
        if(num == State.Numb){
            for(int i=0;i<num;i++){
                cJSON* item = cJSON_GetArrayItem(array, i);
                if(cJSON_IsNumber(item)) {
                   temp = (uint16_t)item->valueint;
                   if(temp < 2){
                        alarmTab[i].writeId = 13; //J_K
                        alarmTab[i].writeData = temp; 
                        alarmTab[i].isWrie = 1;
                        alarmTab[i].wNum = 3;
                   }
                }
            }
            str = "{\"answer\":\"ok\"}";
        }
    }
    SendRequest_U3(str); 
}    
//----------------------------------------------------------------------
void recv(const char *input)
{

    int revId;
    cJSON* root = cJSON_Parse(input);
    if(cJSON_IsObject(root))
    {
        cJSON* num = cJSON_GetObjectItem(root, "id");
        if(cJSON_IsNumber(num))
        {
            revId = num->valueint;
            switch(revId)
            {
                case 1: jsonPrint();break;
                case 2: jsonPrintSetting(); break;
                case 3: jsonPrintState(); break;
                
                case 4: recvStby(root); break;
                
                case 5: recvCheck(root); break;  
                case 6: recvEN(root); break;   
                case 7: recvSW(root); break;
                case 8: recvMODE(root); break;
                
                case 9: recvCF(root); break;  
                case 10: recvPWM(root); break;
                
                case 11: recvSV(root); break;
                case 12: recvCH(root); break;
                case 13: recvAH(root); break;
                case 14: recvAL(root); break;
                case 15: recvJK(root); break;
            }            
        }   
    }
    cJSON_Delete(root);  
    
}

