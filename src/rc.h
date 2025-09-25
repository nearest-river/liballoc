#include "vtable.h"
#include "../include/alloc.h"
#include "../include/prelude.h"





typedef struct {
  usize weak;
  usize strong;
  // T value; (this one isn't directly added to prevent indirection.)
} RcInner;


typedef struct {
  const VTable vtable;
  const RcInner* const ptr;
} Rc;






















