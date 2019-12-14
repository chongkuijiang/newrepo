/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _READTEMP_H
#define _READTEMP_H

extern const unsigned short temp_AD[801];

unsigned short read_temper(void);
unsigned short Get_Temp(unsigned short vlotage);

#endif 