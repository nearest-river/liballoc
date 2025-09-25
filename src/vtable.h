#ifndef LIBALLOC_VTABLE_H
#define LIBALLOC_VTABLE_H
#include "../include/alloc.h"
#include "../include/prelude.h"


/* VTable for general data structures. */
typedef struct {
  Layout layout;
  Destructor* destructor;
  Clone* clone;
  ComparisonFn* cmp;
  Eq* eq;
  Allocator* allocator;
} VTable;



#endif // LIBALLOC_VTABLE_H
