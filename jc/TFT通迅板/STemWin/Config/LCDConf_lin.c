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

#include <stddef.h>


#include "GUI.h"
#include "GUIDRV_FlexColor.h"

/*********************************************************************
*
*       Layer configuration
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
#define COLOR_CONVERSION GUICC_565

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR

//
// Orientation
//
//#define DISPLAY_ORIENTATION (0)
#define DISPLAY_ORIENTATION (GUI_MIRROR_X)
// #define DISPLAY_ORIENTATION (GUI_MIRROR_Y)//
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y | GUI_SWAP_XY)

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VRAM_ADDR
  #define VRAM_ADDR 0 // TBD by customer: This has to be the frame buffer start address
#endif
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
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
#ifndef   DISPLAY_ORIENTATION
  #define DISPLAY_ORIENTATION 0
#endif

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
*   Initializes the display controller
*/
void ff(){
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
        LCD_X_DATA(0x3e); //¶Ô±È¶Èµ÷½Ú
        LCD_X_DATA(0x28); 
 
        LCD_X_REG(0xC7);    //VCM control2 
        LCD_X_DATA(0x86);  //--
 
        LCD_X_REG(0x36);    // Memory Access Control 
        LCD_X_DATA(0x48); //C8	   //48 68ÊúÆÁ//28 E8 ºáÆÁ

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
		GUI_X_Delay(100);
}
static void _InitController(void) {
	ff();
 /* #ifndef WIN32
//  GUI_X_Delay(10);
  LCD_X_Init();			
  GUI_X_Delay(10);
	
  LCD_X_REG(0x01); //Soft Reset
  GUI_X_Delay(30);
  LCD_X_REG(0x28); //Display OFF

  LCD_X_REG(0xf7);
  LCD_X_DATA(0x20);

  LCD_X_REG(0xea);
  LCD_X_DATA(0x00);
  LCD_X_DATA(0x00);

  LCD_X_REG(0xc0); //PWCTR1
  LCD_X_DATA(0x26);    //VRH: 4.75V

  LCD_X_REG(0xc1); //PWCTR2
  LCD_X_DATA(0x11);    //AVDD: VCIx2, VGH: VCIx7, VGL: -VCIx3

  LCD_X_REG(0xc5); //VMCTR1
  LCD_X_DATA(0x35);    //4.025V VCOMH
  LCD_X_DATA(0x3e);    //-0.95V VCOML

  LCD_X_REG(0xc7); //VMCTR2
  LCD_X_DATA(0xbe);    //VMF: VMH–2, VML–2
    
  LCD_X_REG(0x3a); //Pixel Format Set
  LCD_X_DATA(0x55);    // 16 bits / pixel

  LCD_X_REG(0xb1); //Frame Control (Normal mode)
  LCD_X_DATA(0x00);    //Default: 0x00
  LCD_X_DATA(0x1B);    //Default: 0x1B

  LCD_X_REG(0x26); //Gamma Set
  LCD_X_DATA(0x01);    //Default: 0x01

  LCD_X_REG(0x11); //Sleep OUT
  GUI_X_Delay(100);
  LCD_X_REG(0x29); //Display ON
  
  GUI_X_Delay(100);
  #endif*/
}

/*********************************************************************
*
*       _ReadPixel
*/
static U16 _ReadPixel(int LayerIndex) {
  #ifndef WIN32
  U16 aData[3];
  U16 Index;

  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
  LCD_X_REG(0x2E);
  //
  // Dummy- and data read
  //
  LCD_X_Read_M_DATA(aData, GUI_COUNTOF(aData));
  //
  // Convert to index
  //
  Index = (aData[2] >> 11) | ((aData[1] & 0xfc) << 3) | (aData[1] & 0xf800);
  #else
  U16 Index;

  GUI_USE_PARA(LayerIndex);
  Index = 0;
  #endif
  return Index;
}

/*********************************************************************
*
*       _ReadMPixels
*/
static void _ReadMPixels(int LayerIndex, U16 * pBuffer, U32 NumPixels) {
  #ifndef WIN32
  U16 Data;
  U16 Index;
  int State;

  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
  LCD_X_REG(0x2E);
  //
  // Dummy- read
  //
  LCD_X_Read_Data();
  for (State = 0; NumPixels; NumPixels--, State ^= 1) {
    switch (State) {
    case 0:
      Data = LCD_X_Read_Data();
      Index  =  Data & 0xf800;        // blue
      Index |= (Data & 0x00fc) <<  3; // green
      Data = LCD_X_Read_Data();
      Index |= (Data & 0xf800) >> 11; // red
      break;
    case 1:
      Index  = (Data & 0x00f8) <<  8; // blue
      Data = LCD_X_Read_Data();
      Index |= (Data & 0xfc00) >>  5; // green
      Index |= (Data & 0x00f8) >>  3; // red
      break;
    }
    *pBuffer++ = Index;
  }
  #else
  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pBuffer);
  GUI_USE_PARA(NumPixels);
  #endif
  return;
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
  GUI_DEVICE * pDevice;
  GUI_PORT_API PortAPI = {0};
  CONFIG_FLEXCOLOR Config = {0};

  //
  // Set display driver and color conversion for 1st layer
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
  if (DISPLAY_ORIENTATION & GUI_SWAP_XY) {
    LCD_SetSizeEx (0, YSIZE_PHYS,   XSIZE_PHYS);
    LCD_SetVSizeEx(0, VYSIZE_PHYS,  VXSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS,   YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS,  VYSIZE_PHYS);
  }
 
  //
  // Function selection, hardware routines (PortAPI) and operation mode (bus, bpp and cache)
  //

  #ifndef WIN32
  PortAPI.pfWrite16_A0  = LCD_X_REG;
  PortAPI.pfWrite16_A1  = LCD_X_DATA;
                         // PortAPI.pfWriteM16_A0 = LCD_X_Write_REG;
  PortAPI.pfWriteM16_A1 = LCD_X_Write_Data;
                         //  PortAPI.pfRead16_A0   = LCD_X_Read_REG;
  //PortAPI.pfRead16_A1   = LCD_X_Read_Data;
                         //  PortAPI.pfReadM16_A0  = LCD_X_Read_M_REG;
  PortAPI.pfReadM16_A1  = LCD_X_Read_M_DATA;
  #endif
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
  //
  // Orientation
  //
  Config.Orientation = DISPLAY_ORIENTATION;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set custom reading function(s)
  //
 // LCD_SetDevFunc(0, LCD_DEVFUNC_READPIXEL,   (void(*)(void))_ReadPixel);
 // LCD_SetDevFunc(0, LCD_DEVFUNC_READMPIXELS, (void(*)(void))_ReadMPixels);
 //GUIDRV_FlexColor_SetReadFunc66709_B16(pDevice, GUIDRV_FLEXCOLOR_READ_FUNC_III);
}
//
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
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;

  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pData);
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
   
    return 0;
  }
  case LCD_X_SETORG: {
   
    return 0;
  }
  case LCD_X_SHOWBUFFER: {
   
    return 0;
  }
  case LCD_X_SETLUTENTRY: {
   
    return 0;
  }
  case LCD_X_ON: {
   
    return 0;
  }
  case LCD_X_OFF: {
   
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
