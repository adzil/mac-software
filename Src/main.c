#include "mac-core.h"

int main(void) {
  MAC_Instance Coord;
  MAC_Instance Device[3];
  uint8_t Buffer[128];
  size_t Length;
  int i;

  MAC_Init(&Coord, 0x1, MAC_PIB_VPAN_COORDINATOR);
  MAC_Init(&Device[0], 0x2, MAC_PIB_VPAN_DEVICE);
  MAC_Init(&Device[1], 0x3, MAC_PIB_VPAN_DEVICE);
  MAC_Init(&Device[2], 0x4, MAC_PIB_VPAN_DEVICE);

  Coord.Pib.ShortAdr = 0x3af1;

  for (i = 0; i < 3; i++) {
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

  for (i = 0; i < 3; i++) {
    MAC_CmdDataRequestSend(&Device[i]);
    MAC_CoreFrameSend(&Device[i], Buffer, &Length);
    MAC_CoreFrameReceived(&Coord, Buffer, Length);
  }

  return 0;
}
