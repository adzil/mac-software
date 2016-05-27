#ifndef __MAC_H__
#define __MAC_H__

#include <inttypes.h>
#include "mac-common.h"
#include "mac-core.h"
#include "mac-command.h"

void MAC_AppInit(MAC_Handle *H);
void MAC_AppFrameReceived(MAC_Handle *H, uint8_t *Data, size_t Length);

#endif // __MAC_H__