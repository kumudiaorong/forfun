#pragma once
#ifndef XSL_TYPE_SUPPORT_AS
#define XSL_TYPE_SUPPORT_AS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
#include <xsl/bits/ts/remove.hpp>
XSL_BEGIN
namespace ts {
  namespace as {
    template <class T>
    using lref = rm::ref<T>&;

    template <class T>
    using rref = rm::ref<T>&&;

    template <class T>
    using cst = const rm::cst<T>;
  }  // namespace as
}  // namespace ts
XSL_END
#endif  // TOT_H