#pragma once
#ifndef XSL_TYPE_SUPPORT_UNVS
#define XSL_TYPE_SUPPORT_UNVS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
#include <xsl/bits/ts/is.hpp>
#include <xsl/bits/ts/remove.hpp>
XSL_BEGIN
namespace ts {
  namespace impl {
    template <bool>
    struct enable : tp::_1<bool> {};

    template <>
    struct enable<false> {};
  }  // namespace impl

  template <bool Con>
  using enable = typename impl::enable<Con>::type;
  namespace impl {
    // template <bool Con>
    // struct conditional {
    //   template <class T, class>
    //   using type = T;
    // };
    // //
    // template <>
    // struct conditional<false> {
    //   template <class, class T>
    //   using type = T;
    // };
    //
    template <bool Con, class True, class False>
    struct conditional : tp::_1<True> {};
    //
    template <class True, class False>
    struct conditional<false, True, False> : tp::_1<False> {};

  }  // namespace impl
  template <bool Con, class True, class False>
  using conditional = typename impl::conditional<Con, True, False>::type;
  template <class T>
  inline constexpr bool is_func = !is::ref<T> && (is::same<rm::cst<T>, const T> || !ts::is::same<T, T>);
  namespace impl {
    template <class T>
    struct decay {
      typedef rm::ref<T> No_Ref;
      typedef ts::conditional<is_func<No_Ref>, No_Ref *,
        ts::conditional<is::array<No_Ref>, rm::extent<No_Ref> *, rm::cv<No_Ref>>>
        type;
    };
  }  // namespace impl

  template <class T>
  using decay = typename impl::decay<T>::type;

}  // namespace ts
XSL_END
#endif  // TOT_H