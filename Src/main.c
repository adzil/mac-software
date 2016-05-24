#include <stdio.h>
#include <mac.h>

uint8_t Payload[] = "Test data";

int main(void) {
  MAC_Frame *F, *G;
  uint8_t *Data;
  uint16_t Len;

  F = MAC_CreateFrame();
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
  MAC_CreatePayload(F);
  memcpy(F->Payload.Data, Payload, F->Payload.Length);

  F->Sequence = rand() & 0xff;

  Len = MAC_GetFrameSize(F);
  Data = MEM_Alloc(Len);
  MAC_EncodeFrame(F, Data);
  // Decode frame
  G = MAC_CreateFrame();
  MAC_DecodeFrame(G, Data, Len);

  MAC_DestroyFrame(F);
  MAC_DestroyFrame(G);

  return 0;
}