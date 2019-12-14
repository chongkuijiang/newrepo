#ifndef _FUZZY_C
#define _FUZZY_C
#include "Fuzzy.h"
//模糊规则表
const char kp[7][7]={
  {PB,PB,PM,PM,PS,ZO,ZO},
  {PB,PB,PM,PS,PS,ZO,ZO},
  {PM,PM,PM,PS,ZO,NS,NS},
  {PM,PM,PS,ZO,NS,NM,NM},
  {PS,PS,ZO,NS,NS,NM,NM},
  {PS,ZO,NS,NM,NM,NM,NB},
  {ZO,ZO,NM,NM,NM,NB,NB}};
const char kd[7][7]={
  {PS,NS,NB,NB,NB,NM,PS},
  {PS,NS,NB,NM,NM,NS,ZO},
  {ZO,NS,NM,NM,NS,NS,ZO},
  {ZO,NS,NS,NS,NS,NS,ZO},
  {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
  {PB,NS,PS,PS,PS,PS,PB},
  {PB,PM,PM,PM,PS,PS,PB}};
const char ki[7][7]={
  {NB,NB,NM,NM,NS,ZO,ZO},
  {NB,NB,NM,NS,NS,ZO,ZO},
  {NB,NM,NS,NS,ZO,PS,PS},
  {NM,NM,NS,ZO,PS,PM,PM},
  {NM,NS,ZO,PS,PS,PM,PB},
  {ZO,ZO,PS,PS,PM,PB,PB},
  {ZO,ZO,PS,PM,PM,PB,PB}};

float e=0,ec=0,e1=0;//eg=25; 
float es[7],ecs[7];//n=26;
unsigned char Fuzzy_flag=0;
unsigned int  Fuzzy_cnt=0;
float f0_1to1_0(float x,float a,float  b,float c)//0-1->1-0
{
  if(x<=a)
    return 0;
  else if(x>c)
    return 0;
  else if((a<x)&&(x<=b))
    return ((x-a)/(b-a));
  else //if((b<x)&&(x<=c))
    return ((c-x)/(c-b));
  
}

float cuf(float x,float a,float b,float c)
{  
  float y,z;
  z=(b-a)*x+a;
  y=c-(c-b)*x;
  return ((y+z)/2);
}

float f1_0(float x,float a,float b)//1-0
{
  if(x<=a) 
    return 1;
  else if(x>b)
    return 0;
  else //if((a<x)&&(x<=b))
    return ((b-x)/(b-a));
}

float cufl(float x,float a,float b)
{
  
  return b-(b-a)*x;
}

float f0_1(float x,float a,float b)//0-1
{
  if(x<=a)
    return 0;
  else if(x>=b)
    return 1;
  else //if((a<x)&&(x<b))
    return ((x-a)/(b-a));
}

float cufr(float x,float a,float b)
{
  return ((b-a)*x+a);
}

float min(float a,float b)  //获取小值
{
  return ((a<b)?a:b);
}

float max(float a,float b)//获取大值
{
  return ((a<b)?b:a);
}


void Control_PID(float Set_Value,float Current_Value,float *kpint,float *kiint,float *kdint)
{
  float tab[7][7];  //表
  unsigned char a=0,b=0; 
  float lsd;   //列表最大值
  int p,d,in;  //PID当前规则值
  float detkp,detkd,detki; //PID参数的微调偏差
  
  e=Set_Value-Current_Value;
  ec=e-e1;
  e1=e;
  //-3,-2,-1,0,1,2,3
  es[NB]=f1_0(e,-3,-1);  //e 
  es[NM]=f0_1to1_0(e,-3,-2,0);
  es[NS]=f0_1to1_0(e,-3,-1,1);
  es[ZO]=f0_1to1_0(e,-2,0,2);
  es[PS]=f0_1to1_0(e,-1,1,3);
  es[PM]=f0_1to1_0(e,0,2,3);
  es[PB]=f0_1(e,1,3);
  
  ecs[NB]=f1_0(ec,-0.3,-0.1);//ec
  ecs[NM]=f0_1to1_0(ec,-0.3,-0.2,0);
  ecs[NS]=f0_1to1_0(ec,-0.3,-0.1,0.1);
  ecs[ZO]=f0_1to1_0(ec,-0.2,0,0.2);
  ecs[PS]=f0_1to1_0(ec,-0.1,0.1,0.3);
  ecs[PM]=f0_1to1_0(ec,0,0.2,0.3);
  ecs[PB]=f0_1(ec,0.1,0.3);
  
  for(unsigned char i=0;i<7;i++) //得到表
  {
    float w,h,r;
    for(unsigned char j=0;j<7;j++)
    {
      h=es[i];
      r=ecs[j];
      w=min(h,r); //
      tab[i][j]=w;
    }
  }
  
  for(unsigned char i=0;i<7;i++)  //获取表的最大值所在的位置
  {
    for(unsigned char j=0;j<7;j++)
    {
      if(tab[a][b]<tab[i][j]) 
      {
        a=i;
        b=j;
      }
    }
  }

  lsd=tab[a][b];
  p=kp[a][b];
  d=kd[a][b];   
  in=ki[a][b];
  
  if(p==NB)
    detkp=cufl(lsd,-0.3,-0.1);
  else if(p==NM)
    detkp=cuf(lsd,-0.3,0.2,0);
  else if(p==NS)
    detkp=cuf(lsd,-0.3,0.1,0.1);
  else if(p==ZO)
    detkp=cuf(lsd,-0.2,0,0.2);
  else if(p==PS)
    detkp=cuf(lsd,-0.1,0.1,0.3);
  else if(p==PM)
    detkp=cuf(lsd,0,0.2,0.3);
  else if(p==PB)
    detkp=cufr(lsd,0.1,0.3);
  
  
  if(d==NB)
    detkd=cufl(lsd,-3,-1);
  else if(d==NM)
    detkd=cuf(lsd,-3,2,0);
  else if(d==NS)
    detkd=cuf(lsd,-3,1,1);
  else if(d==ZO)
    detkd=cuf(lsd,-2,0,2);
  else if(d==PS)
    detkd=cuf(lsd,-1,1,3);
  else if(d==PM)
    detkd=cuf(lsd,0,2,3);
  else if(d==PB)
    detkd=cufr(lsd,1,3);
  
  if(in==NB)
    detki=cufl(lsd,-0.06,-0.02);
  else if(in==NM)
    detki=cuf(lsd,-0.06,-0.04,0);
  else if(in==NS)
    detki=cuf(lsd,-0.06,-0.02,0.02);
  else if(in==ZO)
    detki=cuf(lsd,-0.04,0,0.04);
  else if(in==PS)
    detki=cuf(lsd,-0.02,0.02,0.06);
  else if(in==PM)
    detki=cuf(lsd,0,0.04,0.06);
  else if (in==PB)
    detki=cufr(lsd,0.02,0.06);
  
  *kpint=(*kpint)+detkp;
  *kdint=(*kdint)+detkd;
  *kiint=(*kiint)+detki;
}
//void gfc(float Ek)
//{
//  static float Ek_1;
//  float p,i,d,ECk;
//  ECk = Ek - Ek_1;
//  Ek_1 = Ek;
//  if(abs(Ek)>100) {
//    p=100;
//    i=0;
//    d=0;
//  }else{
//    p=20;
//    i=20;
//    if(abs(ECk)>50) d=50; else d=100;  
// }  
//}
//模糊化




//void OUT (float kpint,float kiint,float kdint)
//{
//  float det_u,u,e2,u1;
//  det_u=kpint*(ec)+kpint*kiint*e+kpint*kdint*(e-2*e1+e2);
//  u=det_u+u1;
//  e2=e1; 
//  u1=u;
//  if(u>=10)
//  {u=10;}
//  if(u<=0)
//  {u=0;}
//}



#endif