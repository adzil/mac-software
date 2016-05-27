#include "mac.h"

void MAC_AppInit(MAC_Handle *H) {
  // Memory initialization
  MAC_MemPoolInit(&H->Mem);
  MAC_MemQueueInit(&H->Mem);
}

void MAC_AppFrameReceived(MAC_Handle *H, uint8_t *Data, size_t Length) {
  MAC_Frame *F;

  // Allocate new frame
  F = MAC_MemFrameAlloc(&H->Mem);
  if (!F) return;

  // Convert received data to frame structure
  if (MAC_FrameDecode(F, Data, Length) != MAC_STATUS_OK) {
    MAC_MemFrameFree(&H->Mem, F);
    return;
  }
  // Check if the frame is acknowledgement
  // TODO: BACK-ACK check
  // Check frame addressing
  if (MAC_CoreCheckAddressing(H, F) != MAC_STATUS_OK) {
    MAC_MemFrameFree(&H->Mem, F);
    return;
  }

  // Process the packet
  switch (F->FrameControl.FrameType) {
    case MAC_FRAMETYPE_DATA:
      // TODO: Pass data to higher layer
      break;

    case MAC_FRAMETYPE_COMMAND:
      MAC_CmdExecute(H, F);
      break;

    default:
      break;
  }

  MAC_MemFrameFree(&H->Mem, F);
}
