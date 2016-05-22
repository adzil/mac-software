#include <mac.h>
#include <crc16.h>

MAC_Frame *MAC_CreateFrame(void) {
  // Create the frame buffer
  return MEM_Alloc(sizeof(MAC_Frame));
}

MAC_Status MAC_CreatePayload(MAC_Frame *F, uint16_t Len) {
  F->Payload.Length = Len;
  F->Payload.Data = MEM_Alloc(Len);
  if (!F->Payload.Data) return MAC_STATUS_MEMORY_ERROR;
  return MAC_STATUS_OK;
}

void MAC_DestroyFrame(MAC_Frame *F) {
  // Destroy the payload buffer, if available
  if (F->Payload.Length > 0) {
    MEM_Free(F->Payload.Data);
  }
  // Destroy the frame buffer
  MEM_Free(F);
}

void MAC_UpdatePayloadStart(MAC_Frame *F) {
  // 1byte FrameControl + 1byte Sequence + n bytes SrcAdr + n bytes DstAdr
  F->Payload.Start = 2 + MAC_GetAddressSize(F->FrameControl.DstAdrMode) +
                         MAC_GetAddressSize(F->FrameControl.SrcAdrMode);
}

uint16_t MAC_GetFrameSize(MAC_Frame *F) {
  // Get start size
  MAC_UpdatePayloadStart(F);
  // 2 bytes FCS + start + length
  return 2 + F->Payload.Start + F->Payload.Length;
}

/* Encode the structured frame into data stream */
MAC_Status MAC_EncodeFrame(MAC_Frame *F, uint8_t *Data) {
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
MAC_Status MAC_DecodeFrame(MAC_Frame *F, uint8_t *Data, uint16_t Len) {
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
    // Allocate memory for payload
    if (MAC_CreatePayload(F, F->Payload.Length) != MAC_STATUS_OK)
      return MAC_STATUS_MEMORY_ERROR;
    // Copy the payload contents
    MAC_ReadStream(F->Payload.Data, Data, F->Payload.Length);
  }

  return MAC_STATUS_OK;
}
