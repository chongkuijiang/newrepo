#ifndef _FLASH_C
#define _FLASH_C
#include "stm32f10x_lib.h"
#include "Flash.h"

void flash_save(unsigned short *data)
{
  unsigned short address;
  unsigned short ad2;
  ad2=0;
  FLASH_Unlock();
  
  FLASH_ErasePage(0x0803fe00);
  //FLASH_ErasePage(0x0801fa00);

  for(address = 0;address < 100;address +=2)
  {
    FLASH_ProgramHalfWord( (0x0803fe00 + address), data[ad2]);
    ad2++;
  }
  FLASH_Lock();

}

void flash_load(unsigned short *data)
{
  unsigned short address;
  unsigned short *flash_program;
  
  flash_program = (unsigned short *)(0x0803fe00);
  
  //从FLASH中读取程序
  for(address = 0;address < 100;address ++)//0---1468
  {
    data[address]  = flash_program[address];
  }
  
}

#endif /* _FLASH_C */