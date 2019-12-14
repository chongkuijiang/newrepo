
#include "stm32f10x.h"
#include <string.h>
#include <jansson.h>
#include "usart.h"

#define   NUM   16

uint16_t time,rxifg;
uint8_t butgroup,butopen,voltage,signal;
bit16_t outbuf,outsign;

timer_t timer[NUM];
delay_t delay[NUM];   
delayOut_t dOut[6] ={{.delay=0,.out=1},{.delay=800},{.delay=400},{.delay=266},{.delay=200},{.delay=160}};
 

int main(void)
{	
	GPIO_Configuration(); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Config(SystemCoreClock/10000);	
    uart_init();
	
	while(1)
	{
		if(rxifg == 1){
			rxifg = 0;			 
			sendJson(); 			
		}
			  
		if(time){			
			time = 0;
			proctime();
			pushing();
		}
	} 	
}




//-------------------------------------------------------------------------
void sendJson(void)  
{		
	json_t * array = json_array();	
	for(int i=0;i<NUM;i++){	
        json_t* data = json_object();	
        json_object_set_new(data, "ot", json_integer(timer[i].out));		
		json_object_set_new(data, "d1", json_integer(delay[i].delayOut));
		json_object_set_new(data, "o1", json_integer(delay[i].outDelay));	
        json_object_set_new(data, "d2", json_integer(delay[i].delayOut2));
		json_object_set_new(data, "o2", json_integer(delay[i].outDelay2));		
		json_array_append_new(array,data);		
	}
    json_t* root = json_object();	
	json_object_set_new(root, "vol", json_integer(voltage));
	json_object_set_new(root, "sig", json_integer(signal));		
	json_object_set_new(root, "data", array);
		
	char* s = json_dumps( root,JSON_COMPACT|JSON_REAL_PRECISION(5));
	if(s != NULL){
		U1sendn(s);
	    free(s);	
	}	
	json_decref(root);
}

//-----
json_error_t  * error;
void procJson(const char *input)
{	
	json_t* root = json_loads(input,JSON_DISABLE_EOF_CHECK,error );
	if(NULL != root && json_is_object(root))
	{
		json_t* obj;
		int id;
		if(json_unpack(root, "{so,si}","data", &obj,"id", &id)==0)
		{
			if(json_is_object(obj)){		
				butgroup = json_is_true(json_object_get(obj,"butgroup"))? 1:0;
				butopen = json_is_true(json_object_get(obj,"butopen"))? 1:0;
				json_t* array = json_object_get(obj,"group");
				int len = json_array_size(array);
				for(int i=0;i<len;i++){
					timer[i].group = json_integer_value(json_array_get( array,i));
				}
			}
			else if(json_is_array(obj)){
				int len = json_array_size(obj);
				for(int i=0;i<len;i++){   
					json_t* obj2 = json_array_get( obj,i);
					timer[i].mode = (uint8_t)json_integer_value( json_object_get(obj2,"mode"));
					timer[i].onOff = (uint8_t)json_integer_value( json_object_get(obj2,"onOff"));
					timer[i].push = (uint8_t)json_integer_value( json_object_get(obj2,"push"));
					if(timer[i].push>5) timer[i].push = 0;
					timer[i].delayOut = json_number_value(json_object_get(obj2,"delayOut"))*1000;
					timer[i].outDelay = json_number_value(json_object_get(obj2,"outDelay"))*1000;
					timer[i].delayOut2 = json_number_value(json_object_get(obj2,"delayOut2"))*1000;
					timer[i].outDelay2 = json_number_value(json_object_get(obj2,"outDelay2"))*1000;	
					load(i);				
				}	
			}
			json_decref(root);		
		}
		 
	}
	
}

//-------------------------------------------------------------------------
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef portSet;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	
	//port1-16
	portSet.GPIO_Speed = GPIO_Speed_50MHz;
	portSet.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&portSet);	
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	portSet.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&portSet);	
	GPIO_SetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	portSet.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOC,&portSet);	
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	//485
	portSet.GPIO_Pin = GPIO_Pin_0;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&portSet);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	
	//buzzer
	portSet.GPIO_Pin = GPIO_Pin_8;
	portSet.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&portSet);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
    //led-red
    portSet.GPIO_Pin = GPIO_Pin_11;
	portSet.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&portSet);
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);	
    //led-blue
    portSet.GPIO_Pin = GPIO_Pin_12;
	portSet.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&portSet);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);	   
	
	
    //24-220v	
	portSet.GPIO_Pin = GPIO_Pin_14;
	portSet.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&portSet);
	//input-	
	portSet.GPIO_Pin = GPIO_Pin_1;
	portSet.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&portSet);
		
	
}

