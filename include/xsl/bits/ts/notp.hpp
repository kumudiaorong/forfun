#pragma once
#ifndef XSL_TYPE_SUPPORT_NOTP
#define XSL_TYPE_SUPPORT_NOTP
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>

namespace xsl::ts::nt {
  namespace impl {
    template <class Rep, uint_8 Count>
    struct move_with_rep : Rep {};

    template <class T, template <class, T...> class Left, T... Vs, template <class, T...> class Right, T This,
      T... Rest, uint_8 Count>
    struct move_with_rep<tp::_2<Left<T, Vs...>, Right<T, This, Rest...>>, Count>
      : conditional<Count != 0,
          move_with_rep<tp::_2<Left<T, Vs..., This>, Right<T, Rest...>>, ts::nt::subtraction<uint_8, Count, 1>>,
          tp::_2<Left<T, Vs...>, Right<T, This, Rest...>>> {};
  }  // namespace impl
  template <class Rep, uint_8 Count>
  using move_with_rep = typename impl::move_with_rep<Rep, Count>::self;
  //
  // Detached
  template <class LeftRep, class RightRep, uint_8 Count>
  using move = move_with_rep<tp::_2<LeftRep, RightRep>, Count>;
  //
  template <class Rep, uint_8 Count>
  using remove_front = typename move_with_rep<tp::_2<_n<typename Rep::type>, Rep>, Count>::type2;
  //
  template <class Rep>
  using remove_first = remove_front<Rep, 1>;

  template <uint_8... Vars>
  using uint8_s = _n<uint_8, Vars...>;

  // complex template
  template <uint_8 This, uint_8... Rest>
  struct make_integer_sequence_impl : make_integer_sequence_impl<This - 1, This, Rest...> {};

  template <uint_8... Vars>
  struct make_integer_sequence_impl<0, Vars...> : uint8_s<0, Vars...> {};

  template <uint_8... Vars>
  using make_integer_sequence = typename make_integer_sequence_impl<Vars...>::self;
  namespace impl {
    template <uint_8 Count>
    struct make_index_sequence : nt::make_integer_sequence<Count - 1> {};
    //
    template <>
    struct make_index_sequence<0> : uint8_s<> {};
  }  // namespace impl
  template <uint_8 Count>
  using make_index_sequence = typename impl::make_index_sequence<Count>::self;
}  // namespace xsl::ts::nt
#endif  // TOT_H