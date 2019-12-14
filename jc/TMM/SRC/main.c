#define USER_Globals

#include "stm32f10x.h"
#include "tmm.h"
#include <jansson.h>

uint8_t adress;    
uint16_t led232,led485;

void GPIO_Configuration(void);
uint8_t getAdress(void);
void ledfig(void);
uint8_t tim;

int main(void)
{		
	GPIO_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Config(SystemCoreClock/10000);	
    uart_init();	
	ADCx_Init();
	
    
	int i;
	while(1)
	{
		if(tim){
			
			tim = 0;
			if(i++ ==100){ //10ms
				i=0;
				adress = getAdress();
				ledfig();
				
			}
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
     
	//adress
	portSet.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	portSet.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&portSet);	
	portSet.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11;
	portSet.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&portSet);
	//led-232-485	
	portSet.GPIO_Speed = GPIO_Speed_50MHz;
	portSet.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&portSet);
	GPIO_SetBits(GPIOA, GPIO_Pin_11|GPIO_Pin_12);	
	//485
	portSet.GPIO_Speed = GPIO_Speed_50MHz;
	portSet.GPIO_Pin = GPIO_Pin_12;
	portSet.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOC,&portSet);
	GPIO_ResetBits(GPIOC,GPIO_Pin_12);		
}


uint8_t getAdress(void){
	bit8_t adr;
	adr.bit.b0 = ~GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
	adr.bit.b1 = ~GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
	adr.bit.b2 = ~GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
	adr.bit.b3 = ~GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
	adr.bit.b4 = ~GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
	adr.bit.b5 = ~GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
	adr.bit.b6 = ~GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	adr.bit.b7 = 0;
	return adr.dat;
}

void ledfig(void){	
	if(led232>0){
		led232--;
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	}else{
		GPIO_SetBits(GPIOA, GPIO_Pin_12);	
	}
	if(led485>0){
		led485--;
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	}else{
		GPIO_SetBits(GPIOA, GPIO_Pin_11);	
	}
} 
