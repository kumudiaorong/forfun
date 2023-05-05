#pragma once
#ifndef XSL_STRING
#define XSL_STRING
#include <xsl/bits/string/algorithm.hpp>
#include <xsl/bits/string/string.hpp>
namespace xsl {
  template <class Val>
  using basic_string_view = str::basic_string_view<Val>;
  template <class Val, class Alloc = alc::default_allocator<Val>>
  using basic_string = str::basic_string<Val, Alloc>;
}  // namespace xsl
#endif  // !XSL_BASIC_STRING
