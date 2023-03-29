#pragma once
#ifndef XSL_TYPE_SUPPORT_PTR
#define XSL_TYPE_SUPPORT_PTR
#include <xsl/bits/def.hpp>
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
  }  // namespace is
  namespace tp {
    // basic template
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
    // Template unit
    namespace impl {
      template <class T, uint_8 Inedx, class V = void>
      struct extract : _1<T> {};
      //
      template <class T>
      struct extract<T, 0, _0<class T::Type>> : _1<typename T::Type> {};
      //
      template <class T>
      struct extract<T, 1, _0<class T::Type1>> : _1<typename T::Type1> {};
      //
      template <class T>
      struct extract<T, 2, _0<class T::Type2>> : _1<typename T::Type2> {};
      //
    }  // namespace impl
    template <class T, uint_8 Index = 0>
    using extract = typename impl::extract<T, Index>::Type;
    // Template unit
    namespace impl {
      template <class Rep>
      struct first;
      //
      template <template <class...> class Rep, class This, class... Rest>
      struct first<Rep<This, Rest...>> : _1<This> {};
    }  // namespace impl
    //
    template <class Rep>
    using first = typename impl::first<Rep>::Type;
    // Template unit
    //
  }  // namespace tp
  namespace impl {
    // template <bool Con>
    // struct conditional {
    //   template <class T, class>
    //   using Type = T;
    // };
    // //
    // template <>
    // struct conditional<false> {
    //   template <class, class T>
    //   using Type = T;
    // };
    //
    template <bool Con, class True, class False>
    struct conditional : tp::_1<True> {};
    //
    template <class True, class False>
    struct conditional<false, True, False> : tp::_1<False> {};

  }  // namespace impl
  template <bool Con, class True, class False>
  using conditional = typename impl::conditional<Con, True, False>::Type;

  template <bool Con, template <class> class Pre, class T>
  using conditional_apply = conditional<Con, Pre<T>, T>;
  // Template unit
  //
  //  template<class...T>
  //  constexpr void unpack(T...) {}
  //  //Template unit
  //  //
  template <class T>
  constexpr T eval_type();
  // Template unit
  //
  namespace has {
    template <class LMBD, class Rep, class V = void>
    constexpr bool some = false;
    //
    template <class LMBD, class... T>
    constexpr bool some<LMBD, tp::_n<T...>, tp::_0<decltype(eval_type<LMBD>()(eval_type<T>()...))>> = true;
    // Dependent Templates	:eval_type
    // Description			:types T have some attributes
  }  // namespace has
  namespace is {
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
  }  // namespace is

  namespace has {
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
  namespace is {
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
  }  // namespace is
  namespace tp {
    namespace impl {
      template <class Rep>
      struct merge_with_rep;
      //
      template <template <class...> class Rep, template <class...> class Left, class... Ts1,
        template <class...> class Right, class... Ts2>
      struct merge_with_rep<Rep<Left<Ts1...>, Right<Ts2...>>> : _2<Left<Ts1..., Ts2...>, Right<Ts1..., Ts2...>> {};
    }  // namespace impl
    //
    template <class Rep>
    using merge_with_rep = typename impl::merge_with_rep<Rep>::Self;
    // Template unit
    //
    template <class LeftRep, class RightRep>
    using merge = merge_with_rep<_2<LeftRep, RightRep>>;
    // Template unit
    //
  }  // namespace tp
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
  namespace nt {
    template <class T, T V1, T V2>
    constexpr T subtraction = V1 - V2;
  }
  namespace tp {
    namespace impl {
      template <class Rep, uint_8 Count>
      struct move_with_rep;
      // if Rep is empty
      template <template <class...> class Left, class... Ts, template <class...> class Right, class This, class... Rest,
        uint_8 Count>
      struct move_with_rep<_2<Left<Ts...>, Right<This, Rest...>>, Count>
        : conditional<Count != 0,
            move_with_rep<_2<Left<Ts..., This>, Right<Rest...>>, nt::subtraction<uint_8, Count, 1>>,
            _2<Left<Ts...>, Right<This, Rest...>>> {};
    }  // namespace impl
    template <class Rep, uint_8 Count>
    using move_with_rep = typename impl::move_with_rep<Rep, Count>::Self;
    // Template unit
    //
    template <class LeftRep, class RightRep, uint_8 Count>
    using move = move_with_rep<_2<LeftRep, RightRep>, Count>;
    // Template unit
    //
    template <class Rep, uint_8 Id>
    using find_by_id_with_rep = first<typename move<_n<>, Rep, Id>::Type2>;
    // Dependent Templates:move
    //
    template <uint_8 Id, class... Ts>
    using find_by_id = first<typename move<_n<>, _n<Ts...>, Id>::Type2>;
    // Dependent Templates:move
    //
    template <class Rep>
    using remove_first = typename move<_n<>, Rep, 1>::Type2;
    // Dependent Templates:move
    //
    template <class Rep, uint_8 Count>
    using remove_front = typename move<_n<>, Rep, Count>::Type2;
    // Dependent Templates:move
    //
    template <class Rep, uint_8 Count>
    using remove_by_id =
      typename merge<class move<_n<>, Rep, Count>::Type1, class move<_n<>, Rep, Count + 1>::Type2>::Type1;
    // Dependent Templates:merge,move
    //
    template <class EdRep, class Er>
    using replace_first = typename merge<_n<Er>, typename move<_n<>, EdRep, 1>::Type2>::Type2;
    // Dependent Templates:merge,move
    //
    template <class LeftRep, class RightRep, uint_8 Count, class LP = move<_n<>, LeftRep, Count>,
      class RP = move<_n<>, RightRep, Count>>
    using swap_front =
      _2<class merge<class RP::Type1, class LP::Type2>::Type2, class merge<class LP::Type1, class RP::Type2>::Type2>;
    // Dependent Templates:merge,move
    //
    template <class Rep>
    inline constexpr uint_8 size = 0;
    //
    template <template <class...> class Rep, class... Ts>
    inline constexpr uint_8 size<Rep<Ts...>> = sizeof...(Ts);
    // Template unit
    //
    template <class Rep, uint_8 Count>
    using remove_back = typename move<_n<>, Rep, size<Rep> - Count>::Type1;
    // Dependent Templates:move,size
    //
    namespace impl {
      template <class Rep, template <class...> class Tl>
      struct swap_template;
      //
      template <template <class...> class Rep, class... Ts, template <class...> class Tl>
      struct swap_template<Rep<Ts...>, Tl> : _1<Tl<Ts...>> {};
    }  // namespace impl
    //
    template <class Rep, template <class...> class Tl>
    using swap_template = typename impl::swap_template<Rep, Tl>::Self;
    // Template unit
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
    using swap = typename impl::swap<LeftRep, RightRep>::Self;
    // Template unit
    //
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
    using repeat_with_rep = typename impl::repeat_with_rep<_2<Rep, _n<>>, Count>::Self::Type;
    // Dependent Templates:swap
    //
    template <uint_8 Count, class... Ts>
    using repeat = repeat_with_rep<_n<Ts...>, Count>;
    // Dependent Templates:swap
    //
    template <class EdRep, class ErRep, uint_8 Count>
    using fill_front_with_rep = typename merge<repeat_with_rep<_2<ErRep, _n<>>, Count>, EdRep>::Type2;
    // Dependent Templates:merge,repeat
    //
    template <class EdRep, uint_8 Count, class... Er>
    using fill_front = fill_front_with_rep<EdRep, _n<Er...>, Count>;
    // Dependent Templates:merge,repeat
    //
    template <class EdRep, class ErRep, uint_8 Count>
    using fill_back_with_rep = typename merge<EdRep, class repeat_with_rep<_2<ErRep, _n<>>, Count>::Type>::Type1;
    // Dependent Templates:merge,repeat
    //
    template <class EdRep, uint_8 Count, class... Er>
    using fill_back = typename merge<EdRep, class repeat_with_rep<_2<_n<Er...>, _n<>>, Count>::Type>::Type1;
    // Dependent Templates:merge,repeat
    //
    namespace impl {
      template <class LeftRep, class RightRep>
      struct swap_first;
      //
      template <template <class...> class Left, class T1, class... Ts1, template <class...> class Right, class T2,
        class... Ts2>
      struct swap_first<Left<T1, Ts1...>, Right<T2, Ts2...>> : _2<Left<T2, Ts1...>, Right<T1, Ts2...>> {};
    }  // namespace impl
    //
    template <class LeftRep, class RightRep>
    using swap_first = typename impl::swap_first<LeftRep, RightRep>::Self;
    // Template unit
    //
    // trace(typeid(fill_front<int, 2, T_s<char>>::Type).name());
    // trace(typeid(fill_front<T_1<int>, 2, T_s<char>>::Type).name());
    // trace(typeid(fill_front<T_s<int>, 2, T_s<char>>::Type).name());
    // trace(typeid(fill_back<int, 2, T_s<char>>::Type).name());
    // trace(typeid(fill_back<T_1<int>, 2, T_s<char>>::Type).name());
    // trace(typeid(fill_back<T_s<int>, 2, T_s<char>>::Type).name());

    template <class Rep, uint_8 Id>
    using split_by_id = typename move<typename swap<Rep, _n<>>::Type1, Rep, Id>::Self;

    // template <class Left, class Right, class T>
    // struct split_by_type_impl;

    // template <template <class...> class Left, class... Ts, template <class...> class Right, class This, class...
    // Rest,
    //   class T>
    // struct split_by_type_impl<Left<Ts...>, Right<This, Rest...>, T>
    //   : conditional<same<T, This>, _2<Left<Ts...>, Right<This, Rest...>>,
    //       split_by_type_impl<Left<Ts..., This>, Right<Rest...>, T>> {};

    // template <class Rep, class T>
    // using split_by_type = typename split_by_type_impl<class swap<Rep, _n<>>::Type1, Rep, T>::Self;
    //
    //
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
    // using get_one_type=type_traits<T,decltype([]<class U>(U)->typename U::one_type{}),S>::Type;
    // if type T has member type one_type, you will get it,or get type S;

  }  // namespace tp
  namespace impl {
    template <bool>
    struct enable : tp::_1<bool> {};

    template <>
    struct enable<false> {};
  }  // namespace impl

  template <bool Con>
  using enable = typename impl::enable<Con>::Type;
  namespace fn {
    namespace impl {
      template <class Class, class Func>
      struct func_add_class;

      template <class Class, class Ret, class... Args>
      struct func_add_class<Class, Ret(Args...)> : tp::_1<Ret (Class::*)(Args...)> {};
    }  // namespace impl
    template <class Class, class Ret, class... Args>
    using func_add_class = typename impl::func_add_class<Class, Ret(Args...)>::Self::Type;

    namespace impl {
      template <class>
      struct func_add_ptr;

      template <class Ret, class... Args>
      struct func_add_ptr<Ret(Args...)> : tp::_1<Ret (*)(Args...)> {};
    }  // namespace impl
    template <class Func>
    using func_add_ptr = typename impl::func_add_ptr<Func>::Self::Type;

    template <class>
    struct func_info;

    template <class _Ret, class... _Args>
    struct func_info<_Ret(_Args...)> {
      using Ret = _Ret;
      using Rep = tp::_n<_Args...>;
    };

    template <class Ret, class... Args>
    struct func_info<Ret (*)(Args...)> : func_info<Ret(Args...)> {};

    template <class Ret, class _Class, class... Args>
    struct func_info<Ret (_Class::*)(Args...)> : func_info<Ret(Args...)> {
      using Class = _Class;
    };

    template <class Ret, class Class, class... Args>
    struct func_info<Ret (Class::*)(Args...) const> : func_info<Ret (Class::*)(Args...)> {};
    namespace impl {
      template <class Func>
      struct func_rm_class : tp::_1<Func> {};

      template <class Class, class Ret, class... Args>
      struct func_rm_class<Ret (Class::*)(Args...)> : tp::_1<Ret (*)(Args...)> {};
    }  // namespace impl

    template <class Func>
    using func_rm_class = typename impl::func_rm_class<Func>::Self::Type;
    namespace impl {
      template <class>
      struct func_rm_ptr;
      //
      template <class Ret, class... Args>
      struct func_rm_ptr<Ret (*)(Args...)> : tp::_1<Ret(Args...)> {};
    }  // namespace impl

    template <class Ret, class... Args>
    using func_rm_ptr = typename impl::func_rm_ptr<Ret (*)(Args...)>::Self::Type;

    template <class, class>
    struct func_swap;

    template <class Ret1, class Class1, class... Args1, class Ret2, class Class2, class... Args2>
    struct func_swap<Ret1 (Class1::*)(Args1...), Ret2 (Class2::*)(Args2...)> {
      using Type1 = tp::_2<Ret2 (Class1::*)(Args1...), Ret1 (Class2::*)(Args2...)>;
      using Type2 = tp::_2<Ret1 (Class2::*)(Args1...), Ret2 (Class1::*)(Args2...)>;
      using Type3 = tp::_2<Ret1 (Class1::*)(Args2...), Ret2 (Class2::*)(Args1...)>;
    };

    template <class Ret1, class Class1, class... Args1, class Ret2, class... Args2>
    struct func_swap<Ret1 (Class1::*)(Args1...), Ret2 (*)(Args2...)> {
      using Type1 = tp::_2<Ret2 (Class1::*)(Args1...), Ret1 (*)(Args2...)>;
      using Type3 = tp::_2<Ret1 (Class1::*)(Args2...), Ret2 (*)(Args1...)>;
    };

    template <class Ret1, class... Args1, class Ret2, class Class2, class... Args2>
    struct func_swap<Ret1 (*)(Args1...), Ret2 (Class2::*)(Args2...)> {
      using Type1 = tp::_2<Ret2 (*)(Args1...), Ret1 (Class2::*)(Args2...)>;
      using Type3 = tp::_2<Ret1 (*)(Args2...), Ret2 (Class2::*)(Args1...)>;
    };

    template <class Ret1, class... Args1, class Ret2, class... Args2>
    struct func_swap<Ret1 (*)(Args1...), Ret2 (*)(Args2...)> {
      using Type1 = tp::_2<Ret2 (*)(Args1...), Ret1 (*)(Args2...)>;
      using Type3 = tp::_2<Ret1 (*)(Args2...), Ret2 (*)(Args1...)>;
    };
    namespace impl {
      template <class, class>
      struct func_swap_args;

      template <class Ret, class... Args1, template <class...> class Rep, class... Args2>
      struct func_swap_args<Ret(Args1...), Rep<Args2...>> : tp::_1<Ret(Args2...)> {};

      template <class Ret, class... Args1, template <class...> class Rep, class... Args2>
      struct func_swap_args<Ret (*)(Args1...), Rep<Args2...>> : tp::_1<Ret (*)(Args2...)> {};

      template <class Ret, class Class, class... Args1, template <class...> class Rep, class... Args2>
      struct func_swap_args<Ret (Class::*)(Args1...), Rep<Args2...>> : tp::_1<Ret (Class::*)(Args2...)> {};
    }  // namespace impl

    template <class Func, class Args>
    using func_swap_args = typename impl::func_swap_args<Func, Args>::Self::Type;
  }  // namespace fn
  namespace is {
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
  // template<class T>
  // struct const_impl :tp::_2<T, const T> { };
  //
  // template<class T>
  // struct const_impl <const T> :tp::_2<T, const T> { };
  //
  // template<class T>
  // using rm_c = const_impl<T>::Type1;
  //
  // template<class T>
  // using as_c = const_impl<T>::Type2;
  namespace impl {
    template <class T>
    struct const_traits : tp::_1<T> {};
    //
    template <class T>
    struct const_traits<const T> : tp::_1<T> {};
  }  // namespace impl
  template <class T>
  using rm_c = typename impl::const_traits<T>::Type;

  template <class T>
  using as_c = const typename impl::const_traits<T>::Type;

  template <class T>
  inline constexpr bool Const = is::same<T, as_c<T>>;

  template <class T>
  struct reference_impl : tp::_1<T> {};

  template <class T>
  struct reference_impl<T&> : tp::_1<T> {};

  template <class T>
  struct reference_impl<T&&> : tp::_1<T> {};

  template <class T>
  using rm_ref = typename reference_impl<T>::Type;

  template <class T>
  using as_lref = typename reference_impl<T>::Type&;

  template <class T>
  using as_rref = typename reference_impl<T>::Type&&;
  namespace is {
    template <class T>
    inline constexpr bool lref = ts::is::same<T, typename reference_impl<T>::Type&>;

    template <class T>
    inline constexpr bool rref = ts::is::same<T, typename reference_impl<T>::Type&&>;

    template <class T>
    inline constexpr bool ref = lref<T> || rref<T>;
  }  // namespace is
  namespace has {
    template <class L, class R>
    inline constexpr bool opeartor_simple_assignment =
      some<decltype([]<class T, class U>(
                      T, U) -> decltype(eval_type<as_lref<T>>() = (eval_type<U>()), 0) { return 0; }),
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
  namespace is {
    template <class T>
    inline constexpr bool func = !ref<T> && (ts::is::same<rm_c<T>, const T> || !ts::is::same<T, T>);
  }
  template <class T, class V = void>
  struct pointer_impl : tp::_2<T, T> {};

  template <class T>
  struct pointer_impl<T, tp::_0<rm_ref<T> *>> : tp::_2<T, rm_ref<T> *> {};

  template <class T>
  struct pointer_impl<T *> : tp::_2<T, T *> {};

  template <class T>
  using rm_p = typename pointer_impl<T>::Type1;

  template <class T, class V = void>
  using as_p = typename pointer_impl<T>::Type2;

  template <class T>
  struct array_impl : tp::_1<T> {};

  template <class T>
  struct array_impl<T[]> : tp::_1<T> {};

  template <class T, size_t Size>
  struct array_impl<T[Size]> : tp::_1<T> {};

  template <class T>
  using rm_e = typename array_impl<T>::Type;

  template <class T>
  inline constexpr bool array = !ts::is::same<T, rm_e<T>>;

  template <class T>
  struct volatile_impl : tp::_1<T> {};

  template <class T>
  struct volatile_impl<volatile T> : tp::_1<T> {};

  template <class T>
  using rm_v = typename volatile_impl<T>::Type;

  template <class T>
  using rm_cv = rm_v<rm_c<T>>;

  template <class T>
  using rm_vr = rm_v<rm_ref<T>>;

  template <class T>
  using rm_cvr = rm_v<rm_c<rm_ref<T>>>;
  namespace impl {
    template <class T>
    struct decay {
      using No_Ref = rm_ref<T>;
      using Type = typename ts::conditional<is::func<No_Ref>, as_p<No_Ref>,
        typename ts::conditional<array<No_Ref>, as_p<rm_e<No_Ref>>, rm_cv<No_Ref>>>;
    };
  }  // namespace impl

  template <class T>
  using decay = typename impl::decay<T>::Type;

  template <class This, class Other>
  using enable_construct = enable<!ts::is::same<This, rm_cvr<Other>>>;

  namespace nt {
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
    using move_with_rep = typename impl::move_with_rep<Rep, Count>::Self;
    //
    // Detached
    template <class LeftRep, class RightRep, uint_8 Count>
    using move = move_with_rep<tp::_2<LeftRep, RightRep>, Count>;
    //
    template <class Rep, uint_8 Count>
    using remove_front = typename move_with_rep<tp::_2<_n<typename Rep::Type>, Rep>, Count>::Type2;
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
    using make_integer_sequence = typename make_integer_sequence_impl<Vars...>::Self;
    namespace impl {
      template <uint_8 Count>
      struct make_index_sequence : nt::make_integer_sequence<Count - 1> {};
      //
      template <>
      struct make_index_sequence<0> : uint8_s<> {};
    }  // namespace impl
    template <uint_8 Count>
    using make_index_sequence = typename impl::make_index_sequence<Count>::Self;
  }  // namespace nt
     /*template<int_t Max, int_t...>
     struct max :var_1<Max> {};
     template<int_t Max, int_t This, int_t...Rest>
     struct max<Max, This, Rest...> :max < This<Max ? Max : This, Rest... > {};
   
   
     template<int_t Min, int_t...>
     struct min :var_1<Min> {};
     template<int_t Min, int_t This, int_t...Rest>
     struct min<Min, This, Rest...> :min < This<Min ? This : Min, Rest... > {};*/
  namespace tp {
    template <class Rep, class NewRep, uint_8 Count>
    struct as_rref_n_impl
      : as_rref_n_impl<remove_first<Rep>, class merge<_n<as_rref<first<Rep>>>, NewRep>::Type2, Count - 1> {};

    template <class Rep, class NewRep>
    struct as_rref_n_impl<Rep, NewRep, 0> : _1<NewRep> {};

    template <class Rep, uint_8 Count>
    using as_rref_n = as_rref_n_impl<Rep, class swap<Rep, _n<>>::Type1, Count>;
  }  // namespace tp
}  // namespace ts
XSL_END
#endif  // TOT_H