#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <inttypes.h>
#include <stddef.h>
#include <lock.h>

typedef struct QUE_Item {
  struct QUE_Item *Next;
  void *Data;
} QUE_Item;

typedef struct {
  QUE_Item *First;
  QUE_Item *Last;
  LOCK_Handle Lock;
} QUE_Head;

#endif // __QUEUE_H__