#include "mac-core.h"

/**
 *  This is the core function of the MAC such as data checking
 */

MAC_Status MAC_CoreCheckAddressing(MAC_Handle *H, MAC_Frame *F) {
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
      if (F->Address.Dst.Extended == MAC_CONFIG_EXTENDED_ADDRESS)
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