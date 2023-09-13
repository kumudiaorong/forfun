#pragma once
#ifndef XSL_VECTOR
#define XSL_VECTOR
#include <xsl/bits/vector/vector.hpp>
namespace xsl {
  template <class Val, class Alloc = alc::default_allocator<Val>>
  using vector = vec::vector<Val, Alloc>;
}
#endif  // !XSL_BASIC_STRING
