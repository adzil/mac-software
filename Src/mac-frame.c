#include <mac-frame.h>
#include <mac-common.h>
#include <mac-pib.h>
#include <mac-config.h>
#include "mac-frame.h"

/* Encode the structured frame into data stream */
MAC_Status MAC_FrameEncode(MAC_Frame *F, uint8_t *Data) {
  uint8_t *DataPtr;
  uint16_t Checksum;

  // Check for data length
  if (F->Payload.Length > MAC_CONFIG_MAX_PAYLOAD_LENGTH)
    return MAC_STATUS_INVALID_LENGTH;

  // Store the data pointer
  DataPtr = Data;
  // Copy the frame control and sequence
  MAC_WriteByte(Data, &F->FrameControl);
  MAC_WriteByte(Data, &F->Sequence);
  // Copy the address
  switch (F->FrameControl.DstAdrMode) {
    case MAC_ADRMODE_SHORT:
      MAC_WriteWord(Data, &F->Address.Dst.Short);
      break;
    case MAC_ADRMODE_EXTENDED:
      MAC_WriteWord(Data, &F->Address.Dst.Extended);
      break;
    default:
      break;
  }
  switch (F->FrameControl.SrcAdrMode) {
    case MAC_ADRMODE_SHORT:
      MAC_WriteWord(Data, &F->Address.Src.Short);
      break;
    case MAC_ADRMODE_EXTENDED:
      MAC_WriteWord(Data, &F->Address.Src.Extended);
      break;
    default:
      break;
  }

  // Copy the payload data, if available
  if (F->Payload.Length > 0) {
    MAC_WriteStream(Data, F->Payload.Data, F->Payload.Length);
  }

  // Calculate checksum
  Checksum = CRC_Checksum(DataPtr, (size_t)(Data - DataPtr));
  MAC_WriteWord(Data, &Checksum);

  return MAC_STATUS_OK;
}

/* Decodes the data stream into structured frame */
MAC_Status MAC_FrameDecode(MAC_Frame *F, uint8_t *Data, size_t Len) {
  size_t StartLen;

  // Check for frame buffer length
  if (Len > MAC_CONFIG_MAX_FRAME_BUFFER)
    return MAC_STATUS_INVALID_LENGTH;
  // Check the checksum first before process the data
  if (CRC_Checksum(Data, Len)) return MAC_STATUS_INVALID_CHECKSUM;
  // Get frame control and byte sequence
  MAC_ReadByte(&F->FrameControl, Data);
  MAC_ReadByte(&F->Sequence, Data);

  // Update the payload info
  StartLen = MAC_GetFrameAddressSize(F->FrameControl.DstAdrMode) +
             MAC_GetFrameAddressSize(F->FrameControl.SrcAdrMode) + 2;
  // Check for length issues
  if (StartLen + 2 > Len) return MAC_STATUS_INVALID_LENGTH;
  // Calculate the payload length (Total - Start - 2byte FCS)
  F->Payload.Length = Len - StartLen - 2;

  // Get the address
  switch (F->FrameControl.DstAdrMode) {
    case MAC_ADRMODE_SHORT:
      MAC_ReadWord(&F->Address.Dst.Short, Data);
      break;
    case MAC_ADRMODE_EXTENDED:
      MAC_ReadDword(&F->Address.Dst.Extended, Data);
      break;
    default:
      break;
  }
  switch (F->FrameControl.SrcAdrMode) {
    case MAC_ADRMODE_SHORT:
      MAC_ReadWord(&F->Address.Src.Short, Data);
      break;
    case MAC_ADRMODE_EXTENDED:
      MAC_ReadDword(&F->Address.Src.Extended, Data);
      break;
    default:
      break;
  }

  // Get the data (if there is data available)
  if (F->Payload.Length > 0) {
    // Copy the payload contents
    MAC_ReadStream(F->Payload.Data, Data, F->Payload.Length);
  }

  return MAC_STATUS_OK;
}

void MAC_FrameCommandEncode(MAC_Frame *F, MAC_FrameCommand *C) {
  uint8_t *Data;

  // Set data pointer
  Data = F->Payload.Data;

  // Get the command ID
  MAC_WriteByte(Data, &C->CommandId);
  F->Payload.Length = 1;
  // Check for command ID
  if (C->CommandId == MAC_COMMAND_ID_ASSOC_RESPONSE) {
    MAC_WriteDword(Data, &C->ShortAddress);
    MAC_WriteByte(Data, &C->AssocStatus);
    F->Payload.Length = 4;
  }
}

void MAC_FrameCommandDecode(MAC_Frame *F, MAC_FrameCommand *C) {
  uint8_t *Data;

  // Set data pointer
  Data = F->Payload.Data;

  // Get the command ID
  MAC_ReadByte(&C->CommandId, Data);
  // Check for command ID
  if (C->CommandId == MAC_COMMAND_ID_ASSOC_RESPONSE) {
    MAC_ReadDword(&C->ShortAddress, Data);
    MAC_ReadByte(&C->AssocStatus, Data);
  }
}

void MAC_GenFrameSrcAdr(MAC_Handle *H, MAC_Frame *F) {
  if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR) {
    if (F->FrameControl.DstAdrMode == MAC_ADRMODE_SHORT)
      // No source address for short destination addresses
      MAC_SetFrameNoSrcAdr(F);
    else if (H->Pib.ShortAdr != MAC_CONST_USE_EXTENDED_ADDRESS &&
             H->Pib.ShortAdr != MAC_CONST_ADDRESS_UNKNOWN)
      // Use short address if already defined
      MAC_SetFrameShortSrcAdr(F, H->Pib.ShortAdr);
    else
      // Fallback using extended address
      MAC_SetFrameExtendedSrcAdr(F, H->Config.ExtendedAddress);
  } else {
    if (H->Pib.AssociatedCoord == MAC_PIB_ASSOCIATED_SET &&
        H->Pib.ShortAdr != MAC_CONST_USE_EXTENDED_ADDRESS &&
        H->Pib.ShortAdr != MAC_CONST_ADDRESS_UNKNOWN)
      // If associated and address assigned, use it
      MAC_SetFrameShortSrcAdr(F, H->Pib.ShortAdr);
    else
      // Fallback using extended address
      MAC_SetFrameExtendedSrcAdr(F, H->Config.ExtendedAddress);
  }
}

void MAC_GenFrameSequence(MAC_Handle *H, MAC_Frame *F) {
  MAC_SetFrameSequence(F, H->Pib.DSN++);
}