
#include "stm32f10x.h"

#ifndef LCD_DELAY
  #define LCD_DELAY(x)
#endif



//SPI��ʾ���ӿ�
//LCD_RST
#define LCD_SET_RST()  GPIO_SetBits(GPIOB, GPIO_Pin_1)//PB1
#define LCD_CLR_RST()  GPIO_ResetBits(GPIOB, GPIO_Pin_1)//PB1 
//LCD_RS//dc  
#define LCD_SET_A0()    GPIO_SetBits(GPIOB, GPIO_Pin_8)//PB8
#define LCD_CLR_A0()    GPIO_ResetBits(GPIOB, GPIO_Pin_8)//PB8
//LCD_CS  
#define LCD_SET_CS()    GPIO_SetBits(GPIOA, GPIO_Pin_4)//PA4 
#define LCD_CLR_CS()    GPIO_ResetBits(GPIOA, GPIO_Pin_4)//PA4 



//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

/*********************************************************************
*
*           Hardware configuration
*
**********************************************************************

  Needs to be adapted to your target hardware.
*/



/*********************************************************************
*
*           Initialisation
*
**********************************************************************
  This routine should be called from your application program
  to set port pins to their initial values
*/

void LCD_X_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
   	SPI_InitTypeDef  SPI_InitStructure;

	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );//PORTAʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;	  //Pin_5 sck  pin7 MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/7/����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//PA4	CS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  //PB8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);//PC4	DC

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);//PB1	LCD_RST
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��  SPI_Direction_1Line_Tx
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���16λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
}


/*********************************************************************
*
*           Send1 routine
*
**********************************************************************

  Usually, there is no need to modify this routine.
  It should be sufficient ot modify the low-level macros above or
  in a configuration file.
*/

/* Write 1 byte, MSB first */
u8 SPI_WriteByte(u8 data)
{
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);		//�ȴ���������	  
	SPI1->DR=data;	 	   
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);//�ȴ�������
	return SPI1->DR;          	     //�����յ�������			
}

/*********************************************************************
*
*           Read1 routine
*
**********************************************************************
*/

/* Read 1 byte, MSB first */
u16 SPI1_ReadWriteByte(u16 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	SPI_WriteByte(0x2C);  
}	
/*********************************************************************
*
*           Controller 0
*
**********************************************************************


  
  
  
  
  
  
*/

/* Write to controller, with A0 = 0 */
void LCD_X_REG(u16 Cmd) {
  LCD_CLR_CS();
  LCD_CLR_A0();
  SPI_WriteByte(Cmd);
  LCD_SET_CS();
}
/* Write to controller, with A0 = 1 */
void LCD_X_DATA(u16 Data) {
  LCD_CLR_CS();
  LCD_SET_A0();
  SPI_WriteByte(Data);
  LCD_SET_CS();
}

//void LCD_D_DATA(u16 data)
//{
// 	LCD_CLR_CS();
//    LCD_SET_A0();
//	SPI_WriteByte(data>>8);
//	SPI_WriteByte(data);
//	LCD_SET_CS();
//}




/*void (* pfWriteM16_A0) (U16 * pData, int NumItems);*/
//void LCD_X_Write_REG(u16 * pData, int NumBytes) {
//  LCD_CLR_CS();
//  LCD_CLR_A0();
//  for (; NumBytes; NumBytes--) {
//    SPI_WriteByte(*pData++);
//  }
//  LCD_SET_CS();
//}



/* Write multiple bytes to controller, with A0 = 1 */
//void LCD_X_Write_Data(u16 * pData, int NumBytes) {	
////  LCD_X_REG(0x2C); //WriteRAM	
//	
//  LCD_CLR_CS();
//  LCD_SET_A0();
//  for (; NumBytes; NumBytes--) {
//	  SPI_WriteByte(*pData>>8);
//      SPI_WriteByte(*pData);
//	  pData++;
//  }
//  LCD_SET_CS();
//}

/*U16   (* pfRead16_A0)   (void);*/
//u16 LCD_X_Read_REG(void) {
//  unsigned char Data;
//  LCD_CLR_CS();
//  LCD_CLR_A0();
//  Data = SPI1_ReadWriteByte(0);
//  LCD_SET_CS();
//  return Data;
//}
/*void (* pfReadM16_A0)  (U16 * pData, int NumItems);*/
//void LCD_X_Read_M_REG(u16 * pData, int NumBytes) {
//  LCD_CLR_CS();
//  LCD_CLR_A0();
//  for (; NumBytes; NumBytes--) {
//	 *pData = SPI1_ReadWriteByte(0);
//     pData++;
//  }
//  LCD_SET_CS();
//}

