#include <stdio.h>
#include <mac.h>

uint8_t Payload[] = "Test data";

int main(void) {
  MAC_Frame *F, *G;
  uint8_t *Data;
  uint16_t Len;

  F = MEM_Alloc(sizeof(MAC_Frame));
  G = MEM_Alloc(sizeof(MAC_Frame));

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
  F->Payload.Data = Payload;
  F->Payload.Length = sizeof(Payload);
  F->Sequence = rand() & 0xff;

  Len = MAC_GetFrameSize(F);
  Data = MEM_Alloc(Len);
  MAC_EncodeFrame(F, Data);
  // Decode frame
  MAC_DecodeFrame(G, Data, Len);

  return 0;
}