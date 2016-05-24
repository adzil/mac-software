#include <stdio.h>
//#include "mac-queue.h"
#include "queue.h"

//QUE_AllocMem(MAC_Queue, 10);
QUE_AllocMem(MAC_List, 10);

uint8_t Payload[] = "Test data";

int *QUE_ListPopInt(QUE_List *L, int Num) {
  int *F;

  QUE_ListPopInit(L);
  do {
    F = (int *) QUE_ListPop(L);
    if (!F) break;
    if (*F == Num) break;
  } while(1);
  QUE_ListPopEnd(L);

  return F;
}

int main(void) {
  /*
  MAC_Frame *F, *G;
  uint8_t *Data;
  uint16_t Len;
   */
  //QUE_Queue Q;
  int i, *Data;
  QUE_List L;

  // Initialize queue memory
  //QUE_QueueInit(&Q, QUE_CallMem(MAC_Queue), 10);
  QUE_ListInit(&L, QUE_CallMem(MAC_List), 10);

  for (i = 0; i < 20; i++) {
    Data = MEM_Alloc(sizeof(int));
    *Data = i;
    MEM_Free(QUE_ListPush(&L, Data));
  }

  for (i = 0; i < 20; i++) {
    Data = QUE_ListPopInt(&L, i);
    if (Data)
      printf("%d\n", *Data);
    else
      printf("Empty\n");
    MEM_Free(Data);
  }

  /*
  F = MAC_FrameCreate();
  if (F == NULL) return 100;

  // Frame control
  F->FrameControl.DstAdrMode = MAC_ADRMODE_SHORT;
  F->FrameControl.SrcAdrMode = MAC_ADRMODE_SHORT;
  F->FrameControl.AckRequest = MAC_ACKREQUEST_RESET;
  F->FrameControl.FramePending = MAC_FRAMEPENDING_RESET;
  F->FrameControl.FrameType = MAC_FRAMETYPE_DATA;
  // Address
  F->Address.Dst.Short = 0xdafa;
  F->Address.Src.Short = 0xadfa;
  // Payload assignment
  F->Payload.Length = sizeof(Payload);
  MAC_FrameCreatePayload(F);
  memcpy(F->Payload.Data, Payload, F->Payload.Length);

  F->Sequence = rand() & 0xff;

  Len = MAC_FrameGetSize(F);
  Data = MEM_Alloc(Len);
  MAC_FrameEncode(F, Data);
  // Decode frame
  G = MAC_FrameCreate();
  MAC_FrameDecode(G, Data, Len);

  MAC_FrameDestroy(F);
  MAC_FrameDestroy(G);
  */

  return 0;
}