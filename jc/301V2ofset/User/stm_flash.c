#include "stm32f10x.h"
#include "userApp.h"

#define STM32_FLASH_SIZE  128 
#define	PARA_START_ADDR   0x0801F800	//EEpromAddress = 1K*2

//------------------------------------------------------------------------------
#if STM32_FLASH_SIZE<256
  #define STM_SECTOR_SIZE  1024 //字节
#else 
  #define STM_SECTOR_SIZE	 2048
#endif


static uint16_t STMFLASH_BUF [ STM_SECTOR_SIZE / 2 ];//最多是2K字节



/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 读取指定地址的半字(16位数据)
  * 输入参数: faddr:读地址(此地址必须为2的倍数!!)
  * 返 回 值: 返回值:对应数据.
  * 说    明：无
  */
uint16_t STMFLASH_ReadHalfWord ( uint32_t faddr )
{
	return *(vu16*)faddr; 
}
/**
  * 函数功能: 从指定地址开始读出指定长度的数据
  * 输入参数: ReadAddr:起始地址
  *           pBuffer:数据指针
  *           NumToRead:半字(16位)数
  * 返 回 值: 无
  * 说    明：无
  */
void STMFLASH_Read ( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead )   	
{
	uint16_t i;
	
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

/**
  * 函数功能: 不检查的写入
  * 输入参数: WriteAddr:起始地址
  *           pBuffer:数据指针
  *           NumToWrite:半字(16位)数
  * 返 回 值: 无
  * 说    明：无
  */
void STMFLASH_Write_NoCheck ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )   
{ 			 		 
	uint16_t i;	
	
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	  WriteAddr+=2;                                    //地址增加2.
	}  
} 

/**
  * 函数功能: 从指定地址开始写入指定长度的数据
  * 输入参数: WriteAddr:起始地址(此地址必须为2的倍数!!)
  *           pBuffer:数据指针
  *           NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
  * 返 回 值: 无
  * 说    明：无
  */
void STMFLASH_Write ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )	
{
	uint16_t secoff;	   //扇区内偏移地址(16位字计算)
	uint16_t secremain; //扇区内剩余地址(16位字计算)	   
 	uint16_t i;    
	uint32_t secpos;	   //扇区地址
	uint32_t offaddr;   //去掉0X08000000后的地址
	
	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	
	FLASH_Unlock();						//解锁
	
	offaddr=WriteAddr-FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain;	//写地址偏移	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}
//--------------------------------------------------------------------------------------------------------
UCHAR CommIIC(void *data, UINT number,UINT startAddress, UCHAR WR )
{		
	if(WR == 0) //0=write,1=read
	{
		STMFLASH_Write ( PARA_START_ADDR, (uint16_t *)SYS.DataSet, sizeof(SYS.DataSet)/2);	
	}
	else
	{
		STMFLASH_Read ( PARA_START_ADDR + startAddress*2, (uint16_t *)SYS.DataSet, number/2 );
	}
	return 0;
}
//-------------------------------------------------------------------
void Task3(void)
{
	static u16 i=0,bak=0;
		if((SYS.BlinkNum==0)&&(SYS.BlinkState==0))
		{
			if(SYS.WriteState)
			{
			 	CommIIC(&SYS.DataSet[SYS.WriteState],2,SYS.WriteState<<1,0);
				if(SYS.WriteState==ST_T) SYS.StandbyTime=SYS.DataSet[ST_T]/100*3600+SYS.DataSet[ST_T]%100*60;
				else if(SYS.WriteState==ST_R) SYS.DataGet[Standby_SV]=SYS.DataSet[Tem_SV]*SYS.DataSet[ST_R]/100;
				else if(SYS.WriteState==DPSW) {if(SYS.DataSet[DPSW]==0) CommIIC(&SYS.DataSet[Tem_SV],2,Tem_SV<<1,0);}
				SYS.WriteState=0;
			}
		}
		if(SYS.ErrorCode!=NoError)
		{
			switch(SYS.ErrorCode)
			{
				case ALARM_H: bak=4;break;
				case ALARM_L: bak=5;break;
				case F1_OP: bak=9;break;
				case F2_OP: bak=10;break;
				case HT_OP: bak=6;break;
				case TC_OP: bak=1;break;
				case TC_ST: bak=2;break;
				case TC_RE: bak=3;break;
				case HT_ST: bak=7;break;
				case TR_ST: bak=8;break;
				default: bak=0;break;
			}
			if(bak!=SYS.DataSet[Error])
			{
				for(i=0;i<19;i++) SYS.DataSet[Error+19-i]=SYS.DataSet[Error+18-i];
				SYS.DataSet[Error]=bak;
				CommIIC(&SYS.DataSet[Error],40,Error<<1,0);
			}
		}

		if(SYS.DataSet[C_F]) {LED_C_off;LED_F_on;}
		else {LED_F_off;LED_C_on;}
		if(SYS.DataSet[In]) {LED_J_off;LED_K_on;}
		else {LED_K_off;LED_J_on;}
		if(SYS.DataGet[Rate]) LED_out_on;
		else LED_out_off;
		if(SYS.RunningMode==0)
		{
			LED_manu_off;
			if(SYS.AutoFlag==1)	LED_soft_on;
			else LED_soft_off;
			if(SegLed[8]&0x0400) LED_auto_off;
			else LED_auto_on;
		}
		else if(SYS.RunningMode==1)
		{
			LED_soft_off;
			LED_auto_off;
			LED_stby_on;
		}
		else if(SYS.RunningMode==2)
		{
			LED_soft_off;
			LED_stby_off;
			LED_manu_on;
		}
		else
		{
			LED_soft_off;
			LED_auto_off;
			LED_stby_off;
			LED_manu_off;
		} 
}


