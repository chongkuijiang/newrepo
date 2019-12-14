
#include "stm32f10x.h"

#ifndef LCD_DELAY
  #define LCD_DELAY(x)
#endif



//SPI显示屏接口
//LCD_RST
#define LCD_SET_RST()  GPIO_SetBits(GPIOB, GPIO_Pin_1)//PB1
#define LCD_CLR_RST()  GPIO_ResetBits(GPIOB, GPIO_Pin_1)//PB1 
//LCD_RS//dc  
#define LCD_SET_A0()    GPIO_SetBits(GPIOB, GPIO_Pin_8)//PB8
#define LCD_CLR_A0()    GPIO_ResetBits(GPIOB, GPIO_Pin_8)//PB8
//LCD_CS  
#define LCD_SET_CS()    GPIO_SetBits(GPIOA, GPIO_Pin_4)//PA4 
#define LCD_CLR_CS()    GPIO_ResetBits(GPIOA, GPIO_Pin_4)//PA4 



//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

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

	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );//PORTA时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;	  //Pin_5 sck  pin7 MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/7/复用推挽输出 
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
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工  SPI_Direction_1Line_Tx
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收16位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
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
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPI1->DR=data;	 	   
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完
	return SPI1->DR;          	     //返回收到的数据			
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
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

//开始写GRAM
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







//画点
//x,y:坐标
//Pixel:此点的颜色
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
    SPI_WriteByte(0X2C);  //开始写入GRAM
	
    LCD_SET_A0();
	SPI_WriteByte(Pixel>>8);  //data
	SPI_WriteByte(Pixel);
	
    LCD_SET_CS(); 
} 

//读点
//x,y:坐标
//Pixel:此点的颜色
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
    SPI_WriteByte(0X2E);  //开始读入GRAM
	
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
		SPI_WriteByte(0X2C);  //开始写入GRAM
		
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
	SPI_WriteByte(0X2C);  //开始写入GRAM
	
	LCD_SET_A0();
    for (;xsize > 0; xsize--, x++, p++) {
		SPI_WriteByte(*p>>8);  //data
		SPI_WriteByte(*p);
    }
    LCD_SET_CS(); 
}










