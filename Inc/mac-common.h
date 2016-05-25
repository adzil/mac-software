#ifndef __MAC_COMMON_H__
#define __MAC_COMMON_H__

#include <string.h>
#include "appdef.h"

/* Enumeration types */
typedef enum {
  MAC_STATUS_OK,
  MAC_STATUS_INVALID_PARAM,
  MAC_STATUS_INVALID_CHECKSUM,
  MAC_STATUS_INVALID_LENGTH,
  MAC_STATUS_MEMORY_ERROR
} MAC_Status;

/* Copy utility */
#define MAC_ReadByte(Dst, Src) do { \
*((uint8_t *) Dst) = *((uint8_t *) Src); Src += 1; } while(0)
#define MAC_ReadWord(Dst, Src) do { \
*((uint16_t *) Dst) = __REV16(*((uint16_t *) Src)); Src += 2; } while(0)
#define MAC_ReadDword(Dst, Src) do { \
*((uint32_t *) Dst) = __REV(*((uint32_t *) Src)); Src += 4; } while(0)
#define MAC_ReadStream(Dst, Src, Len) do {\
memcpy(Dst, Src, Len); Src += Len; } while(0)

#define MAC_WriteByte(Dst, Src) do { \
*((uint8_t *) Dst) = *((uint8_t *) Src); Dst += 1; } while(0)
#define MAC_WriteWord(Dst, Src) do { \
*((uint16_t *) Dst) = __REV16(*((uint16_t *) Src)); Dst += 2; } while(0)
#define MAC_WriteDword(Dst, Src) do { \
*((uint32_t *) Dst) = __REV(*((uint32_t *) Src)); Dst += 4; } while(0)
#define MAC_WriteStream(Dst, Src, Len) do {\
memcpy(Dst, Src, Len); Dst += Len; } while(0)

#endif // __MAC_COMMON_H__