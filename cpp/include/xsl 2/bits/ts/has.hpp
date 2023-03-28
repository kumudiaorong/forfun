#pragma once
#ifndef XSL_TYPE_SUPPORT_HAS
#define XSL_TYPE_SUPPORT_HAS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
XSL_BEGIN
namespace ts {
  namespace has {
    template <class LMBD, class Rep, class V = void>
    constexpr bool some = false;
    //
    template <class LMBD, class... T>
    constexpr bool some<LMBD, tp::_n<T...>, tp::_0<decltype(eval_type<LMBD>()(eval_type<T>()...))>> = true;
    // Dependent Templates	:eval_type
    // Description			:types T have some attributes
    template <class Base, class Derive>
    inline constexpr bool base =
      has::some<decltype([]<class T, class U>(T, U) -> decltype(eval_type<T *&>() = eval_type<U *>(), 0) { return 0; }),
        tp::_n<Base, Derive>>;
    // Dependent Templates	:some
    // Description			:Base is base of Derive
    // Example				:
    //  auto xsl_is_base_test() {
    //  	struct A {};
    //  	struct B:A {};
    //  	static bool arr[] = {
    //  	xsl::tp::base<A, B>,
    //  	xsl::tp::base<A, A> ,
    //  	xsl::tp::base<B, a>
    //  	};
    //  	return arr;
    //  }
    template <class T, class... Args>
    inline constexpr bool constructible =
      has::some<decltype([]<class U, class... V>(U, V...) -> decltype(U{eval_type<V>()...}, 0) { return 0; }),
        tp::_n<T, Args...>>;
    // Dependent Templates	:some
    // Description			:T is constructed by Args
    // Example				:
    //  auto xsl_is_constructible_test() {
    //      struct A {};
    //      struct B {
    //          B(A) {}
    //      };
    //      struct C:A {};
    //      static bool arr[] = {
    //      xsl::tp::constructible<A, A>,
    //      xsl::tp::constructible<B, C>,
    //      xsl::tp::constructible<A, B>,
    //      xsl::tp::constructible<B, A> ,
    //      xsl::tp::constructible<A, C>,
    //      xsl::tp::constructible<C, A>,
    //      };
    //      return arr;
    //  }
    template <class T>
    inline constexpr bool traversable =
      has::some<decltype([]<class U>(U) -> decltype(eval_type<U>().begin(), eval_type<U>().end(), 0) { return 0; }),
        tp::_n<T>>;
    // Dependent Templates	:some
    // Description			:T is constructed by Args
    // Example				:
    //  auto xsl_is_constructible_test() {
    //      struct A {};
    //      struct B {
    //          B(A) {}
    //      };
    //      struct C:A {};
    //      static bool arr[] = {
    //      xsl::tp::constructible<A, A>,
    //      xsl::tp::constructible<B, C>,
    //      xsl::tp::constructible<A, B>,
    //      xsl::tp::constructible<B, A> ,
    //      xsl::tp::constructible<A, C>,
    //      xsl::tp::constructible<C, A>,
    //      };
    //      return arr;
    //  }
    template <class T>
    inline constexpr bool opeartor_pre_increment =
      has::some<decltype([]<class U>(U) -> decltype(++eval_type<U>(), 0) { return 0; }), tp::_n<T>>;
    // Dependent Templates	:eval_type
    // Description			:T has member function opeartor++
    // Example				:
    //  auto xsl_opeartor_pre_increment_test() {
    //      struct A {};
    //      struct B {
    //          void operator++() {}
    //      };
    //      static bool arr[] = {
    //      xsl::tp::opeartor_pre_increment<A>,
    //      xsl::tp::opeartor_pre_increment<B>,
    //      };
    //      return arr;
    //  }
    template <class T>
    inline constexpr bool opeartor_indirection =
      has::some<decltype([]<class U>(U) -> decltype(*eval_type<U>(), 0) { return 0; }), tp::_n<T>>;
    // Dependent Templates	:eval_type
    // Description			:T has member function opeartor*
    // Example				:
    //  auto xsl_opeartor_indirection_test() {
    //      struct A {};
    //      struct B {
    //          void operator*() {}
    //      };
    //      static bool arr[] = {
    //      xsl::tp::opeartor_indirection<A>,
    //      xsl::tp::opeartor_indirection<B>,
    //      };
    //      return arr;
    //  }
    template <class T>
    inline constexpr bool opeartor_equal =
      has::some<decltype([]<class U>(U) -> decltype(eval_type<const U&>() == eval_type<const U&>(), 0) { return 0; }),
        tp::_n<T>>;
    // Dependent Templates	:eval_type
    // Description			:T has member function opeartor==
    // Example				:
    //  auto xsl_opeartor_equal_test() {
    //      struct A {};
    //      struct B {
    //          void operator==(const B&) {}
    //      };
    //      static bool arr[] = {
    //      xsl::tp::opeartor_equal<A>,
    //      xsl::tp::opeartor_equal<B>,
    //      };
    //      return arr;
    //  }
    template <class L, class R>
    inline constexpr bool opeartor_addition =
      has::some<decltype([]<class T, class U>(T, U) -> decltype(eval_type<T>() + (eval_type<U>()), 0) { return 0; }),
        tp::_n<L, R>>;
    // Dependent Templates	:addition
    // Description			:T has member function opeartor+
    // Example				:
    //  auto xsl_opeartor_addition_test() {
    //      struct A {};
    //      struct B {
    //          void operator+(const A&) {}
    //      };
    //      static bool arr[] = {
    //      xsl::tp::opeartor_addition<A,B>,
    //      xsl::tp::opeartor_addition<B,A>,
    //      };
    //      return arr;
    //  }
    template <class L, class R>
    inline constexpr bool opeartor_addition_assignment =
      has::some<decltype([]<class T, class U>(T, U) -> decltype(eval_type<T>() += (eval_type<U>()), 0) { return 0; }),
        tp::_n<L, R>>;
    // Dependent Templates	:addition assignment
    // Description			:T has member function opeartor+=
    // Example				:
    //  auto xsl_opeartor_addition_assignment_test() {
    //      struct A {};
    //      struct B {
    //          void operator+=(const A&) {}
    //      };
    //      static bool arr[] = {
    //      xsl::tp::opeartor_addition_assignment<A,B>,
    //      xsl::tp::opeartor_addition_assignment<B,A>,
    //      };
    //      return arr;
    //  }
  }  // namespace has
  namespace has {
    template <class L, class R>
    inline constexpr bool opeartor_simple_assignment =
      some<decltype([]<class T, class U>(T, U) -> decltype(eval_type<T&>() = (eval_type<U>()), 0) { return 0; }),
        tp::_n<L, R>>;
    // Dependent Templates	:simple assignment
    // Description			:T has member function opeartor=
    // Example				:
    //  auto xsl_opeartor_simple_assignment_test() {
    //      struct A {};
    //      struct B {
    //          void operator=(const A&) {}
    //      };
    //      static bool arr[] = {
    //      xsl::tp::opeartor_simple_assignment<A,B>,
    //      xsl::tp::opeartor_simple_assignment<B,A>,
    //      };
    //      return arr;
    //  }
  }  // namespace has
}  // namespace ts
XSL_END
#endif  // TOT_H