#ifndef __MAC_QUEUE_H__
#define __MAC_QUEUE_H__

/* MAC-Specific Queue functions */
#include "queue.h"
#include "appdef.h"
#include "mac-frame.h"

force_inline void MAC_QueueAppend(QUE_Queue *H, MAC_Frame *F) {
  QUE_QueueAppend(H, (void *) F);
}

force_inline void MAC_QueuePush(QUE_Queue *H, MAC_Frame *F) {
  QUE_QueuePush(H, (void *) F);
}

force_inline MAC_Frame *MAC_QueuePop(QUE_Queue *H) {
  return (MAC_Frame *) QUE_QueuePop(H);
}

force_inline void MAC_ListPush(QUE_List *L, MAC_Frame *F) {
  QUE_ListPush(L, (void *) F);
}

force_inline MAC_Frame *MAC_ListPopShort(QUE_List *L, uint16_t ShortAdr) {
  MAC_Frame *F;

  QUE_ListPopInit(L);
  do {
    F = (MAC_Frame *) QUE_ListPop(L);
    if (!F) break;
    if (F->Address.Dst.Short == ShortAdr) break;
  } while(1);
  QUE_ListPopEnd(L);

  return F;
}

force_inline MAC_Frame *MAC_ListPopExtended(QUE_List *L, uint32_t ExtendedAdr) {
  MAC_Frame *F;

  QUE_ListPopInit(L);
  do {
    F = (MAC_Frame *) QUE_ListPop(L);
    if (!F) break;
    if (F->Address.Dst.Extended == ExtendedAdr) break;
  } while(1);
  QUE_ListPopEnd(L);

  return F;
}

#endif // __MAC_QUEUE_H__