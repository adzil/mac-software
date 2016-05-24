#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <inttypes.h>
#include <stddef.h>
#include "lock.h"
#include "memory.h"
#include "appdef.h"

typedef struct QUE_Item {
  struct QUE_Item *Next;
  void *Data;
} QUE_Item;

typedef struct {
  QUE_Item *First;
  QUE_Item *Last;
  QUE_Item *Free;
  LOCK_Handle Lock;
} QUE_Queue;

typedef struct {
  QUE_Item *List;
  QUE_Item *Free;
  QUE_Item *Find;
  QUE_Item *FindLast;
  LOCK_Handle Lock;
} QUE_List;

#define QUE_CallMem(Name) QUE_Mem##Name
#define QUE_AllocMem(Name, Len) static QUE_Item QUE_CallMem(Name)[Len]

void QUE_QueueInit(QUE_Queue *H, QUE_Item *Mem, int Len);
void *QUE_QueuePush(QUE_Queue *H, void *Data);
void *QUE_QueueAppend(QUE_Queue *H, void *Data);
void *QUE_QueuePop(QUE_Queue *H);

void QUE_ListInit(QUE_List *L, QUE_Item *Mem, int Len);
void *QUE_ListPush(QUE_List *L, void *Data);

void QUE_ListPopInit(QUE_List *L);
void QUE_ListPopEnd(QUE_List *L);

/* List inline functions */
force_inline void *QUE_ListPop(QUE_List *L) {
  if (L->Find) {
    L->FindLast = L->Find;
    L->Find = L->Find->Next;
  } else {
    L->Find = L->List;
    L->FindLast = L->Find;
  }

  if (!L->Find) return NULL;
  return L->Find->Data;
}

#endif // __QUEUE_H__