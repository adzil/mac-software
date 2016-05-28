#include <stdio.h>
#include "mac-core.h"

MAC_Handle MAC;

int main(void) {
  MAC_HandleInit(&MAC);

  return 0;
}

/*
int main(void) {
  MAC_Frame *F;
  int i;
  MAC_FrameAddress Address;

  // Initialize pool memory
  MAC_MemPoolInit(&M);
  MAC_MemQueueInit(&M);

  for (i = 0; i < 64; i++) {
    F = MAC_MemFrameAlloc(&M);
    if (!F) break;
    F->Address.Dst.Short = i;
    F->FrameControl.DstAdrMode = MAC_ADRMODE_SHORT;

    F = MAC_QueueFrameAppend(&M.Tx, F);
    if (F) {
      MAC_MemFrameFree(&M, F);
    }
  }

  for (i = 63; i >= 50; i-=2) {
    Address.Short = i;
    F = MAC_QueueFrameFind(&M.Tx, MAC_ADRMODE_SHORT, Address);
    if (F) {
      printf("%d\n", F->Address.Dst.Short);
      MAC_MemFrameFree(&M, F);
    } else {
      printf("Empty\n");
    }
  }

  for (i = 62; i >= 50; i-=2) {
    Address.Short = i;
    F = MAC_QueueFrameFind(&M.Tx, MAC_ADRMODE_SHORT, Address);
    if (F) {
      printf("%d\n", F->Address.Dst.Short);
      MAC_MemFrameFree(&M, F);
    } else {
      printf("Empty\n");
    }
  }

  return 0;
}*/