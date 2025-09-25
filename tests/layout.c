#include "../include/alloc.h"
#include "../src/rc.h"
#include <stdio.h>


union XD { int i;long l; long long ll; };
typedef struct {
  usize u69;
  isize i69;
  union XD xd;
  char buf[169];
} Foo;



inline
static usize size_rounded_up_to_custom_align(Layout self,Alignment align) {
  // SAFETY:
  // Rounded up value is:
  //   size_rounded_up = (size + align - 1) & !(align - 1);
  //
  // The arithmetic we do here can never overflow:
  //
  // 1. align is guaranteed to be > 0, so align - 1 is always
  //    valid.
  //
  // 2. size is at most `ISIZE_MAX`, so adding `align - 1` (which is at
  //    most `ISIZE_MAX`) can never overflow a `usize`.
  //
  // 3. masking by the alignment can remove at most `align - 1`,
  //    which is what we just added, thus the value we return is never
  //    less than the original `size`.
  //
  // (Size 0 Align MAX is already aligned, so stays the same, but things like
  // Size 1 Align MAX or Size ISIZE_MAX Align 2 round up to `ISIZE_MAX + 1`.)
  const usize align_m1=((usize)align)-1;
  const usize size_rounded_up=(self.size+align_m1) & ~align_m1;
  return size_rounded_up;
}

inline
LayoutResult layout_from_size_alignment(usize size,usize align) {
  if(usize_is_power_of_two(align)) {
    return (LayoutResult){
      .layout={
        .size=0,
        .align=(Alignment)align,
      }
    };
  } else {
    return (LayoutResult){
      .ok=0
    };
  }
}

inline
LayoutOffsetResult layout_extend(const Layout self,const Layout next) {
  const Alignment new_align=MAX(self.align,next.align);
  const usize offset=size_rounded_up_to_custom_align(self,next.align);
  // SAFETY: `offset` is at most `ISIZE_MAX + 1` (such as from aligning
  // to `ALIGNMENT_MAX`) and `next.size` is at most `ISIZE_MAX` (from the
  // `Layout` type invariant).  Thus the largest possible `new_size` is
  // `ISIZE_MAX + 1 + ISIZE_MAX`, which is `USIZE_MAX`, and cannot overflow.
  const usize new_size=offset+next.size;
  const LayoutResult layout=layout_from_size_alignment(new_size,new_align);

  if(layout.ok==0) {
    return (LayoutOffsetResult){
      .ok=0
    };
  }

  return (LayoutOffsetResult){
    .layout_offset={
      .offset=offset,
      .layout=layout.layout,
    }
  };
}



int main(int argc,const char** argv) {
  let inner_layout=layout_of(RcInner);
  let extended=layout_extend(inner_layout,layout_of(Foo)).layout_offset;
  printf("align: %lu, offset: %lu\n",alignof(Foo),extended.offset);

  return 0;
}




