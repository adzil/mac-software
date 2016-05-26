#ifndef __MAC_CORE_H__
#define __MAC_CORE_H__

#include <inttypes.h>
/* Include all mac sub-functions */
#include "mac-config.h"
#include "mac-common.h"
#include "mac-frame.h"
#include "mac-memory.h"
#include "mac-queue.h"
#include "mac-pib.h"

// MAC Handle for single MAC instance
typedef struct {
  MAC_Pib Pib;
  // The memory part should be on the bottom
  MAC_Mem Mem;
} MAC_Handle;

MAC_Status MAC_CoreCheckAddressing(MAC_Handle *H, MAC_Frame *F);

#endif // __MAC_CORE_H__