#include "mac-queue.h"

MAC_Frame *MAC_QueueFrameFind(QUE_Queue *H, MAC_FrameAddressMode AdrMode,
                              MAC_FrameAddress Address) {
  MAC_Frame *F;

  if (AdrMode != MAC_ADRMODE_EXTENDED && AdrMode != MAC_ADRMODE_SHORT)
    return NULL;

  LOCK_Start(&H->Lock);
  QUE_QueueFindInit(H);

  do {
    F = (MAC_Frame *) QUE_QueueFind(H);
    if (!F) break;
    if (F->FrameControl.DstAdrMode == AdrMode) {
      if (AdrMode == MAC_ADRMODE_SHORT) {
        if (F->Address.Dst.Short == Address.Short) break;
      } else {
        if (F->Address.Dst.Extended == Address.Extended) break;
      }
    }
  } while(1);

  QUE_QueueFindEnd(H);
  LOCK_End(&H->Lock);

  return F;
}