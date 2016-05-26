#include <stdio.h>
#include <mac-memory.h>
#include "mac-memory.h"
#include "mac-queue.h"

QUE_MemAlloc(MAC_List, 10);

uint8_t Payload[] = "Test data";

int main(void) {
  MAC_Frame *F;
  int i;
  QUE_Queue Q;
  MAC_Mem M;
  MAC_FrameAddress Address;

  // Initialize queue memory
  QUE_QueueInit(&Q, QUE_MemCall(MAC_List), 10);
  // Initialize pool memory
  MAC_MemAllInit(&M);

  for (i = 0; i < 64; i++) {
    F = MAC_MemAlloc(&M.Frame);
    if (!F) break;
    F->Address.Dst.Short = i;
    F->FrameControl.DstAdrMode = MAC_ADRMODE_SHORT;

    F = MAC_QueueFrameAppend(&Q, F);
    if (F) {
      MAC_MemFree(&M.Frame, F);
    }
  }

  for (i = 63; i >= 50; i-=2) {
    Address.Short = i;
    F = MAC_QueueFrameFind(&Q, MAC_ADRMODE_SHORT, Address);
    if (F) {
      printf("%d\n", F->Address.Dst.Short);
      MAC_MemFree(&M.Frame, F);
    } else {
      printf("Empty\n");
    }
  }

  for (i = 62; i >= 50; i-=2) {
    Address.Short = i;
    F = MAC_QueueFrameFind(&Q, MAC_ADRMODE_SHORT, Address);
    if (F) {
      printf("%d\n", F->Address.Dst.Short);
      MAC_MemFree(&M.Frame, F);
    } else {
      printf("Empty\n");
    }
  }


  /*
  do {
    F = MAC_QueueFramePop(&Q);
    if (!F) break;
    printf("%d\n", F->Address.Dst.Short);
    MAC_MemFrameFree(&P, F);
  } while(1);
  */

  return 0;
}