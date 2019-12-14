#include "stm32f10x.h"
#include "userApp.h"
#include "Out.txt"
#include "TmTable.txt"
UINT ConverTime(UINT data1,UINT data2,UINT data3)
{
	UINT dataBuff;
	if(data1==0) dataBuff=65535;
	else if(data3)
	{
		if(data2==100) dataBuff=TBL_out[999-data1]+800;
		else dataBuff=(TBL_out[999-data1]+800)*5/6;	
	}
	else {dataBuff=(1005-data1)*data2/1000;}
	return dataBuff;
}
s16 Get1(s32 data1)
{	
	static s32 findStateNTC=1;
	s8 i=0;
	if(data1<=TemTblNTC[findStateNTC])
	{
		for(i=0;i<33;i++)
		{
			if(findStateNTC==32) {data1=TemTblNTC[32];break;}
			if(data1<=TemTblNTC[findStateNTC]) findStateNTC++;
			else break;		
		}
	}
	else
	{
		for(i=0;i<33;i++)
		{
			if(data1>=TemTblNTC[findStateNTC]) findStateNTC--;
			else break;
			if(findStateNTC==1) break;
		}
		findStateNTC++;
	}
  data1=-5000+500*findStateNTC-500*(data1-TemTblNTC[findStateNTC])/(TemTblNTC[findStateNTC-1]-TemTblNTC[findStateNTC]);
	return data1;
}
s16 Get1ToGet2(s16 data1,s16 data2,UCHAR i)
{
	const u16 *pt=0;
	if(data1==0) pt=TemTblJ;
	else pt=TemTblK;
	data2=pt[i]-((1000*i-data2)*(pt[i]-pt[i-1])/1000);
	return data2;
}
s32 Get2(s32 data1,s16 data2)
{
	static s32 findStateHT=3;
	const u16 *p;
	u8 i;
	if(data2==0) p=TemTblJ;
	else p=TemTblK;
	if(data1>=p[findStateHT])
	{
		for(i=0;i<70;i++)
		{
			if(data1>=p[findStateHT]) findStateHT++;
			else {findStateHT--;break;}
			if(findStateHT>69) {data1=p[69];findStateHT=69;break;}
		}
	}
	else
	{
		for(i=0;i<70;i++)
		{
		 	if(findStateHT<1) {findStateHT=1;break;}
			if(data1<=p[findStateHT]) findStateHT--;
			else break;
		}
	}
	data1=findStateHT*10000-((s32)p[findStateHT]-data1)*10000/(p[findStateHT]-p[findStateHT-1]);
	return data1;
}






