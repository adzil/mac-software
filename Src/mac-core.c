#include <mac-instance.h>
#include "mac-core.h"

void MAC_CoreFrameReceived(MAC_Instance *H, uint8_t *Data, size_t Length) {
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
      MAC_CmdFrameHandler(H, F);
      break;

    default:
      break;
  }

  MAC_MemFrameFree(&H->Mem, F);
}

size_t MAC_CoreFrameSend(MAC_Instance *H, uint8_t *Data) {
  MAC_Frame *F;
  size_t Length;

  Length = 0;
  F = MAC_TransmitGetFrame(H);
  if (F)
    MAC_FrameEncode(F, Data, &Length);

  MAC_MemFrameFree(&H->Mem, F);

  return Length;
}

MAC_Status MAC_CoreCheckAddressing(MAC_Instance *H, MAC_Frame *F) {
  // Check the destination addressing
  switch (F->FrameControl.DstAdrMode) {
    case MAC_ADRMODE_NOT_PRESENT:
      // This only available as coordinator
      if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR)
        break;
      return MAC_STATUS_INVALID_DESTINATION;

    case MAC_ADRMODE_SHORT:
      // This is only available when the client is associated
      if (H->Pib.AssociatedCoord == MAC_PIB_ASSOCIATED_SET ||
          H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR)
        if (F->Address.Dst.Short == H->Pib.ShortAdr)
          break;
      return MAC_STATUS_INVALID_DESTINATION;

    case MAC_ADRMODE_EXTENDED:
      if (F->Address.Dst.Extended == H->Config.ExtendedAddress)
        break;
      return MAC_STATUS_INVALID_DESTINATION;
  }

  // Check source addressing
  if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR) {
    // Check addressing on coordinator
    switch (F->FrameControl.SrcAdrMode) {
      case MAC_ADRMODE_NOT_PRESENT:
        // This is unacceptable
        return MAC_STATUS_INVALID_SOURCE;

      case MAC_ADRMODE_SHORT:
        if (MAC_QueueAdrListFind(&H->Mem.Address, MAC_ADRMODE_SHORT,
                                 F->Address.Src))
          break;
        return MAC_STATUS_INVALID_SOURCE;

      case MAC_ADRMODE_EXTENDED:
        // Well, accept all extended address source
        break;
    }
  } else {
    // Check addressing on device
    switch (F->FrameControl.SrcAdrMode) {
      case MAC_ADRMODE_NOT_PRESENT:
        // Accept frame from coordinator
        break;

      case MAC_ADRMODE_SHORT:
        // Only receive short address from coordinator, if associated
        if (H->Pib.AssociatedCoord == MAC_PIB_ASSOCIATED_RESET)
          break;
        else
          if (H->Pib.CoordShortAdr == F->Address.Src.Short)
            break;
        return MAC_STATUS_INVALID_SOURCE;

      case MAC_ADRMODE_EXTENDED:
        // Well, accept all extended address source
        break;
    }
  }

  return MAC_STATUS_OK;
}