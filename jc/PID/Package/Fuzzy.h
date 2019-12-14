#ifndef _FUZZY_H
#define _FUZZY_H

#define NB 0
#define NM 1
#define NS 2
#define ZO 3
#define PS 4
#define PM 5
#define PB 6
extern unsigned char Fuzzy_flag;
extern unsigned int  Fuzzy_cnt;
void Control_PID(float Set_Value,float Current_Value,float *kpint,float *kiint,float *kdint);


#endif