uint8_t readSignal(void)
{
	static int upnum,downnum,sign;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
	{
		downnum =0;
		if(sign==0){
			if(++upnum == 8) sign = 1;
		}
	}
	else 
	{
		upnum = 0;
		if(sign==1){
			if(++downnum == 8) sign = 0;
		}
	} 
	return sign;
}


void output(void)
{
	for(int i=0;i<NUM;i++){
		timer[i].pOut = dOut[timer[i].push].out;
	}
	
	if(timer[0].out >0) {		
		if(timer[0].pOut >0) GPIO_ResetBits(GPIOA,GPIO_Pin_4); else  GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}else  GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	if(timer[1].out >0) {
		if(timer[1].pOut >0) GPIO_ResetBits(GPIOA,GPIO_Pin_5); else  GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}else  GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
	if(timer[2].out >0) {
		if(timer[2].pOut >0) GPIO_ResetBits(GPIOA,GPIO_Pin_6); else  GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}else  GPIO_SetBits(GPIOA,GPIO_Pin_6);
	
	if(timer[3].out >0) {
		if(timer[3].pOut >0) GPIO_ResetBits(GPIOA,GPIO_Pin_7); else  GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}else  GPIO_SetBits(GPIOA,GPIO_Pin_7);
	
	if(timer[4].out >0) {
		if(timer[4].pOut >0) GPIO_ResetBits(GPIOC,GPIO_Pin_4); else  GPIO_SetBits(GPIOC,GPIO_Pin_4);
	}else  GPIO_SetBits(GPIOC,GPIO_Pin_4);
	
	if(timer[5].out >0) {
		if(timer[5].pOut >0) GPIO_ResetBits(GPIOC,GPIO_Pin_5); else  GPIO_SetBits(GPIOC,GPIO_Pin_5);
	}else  GPIO_SetBits(GPIOC,GPIO_Pin_5);
	
	if(timer[6].out >0) {
		if(timer[6].pOut >0) GPIO_ResetBits(GPIOB,GPIO_Pin_2); else  GPIO_SetBits(GPIOB,GPIO_Pin_2); 
	}else  GPIO_SetBits(GPIOB,GPIO_Pin_2);
	
	if(timer[7].out >0) {
		if(timer[7].pOut >0) GPIO_ResetBits(GPIOB,GPIO_Pin_10); else  GPIO_SetBits(GPIOB,GPIO_Pin_10);	
	}else  GPIO_SetBits(GPIOB,GPIO_Pin_10);	
	
	if(timer[8].out >0) {
		if(timer[8].pOut >0) GPIO_ResetBits(GPIOB,GPIO_Pin_12); else  GPIO_SetBits(GPIOB,GPIO_Pin_12);	
	}else  GPIO_SetBits(GPIOB,GPIO_Pin_12);	
	
	if(timer[9].out >0) {
		if(timer[9].pOut >0) GPIO_ResetBits(GPIOB,GPIO_Pin_13); else  GPIO_SetBits(GPIOB,GPIO_Pin_13);
	}else  GPIO_SetBits(GPIOB,GPIO_Pin_13);
	
	if(timer[10].out >0) {
		if(timer[10].pOut >0) GPIO_ResetBits(GPIOB,GPIO_Pin_14); else  GPIO_SetBits(GPIOB,GPIO_Pin_14);
	}else  GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	if(timer[11].out >0) {
		if(timer[11].pOut >0) GPIO_ResetBits(GPIOB,GPIO_Pin_15); else  GPIO_SetBits(GPIOB,GPIO_Pin_15);
	}else  GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
	if(timer[12].out >0) {
		if(timer[12].pOut >0) GPIO_ResetBits(GPIOC,GPIO_Pin_6); else  GPIO_SetBits(GPIOC,GPIO_Pin_6);
	}else  GPIO_SetBits(GPIOC,GPIO_Pin_6);
	
	if(timer[13].out >0) {
		if(timer[13].pOut >0) GPIO_ResetBits(GPIOC,GPIO_Pin_7); else  GPIO_SetBits(GPIOC,GPIO_Pin_7);
	}else  GPIO_SetBits(GPIOC,GPIO_Pin_7);
	
	if(timer[14].out >0) {
		if(timer[14].pOut >0) GPIO_ResetBits(GPIOC,GPIO_Pin_8); else  GPIO_SetBits(GPIOC,GPIO_Pin_8);
	}else  GPIO_SetBits(GPIOC,GPIO_Pin_8);
	
	if(timer[15].out >0) {
		if(timer[15].pOut >0) GPIO_ResetBits(GPIOC,GPIO_Pin_9); else  GPIO_SetBits(GPIOC,GPIO_Pin_9);
	}else  GPIO_SetBits(GPIOC,GPIO_Pin_9);
	
}

