#include <stdio.h>
#include "mac-core.h"

int main(void) {
  MAC_Instance Coord;
  MAC_Instance Device;
  uint8_t Buffer[128];
  size_t Length;

  MAC_Init(&Coord, 0x1, MAC_PIB_VPAN_COORDINATOR);
  MAC_Init(&Device, 0x2, MAC_PIB_VPAN_DEVICE);

  MAC_CmdAssocRequestSend(&Device);
  MAC_CoreFrameSend(&Device, Buffer, &Length);
  MAC_CoreFrameReceived(&Coord, Buffer, Length);

  MAC_CmdDataRequestSend(&Device);
  MAC_CoreFrameSend(&Device, Buffer, &Length);
  MAC_CoreFrameReceived(&Coord, Buffer, Length);

  MAC_CoreFrameSend(&Coord, Buffer, &Length);
  MAC_CoreFrameReceived(&Device, Buffer, Length);

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