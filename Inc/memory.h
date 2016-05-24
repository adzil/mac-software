#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>
#include <appdef.h>

/* Memory utility */
force_inline void *MEM_Alloc(size_t Size) {
  return malloc(Size);
}

force_inline void MEM_Free(void *Ptr) {
  return free(Ptr);
}

#endif // __MEMORY_H__