//-------------------------------------------------------------------------
void load(int i)
{
	delay[i].delayOut = timer[i].delayOut;
	delay[i].outDelay = timer[i].outDelay;
	delay[i].delayOut2 = timer[i].delayOut2;
	delay[i].outDelay2 = timer[i].outDelay2;
	timer[i].step = 1;
}
  
void dectimes(int i,int num)
{
	switch(timer[i].step)
	{
		case 1:
			if(delay[i].delayOut>0)
			{
				delay[i].delayOut--;
				if(delay[i].delayOut==0){
					timer[i].out = 1;
					if(timer[i].step<num) timer[i].step++;
				}	
			}					
			break;
		case 2:
			if(delay[i].outDelay>0)
			{
				delay[i].outDelay--;
				if(delay[i].outDelay==0){
					timer[i].out = 0;
					if(timer[i].step<num) timer[i].step++;
				}
			}
			break;
		case 3:
			if(delay[i].delayOut2>0)
			{
				delay[i].delayOut2--;
				if(delay[i].delayOut2==0){
					timer[i].out = 1;
					if(timer[i].step<num) timer[i].step++;
				}
			}
			break;
		case 4:
			if(delay[i].outDelay2>0)
			{
				delay[i].outDelay2--;
				if(delay[i].outDelay2==0){
					timer[i].out = 0;
				}
			}
			break;
	}
}
void pushing(void)
{
	static uint16_t numb = 6;
	static uint16_t tim[6];
	if(numb++ == 14) numb = 0; else return;
	for(int i=1;i<6;i++){
		if(++tim[i] >= dOut[i].delay){
			if(dOut[i].out >0) dOut[i].out = 0; else dOut[i].out = 1;
			tim[i] = 0;
		}
	}
}

void proctime(void) 
{
	static uint8_t num,signlast,del;
	signal = readSignal(); 
	if(num++ == 14) num = 0; else return;	
	voltage = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)? 24 : 220;
	if((signlast==1)&&(signal==0))
	{
		for(int i=0;i<NUM;i++){
			load(i);
			timer[i].out = 0;    
		}
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		del = 3;
	}
	else if((signlast==0)&&(signal==1))
	{
		for(int i=0;i<NUM;i++){
			timer[i].out = 0;
		}
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		del = 3;
	}
    else if(signal==1)
	{
		for(int i=0;i<NUM;i++)
		{
			if(timer[i].onOff == 1)
			{			
				switch(timer[i].mode)
				{
					case 1: 
						dectimes(i,1);
						break;
	 				case 2:
						dectimes(i,2);
						break;
					case 3:
						dectimes(i,4);
						break;
				}
			}	     
		}
	}
    else if(signal==0)
	{
		if(butgroup)
		{
			for(int i=0;i<NUM;i++)
			{
				if(butopen)
				{
					timer[i].out = (timer[i].group==1)? 1:0;
				}
			    else
				{
					timer[i].out = 0;
				}
			}			
		}
		else
		{
			for(int i=0;i<NUM;i++)
			{
			     if(timer[i].onOff == 1)
				 {
					 timer[i].out = butopen? 1:0;
				 }
				 else timer[i].out = 0;
			}	 
		}
	}
	if(del>0){
		del--;
		if(del==0) GPIO_ResetBits(GPIOA,GPIO_Pin_8);		   
	}	
	signlast = signal;
	output();
}





