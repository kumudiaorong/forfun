#pragma once
#ifndef XSL_TUPLE
#define XSL_TUPLE
#include <xsl/bits/tuple/tuple.hpp>
namespace xsl {
  template <class... T>
  using tuple = tpl::tuple<T...>;
}
#endif