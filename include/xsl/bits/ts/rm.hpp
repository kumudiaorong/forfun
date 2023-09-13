#pragma once
#ifndef XSL_TYPE_SUPPORT_REMOVE
#define XSL_TYPE_SUPPORT_REMOVE
#include "../def.hpp"
#include "def.hpp"
namespace xsl::ts::rm {
  namespace impl {
    template <class T>
    struct array
      : tp::_1<T>
      , var::_1<0> {};

    template <class T>
    struct array<T[]>
      : tp::_1<T>
      , var::_1<1> {};

    template <class T, size_t Size>
    struct array<T[Size]>
      : tp::_1<T>
      , var::_1<1> {};
  }  // namespace impl

  template <class T>
  using extent = typename impl::array<T>::type;

  namespace impl {
    template <class T>
    struct const_ : tp::_1<T> {};
    template <class T>
    struct const_<const T> : tp::_1<T> {};
  }  // namespace impl
  template <class T>
  using cst = typename impl::const_<T>::type;
  namespace impl {
    template <class T>
    struct volatile_ : tp::_1<T> {};
    template <class T>
    struct volatile_<volatile T> : tp::_1<T> {};
  }  // namespace impl
  template <class T>
  using vol = typename impl::volatile_<T>::type;
  template <class T>
  using cv = vol<cst<T>>;
  namespace impl {
    template <class T>
    struct reference : tp::_1<T> {};
    template <class T>
    struct reference<T&> : tp::_1<T> {};
    template <class T>
    struct reference<T&&> : tp::_1<T> {};
  }  // namespace impl
  template <class T>
  using ref = typename impl::reference<T>::type;
  template <class T>
  using cvr = vol<cst<ref<T>>>;
}  // namespace xsl::ts::rm
#endif  // TOT_H