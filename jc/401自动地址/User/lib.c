#include <math.h>
#include  <stdlib.h>

#include "stm32f10x.h"
#include "userApp.h"

float DataConverter(int P,int e0,int e1,int sv,int pv)
{
		static float UdBak0=0.0;
		float result,UdBak1;
		int dataBuff,dataBuff1,DataP;
		/*dataBuff=abs(sv-pv)*P;
		if(dataBuff>100000)
		{	
				DataP=P*9000;	
		}
		else 
		{
				if(dataBuff>50000) DataP=(350000+dataBuff*2)*P/50;				
				else DataP=(550000-dataBuff*2)*P/50;
		}*/
		dataBuff=abs(sv-e1)*(P+100)/200;
		if((e1>sv)&&(e0>e1)) dataBuff=0;
		else if((e1<sv)&&(e0<e1)) dataBuff=0;
		if(dataBuff<1000) dataBuff1=5240-pow(dataBuff,1.24);
		else dataBuff1=-pow(dataBuff-1000,1.0);
						
		dataBuff=8000+dataBuff1;
		if(dataBuff<3000) dataBuff=3000;
		DataP=dataBuff*P;
		UdBak1=UdBak0;
		UdBak0=(float)(3000000*P*pow((float)abs(e1-e0),0.80))/(100000+dataBuff);
		if(e1<e0) UdBak0=-UdBak0; 																
		UdBak0=(float)(UdBak0+UdBak1*5)/6;
		result=UdBak0-UdBak1;
		if((pv>sv)&&(e0>e1)) if(dataBuff>0) dataBuff=0; 
		dataBuff1=sv-pv;	
		/*if(dataBuff1<0) dataBuff1-=10;
		else dataBuff1+=10;
            if(dataBuff1>5000)	dataBuff1=5000;
		else if(dataBuff1<-5000)	dataBuff1=-5000;
		result+=dataBuff1*0.75;	*/
		dataBuff1=abs(sv-pv);						
		if(sv>pv) result+=(10+pow((float)dataBuff1,0.95))*P/(P+(P+40)/3);
		else if(sv<pv) result+=(-10-pow((float)dataBuff1,0.95))*P/(P+(P+40)/3);		
             if(e1>=e0) result+=(float)(pow((e1-e0),1.2)*DataP)/15000;//***
		else result+=(float)(pow((e0-e1),1.2)*DataP)/-15000;//***		
		return result;
}

s16 ModData1(s32 data)
{
	data=pow((25+data),0.5)*2-10;
	if(data>150) data=150;
	else if(data<10) data=10;
	return data;
}
s16 ModData2(s32 data1,s32 data2,s32 data3)
{
		return (20*pow((float)((data1+10)*20)/(data2-data3+400),0.50));
}
