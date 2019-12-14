#ifndef __Example_H
#define __Example_H
typedef unsigned short UINT;
typedef	unsigned char  BOOL;	
typedef	unsigned char  UCHAR;	
#include <stm32f10x_lib.h>


float DataConverter1(int P,int e0,int e1,int sv,int pv);
float DataConverter2(int P,int e0,int e1,int sv,int pv);
float DataConverter3(int P,int e0,int e1,int sv,int pv);
float DataConverter4(int P,int e0,int e1,int sv,int pv);
s16 ModData1(s32 data);
s16 ModData2(s32 data1,s32 data2,s32 data3);
void StartUp103x(u32 addNum);
UINT ConverTime(UINT data1,UINT data2,UINT data3);
s16 Get1(s32 data1);
s16 Get1ToGet2(s16 data1,s16 data2,UCHAR i);
s32 Get2(s32 data1,s16 data2);
s32 Get3(s32 data1,s16 data2);
s32 Get4(s32 data1,s16 data2);
s32 Get5(s32 data1,s16 data2);
#endif

