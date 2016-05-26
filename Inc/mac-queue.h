#ifndef __MAC_QUEUE_H__
#define __MAC_QUEUE_H__

/* MAC-Specific Queue functions */
#include "queue.h"
#include "appdef.h"
#include "mac-frame.h"

MAC_Frame *MAC_QueueFrameFind(QUE_Queue *H, MAC_FrameAddressMode AdrMode,
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



#endif // __MAC_QUEUE_H__