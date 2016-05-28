#ifndef __MAC_HANDLE_H__
#define __MAC_HANDLE_H__

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

void MAC_Init(MAC_Instance *H);
void MAC_TransmitPutFrame(MAC_Instance *H, MAC_Frame *F);
void MAC_GenFrameSrcAdr(MAC_Instance *H, MAC_Frame *F);

force_inline void MAC_TransmitSendFrame(MAC_Instance *H, MAC_Frame *F) {
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

/*
force_inline MAC_Frame *MAC_HandlePopQueuedFrame(MAC_Instance *H) {

}*/

#endif // __MAC_HANDLE_H__