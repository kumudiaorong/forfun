#pragma once
#ifndef XSL_TYPE_SUPPORT_REF
#define XSL_TYPE_SUPPORT_REF
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
XSL_BEGIN
namespace ts {
  namespace ref {
    namespace impl {
      template <class T>
      struct reference
        : tp::_1<T>
        , var::_1<0u> {};

      template <class T>
      struct reference<T&>
        : tp::_1<T>
        , var::_1<1u> {};

      template <class T>
      struct reference<T&&>
        : tp::_1<T>
        , var::_1<2u> {};
    }  // namespace impl
    template <class T>
    using rm_ref = typename impl::reference<T>::Type;

    template <class T>
    using as_lref = typename impl::reference<T>::Type&;

    template <class T>
    using as_rref = typename impl::reference<T>::Type&&;

    template <class T>
    constexpr bool is_ref = impl::reference<T>::Val != 0;

    template <class T>
    constexpr bool is_lref = impl::reference<T>::Val == 1;

    template <class T>
    constexpr bool is_rref = impl::reference<T>::Val == 2;
  }  // namespace ref
}  // namespace ts
XSL_END
#endif  // TOT_H