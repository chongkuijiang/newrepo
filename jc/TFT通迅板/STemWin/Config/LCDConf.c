/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.46 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

#include "GUIDRV_Lin.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 240
#define YSIZE_PHYS 320

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_M565

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_TEMPLATE

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VRAM_ADDR
  #define VRAM_ADDR 0
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif
  
 //#define DISPLAY_ORIENTATION (0)
#define DISPLAY_ORIENTATION (GUI_MIRROR_X)
// #define DISPLAY_ORIENTATION (GUI_MIRROR_Y)//
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y | GUI_SWAP_XY) 
  
  
  
void LCD_X_Init(void);
  void LCD_X_REG(unsigned short Cmd);
  void LCD_X_DATA(unsigned short Data);
  void LCD_X_Write_REG(unsigned short * pData, int NumBytes);
  void LCD_X_Write_Data(unsigned short * pData, int NumBytes);
  unsigned short LCD_X_Read_REG(void);
  unsigned short LCD_X_Read_Data(void);
  void LCD_X_Read_M_REG(unsigned short * pData, int NumBytes);
  void LCD_X_Read_M_DATA(unsigned short * pData, int NumBytes);
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Should initialize the display controller
*/
 
static void _InitController(void) {
  LCD_X_Init();			
  GUI_X_Delay(10);
	
  LCD_X_REG(0x01); //Soft Reset
  GUI_X_Delay(30);
  LCD_X_REG(0x28); //Display OFF

	LCD_X_REG(0xCB);  
        LCD_X_DATA(0x39); 
        LCD_X_DATA(0x2C); 
        LCD_X_DATA(0x00); 
        LCD_X_DATA(0x34); 
        LCD_X_DATA(0x02); 

        LCD_X_REG(0xCF);  
        LCD_X_DATA(0x00); 
        LCD_X_DATA(0XC1); 
        LCD_X_DATA(0X30); 
 
        LCD_X_REG(0xE8);  
        LCD_X_DATA(0x85); 
        LCD_X_DATA(0x00); 
        LCD_X_DATA(0x78); 
 
        LCD_X_REG(0xEA);  
        LCD_X_DATA(0x00); 
        LCD_X_DATA(0x00); 
 
        LCD_X_REG(0xED);  
        LCD_X_DATA(0x64); 
        LCD_X_DATA(0x03); 
        LCD_X_DATA(0X12); 
        LCD_X_DATA(0X81); 

        LCD_X_REG(0xF7);  
        LCD_X_DATA(0x20); 
  
        LCD_X_REG(0xC0);    //Power control 
        LCD_X_DATA(0x23);   //VRH[5:0] 
 
        LCD_X_REG(0xC1);    //Power control 
        LCD_X_DATA(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_X_REG(0xC5);    //VCM control 
        LCD_X_DATA(0x3e); //对比度调节
        LCD_X_DATA(0x28); 
 
        LCD_X_REG(0xC7);    //VCM control2 
        LCD_X_DATA(0x86);  //--
 
        LCD_X_REG(0x36);    // Memory Access Control 
        LCD_X_DATA(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_X_REG(0x3A);    
        LCD_X_DATA(0x55); 

        LCD_X_REG(0xB1);    
        LCD_X_DATA(0x00);  
        LCD_X_DATA(0x18); 
 
        LCD_X_REG(0xB6);    // Display Function Control 
        LCD_X_DATA(0x08); 
        LCD_X_DATA(0x82);
        LCD_X_DATA(0x27);  
 
        LCD_X_REG(0xF2);    // 3Gamma Function Disable 
        LCD_X_DATA(0x00); 
 
        LCD_X_REG(0x26);    //Gamma curve selected 
        LCD_X_DATA(0x01); 
 
        LCD_X_REG(0xE0);    //Set Gamma 
        LCD_X_DATA(0x0F); 
        LCD_X_DATA(0x31); 
        LCD_X_DATA(0x2B); 
        LCD_X_DATA(0x0C); 
        LCD_X_DATA(0x0E); 
        LCD_X_DATA(0x08); 
        LCD_X_DATA(0x4E); 
        LCD_X_DATA(0xF1); 
        LCD_X_DATA(0x37); 
        LCD_X_DATA(0x07); 
        LCD_X_DATA(0x10); 
        LCD_X_DATA(0x03); 
        LCD_X_DATA(0x0E); 
        LCD_X_DATA(0x09); 
        LCD_X_DATA(0x00); 

        LCD_X_REG(0XE1);    //Set Gamma 
        LCD_X_DATA(0x00); 
        LCD_X_DATA(0x0E); 
        LCD_X_DATA(0x14); 
        LCD_X_DATA(0x03); 
        LCD_X_DATA(0x11); 
        LCD_X_DATA(0x07); 
        LCD_X_DATA(0x31); 
        LCD_X_DATA(0xC1); 
        LCD_X_DATA(0x48); 
        LCD_X_DATA(0x08); 
        LCD_X_DATA(0x0F); 
        LCD_X_DATA(0x0C); 
        LCD_X_DATA(0x31); 
        LCD_X_DATA(0x36); 
        LCD_X_DATA(0x0F); 
 
        LCD_X_REG(0x11);    //Exit Sleep 
        GUI_X_Delay(100);
				
        LCD_X_REG(0x29);    //Display on 
		//GUI_X_Delay(100);
}

/*********************************************************************
*
*       _SetVRAMAddr
*
* Purpose:
*   Should set the frame buffer base address
*/
static void _SetVRAMAddr(void * pVRAM) {
	
}

/*********************************************************************
*
*       _SetOrg
*
* Purpose:
*   Should set the origin of the display typically by modifying the
*   frame buffer base address register
*/
static void _SetOrg(int xPos, int yPos) {
    
}

/*********************************************************************
*
*       _SetLUTEntry
*
* Purpose:
*   Should set the desired LUT entry
*/
static void _SetLUTEntry(LCD_COLOR Color, U8 Pos) {
  /* TBD by customer */
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void) {
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
  LCD_SetVRAMAddrEx(0, (void *)VRAM_ADDR);
  
 // GUI_TOUCH_SetOrientation(DISPLAY_ORIENTATION);
  //
  // Set user palette data (only required if no fixed palette is used)
  //

  #if defined(PALETTE)
    LCD_SetLUTEx(0, PALETTE);
  #endif
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    _InitController();
    return 0;
  }
  case LCD_X_SETVRAMADDR: {
    //
    // Required for setting the address of the video RAM for drivers
    // with memory mapped video RAM which is passed in the 'pVRAM' element of p
    //
    LCD_X_SETVRAMADDR_INFO * p;
    p = (LCD_X_SETVRAMADDR_INFO *)pData;
    _SetVRAMAddr(p->pVRAM);
    return 0;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    LCD_X_SETORG_INFO * p;
    p = (LCD_X_SETORG_INFO *)pData;
    _SetOrg(p->xPos, p->yPos);
    return 0;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;
    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _SetLUTEntry(p->Color, p->Pos);
    return 0;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    return 0;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    // ...
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
