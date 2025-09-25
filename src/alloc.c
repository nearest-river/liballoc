#include "alloc.h"
#include "../include/prelude.h"
#include <string.h>


static u8* _dummy_alloc(const void* self,Layout layout);
static u8* _dummy_realloc(const void* self,u8* ptr,Layout layout,usize new_size);
static void _dummy_dealloc(const void* self,u8* ptr,Layout layout);

__attribute__((weak))
const GlobalAlloc SYSTEM_GLOBAL_ALLOCATOR={
  .self=NULL,
  .alloc=_dummy_alloc,
  .alloc_zeroed=_dummy_alloc,
  .realloc=_dummy_realloc,
  .dealloc=_dummy_dealloc,
};

static u8* _global_allocator_alloc(const void* self,Layout layout);
static u8* _global_allocator_alloc_zeroed(const void* self,Layout layout);
static u8* _global_allocator_realloc(const void* self,u8* ptr,Layout layout,usize new_size);
static void _global_allocator_dealloc(const void* self,u8* ptr,Layout layout);
static u8* _global_allocator_grow(const void* self,u8* ptr,Layout old_layout,Layout new_layout);
static u8* _global_allocator_grow_zeroed(const void* self,u8* ptr,Layout old_layout,Layout new_layout);
static u8* _global_allocator_shrink(const void* self,u8* ptr,Layout old_layout,Layout new_layout);


const Allocator GLOBAL={
  .self=NULL,
  .allocate=_global_allocator_alloc,
  .allocate_zeroed=_global_allocator_alloc_zeroed,
  .reallocate=_global_allocator_realloc,
  .dealloc=_global_allocator_dealloc,
  .grow=_global_allocator_grow,
  .grow_zeroed=_global_allocator_grow_zeroed,
  .shrink=_global_allocator_shrink
};



inline_always
static u8* _global_allocator_alloc_impl(const void* self,Layout layout,bool zeroed) {
  (void)self;
  if(layout.size==0) {
    return layout_dangling(layout);
  }

  return zeroed?_alloc_zeroed(layout):_alloc(layout);
}

static u8* _global_allocator_alloc(const void* self,Layout layout) {
  return _global_allocator_alloc_impl(self,layout,false);
}

static u8* _global_allocator_alloc_zeroed(const void* self,Layout layout) {
  return _global_allocator_alloc_impl(self,layout,true);
}

static u8* _global_allocator_realloc(const void* self,u8* ptr,Layout layout,usize new_size) {
  if(ptr==NULL || (layout.size==0 && new_size!=layout.size)) {
    Layout new_layout={
      .size=new_size,
      .align=layout.align
    };
    return _alloc(new_layout);
  }

  if(new_size==0) {
    _dealloc(ptr,layout);
    return layout_dangling(layout);
  }

  return _realloc(ptr,layout,new_size);
}

static void _global_allocator_dealloc(const void* self,u8* ptr,Layout layout) {
  if(ptr==NULL || layout.size==0) {
    return;
  }

  return _dealloc(ptr,layout);
}

inline_always
static u8* _global_allocator_grow_impl(const void* self,u8* ptr,Layout old_layout,Layout new_layout,bool zeroed) {
  (void)self;
  assert(new_layout.size>=old_layout.size && "`new_layout.size` must be greater than or equal to `old_layout.size`");

  if(old_layout.size==0) {
    return _global_allocator_alloc_impl(self,new_layout,zeroed);
  }

  if(old_layout.align==new_layout.align) {
    usize new_size=new_layout.size;
    u8* ptr=_realloc(ptr,old_layout,new_size);

    if(zeroed) {
      memset(ptr,0,new_layout.size-old_layout.size);
    }
    return ptr;
  } else {
    u8* new_ptr=_global_allocator_alloc_impl(self,new_layout,zeroed);
    memcpy(new_ptr,ptr,old_layout.size);
    _global_allocator_dealloc(self,ptr,old_layout);
    return new_ptr;
  }
}

static u8* _global_allocator_grow(const void* self,u8* ptr,Layout old_layout,Layout new_layout) {
  return _global_allocator_grow_impl(self,ptr,old_layout,new_layout,false);
}

static u8* _global_allocator_grow_zeroed(const void* self,u8* ptr,Layout old_layout,Layout new_layout) {
  return _global_allocator_grow_impl(self,ptr,old_layout,new_layout,true);
}

static u8* _global_allocator_shrink(const void* self,u8* ptr,Layout old_layout,Layout new_layout) {
  assert(new_layout.size<=old_layout.size && "`new_layout.size` must be smaller than or equal to `old_layout.size`");
  if(new_layout.size==0) {
    _global_allocator_dealloc(self,ptr,old_layout);
    return layout_dangling(new_layout);
  }

  if(old_layout.align==new_layout.align) {
    return _realloc(ptr,old_layout,new_layout.size);
  } else {
    u8* new_ptr=_global_allocator_alloc_impl(self,new_layout,false);

    memcpy(new_ptr,ptr,new_layout.size);
    _global_allocator_dealloc(self,ptr,old_layout);
    return new_ptr;
  }
}



void* _alloc(Layout layout) {
  return SYSTEM_GLOBAL_ALLOCATOR.alloc(SYSTEM_GLOBAL_ALLOCATOR.self,layout);
}

void* _alloc_zeroed(Layout layout) {
  return SYSTEM_GLOBAL_ALLOCATOR.alloc_zeroed(SYSTEM_GLOBAL_ALLOCATOR.self,layout);
}

void* _realloc(void* ptr,Layout layout,usize new_size) {
  return SYSTEM_GLOBAL_ALLOCATOR.realloc(SYSTEM_GLOBAL_ALLOCATOR.self,(u8*)ptr,layout,new_size);
}

void _dealloc(void* ptr,Layout layout) {
  SYSTEM_GLOBAL_ALLOCATOR.dealloc(SYSTEM_GLOBAL_ALLOCATOR.self,(u8*)ptr,layout);
}


static u8* _dummy_alloc(const void* self,Layout layout) {
  (void)self;
  (void)layout;
  return layout.size==0?layout_dangling(layout):NULL;
}

static u8* _dummy_realloc(const void* self,u8* ptr,Layout layout,usize new_size) {
  (void)self;
  (void)ptr;
  (void)layout;
  (void)new_size;
  return layout.size==0?layout_dangling(layout):NULL;
}

static void _dummy_dealloc(const void* self,u8* ptr,Layout layout) {
  (void)self;
  (void)ptr;
  (void)layout;
}


