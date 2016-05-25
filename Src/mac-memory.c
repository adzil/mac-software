#include "mac-memory.h"
#include <stdio.h>

void MAC_MemFrameInit(MAC_MemFrame *P) {
  int Len;
  MAC_MemFrameItem *I;

  // Set variables
  Len = MAC_CONFIG_MAX_FRAME_POOL;
  I = P->MemPool;
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
    I->Next = P->MemPool;
    F = (MAC_Frame *) I;
  } else {
    printf("Mem oVF\n");
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

  I = (MAC_MemFrameItem *) F;

  // Check for memory pool belonging
  if (I->Next != P->MemPool) return;

  LOCK_Start(&P->Lock);

  // Return item to the free memory pool
  I->Next = P->Free;
  P->Free = I;

  LOCK_End(&P->Lock);
}