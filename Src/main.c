#include <stdio.h>
#include "mac-memory.h"
#include "mac-queue.h"

QUE_MemAlloc(MAC_List, 10);

uint8_t Payload[] = "Test data";
MAC_MemFrame P;

int main(void) {
  MAC_Frame *F;
  int i;
  QUE_Queue Q;
  MAC_FrameAddress Address;

  // Initialize queue memory
  QUE_QueueInit(&Q, QUE_MemCall(MAC_List), 10);
  // Initialize pool memory
  MAC_MemFrameInit(&P);

  for (i = 0; i < 33; i++) {
    F = MAC_MemFrameAlloc(&P);
    if (!F) return 1;
    F->Address.Dst.Short = i;
    F->FrameControl.DstAdrMode = MAC_ADRMODE_SHORT;

    F = MAC_FrameQueueAppend(&Q, F);
    if (F) {
      MAC_MemFrameFree(&P, F);
    }
  }

  for (i = 18; i >= 0; i-=2) {
    Address.Short = i;
    F = MAC_FrameQueueFind(&Q, MAC_ADRMODE_SHORT, Address);
    if (F) {
      printf("%d\n", F->Address.Dst.Short);
      MAC_MemFrameFree(&P, F);
    } else {
      printf("Empty\n");
    }
  }

  for (i = 19; i >= 0; i-=2) {
    Address.Short = i;
    F = MAC_FrameQueueFind(&Q, MAC_ADRMODE_SHORT, Address);
    if (F) {
      printf("%d\n", F->Address.Dst.Short);
      MAC_MemFrameFree(&P, F);
    } else {
      printf("Empty\n");
    }
  }

  return 0;
}