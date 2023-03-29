#pragma once
#ifndef XSL_FUNCTOR
#define XSL_FUNCTOR
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/tuple.hpp>

XSL_BEGIN
namespace fctor {
  template <class _Func>
  class identity {
  public:
    // clang-format off
	typedef typename _Func::argument_type 	argument_type;
	typedef typename _Func::result_type 	result_type;
    // clang-format on
    constexpr identity(const _Func& func)
      : Func(func) {}
    template <class T>
    constexpr bool operator()(const argument_type& arg) {
      return Func(arg);
    }
  protected:
    _Func Func;
  };
  template <>
  class identity<void> {
  public:
    template <class T>
    constexpr decltype(auto) operator()(T&& arg) {
      return forward<T>(arg);
    }
  };
  class conjunction {
  public:
    using result_type = bool;
    template <class T, class... Ts>
    constexpr bool operator()(const T& First, Ts&&...Rest) {
      return First && *this(Rest...);
    }
  };
  class equal {
  public:
    using result_type = bool;
    template <class T>
    constexpr bool operator()(const T& Left, const T& Right) {
      return Left == Right;
    }
  };
  template <class _Func>
  class not1 {
  public:
    using argument_type = bool;
    using result_type = bool;
    constexpr not1(const _Func& func)
      : Func(func) {}
    template <class T>
    constexpr bool operator()(const argument_type& arg) {
      return !Func(arg);
    }
  protected:
    _Func Func;
  };

  template <>
  class not1<void> {
  public:
    using argument_type = bool;
    using result_type = bool;
    template <class T>
    constexpr bool operator()(argument_type arg) {
      return !arg;
    }
  };
  template <class T>
  class less {
  public:
    constexpr bool operator()(const T& left, const T& right) { return left < right; }
  };
  template <>
  class less<void> {
  public:
    template <class T1, class T2>
    constexpr bool operator()(T1&& left, T2&& right) {
      return left < right;
    }
  };
  template <class T, class S>
  struct get_first_type : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::first_type {}), S>::Self {};
  template <class T, class S>
  struct get_second_type : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::second_type {}), S>::Self {};
}  // namespace fctor
XSL_END
#endif  // !XSL_FUNCTOR
