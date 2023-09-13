#pragma once
#ifndef XSL_TYPE_SUPPORT_UNVS
#define XSL_TYPE_SUPPORT_UNVS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
#include <xsl/bits/ts/is.hpp>
#include <xsl/bits/ts/rm.hpp>
namespace xsl::ts {
  namespace impl {
    template <bool>
    struct enable : tp::_1<bool> {};

    template <>
    struct enable<false> {};
  }  // namespace impl

  template <bool Con>
  using enable = typename impl::enable<Con>::type;

  template <class This, class Arg>
  using enable_construct = enable<!is::same<This, rm::cvr<Arg>>>;

  template <bool Con, template <class> class Pre, class T>
  using conditional_apply = conditional<Con, Pre<T>, T>;
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

}  // namespace xsl::ts
#endif  // TOT_H