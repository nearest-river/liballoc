#include "rc.h"
#include "vtable.h"
#include "../include/alloc.h"

static Layout rc_inner_layout_for_value_layout(const Layout layout);


Rc rc_new(void* value,VTable vtable) {
  let layout=rc_inner_layout_for_value_layout(vtable.layout);
  let ptr=(RcInner*)vtable.allocator->allocate(vtable.allocator,layout);

  return (Rc){
    .ptr=ptr,
    .vtable=vtable,
  };
}











































static Layout rc_inner_layout_for_value_layout(const Layout layout) {
  let inner_layout=layout_of(RcInner);
  let extended=layout_extend(inner_layout,layout);
  // SAFETY: it's safe not to check for error since the alignments were calculated by the compiler itself.
  return layout_pad_to_align(extended.layout_offset.layout);
}







