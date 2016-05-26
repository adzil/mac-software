#include <mac-memory.h>
#include "mac-memory.h"

void MAC_MemInit(MAC_MemHandle *H, void *I, size_t Size, int Length) {
  // Unlock memory handle
  LOCK_End(&H->Lock);
  H->Free = NULL;

  while (Length -- > 0) {
    *((void **) I) = H->Free;
    H->Free = I;
    I += Size;
  }
}

void *MAC_MemAlloc(MAC_MemHandle *H) {
  void *I;

  // Start lock mechanism
  LOCK_Start(&H->Lock);
  // Check for free memory
  if (!H->Free) return NULL;
  I = H->Free;
  // Move free pointer
  H->Free = *((void **) I);
  *((void **) I) = H;
  LOCK_End(&H->Lock);

  return I + sizeof(void *);
}

void MAC_MemFree(MAC_MemHandle *H, void *I) {
  // Check memory belonging
  I -= sizeof(void *);
  if (*((void **) I) != H) return;

  // Start lock mechanism
  LOCK_Start(&H->Lock);
  *((void **) I) = H->Free;
  H->Free = I;
  LOCK_End(&H->Lock);
}

void MAC_MemAllInit(MAC_Mem *M) {
  MAC_MemInit(&M->Frame,M->Pool.Frame, MAC_MemItemSize(MAC_Frame),
              MAC_CONFIG_MAX_FRAME_POOL);
  MAC_MemInit(&M->AdrList,M->Pool.AdrList, MAC_MemItemSize(MAC_MemAdrList),
              MAC_CONFIG_MAX_ADDRESS_LIST);
}

/*void MAC_MemFrameInit(MAC_MemFrame *P) {
  int Len;
  MAC_MemFrameItem *I;

  // Set variables
  Len = MAC_CONFIG_MAX_FRAME_POOL;
  I = P->Pool;
  P->Free = NULL;
  LOCK_End(&P->Lock);

  while (Len-- > 0) {
    I->Next = P->Free;
    P->Free = I++;
  }
}

MAC_Frame *MAC_MemFrameAlloc(MAC_MemFrame *P) {
  MAC_MemFrameItem *I;
  MAC_Frame *F;

  LOCK_Start(&P->Lock);

  F = NULL;
  I = P->Free;
  if (I) {
    P->Free = I->Next;
    I->Next = (void *) &P->Pool;
    F = &I->Frame;
  }

  LOCK_End(&P->Lock);

  // Initialize frame
  if (F) {
    F->Payload.Length = 0;
    F->Payload.Start = 0;
  }

  return F;
}

void MAC_MemFrameFree(MAC_MemFrame *P, MAC_Frame *F) {
  MAC_MemFrameItem *I;

  I = (MAC_MemFrameItem *) (((void *)F) - offsetof(MAC_MemFrameItem, Frame));

  // Check for memory pool belonging
  if (I->Next != (void *) &P->Pool) return;

  LOCK_Start(&P->Lock);

  // Return item to the free memory pool
  I->Next = P->Free;
  P->Free = I;

  LOCK_End(&P->Lock);
}*/