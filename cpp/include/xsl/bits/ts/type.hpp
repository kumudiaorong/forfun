#pragma once
#ifndef XSL_TYPE_SUPPORT_TYPE
#define XSL_TYPE_SUPPORT_TYPE
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>

namespace xsl::ts::tp {
  template <class T, class LMBD, class S, class V = void>
  struct type_traits : _1<S> {};
  //
  template <class T, class LMBD, class S>
  struct type_traits<T, LMBD, S, _0<decltype(eval_type<LMBD>()(eval_type<T>()))>>
    : _1<decltype(eval_type<LMBD>()(eval_type<T>()))> {};
  // Dependent Templates	:eval_type
  // Description			:T has member function opeartor*
  // Example				:
  // template<class T,class S>
  // using get_one_type=type_traits<T,decltype([]<class U>(U)->typename U::one_type{}),S>::type;
  // if type T has member type one_type, you will get it,or get type S;
  template <class T>
  using get_val_type = typename ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::val_type {}), T>::type;
  template <class T, class S = T>
  using get_type = typename ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::type {}), S>::type;
  namespace impl {
    template <class Rep>
    struct first;
    //
    template <template <class...> class Rep, class This, class... Rest>
    struct first<Rep<This, Rest...>> : _1<This> {};
  }  // namespace impl
  //
  template <class Rep>
  using first = typename impl::first<Rep>::type;
  // Template unit
  //
  //
  namespace impl {
    template <class Rep, uint_8 Count>
    struct move_with_rep;
    // if Rep is empty
    template <template <class...> class Left, class... Ts, template <class...> class Right, class This, class... Rest,
      uint_8 Count>
    struct move_with_rep<_2<Left<Ts...>, Right<This, Rest...>>, Count>
      : conditional<Count != 0, move_with_rep<_2<Left<Ts..., This>, Right<Rest...>>, nt::subtraction<uint_8, Count, 1>>,
          _2<Left<Ts...>, Right<This, Rest...>>> {};
  }  // namespace impl
  template <class Rep, uint_8 Count>
  using move_with_rep = typename impl::move_with_rep<Rep, Count>::self;
  // Template unit
  //
  template <class LeftRep, class RightRep, uint_8 Count>
  using move = move_with_rep<_2<LeftRep, RightRep>, Count>;
  // Template unit
  //
  template <class Rep, uint_8 Id>
  using find_by_id_with_rep = first<typename move<_n<>, Rep, Id>::type2>;
  // Dependent Templates:move
  //
  template <uint_8 Id, class... Ts>
  using find_by_id = first<typename move<_n<>, _n<Ts...>, Id>::type2>;
  // Dependent Templates:move
  //
  namespace impl {
    template <class LeftRep, class RightRep>
    struct swap;
    //
    template <template <class...> class Left, class... Ts1, template <class...> class Right, class... Ts2>
    struct swap<Left<Ts1...>, Right<Ts2...>> : _2<Left<Ts2...>, Right<Ts1...>> {};
  }  // namespace impl
  //
  template <class LeftRep, class RightRep>
  using swap = typename impl::swap<LeftRep, RightRep>::self;
  template <class Rep, uint_8 Id>
  using split_by_id = typename move<typename swap<Rep, _n<>>::type1, Rep, Id>::self;
  namespace impl {
    template <class Rep, uint_8 Count>
    struct repeat_with_rep;
    //
    template <template <class...> class Rep, class... Part, class... Parts, uint_8 Count>
    struct repeat_with_rep<_2<Rep<Part...>, _n<Parts...>>, Count>
      : conditional<Count != 0,
          repeat_with_rep<_2<Rep<Part...>, _n<Part..., Parts...>>, nt::subtraction<uint_8, Count, 1>>,
          _1<Rep<Parts...>>> {};
  }  // namespace impl
  //
  template <class Rep, uint_8 Count>
  using repeat_with_rep = typename impl::repeat_with_rep<_2<Rep, _n<>>, Count>::type;
  // Dependent Templates:swap
  //
  template <uint_8 Count, class... Ts>
  using repeat = repeat_with_rep<_n<Ts...>, Count>;
  // Dependent Templates:swap
  //
}  // namespace xsl::ts::tp
#endif  // TOT_H