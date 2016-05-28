#include "mac-handle.h"

void MAC_HandleInit(MAC_Handle *H) {
  // Memory initialization
  MAC_MemPoolInit(&H->Mem);
  MAC_MemQueueInit(&H->Mem);
}

void MAC_HandleQueueFrame(MAC_Handle *H, MAC_Frame *F) {
  if (F->FrameControl.FrameType != MAC_FRAMETYPE_ACK) {
    MAC_QueueFramePush(&H->Mem.Tx, F);
  } else if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR) {
    MAC_QueueFrameAppend(&H->Mem.Store, F);
  } else {
    MAC_QueueFrameAppend(&H->Mem.Tx, F);
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
