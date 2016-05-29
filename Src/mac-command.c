#include <mac-frame.h>
#include "mac-command.h"

void MAC_CmdSetFrameDstFromSrc(MAC_Frame *F, MAC_Frame *SF) {
  // Copy addressing details from source frame
  MAC_SetFrameDstAdr(F, SF->FrameControl.SrcAdrMode, SF->Address.Src);
}

void MAC_CmdSetFrameHeader(MAC_Instance *H, MAC_Frame *F) {
  // Generate source addressing
  MAC_GenFrameSrcAdr(H, F);
  // Set frame controls
  MAC_SetFrameAckRequest(F);
  MAC_SetFrameNoPending(F);
  MAC_SetFrameType(F, MAC_FRAMETYPE_COMMAND);
}

void MAC_CmdAssocResponseSend(MAC_Instance *H, MAC_Frame *SF,
                              MAC_FrameAssocStatus Status, uint16_t ShortAdr) {
  MAC_Frame *F;
  MAC_FrameCommand C;

  F = MAC_MemFrameAlloc(&H->Mem);
  if (!F) return;
  MAC_CmdSetFrameDstFromSrc(F, SF);
  MAC_CmdSetFrameHeader(H, F);
  MAC_SetFrameCmdAssocResponse(&C, ShortAdr, Status);
  MAC_FrameCommandEncode(F, &C);

  MAC_TransmitPutFrame(H, F);
}

void MAC_CmdAssocRequestSend(MAC_Instance *H) {
  MAC_Frame *F;
  MAC_FrameCommand C;

  F = MAC_MemFrameAlloc(&H->Mem);
  if (!F) return;
  MAC_CmdSetFrameHeader(H, F);
  MAC_SetFrameNoDstAdr(F);
  MAC_SetFrameCmdAssocRequest(&C);
  MAC_FrameCommandEncode(F, &C);

  MAC_TransmitPutFrame(H, F);
}

void MAC_CmdDataRequestSend(MAC_Instance *H) {
  MAC_Frame *F;
  MAC_FrameCommand C;

  F = MAC_MemFrameAlloc(&H->Mem);
  if (!F) return;
  MAC_CmdSetFrameHeader(H, F);
  MAC_SetFrameNoDstAdr(F);
  MAC_SetFrameCmdDataRequest(&C);
  MAC_FrameCommandEncode(F, &C);

  MAC_TransmitPutFrame(H, F);
}

void MAC_CmdAssocRequestHandler(MAC_Instance *H, MAC_Frame *F) {
  MAC_AdrList *A;
  MAC_FrameAddress Adr;

  // Check for existing address on list
  A = MAC_QueueAdrListFind(&H->Mem.Address, MAC_ADRMODE_EXTENDED,
                           F->Address.Src);
  if (A) {
    Adr.Short = A->ShortAdr;
  } else {
    do {
      // Keep generating short address if exists on the list
      Adr.Short = (uint16_t) rand(); // TODO: Change to uC specific random
      A = MAC_QueueAdrListFind(&H->Mem.Address, MAC_ADRMODE_SHORT, Adr);
    } while (A);
  }

  A = MAC_MemAdrListAlloc(&H->Mem);
  if (!A) {
    MAC_CmdAssocResponseSend(H, F, MAC_ASSOCSTATUS_FAILED,
                              MAC_CONST_ADDRESS_UNKNOWN);
  } else {

    A->ExtendedAdr = F->Address.Src.Extended;
    A->ShortAdr = Adr.Short;
    A = MAC_QueueAdrListAppend(&H->Mem.Address, A);
    if (A) MAC_MemAdrListFree(&H->Mem, A);
    MAC_CmdAssocResponseSend(H, F, MAC_ASSOCSTATUS_SUCCESS, Adr.Short);
  }
}

void MAC_CmdAssocResponseHandler(MAC_Instance *H, MAC_Frame *F,
                                 MAC_FrameCommand *C) {
  if (C->AssocStatus == MAC_ASSOCSTATUS_SUCCESS) {
    // Set coordinator address
    if (F->FrameControl.SrcAdrMode == MAC_ADRMODE_SHORT) {
      H->Pib.CoordShortAdr = F->Address.Src.Short;
    } else if (F->FrameControl.SrcAdrMode == MAC_ADRMODE_EXTENDED) {
      H->Pib.CoordShortAdr = MAC_CONST_USE_EXTENDED_ADDRESS;
      H->Pib.CoordExtendedAdr = F->Address.Src.Extended;
    } else {
      return;
    }
    // Set PIB to associated
    H->Pib.AssociatedCoord = MAC_PIB_ASSOCIATED_SET;
    H->Pib.ShortAdr = C->ShortAddress;
  } else {
    H->Pib.AssociatedCoord = MAC_PIB_ASSOCIATED_RESET;
  }
}

void MAC_CmdDataRequestHandler(MAC_Instance *H, MAC_Frame *F) {
  MAC_TransmitLookupFrame(H, F->FrameControl.SrcAdrMode, F->Address.Src);
}

MAC_Status MAC_CmdFrameHandler(MAC_Instance *H, MAC_Frame *F) {
  MAC_FrameCommand C;

  // Decode command frame
  MAC_FrameCommandDecode(F, &C);
  switch (C.CommandId) {
    case MAC_COMMAND_ID_ASSOC_REQUEST:
      // All association request should come from extended address
      if (F->FrameControl.SrcAdrMode != MAC_ADRMODE_EXTENDED)
        return MAC_STATUS_INVALID_SOURCE;
      // All association request should be processed only by coordinator
      if (H->Pib.VpanCoordinator != MAC_PIB_VPAN_COORDINATOR)
        return MAC_STATUS_INVALID_COMMAND;
      MAC_CmdAssocRequestHandler(H, F);
      break;

    case MAC_COMMAND_ID_ASSOC_RESPONSE:
      // Only device should respond to association response
      if (H->Pib.VpanCoordinator == MAC_PIB_VPAN_COORDINATOR)
        return MAC_STATUS_INVALID_COMMAND;
      MAC_CmdAssocResponseHandler(H, F, &C);
      break;

    case MAC_COMMAND_ID_DATA_REQUEST:
      // Only coordinator should process data request
      if (H->Pib.VpanCoordinator != MAC_PIB_VPAN_COORDINATOR)
        return MAC_STATUS_INVALID_COMMAND;
      MAC_CmdDataRequestHandler(H, F);
      break;

    default:
      return MAC_STATUS_INVALID_COMMAND;
  }

  return MAC_STATUS_OK;
}