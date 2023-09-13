#pragma once
#ifndef XSL_TYPE_SUPPORT_DEF
#define XSL_TYPE_SUPPORT_DEF
#include "../def.hpp"
namespace xsl::ts {
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
    class _1 {
    public:
      typedef T type;
      typedef _1 self;
    };
    // Template unit
    template <class T1, class T2>
    struct _2 {
      typedef T1 type1;
      typedef T2 type2;
      typedef _2 self;
    };
    // Template unit
    template <class T1, class T2, class T3>
    struct _3 {
      typedef T1 type1;
      typedef T2 type2;
      typedef T3 type3;
      typedef _3 self;
    };
    // Template unit
    template <class... Ts>
    struct _n {
      typedef _n self;
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
      typedef _1 self;
    };
    template <class T, T V1, T V2>
    struct _2 : tp::_1<T> {
      typedef _2 self;
    };
    template <class T, T... Vars>
    struct _n : tp::_1<T> {
      typedef _n self;
    };
    template <class T, T V1, T V2>
    constexpr T subtraction = V1 - V2;
  }  // namespace nt
  namespace var {
    template <auto V>
    struct _1 {
      static constexpr decltype(V) value = V;
    };
  }  // namespace var
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
}  // namespace xsl::ts
#endif  // TOT_H