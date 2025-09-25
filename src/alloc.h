#ifndef LIBALLOC_ALLOC_H
#define LIBALLOC_ALLOC_H
#include "../include/alloc.h"


extern const GlobalAlloc SYSTEM_GLOBAL_ALLOCATOR;
extern const Allocator GLOBAL;

void* _alloc(Layout layout);
void* _alloc_zeroed(Layout layout);
void* _realloc(void* ptr,Layout layout,usize new_size);
void _dealloc(void* ptr,Layout layout);




#endif // LIBALLOC_ALLOC_H
