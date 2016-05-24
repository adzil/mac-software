#ifndef __LOCK_H__
#define __LOCK_H__

#include <appdef.h>

typedef int LOCK_Handle;

force_inline void LOCK_Start(LOCK_Handle *Handle) {
  *Handle = 1;
}

force_inline void LOCK_End(LOCK_Handle *Handle) {
  *Handle = 0;
}

#endif // __LOCK_H__