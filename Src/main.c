#include "mac-core.h"

int main(void) {
  MAC_Instance Coord;
  MAC_Instance Device[3];
  uint8_t Buffer[128];
  size_t Length;
  MAC_Frame *F;
  int i;

  MAC_Init(&Coord, 0x1, MAC_PIB_VPAN_COORDINATOR);
  MAC_Init(&Device[0], 0x2, MAC_PIB_VPAN_DEVICE);
  MAC_Init(&Device[1], 0x3, MAC_PIB_VPAN_DEVICE);
  MAC_Init(&Device[2], 0x4, MAC_PIB_VPAN_DEVICE);

  Coord.Pib.ShortAdr = 0xb782;

  for (i = 0; i < 3; i++) {
    // Discover request
    MAC_CmdDiscoverRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
    // Discover req reply
    MAC_CoreFrameSend(&Coord, Buffer, &Length);
    MAC_CoreFrameReceived(&Device[i], Buffer, Length);
    // Assoc Request send
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
    // Data request send
    MAC_CmdDataRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
    // Get data reply
    MAC_CoreFrameSend(&Coord, Buffer, &Length);
    MAC_CoreFrameReceived(&Device[i], Buffer, Length);
  }
/*
  printf("- Association Request Test\n");

  for (i = 0; i < 3; i++) {
    MAC_CmdAssocRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
  }

  printf("- Data Request Test\n");

  for (i = 0; i < 3; i++) {
    MAC_CmdDataRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
  }

  printf("- Association Response Test\n");

  for (i = 0; i < 3; i++) {
    MAC_CoreFrameSend(&Coord, Buffer, &Length);
    MAC_CoreFrameReceived(&Device[0], Buffer, Length);
    MAC_CoreFrameReceived(&Device[1], Buffer, Length);
    MAC_CoreFrameReceived(&Device[2], Buffer, Length);
  }

  printf("- Coordinator to Device Transmission Test\n");
  printf("  Coordinator will begin to create and queue new data frame with "
             "destination to Device 1.\n");

  // Create dummy data frame
  F = MAC_MemFrameAlloc(&Coord.Mem);
  if (F) {
    MAC_SetFrameShortDstAdr(F, Device[0].Pib.ShortAdr);
    MAC_GenFrameSrcAdr(&Coord, F);
    MAC_GenFrameSequence(&Coord, F);
    MAC_SetFrameNoAckRequest(F);
    MAC_SetFrameNoPending(F);
    MAC_SetFrameType(F, MAC_FRAMETYPE_DATA);

    memcpy(F->Payload.Data, "Test Data", 9);
    F->Payload.Length = 9;

    MAC_TransmitPutFrame(&Coord, F);
  }

  printf("- Collective data request\n"
             "   Coordinator will only send data to device 1 "
             "(from prev queue).\n");

  for (i = 0; i < 3; i++) {
    MAC_CmdDataRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
  }

  printf("- Collective data retrieval\n"
             "   Device MAC will filter out data only to Device 1.\n");

  for (i = 0; i < 3; i++) {
    MAC_CoreFrameSend(&Coord, Buffer, &Length);
    MAC_CoreFrameReceived(&Device[0], Buffer, Length);
    MAC_CoreFrameReceived(&Device[1], Buffer, Length);
    MAC_CoreFrameReceived(&Device[2], Buffer, Length);
  }

  printf("- Device to Coordinator Transmission Test\n"
         "  Device 2 will create new data frame with dst to Coordinator.\n");

  // Send data from client
  F = MAC_MemFrameAlloc(&Device[2].Mem);
  if (F) {
    MAC_SetFrameNoDstAdr(F);
    MAC_GenFrameSrcAdr(&Device[2], F);
    MAC_GenFrameSequence(&Device[2], F);
    MAC_SetFrameNoAckRequest(F);
    MAC_SetFrameNoPending(F);
    MAC_SetFrameType(F, MAC_FRAMETYPE_DATA);

    memcpy(F->Payload.Data, "Reply Data", 10);
    F->Payload.Length = 10;

    MAC_TransmitPutFrame(&Device[2], F);
  }


  printf("- Receive event from coordinator\n");

  MAC_CoreFrameSend(&Device[2], Buffer, &Length);
  MAC_CoreFrameReceived(&Coord, Buffer, Length);

  printf("- Re-Association Request Test\n");

  for (i = 2; i >= 0; i--) {
    // Reset association status
    Device[i].Pib.AssociatedCoord = MAC_PIB_ASSOCIATED_RESET;
    Device[i].Pib.ShortAdr = MAC_CONST_BROADCAST_ADDRESS;
    // Restart association
    MAC_CmdAssocRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
  }

  for (i = 0; i < 3; i++) {
    MAC_CmdDataRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
  }

  for (i = 0; i < 3; i++) {
    MAC_CoreFrameSend(&Coord, Buffer, &Length);
    MAC_CoreFrameReceived(&Device[0], Buffer, Length);
    MAC_CoreFrameReceived(&Device[1], Buffer, Length);
    MAC_CoreFrameReceived(&Device[2], Buffer, Length);
  }
*/
  return 0;
}
