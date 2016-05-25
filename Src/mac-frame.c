#include "mac-frame.h"

void MAC_UpdatePayloadStart(MAC_Frame *F) {
  // 1byte FrameControl + 1byte Sequence + n bytes SrcAdr + n bytes DstAdr
  F->Payload.Start = 2 + MAC_FrameGetAddressSize(F->FrameControl.DstAdrMode) +
                         MAC_FrameGetAddressSize(F->FrameControl.SrcAdrMode);
}

uint16_t MAC_FrameGetSize(MAC_Frame *F) {
  // Get start size
  MAC_UpdatePayloadStart(F);
  // 2 bytes FCS + start + length
  return 2 + F->Payload.Start + F->Payload.Length;
}

/* Encode the structured frame into data stream */
MAC_Status MAC_FrameEncode(MAC_Frame *F, uint8_t *Data) {
  uint8_t *DataPtr;
  uint16_t Checksum;

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
MAC_Status MAC_FrameDecode(MAC_Frame *F, uint8_t *Data, uint16_t Len) {
  // Check the checksum first before process the data
  if (CRC_Checksum(Data, Len)) return MAC_STATUS_INVALID_CHECKSUM;
  // Get frame control and byte sequence
  MAC_ReadByte(&F->FrameControl, Data);
  MAC_ReadByte(&F->Sequence, Data);

  // Update the payload info
  MAC_UpdatePayloadStart(F);
  // Check for length issues
  if (F->Payload.Start + 2 > Len) return MAC_STATUS_INVALID_LENGTH;
  // Calculate the payload length (Total - Start - 2byte FCS)
  F->Payload.Length = Len - F->Payload.Start - 2;

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