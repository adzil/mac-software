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
} MAC_Handle;

void MAC_HandleInit(MAC_Handle *H);
void MAC_HandleQueueFrame(MAC_Handle *H, MAC_Frame *F);
void MAC_GenFrameSrcAdr(MAC_Handle *H, MAC_Frame *F);

force_inline void MAC_HandleDirectQueueFrame(MAC_Handle *H, MAC_Frame *F) {
  MAC_QueueFrameAppend(&H->Mem.Tx, F);
}

force_inline void MAC_GenFrameSequence(MAC_Handle *H, MAC_Frame *F) {
  MAC_SetFrameSequence(F, H->Pib.DSN++);
}

force_inline MAC_Frame *MAC_HandlePopQueuedFrame(MAC_Handle *H) {

}

#endif // __MAC_HANDLE_H__