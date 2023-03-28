#pragma once
#ifndef XSL_TYPE_SUPPORT_DEF
#define XSL_TYPE_SUPPORT_DEF
#include <xsl/bits/def.hpp>
XSL_BEGIN
namespace ts {
  namespace tp {
    struct true_type {
      constexpr static bool Val = true;
    };
    struct false_type {
      constexpr static bool Val = false;
    };
    template <class... Ts>
    using _0 = void;
    // Template unit
    template <class T>
    struct _1 {
      using Type = T;
      using Self = _1;
    };
    // Template unit
    template <class T1, class T2>
    struct _2 {
      using Type1 = T1;
      using Type2 = T2;
      using Self = _2;
    };
    // Template unit
    template <class T1, class T2, class T3>
    struct _3 {
      using Type1 = T1;
      using Type2 = T2;
      using Type3 = T3;
      using Self = _3;
    };
    // Template unit
    template <class... Ts>
    struct _n {
      using Self = _n;
    };
  }  // namespace tp
  // Template unit
  template <class T>
  constexpr T eval_type();
  // Template unit
  //
  namespace nt {
    // basic template
    template <class T, T V>
    struct _1 : tp::_1<T> {
      using Self = _1;
    };
    template <class T, T V1, T V2>
    struct _2 : tp::_1<T> {
      using Self = _2;
    };
    template <class T, T... Vars>
    struct _n : tp::_1<T> {
      using Self = _n;
    };
    // template <class Left, class Right>
    // constexpr auto addition;
    // template <class Left, Left Vl, class Right, Left Vr>
    // constexpr auto addition<_1<Left, Vl>, _1<Right, Vr>> =
  }  // namespace nt
  namespace var {
    template <auto V>
    struct _1 {
      static constexpr decltype(V) Val = V;
    };
    // basic template
    // template <class Left, class Right>
    // constexpr auto addition;
    // template <class Left, Left Vl, class Right, Left Vr>
    // constexpr auto addition<_1<Left, Vl>, _1<Right, Vr>> =
  }  // namespace var
}  // namespace ts
XSL_END
#endif  // TOT_H