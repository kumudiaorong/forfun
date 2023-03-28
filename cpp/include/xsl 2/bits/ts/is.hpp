#pragma once
#ifndef XSL_TYPE_SUPPORT_IS
#define XSL_TYPE_SUPPORT_IS
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
XSL_BEGIN
namespace ts {
  namespace is {
    template <class>
    inline constexpr bool norm_func = false;

    template <class Ret, class... Args>
    inline constexpr bool norm_func<Ret(Args...)> = true;

    template <class>
    inline constexpr bool norm_func_ptr = false;

    template <class Ret, class... Args>
    inline constexpr bool norm_func_ptr<Ret (*)(Args...)> = true;

    template <class>
    inline constexpr bool member_func_ptr = false;

    template <class Ret, class Class>
    inline constexpr bool member_func_ptr<Ret(Class::*)> = true;

    template <class T>
    inline constexpr bool pointer = false;

    template <class T>
    inline constexpr bool pointer<T *> = true;

    template <class T>
    inline constexpr bool pointer<T *const> = true;

    template <class T>
    inline constexpr bool pointer<T *volatile> = true;

    template <class T>
    inline constexpr bool pointer<T *const volatile> = true;

    template <class This, class Another>
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
    inline constexpr bool Nullptr = same<T, decltype(nullptr)>;

    template <class T>
    inline constexpr bool Void = same<T, void>;

    template <class T>
    inline constexpr bool wchar = same<T, wchar_t>;

    template <class This, class Other>
    inline constexpr bool same_template = false;

    template <template <class...> class Rep, class... Ts1, class... Ts2>
    inline constexpr bool same_template<Rep<Ts1...>, Rep<Ts2...>> = true;

    template <class T, class Rep>
    inline constexpr bool existing = false;
    //
    template <class T, template <class...> class Rep, class This, class... Rest>
    inline constexpr bool existing<T, Rep<This, Rest...>> = same<T, This> || existing<T, Rep<Rest...>>;
    // Template unit
    //
    template <class T, class Rep>
    inline constexpr bool existing_template = false;
    //
    template <class T, template <class...> class Rep, class This, class... Rest>
    inline constexpr bool existing_template<T, Rep<This, Rest...>> =
      same_template<T, This> || existing_template<T, Rep<Rest...>>;
    // Template unit
    //
    /*cout << is_character<char> << ' ' << is_character<signed char> << ' ' << is_character<unsigned char> << ' '
    << is_character<short> << ' ' << is_character<unsigned short> << ' ' << is_character<int> << ' ' <<
    is_character<unsigned int> << ' ' << is_character<long> << ' ' << is_character<unsigned long> << ' ' <<
    is_character<long long> << ' ' << is_character<unsigned long long> << ' '
    << is_character<float> << ' ' << is_character<double> << ' ' << is_character<long double> << '\n';
    cout << is_integer<char> << ' ' << is_integer<signed char> << ' ' << is_integer<unsigned char> << ' '
            << is_integer<short> << ' ' << is_integer<unsigned short> << ' ' << is_integer<int> << ' ' <<
    is_integer<unsigned int> << ' ' << is_integer<long> << ' ' << is_integer<unsigned long> << ' ' << is_integer<long
    long>
    << ' ' << is_integer<unsigned long long> << ' '
            << is_integer<float> << ' ' << is_integer<double> << ' ' << is_integer<long double> << '\n';
    cout << is_floating_point<char> << ' ' << is_floating_point<signed char> << ' ' << is_floating_point<unsigned char>
    <<
    '
    '
            << is_floating_point<short> << ' ' << is_floating_point<unsigned short> << ' ' << is_floating_point<int> <<
    '
    '
    << is_floating_point<unsigned int> << ' ' << is_floating_point<long> << ' ' << is_floating_point<unsigned long> << '
    '
    << is_floating_point<long long> << ' ' << is_floating_point<unsigned long long> << ' '
            << is_floating_point<float> << ' ' << is_floating_point<double> << ' ' << is_floating_point<long double> <<
    '\n';
    */
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
    inline constexpr bool basic_type = arithmetic<T> || pointer<T> || member_func_ptr<T> || Nullptr<T> || Void<T>;
  }  // namespace is
}  // namespace ts
XSL_END
#endif  // TOT_H