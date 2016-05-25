#ifndef __MAC_FRAME_H__
#define __MAC_FRAME_H__

#include <inttypes.h>
#include "mac-common.h"
#include "mac-config.h"
#include "crc16.h"

/* Enumeration types */
typedef enum {
  MAC_ADRMODE_NOT_PRESENT = 0,
  MAC_ADRMODE_SHORT = 2,
  MAC_ADRMODE_EXTENDED = 3
} MAC_FrameAddressMode;

typedef enum {
  MAC_ACKREQUEST_RESET = 0,
  MAC_ACKREQUEST_SET = 1
} MAC_FrameAckRequest;

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
} MAC_FrameCommandId;

typedef enum {
  MAC_ASSOCSTATUS_SUCCESS = 0,
  MAC_ASSOCSTATUS_FAILED = 1
} MAC_FrameAssocStatus;

/* Address typedef */
typedef union {
  uint16_t Short;
  uint32_t Extended;
} MAC_FrameAddress;

typedef struct {
  MAC_FrameAddress Dst;
  MAC_FrameAddress Src;
} MAC_FrameAddressField;

/* Payload typedef */
typedef struct {
  MAC_FrameCommandId CommandId;
  MAC_FrameAssocStatus AssocStatus;
  uint16_t ShortAddress;
} MAC_FramePayloadCommand;

typedef struct {
  uint8_t Data[MAC_CONFIG_MAX_PAYLOAD_LENGTH];
  uint16_t Start;
  uint16_t Length;
} MAC_FramePayload;

/* Frame control type */
typedef struct {
  MAC_FrameAddressMode DstAdrMode : 2;
  MAC_FrameAddressMode SrcAdrMode : 2;
  MAC_FrameAckRequest AckRequest : 1;
  MAC_FramePending FramePending : 1;
  MAC_FrameType FrameType : 2;
} MAC_FrameControl;

/* General MAC Frame structure */
typedef struct {
  MAC_FrameControl FrameControl;
  MAC_FrameAddressField Address;
  MAC_FramePayload Payload;
  uint8_t Sequence;
} MAC_Frame;

/* Getter utility */
static uint16_t MAC_FrameGetAddressSize(MAC_FrameAddressMode Mode) {
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

MAC_Status MAC_FrameEncode(MAC_Frame *F, uint8_t *Data);
MAC_Status MAC_FrameDecode(MAC_Frame *F, uint8_t *Data, uint16_t Len);
uint16_t MAC_FrameGetSize(MAC_Frame *F);

#endif // __MAC_FRAME_H__