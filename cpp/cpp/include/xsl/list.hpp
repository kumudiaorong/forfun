#pragma once
#ifndef XSL_LIST
#define XSL_LIST
#include <xsl/bits/list/list.hpp>
namespace xsl {
  template <class Val, class Alloc = alc::default_allocator<Val>>
  using list = ls::list<Val, Alloc>;
}
#endif  // !XSL_LIST