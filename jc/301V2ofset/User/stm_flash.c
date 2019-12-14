#include "stm32f10x.h"
#include "userApp.h"

#define STM32_FLASH_SIZE  128 
#define	PARA_START_ADDR   0x0801F800	//EEpromAddress = 1K*2

//------------------------------------------------------------------------------
#if STM32_FLASH_SIZE<256
  #define STM_SECTOR_SIZE  1024 //�ֽ�
#else 
  #define STM_SECTOR_SIZE	 2048
#endif


static uint16_t STMFLASH_BUF [ STM_SECTOR_SIZE / 2 ];//�����2K�ֽ�



/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ȡָ����ַ�İ���(16λ����)
  * �������: faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
  * �� �� ֵ: ����ֵ:��Ӧ����.
  * ˵    ������
  */
uint16_t STMFLASH_ReadHalfWord ( uint32_t faddr )
{
	return *(vu16*)faddr; 
}
/**
  * ��������: ��ָ����ַ��ʼ����ָ�����ȵ�����
  * �������: ReadAddr:��ʼ��ַ
  *           pBuffer:����ָ��
  *           NumToRead:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Read ( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead )   	
{
	uint16_t i;
	
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

/**
  * ��������: ������д��
  * �������: WriteAddr:��ʼ��ַ
  *           pBuffer:����ָ��
  *           NumToWrite:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Write_NoCheck ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )   
{ 			 		 
	uint16_t i;	
	
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	  WriteAddr+=2;                                    //��ַ����2.
	}  
} 

/**
  * ��������: ��ָ����ַ��ʼд��ָ�����ȵ�����
  * �������: WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
  *           pBuffer:����ָ��
  *           NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Write ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )	
{
	uint16_t secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; //������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t secpos;	   //������ַ
	uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	
	FLASH_Unlock();						//����
	
	offaddr=WriteAddr-FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
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


