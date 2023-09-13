#pragma once
#ifndef XSL_TYPE_SUPPORT_AS
#define XSL_TYPE_SUPPORT_AS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
#include <xsl/bits/ts/rm.hpp>

namespace xsl::ts::as {
  template <class T>
  using lref = rm::ref<T>&;

  template <class T>
  using rref = rm::ref<T>&&;

  template <class T>
  using cst = const rm::cst<T>;
}  // namespace xsl::ts::as
#endif  // TOT_H