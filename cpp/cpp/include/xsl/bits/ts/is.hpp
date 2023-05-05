#pragma once
#ifndef XSL_TYPE_SUPPORT_IS
#define XSL_TYPE_SUPPORT_IS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
namespace xsl::ts::is {
  template <class T>
  inline constexpr bool array = false;

  template <class T>
  inline constexpr bool array<T[]> = true;

  template <class T, size_t Size>
  inline constexpr bool array<T[Size]> = true;

  template <class T>
  inline constexpr bool lref = false;

  template <class T>
  inline constexpr bool lref<T&> = true;

  template <class T>
  inline constexpr bool rref = false;

  template <class T>
  inline constexpr bool rref<T&&> = true;

  template <class T>
  inline constexpr bool ref = lref<T> || rref<T>;

  template <class T>
  inline constexpr bool norm_func_ptr = false;

  template <class Ret, class... Args>
  inline constexpr bool norm_func_ptr<Ret (*)(Args...)> = true;

  template <class>
  inline constexpr bool member_func_ptr = false;

  template <class Ret, class Class>
  inline constexpr bool member_func_ptr<Ret(Class::*)> = true;

  template <class This, class Ano>
  inline constexpr bool same = false;

  template <class T>
  inline constexpr bool same<T, T> = true;

  template <class T>
  inline constexpr bool char8 = same<T, char8_t>;

  template <class T>
  inline constexpr bool char16 = same<T, char16_t>;

  template <class T>
  inline constexpr bool char32 = same<T, char32_t>;

  template <class T>
  inline constexpr bool nullptr_ = same<T, decltype(nullptr)>;

  template <class T>
  inline constexpr bool ptr = false;

  template <class T>
  inline constexpr bool ptr<T *> = true;

  template <class T>
  inline constexpr bool ptr<T *const> = true;

  template <class T>
  inline constexpr bool ptr<T *volatile> = true;

  template <class T>
  inline constexpr bool ptr<T *const volatile> = true;

  template <class T>
  inline constexpr bool void_ = same<T, void>;

  template <class T>
  inline constexpr bool wchar = same<T, wchar_t>;

  template <class T, class Rep>
  inline constexpr bool existing = false;
  //
  template <class T, template <class...> class Rep, class This, class... Rest>
  inline constexpr bool existing<T, Rep<This, Rest...>> = same<T, This> || existing<T, Rep<Rest...>>;
  // Template unit
  //
  template <class T>
  inline constexpr bool character = ts::is::existing<T, tp::_n<char, signed char, unsigned char, char8_t>>;

  template <class T>
  inline constexpr bool floating_point = ts::is::existing<T, tp::_n<float, double, long double>>;

  template <class T>
  inline constexpr bool integer =
    ts::is::existing<T, tp::_n<bool, char, signed char, unsigned char, char8_t, wchar_t, char16_t, char32_t, short,
                          unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>>;

  template <class T>
  inline constexpr bool arithmetic = integer<T> || floating_point<T>;

  template <class T>
  inline constexpr bool basic_type =
    arithmetic<T> || ptr<T> || norm_func_ptr<T> || member_func_ptr<T> || nullptr_<T> || void_<T>;
}  // namespace xsl::ts::is
#endif  // TOT_H