/* Read from controller, with A0 = 1 */
//u16 LCD_X_Read_Data(void) {
//  u16 Data;
//  LCD_CLR_CS();
//  LCD_CLR_A0();
//  SPI_WriteByte(0x2E);

//  LCD_SET_A0();
//  Data = SPI1_ReadWriteByte(0);
//  LCD_SET_CS();
//  return Data;
//}


/*void (* pfReadM16_A1)  (U16 * pData, int NumItems);*/
//void LCD_X_Read_M_DATA(u16 * pData, int NumBytes) {
//  LCD_CLR_CS();
////  LCD_CLR_A0();
////  SPI_WriteByte(0x2E);

//  LCD_SET_A0();
//  for (; NumBytes; NumBytes--) {
//	 *pData = SPI1_ReadWriteByte(0x22);
//     pData++;
//  }
//  LCD_SET_CS();
//}







//����
//x,y:����
//Pixel:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y,u16 Pixel)
{
	LCD_CLR_CS();
	
    LCD_CLR_A0();
    SPI_WriteByte(0X2A);  //reg-x
	
    LCD_SET_A0();
	SPI_WriteByte(x>>8);  //data
	SPI_WriteByte(x);
	
	LCD_CLR_A0();
    SPI_WriteByte(0X2B);  //reg-y
	
    LCD_SET_A0();
	SPI_WriteByte(y>>8);  //data
	SPI_WriteByte(y);
	
	LCD_CLR_A0();
    SPI_WriteByte(0X2C);  //��ʼд��GRAM
	
    LCD_SET_A0();
	SPI_WriteByte(Pixel>>8);  //data
	SPI_WriteByte(Pixel);
	
    LCD_SET_CS(); 
} 

//����
//x,y:����
//Pixel:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 Pixel=0;
	LCD_CLR_CS();
	
    LCD_CLR_A0();
    SPI_WriteByte(0X2A);  //reg-x
	
    LCD_SET_A0();
	SPI_WriteByte(x>>8);  //data
	SPI_WriteByte(x);
	
	LCD_CLR_A0();
    SPI_WriteByte(0X2B);  //reg-y
	
    LCD_SET_A0();
	SPI_WriteByte(y>>8);  //data
	SPI_WriteByte(y);
	
	LCD_CLR_A0();
    SPI_WriteByte(0X2E);  //��ʼ����GRAM
	
    LCD_SET_A0();
	Pixel = SPI1_ReadWriteByte(0);
	Pixel<<=8;
	Pixel += SPI1_ReadWriteByte(0);
	
    LCD_SET_CS(); 
	return Pixel;
} 
void LCD_Color_Fill(int x0, int y0, int x1, int y1,u16 color)
{  
	int x;
	for (; y0 <= y1; y0++) {
		LCD_CLR_CS();
	
		LCD_CLR_A0();
		SPI_WriteByte(0X2A);  //reg-x
		
		LCD_SET_A0();
		SPI_WriteByte(x0>>8);  //data
		SPI_WriteByte(x0);
		
		LCD_CLR_A0();
		SPI_WriteByte(0X2B);  //reg-y
		
		LCD_SET_A0();
		SPI_WriteByte(y0>>8);  //data
		SPI_WriteByte(y0);
		
		LCD_CLR_A0();
		SPI_WriteByte(0X2C);  //��ʼд��GRAM
		
		LCD_SET_A0();
      for (x = x0; x <= x1; x++) {         
		  SPI_WriteByte(color>>8);  //data
		  SPI_WriteByte(color);
      }
	  LCD_SET_CS(); 
    }	
}  

void DrawBitLine(int x, int y, u16 const * p, int xsize) {
	LCD_CLR_CS();

	LCD_CLR_A0();
	SPI_WriteByte(0X2A);  //reg-x
	
	LCD_SET_A0();
	SPI_WriteByte(x>>8);  //data
	SPI_WriteByte(x);
	
	LCD_CLR_A0();
	SPI_WriteByte(0X2B);  //reg-y
	
	LCD_SET_A0();
	SPI_WriteByte(y>>8);  //data
	SPI_WriteByte(y);
	
	LCD_CLR_A0();
	SPI_WriteByte(0X2C);  //��ʼд��GRAM
	
	LCD_SET_A0();
    for (;xsize > 0; xsize--, x++, p++) {
		SPI_WriteByte(*p>>8);  //data
		SPI_WriteByte(*p);
    }
    LCD_SET_CS(); 
}










