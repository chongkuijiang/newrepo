
#ifndef __APP_H
#define __APP_H	

#include <stdint.h>


#define  ArrayNum(a)     (sizeof(a)/sizeof(a[0]))
//------------------------------------------------
typedef enum          //在这里定义系统消息
{
	Time  = 0,
	Heartbeat,
	Tmr_collection,

}MsgID_t;
//------------------------------------------------
typedef struct  
{   
   void (*funproc)(void*);
   uint16_t timeOut;
   void *   arg;   
   uint16_t msg;
   uint16_t temp;
}MsgFu_t;
extern MsgFu_t Msg[3];
//------------------------------------------------

//-----------------------------------------------

//-----------------------------------------------------------

typedef struct  
{
	uint16_t Dat;
}sysPart_t;

extern uint16_t *ptim, msgHeart;

void systick_init(void);


#endif

