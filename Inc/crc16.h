/**
 * CRC-16 / CRC-CCITT Software Implementation
 */

#ifndef __CRC16_H__
#define __CRC16_H__

#include <inttypes.h>

uint16_t CRC_Checksum(uint8_t *Data, uint16_t DataLen);

#endif //__CRC16_H__
