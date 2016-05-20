#include <mac.h>

MAC_Status MAC_UpdatePayloadInfo(MAC_Frame *F, uint16_t Len) {
  F->PayloadInfo.Start = 2 + MAC_GetAddressSize(F->FrameControl.DstAdrMode) +
                         MAC_GetAddressSize(F->FrameControl.SrcAdrMode);
  if (F->PayloadInfo.Start > (Len - 2)) return MAC_STATUS_INVALID_PARAM;
  F->PayloadInfo.Length = Len - (F->PayloadInfo.Start + 2);
}

/* Decodes the data stream into structured frame */
MAC_Status MAC_DecodeFrame(MAC_Frame *F, uint8_t *Data, uint16_t Len) {
  // Get frame control
  MAC_ReadByte(&F->FrameControl, Data);
  // Update the payload info
  MAC_UpdatePayloadInfo(F, Len);
}