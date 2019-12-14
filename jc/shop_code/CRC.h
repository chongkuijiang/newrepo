
#ifndef __CRC_H_
#define __CRC_H_
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif


typedef union
{
	uint8_t  Bit[2];
	uint16_t Dat;
}CRC16_t;


CRC16_t CreatCRC16( uint8_t * data, uint8_t len);

#ifdef __cplusplus
}
#endif


#endif

