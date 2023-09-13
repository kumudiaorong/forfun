#pragma once
#ifndef XSL_FUNCTIONAL_SUPPORT
#define XSL_FUNCTIONAL_SUPPORT
#include <xsl/bits/functional/binder.hpp>
#include <xsl/bits/functional/function.hpp>
namespace xsl {
  template <class Fn>
  using function = fn::function<Fn>;
  using fn::bind;
  using fn::bind_front;
  using fn::bind_back;
}  // namespace xsl
#endif