
#ifndef __CRC_H_
#define __CRC_H_

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include <stdint.h>
typedef union
{
	uint8_t  Bit[2];
	uint16_t Dat;
}uint16_8_t;

uint16_8_t CreatCRC16Value(uint8_t *data, uint8_t len);
#ifdef __cplusplus
}
#endif


#endif

