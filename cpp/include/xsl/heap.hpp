#pragma once
#ifndef XSL_HEAP
#define XSL_HEAP
#include <xsl/bits/functor.hpp>
#include <xsl/bits/heap/heap.hpp>
namespace xsl {
  template <class Ctr, class Comp = fc::less<typename Ctr::val_type>>
  using heap = hp::heap<Ctr, Comp>;
}  // namespace xsl
#endif  // !XSL_LIST