#ifndef __MAC_QUEUE_H__
#define __MAC_QUEUE_H__

/* MAC-Specific Queue functions */
#include "appdef.h"
#include "mac-frame.h"

/* Typedefs */
typedef struct {
  uint32_t ExtendedAdr;
  uint16_t ShortAdr;
} MAC_AdrList;

MAC_Frame *MAC_QueueFrameFind(QUE_Queue *H, MAC_FrameAddressMode AdrMode,
                              MAC_FrameAddress Address);
MAC_AdrList *MAC_QueueAdrListFind(QUE_Queue *H, MAC_FrameAddressMode AdrMode,
                                  MAC_FrameAddress Address);

force_inline MAC_Frame *MAC_QueueFrameAppend(QUE_Queue *H, MAC_Frame *F) {
  return (MAC_Frame *) QUE_QueueAppend(H, (void *) F);
}

force_inline MAC_Frame *MAC_QueueFramePush(QUE_Queue *H, MAC_Frame *F) {
  return (MAC_Frame *) QUE_QueuePush(H, (void *) F);
}

force_inline MAC_Frame *MAC_QueueFramePop(QUE_Queue *H) {
  return (MAC_Frame *) QUE_QueuePop(H);
}

force_inline MAC_AdrList *MAC_QueueAdrListAppend(QUE_Queue *H, MAC_AdrList *A) {
  return (MAC_AdrList *) QUE_QueueAppend(H, (void *) A);
}

#endif // __MAC_QUEUE_H__