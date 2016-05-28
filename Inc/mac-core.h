#ifndef __MAC_CORE_H__
#define __MAC_CORE_H__

#include <inttypes.h>
#include "mac-handle.h"
#include "mac-command.h"

void MAC_CoreFrameReceived(MAC_Handle *H, uint8_t *Data, size_t Length);
MAC_Status MAC_CoreCheckAddressing(MAC_Handle *H, MAC_Frame *F);

#endif // __MAC_CORE_H__