#ifndef __MAC_MEMORY_H__
#define __MAC_MEMORY_H__

#include <stddef.h>
#include "mac-frame.h"
#include "mac-config.h"
#include "lock.h"

#define MAC_MemCreatePool(Name, Item, Len) \
  uint8_t Name[(sizeof(Item) + sizeof(void *)) * Len]
#define MAC_MemItemSize(Item) (sizeof(Item) + sizeof(void *))

/*typedef struct MAC_MemAdrListItem{
  struct MAC_MemAdrListItem *Next;
  uint32_t ExtendedAdr;
  uint16_t ShortAdr;
} MAC_MemAdrListItem;

typedef struct MAC_MemFrameItem {
  struct MAC_MemFrameItem *Next;
  MAC_Frame Frame;
} MAC_MemFrameItem;*/

typedef struct {
  uint32_t ExtendedAdr;
  uint16_t ShortAdr;
} MAC_MemAdrList;

typedef struct {
  MAC_MemCreatePool(Frame, MAC_Frame, MAC_CONFIG_MAX_FRAME_POOL);
  MAC_MemCreatePool(AdrList, MAC_MemAdrList, MAC_CONFIG_MAX_ADDRESS_LIST);
  //MAC_MemFrameItem Frame[MAC_CONFIG_MAX_FRAME_POOL];
  //MAC_MemAdrListItem AdrList[MAC_CONFIG_MAX_ADDRESS_LIST];
} MAC_MemPool;

typedef struct {
  void *Free;
  LOCK_Handle Lock;
} MAC_MemHandle;

typedef struct {
  MAC_MemHandle Frame;
  MAC_MemHandle AdrList;
  MAC_MemPool Pool;
} MAC_Mem;

void MAC_MemInit(MAC_MemHandle *H, void *I, size_t Size, int Length);
void *MAC_MemAlloc(MAC_MemHandle *H);
void MAC_MemFree(MAC_MemHandle *H, void *I);
void MAC_MemAllInit(MAC_Mem *M);

/*typedef struct {
  MAC_MemFrameItem *Free;
  LOCK_Handle Lock;
  MAC_MemFrameItem Pool[MAC_CONFIG_MAX_FRAME_POOL];
} MAC_MemFrame;

typedef struct {
  MAC_MemAdrListItem Pool[MAC_CONFIG_MAX_ADDRESS_LIST];
} MAC_MemAdrList;

typedef struct {
  MAC_MemFrame Frame;
  MAC_MemAdrList AdrList;
} MAC_Mem;

void MAC_MemFrameInit(MAC_MemFrame *P);
MAC_Frame *MAC_MemFrameAlloc(MAC_MemFrame *P);
void MAC_MemFrameFree(MAC_MemFrame *P, MAC_Frame *F);*/

#endif // __MAC_MEMORY_H__