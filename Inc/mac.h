#ifndef __MAC_H__
#define __MAC_H__

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

/* Enumeration types */
typedef enum {
  MAC_STATUS_OK,
  MAC_STATUS_INVALID_PARAM,
  MAC_STATUS_INVALID_CHECKSUM,
  MAC_STATUS_INVALID_LENGTH,
  MAC_STATUS_MEMORY_ERROR
} MAC_Status;

typedef enum {
  MAC_ADRMODE_NOT_PRESENT = 0,
  MAC_ADRMODE_SHORT = 2,
  MAC_ADRMODE_EXTENDED = 3
} MAC_AddressMode;

typedef enum {
  MAC_ACKREQUEST_RESET = 0,
  MAC_ACKREQUEST_SET = 1
} MAC_AckRequest;

typedef enum {
  MAC_FRAMEPENDING_RESET = 0,
  MAC_FRAMEPENDING_SET = 1
} MAC_FramePending;

typedef enum {
  MAC_FRAMETYPE_DATA = 1,
  MAC_FRAMETYPE_ACK = 2,
  MAC_FRAMETYPE_COMMAND = 3
} MAC_FrameType;

typedef enum {
  MAC_COMMAND_ID_ASSOC_REQUEST = 0,
  MAC_COMMAND_ID_ASSOC_RESPONSE = 1,
  MAC_COMMAND_ID_DATA_REQUEST = 4
} MAC_CommandId;

typedef enum {
  MAC_ASSOCSTATUS_SUCCESS = 0,
  MAC_ASSOCSTATUS_FAILED = 1
} MAC_AssocStatus;

/* Address typedef */
typedef union {
  uint16_t Short;
  uint32_t Extended;
} MAC_Address;

typedef struct {
  MAC_Address Dst;
  MAC_Address Src;
} MAC_AddressField;

/* Payload typedef */
typedef struct {
  MAC_CommandId CommandId;
  MAC_AssocStatus AssocStatus;
  uint16_t ShortAddress;
} MAC_PayloadCommand;

typedef union {
  uint8_t *Data;
  uint16_t Start;
  uint16_t Length;
} MAC_Payload;

/* Frame control type */
typedef struct {
  MAC_AddressMode DstAdrMode : 2;
  MAC_AddressMode SrcAdrMode : 2;
  MAC_AckRequest AckRequest : 1;
  MAC_FramePending FramePending : 1;
  MAC_FrameType FrameType : 2;
} MAC_FrameControl;

/* General MAC Frame structure */
typedef struct {
  MAC_FrameControl FrameControl;
  MAC_AddressField Address;
  MAC_Payload Payload;
  uint8_t Sequence;
} MAC_Frame;

/* Reverse utility */
static uint32_t __REV(uint32_t DIN) {
  return (DIN >> 24) | ((DIN & 0xff0000) >> 8) | ((DIN & 0xff00) << 8) |
         (DIN << 24);
}

static uint32_t __REV16(uint32_t DIN) {
  return ((DIN & 0xff00) >> 8) | ((DIN & 0xff) << 8);
}

/* Getter utility */
static uint16_t MAC_GetAddressSize(MAC_AddressMode Mode) {
  switch (Mode) {
    case MAC_ADRMODE_NOT_PRESENT:
      return 0;
    case MAC_ADRMODE_SHORT:
      return 2;
    case MAC_ADRMODE_EXTENDED:
      return 4;
    default:
      return 0;
  }
}

/* Memory utility */
static void *MEM_Alloc(size_t Size) {
  return malloc(Size);
}

static void MEM_Free(void *Ptr) {
  return free(Ptr);
}

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

MAC_Status MAC_EncodeFrame(MAC_Frame *F, uint8_t *Data);
MAC_Status MAC_DecodeFrame(MAC_Frame *F, uint8_t *Data, uint16_t Len);
uint16_t MAC_GetFrameSize(MAC_Frame *F);

#endif // __MAC_H__