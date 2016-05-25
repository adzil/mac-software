#ifndef __MAC_MEMORY_H__
#define __MAC_MEMORY_H__

#include "mac-frame.h"
#include "mac-config.h"
#include "lock.h"

typedef struct MAC_MemFrameItem {
  MAC_Frame Frame;
  struct MAC_MemFrameItem *Next;
} MAC_MemFrameItem;

typedef struct {
  MAC_MemFrameItem *Free;
  LOCK_Handle Lock;
  MAC_MemFrameItem MemPool[MAC_CONFIG_MAX_FRAME_POOL];
} MAC_MemFrame;

typedef struct {
  MAC_MemFrame Pool;
} MAC_Mem;

void MAC_MemFrameInit(MAC_MemFrame *P);
MAC_Frame *MAC_MemFrameAlloc(MAC_MemFrame *P);
void MAC_MemFrameFree(MAC_MemFrame *P, MAC_Frame *F);

#endif // __MAC_MEMORY_H__