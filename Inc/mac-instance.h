#ifndef __MAC_HANDLE_H__
#define __MAC_HANDLE_H__

#include <stdlib.h>
/* Include all mac sub-functions */
#include "mac-common.h"
#include "mac-config.h"
#include "mac-frame.h"
#include "mac-memory.h"
#include "mac-queue.h"
#include "mac-pib.h"

// MAC Handle for single MAC instance
typedef struct {
  MAC_Pib Pib;
  MAC_Config Config;
  // The memory part should be on the bottom
  MAC_Mem Mem;
} MAC_Instance;

#define MAC_DEBUG

#ifdef MAC_DEBUG
typedef enum {
  MAC_DEBUG_RCV,
  MAC_DEBUG_SND,
  MAC_DEBUG_QUE,
  MAC_DEBUG_RDY
} MAC_DebugDir;

#include <stdio.h>
force_inline void MAC_DebugStamp(MAC_Instance *H) {
  printf("(%x/%x) ", H->Config.ExtendedAddress, H->Pib.ShortAdr);
}

force_inline void MAC_DebugFrame(MAC_Instance *H, MAC_Frame *F,
                                 MAC_DebugDir Dir) {
  MAC_FrameCommand C;
  int i;

  MAC_DebugStamp(H);
  printf("FRAME ");
  switch (Dir) {
    case MAC_DEBUG_RCV:
      printf("RCV");
      break;

    case MAC_DEBUG_SND:
      printf("SND");
      break;

    case MAC_DEBUG_QUE:
      printf("QUE");
      break;

    case MAC_DEBUG_RDY:
      printf("RDY");
      break;
  }
  printf(" - ADDRESS: ");
  switch (F->FrameControl.SrcAdrMode) {
    case MAC_ADRMODE_NOT_PRESENT:
      printf("[NA]");
      break;

    case MAC_ADRMODE_SHORT:
      printf("[S:%x]", F->Address.Src.Short);
      break;

    case MAC_ADRMODE_EXTENDED:
      printf("[X:%x]", F->Address.Src.Extended);
      break;
  }
  printf("-->");
  switch (F->FrameControl.DstAdrMode) {
    case MAC_ADRMODE_NOT_PRESENT:
      printf("[NA]");
      break;

    case MAC_ADRMODE_SHORT:
      printf("[S:%x]", F->Address.Dst.Short);
      break;

    case MAC_ADRMODE_EXTENDED:
      printf("[X:%x]", F->Address.Dst.Extended);
      break;
  }

  printf(" SEQN: %d TYPE: ", F->Sequence);
  switch(F->FrameControl.FrameType) {
    case MAC_FRAMETYPE_DATA:
      printf("Data PAYLOAD: ");
      for (i = 0; i < F->Payload.Length; i++)
        printf("%2x ", F->Payload.Data[i]);
      break;

    case MAC_FRAMETYPE_COMMAND:
      printf("Command PAYLOAD: ");
      MAC_FrameCommandDecode(F, &C);
      switch (C.CommandId) {
        case MAC_COMMAND_ID_ASSOC_REQUEST:
          printf("AssocRequest");
          break;

        case MAC_COMMAND_ID_ASSOC_RESPONSE:
          printf("AssocResponse MSG: ");
          switch (C.AssocStatus) {
            case MAC_ASSOCSTATUS_SUCCESS:
              printf("SUCCESS, SHORT ADR: %x", C.ShortAddress);
              break;

            case MAC_ASSOCSTATUS_FAILED:
              printf("FAILED");
              break;
          }
          break;

        case MAC_COMMAND_ID_DATA_REQUEST:
          printf("DataRequest");
          break;
      }
      break;

    case MAC_FRAMETYPE_ACK:
      printf("Ack");
      break;
  }

  printf("\n");
}
#endif

void MAC_Init(MAC_Instance *H, uint32_t ExtendedAdr,
              MAC_PibVpanCoordinator VpanCoord);
void MAC_TransmitPutFrame(MAC_Instance *H, MAC_Frame *F);
void MAC_GenFrameSrcAdr(MAC_Instance *H, MAC_Frame *F);

force_inline void MAC_TransmitSendFrame(MAC_Instance *H, MAC_Frame *F) {
#ifdef MAC_DEBUG
  MAC_DebugFrame(H, F, MAC_DEBUG_RDY);
#endif
  F = MAC_QueueFrameAppend(&H->Mem.Tx, F);
  if (F) MAC_MemFrameFree(&H->Mem, F);
}

force_inline void MAC_TransmitLookupFrame(MAC_Instance *H,
                                          MAC_FrameAddressMode AdrMode,
                                          MAC_FrameAddress Adr) {
  MAC_Frame *F;

  F = MAC_QueueFrameFind(&H->Mem.Store, AdrMode, Adr);
  if (F) {
    MAC_TransmitSendFrame(H, F);
  }
}

force_inline MAC_Frame *MAC_TransmitGetFrame(MAC_Instance *H) {
  return MAC_QueueFramePop(&H->Mem.Tx);
}

force_inline void MAC_GenFrameSequence(MAC_Instance *H, MAC_Frame *F) {
  MAC_SetFrameSequence(F, H->Pib.DSN++);
}

#endif // __MAC_HANDLE_H__