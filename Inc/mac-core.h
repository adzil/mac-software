#ifndef __MAC_CORE_H__
#define __MAC_CORE_H__

#include <inttypes.h>
#include "mac-common.h"

void MAC_CoreSendFrame(MAC_Handle *H, MAC_Frame *F);
void MAC_CoreQueueFrame(MAC_Handle *H, MAC_Frame *F);
MAC_Status MAC_CoreCheckAddressing(MAC_Handle *H, MAC_Frame *F);

#endif // __MAC_CORE_H__