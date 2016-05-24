#include <queue.h>
#include <mac.h>

void QUE_Init(QUE_Head *H) {
  H->First = NULL;
  H->Last = NULL;
  LOCK_End(&H->Lock);
}

void QUE_Append(QUE_Head *H, void *Data) {
  QUE_Item *I;

  // Create new queue item
  I = MEM_Alloc(sizeof(QUE_Item));
  if (!I) return;
  I->Data = Data;
  I->Next = NULL;

  LOCK_Start(&H->Lock);

  if (H->First == NULL) {
    // Queue is empty
    H->First = I;
    H->Last = I;
  } else {
    // Queue is not empty
    H->Last->Next = I;
    H->Last = I;
  }

  LOCK_End(&H->Lock);
}

void QUE_Push(QUE_Head *H, void *Data) {
  QUE_Item *I;

  // Create new queue item
  I = MEM_Alloc(sizeof(QUE_Item));
  if (!I) return;
  I->Data = Data;
  I->Next = NULL;

  LOCK_Start(&H->Lock);

  if (H->First == NULL) {
    // Queue is empty
    H->First = I;
    H->Last = I;
  } else {
    // Queue is not empty
    I->Next = H->First;
    H->First = I;
  }

  LOCK_End(&H->Lock);
}

void *QUE_Pop(QUE_Head *H) {
  QUE_Item *I;
  void *Data;

  if (!H->First) return NULL;

  LOCK_Start(&H->Lock);

  I = H->First;
  H->First = I->Next;

  LOCK_End(&H->Lock);

  Data = I->Data;
  MEM_Free(I);

  return Data;
}

void *QUE_Reset(QUE_Head *H) {
  QUE_Item *I;

  if (!H->First) return NULL;

  LOCK_Start(&H->Lock);

  while (H->First) {
    I = H->First;
    H->First = I->Next;
    MEM_Free(I->Data);
    MEM_Free(I);
  }

  LOCK_End(&H->Lock);
}