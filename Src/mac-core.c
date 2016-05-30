#include <mac-instance.h>
#include <mac-pib.h>
#include <mac-memory.h>
#include <mac-frame.h>
#include "mac-core.h"

void MAC_CoreFrameReceived(MAC_Instance *H, uint8_t *Data, size_t Length) {
  MAC_Frame *F;

  // Abort frame decoding on zero-length
  if (!Length) return;

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

#ifdef MAC_DEBUG
  MAC_DebugFrame(H, F, MAC_DEBUG_RCV);
#endif

  MAC_MemFrameFree(&H->Mem, F);
}

void MAC_CoreFrameSend(MAC_Instance *H, uint8_t *Data, size_t *Len) {
  MAC_Frame *F;

  *Len = 0;
  F = MAC_TransmitGetFrame(H);
  if (F) {
    MAC_FrameEncode(F, Data, Len);
#ifdef MAC_DEBUG
    MAC_DebugFrame(H, F, MAC_DEBUG_SND);
#endif
    MAC_MemFrameFree(&H->Mem, F);
  }
}

MAC_Status MAC_CoreCheckAddressing(MAC_Instance *H, MAC_Frame *F) {
  MAC_FrameCommand C;

  // Check the destination addressing
  switch (F->FrameControl.DstAdrMode) {
    case MAC_ADRMODE_NOT_PRESENT:
      if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR) {
        // Unstated destination address only available on coordinator
        if (MAC_QueueAdrListFind(&H->Mem.Address, F->FrameControl.SrcAdrMode,
                                 F->Address.Src))
          // Check if the source already in the address list
          break;
      } else {
        if (F->FrameControl.FrameType == MAC_FRAMETYPE_COMMAND) {
          MAC_FrameCommandDecode(F, &C);
          if (C.CommandId == MAC_COMMAND_ID_DISCOVER_RESPONSE)
            break;
        }
      }
      return MAC_STATUS_INVALID_DESTINATION;

    case MAC_ADRMODE_SHORT:
      if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR) {
        // Coordinator side
        if (F->Address.Dst.Short == MAC_CONST_BROADCAST_ADDRESS)
          // Accept all broadcast address
          break;
        if (F->Address.Dst.Short != MAC_CONST_USE_EXTENDED_ADDRESS &&
            H->Pib.ShortAdr == F->Address.Dst.Short)
          // Accept matched short address
          break;
      } else {
        // Device side
        if (H->Pib.AssociatedCoord == MAC_PIB_ASSOCIATED_SET &&
            H->Pib.ShortAdr == F->Address.Dst.Short)
          // Accept when associated and matched short address
          break;
      }
      return MAC_STATUS_INVALID_DESTINATION;

    case MAC_ADRMODE_EXTENDED:
      if (F->Address.Dst.Extended == H->Config.ExtendedAddress)
        // Allow all matched extended addresses
        break;
      return MAC_STATUS_INVALID_DESTINATION;
  }

  // Check source addressing
  if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR) {
    // Check addressing on coordinator
    switch (F->FrameControl.SrcAdrMode) {
      case MAC_ADRMODE_NOT_PRESENT:
        if (F->FrameControl.FrameType == MAC_FRAMETYPE_COMMAND) {
          MAC_FrameCommandDecode(F, &C);
          if (C.CommandId == MAC_COMMAND_ID_DISCOVER_REQUEST)
            break;
        }
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
        if (H->Pib.AssociatedCoord == MAC_PIB_ASSOCIATED_SET)
          // Accept frame from coordinator
          break;

      case MAC_ADRMODE_SHORT:
        // Only receive short address from coordinator, if associated
        if (H->Pib.AssociatedCoord == MAC_PIB_ASSOCIATED_RESET)
          break;
        else
          if (H->Pib.CoordShortAdr != MAC_CONST_USE_EXTENDED_ADDRESS &&
              H->Pib.CoordShortAdr != MAC_CONST_BROADCAST_ADDRESS &&
              H->Pib.CoordShortAdr == F->Address.Src.Short)
            break;
        return MAC_STATUS_INVALID_SOURCE;

      case MAC_ADRMODE_EXTENDED:
        // Well, accept all extended address source
        break;
    }
  }

  return MAC_STATUS_OK;
}