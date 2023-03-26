#ifndef XSL_TYPE
#define XSL_TYPE
#include"bits/xsl_def.h"
XSL_BEGIN

template<class>
inline constexpr bool is_member_func = false;

template<class Ret, class Class>
inline constexpr bool is_member_func<Ret(Class::*)> = true;


template<class T>
inline constexpr bool is_pointer = false;

template<class T>
inline constexpr bool is_pointer<T*> = true;

template<class T>
inline constexpr bool is_pointer<T* const> = true;

template<class T>
inline constexpr bool is_pointer<T* volatile> = true;

template<class T>
inline constexpr bool is_pointer<T* const volatile> = true;


template<class This, class Another>
inline constexpr bool is_same = false;

template<class T>
inline constexpr bool is_same<T, T> = true;


template<class T>
inline constexpr bool is_char8 = is_same<T, char8_t>;


template<class T>
inline constexpr bool is_char16 = is_same<T, char16_t>;


template<class T>
inline constexpr bool is_char32 = is_same<T, char32_t>;


template<class T>
inline constexpr bool is_nullptr = is_same<T, decltype(nullptr)>;


template<class T>
inline constexpr bool is_void = is_same<T, void>;


template<class T>
inline constexpr bool is_wchar = is_same<T, wchar_t>;


template<class This, class Other>
inline constexpr bool is_same_template = false;

template<template<class...>class Rep, class...Ts1, class...Ts2>
inline constexpr bool is_same_template<Rep<Ts1...>, Rep<Ts2...>> = true;

/*cout << is_character<char> << ' ' << is_character<signed char> << ' ' << is_character<unsigned char> << ' '
<< is_character<short> << ' ' << is_character<unsigned short> << ' ' << is_character<int> << ' ' << is_character<unsigned int> << ' ' << is_character<long> << ' ' << is_character<unsigned long> << ' ' << is_character<long long> << ' ' << is_character<unsigned long long> << ' '
<< is_character<float> << ' ' << is_character<double> << ' ' << is_character<long double> << '\n';
cout << is_integer<char> << ' ' << is_integer<signed char> << ' ' << is_integer<unsigned char> << ' '
	<< is_integer<short> << ' ' << is_integer<unsigned short> << ' ' << is_integer<int> << ' ' << is_integer<unsigned int> << ' ' << is_integer<long> << ' ' << is_integer<unsigned long> << ' ' << is_integer<long long> << ' ' << is_integer<unsigned long long> << ' '
	<< is_integer<float> << ' ' << is_integer<double> << ' ' << is_integer<long double> << '\n';
cout << is_floating_point<char> << ' ' << is_floating_point<signed char> << ' ' << is_floating_point<unsigned char> << ' '
	<< is_floating_point<short> << ' ' << is_floating_point<unsigned short> << ' ' << is_floating_point<int> << ' ' << is_floating_point<unsigned int> << ' ' << is_floating_point<long> << ' ' << is_floating_point<unsigned long> << ' ' << is_floating_point<long long> << ' ' << is_floating_point<unsigned long long> << ' '
	<< is_floating_point<float> << ' ' << is_floating_point<double> << ' ' << is_floating_point<long double> << '\n';
*/

namespace type {
	//
	template<class...Ts>
	using _0 = void;

	//basic template
	template<class T>
	struct _1 {
		using Type = T;
		using Self = _1;
	};


	template<class T1, class T2>
	struct _2 {
		using Type1 = T1;
		using Type2 = T2;
		using Self = _2;
	};


	template<class T1, class T2, class T3>
	struct _3 {
		using Type1 = T1;
		using Type2 = T2;
		using Type3 = T3;
		using Self = _3;
	};


	template<class...Ts>
	struct _n {
		using Self = _n;
	};


	//template<class T, class V = void>
	//inline constexpr bool Has_KKK = false;

	//template<class T>
	//inline constexpr bool Has_KKK<T, _0<class T::KKK>> = true;


	/*template<class Left, class Right>
	struct back_from_front;

	template<template<class...>class Left, class...Ts, template<class...>class Right, class This, class...Rest>
	struct back_from_front<Left<Ts...>, Right<This, Rest...>> :_2<Left<Ts..., This>, Right<Rest...>> { };*/

	template<bool Con, class True, class False>
	struct conditional_impl;

	template<class True, class False>
	struct conditional_impl<true, True, False> :_1<True> { };

	template<class True, class False>
	struct conditional_impl<false, True, False> :_1<False> { };

	template<bool Con, class True, class False>
	using conditional = conditional_impl<Con, True, False>::Type;

	//Whole
	template<class Rep, uint_8 Count>
	struct move_in_rep_impl;

	template<template<class, class>class Rep, template<class...>class Left, class...Ts, template<class...>class Right, class This, class...Rest, uint_8 Count>
	struct move_in_rep_impl<Rep<Left<Ts...>, Right<This, Rest...>>, Count> :conditional<
		Count - 1,
		move_in_rep_impl<_2<Left<Ts..., This>, Right<Rest...>>, Count - 1>,
		_2<Left<Ts...>, Right<This, Rest...>>
	> { };
	//
	template<class Rep, uint_8 Count>
	using move_in_rep = move_in_rep_impl<Rep, Count>::Self;

	//Detached
	template<class LeftRep, class RightRep, uint_8 Count>
	using move = move_in_rep_impl<_2<LeftRep, RightRep>, Count>::Self;


	template<class Rep>
	struct merge_in_rep_impl;

	template<template<class...>class Rep, template<class...>class Left, class...Ts1, template<class...>class Right, class...Ts2>
	struct merge_in_rep_impl<Rep<Left<Ts1...>, Right<Ts2...>>> :_2<Left<Ts1..., Ts2...>, Right<Ts1..., Ts2...>> { };

	template<class Rep>
	using merge_in_rep = merge_in_rep_impl<Rep>::Self;
	template<class LeftRep, class RightRep>
	using merge = merge_in_rep_impl<_2<LeftRep, RightRep>>::Self;


	template<class Rep>
	struct first_impl;

	template<template<class...>class Rep, class This, class...Rest>
	struct first_impl<Rep<This, Rest...>> :_1<This> { };

	template<class Rep>
	using first = first_impl<Rep>::Type;


	template<class Rep>
	inline constexpr uint_8 size = 0;

	template<template<class...>class Rep, class...Ts>
	inline constexpr uint_8 size<Rep<Ts...>> = sizeof...(Ts);


	template<class LeftRep, class RightRep>
	struct swap_impl;

	template<template<class...>class Left, class...Ts1, template<class...>class Right, class...Ts2>
	struct swap_impl<Left<Ts1...>, Right<Ts2...>> :_2<Left<Ts2...>, Right<Ts1...>> { };

	template<class LeftRep, class RightRep>
	using swap = swap_impl<LeftRep, RightRep>::Self;


	template<class LeftRep, class RightRep>
	struct swap_first_impl;

	template<template<class...>class Left, class T1, class...Ts1, template<class...>class Right, class T2, class...Ts2>
	struct swap_first_impl<Left<T1, Ts1...>, Right<T2, Ts2...>> :_2<Left<T2, Ts1...>, Right<T1, Ts2...>> { };

	template<class LeftRep, class RightRep>
	using swap_first = swap_first_impl<LeftRep, RightRep>::Self;

	//left parts,right parts
	template<class LeftRep, class RightRep, uint_8 Count, class LP = move< _n<>, LeftRep, Count>, class RP = move<_n<>, RightRep, Count>>
	using swap_front = _2<class merge<class RP::Type1, class LP::Type2>::Type2, class merge<class LP::Type1, class RP::Type2>::Type2>;


	template<class Rep, uint_8 Id>
	using find_by_id = first<class move<_n<>, Rep, Id>::Type2>;


	template<class T, class Rep>
	inline constexpr bool is_existing = false;

	template<class T, template<class...>class Rep, class This, class...Rest>
	inline constexpr bool is_existing<T, Rep<This, Rest...>> = is_same<T, This> || is_existing<T, Rep<Rest...>>;


	template<class T, class Rep>
	inline constexpr bool is_existing_template = false;

	template<class T, template<class...>class Rep, class This, class...Rest>
	inline constexpr bool is_existing_template<T, Rep<This, Rest...>> = is_same_template<T, This> || is_existing_template<T, Rep<Rest...>>;


	template<class Rep>
	using remove_first = class move<_n<>, Rep, 1>::Type2;


	template<class Rep, uint_8 Count>
	using remove_front = class move<_n<>, Rep, Count>::Type2;


	template<class Rep, uint_8 Count>
	using remove_by_id = class merge<class move<_n<>, Rep, Count>::Type1, class move<_n<>, Rep, Count + 1>::Type2>::Type1;


	template<class EdRep, class Er>
	using replace_first = class merge<_n<Er>, remove_first<EdRep>>::Type2;


	template<class Rep, class NewRep, uint_8 Count>
	struct repeat_impl :conditional<Count - 1, repeat_impl<Rep, class merge<NewRep, Rep>::Type1, Count - 1>, _1<NewRep>> { };

	template<class Rep, uint_8 Count>
	using  repeat = repeat_impl<Rep, class swap<Rep, _n<>>::Type1, Count>::Type;


	template<class EdRep, class ErRep, uint_8 Count>
	using fill_front = class merge<repeat<ErRep, Count>, EdRep>::Type2;


	template<class FilledRep, class FillerRep, uint_8 Count>
	using fill_back = class merge<FilledRep, repeat<FillerRep, Count>>::Type1;

	//trace(typeid(fill_front<int, 2, T_s<char>>::Type).name());
	//trace(typeid(fill_front<T_1<int>, 2, T_s<char>>::Type).name());
	//trace(typeid(fill_front<T_s<int>, 2, T_s<char>>::Type).name());
	//trace(typeid(fill_back<int, 2, T_s<char>>::Type).name());
	//trace(typeid(fill_back<T_1<int>, 2, T_s<char>>::Type).name());
	//trace(typeid(fill_back<T_s<int>, 2, T_s<char>>::Type).name());

	template<class Rep, uint_8 Id>
	using split_by_id = move<class swap<Rep, _n<>>::Type1, Rep, Id>;


	template<class Left, class Right, class T, class P = move<Left, Right, 1>>
	struct split_by_type :conditional <!is_same<T, first<Right>> && (size<Right> > 1), class swap_front<split_by_type<_n<>, _n<void>, T>, class move<Left, Right, 1>::Self, 2>::Type1, _2<Left, Right >> {};
}

template<bool>
struct enable_impl :type::_1<bool> { };

template<>
struct enable_impl<false> { };

template<bool Con>
using enable = enable_impl<Con>::Type;



template<class Class, class Func>
struct func_add_class_impl;

template<class Class, class Ret, class...Args>
struct func_add_class_impl<Class, Ret(Args...)> :type::_1<Ret(Class::*)(Args...)> { };

template<class Class, class Ret, class...Args>
using func_add_class = func_add_class_impl<Class, Ret(Args...)>::Type;


template<class>
struct func_add_ptr_impl;

template<class Ret, class...Args>
struct func_add_ptr_impl<Ret(Args...)> :type::_1<Ret(*)(Args...)> { };

template<class Func>
using func_add_ptr = func_add_ptr_impl<Func>::Type;


template<class>
struct func_info;

template<class _Ret, class..._Args>
struct func_info<_Ret(_Args...)> {
	using Ret = _Ret;
	using Rep = type::_n<_Args...>;
};

template<class Ret, class...Args>
struct func_info<Ret(*)(Args...)> :func_info<Ret(Args...)> { };

template<class Ret, class _Class, class...Args>
struct func_info<Ret(_Class::*)(Args...)> :func_info<Ret(Args...)> {
	using Class = _Class;
};

template<class Ret, class Class, class...Args>
struct func_info<Ret(Class::*)(Args...)const> :func_info<Ret(Class::*)(Args...)> { };


template<class Func>
struct func_rm_class_impl :type::_1<Func> { };

template<class Class, class Ret, class...Args>
struct func_rm_class_impl <Ret(Class::*)(Args...)> :type::_1<Ret(*)(Args...)> { };

template<class Func>
using func_rm_class = func_rm_class_impl<Func>::Type;


template<class>
struct func_rm_ptr_impl;

template<class Ret, class...Args>
struct func_rm_ptr_impl<Ret(*)(Args...)> :type::_1<Ret(Args...)> { };

template<class Ret, class...Args>
using func_rm_ptr = func_rm_ptr_impl<Ret(*)(Args...)>::Type;


template<class, class>
struct func_swap;

template<class Ret1, class Class1, class...Args1, class Ret2, class Class2, class...Args2>
struct func_swap<Ret1(Class1::*)(Args1...), Ret2(Class2::*)(Args2...)> {
	using Type1 = type::_2<Ret2(Class1::*)(Args1...), Ret1(Class2::*)(Args2...)>;
	using Type2 = type::_2<Ret1(Class2::*)(Args1...), Ret2(Class1::*)(Args2...)>;
	using Type3 = type::_2<Ret1(Class1::*)(Args2...), Ret2(Class2::*)(Args1...)>;
};

template<class Ret1, class Class1, class...Args1, class Ret2, class...Args2>
struct func_swap<Ret1(Class1::*)(Args1...), Ret2(*)(Args2...)> {
	using Type1 = type::_2<Ret2(Class1::*)(Args1...), Ret1(*)(Args2...)>;
	using Type3 = type::_2<Ret1(Class1::*)(Args2...), Ret2(*)(Args1...)>;
};

template<class Ret1, class...Args1, class Ret2, class Class2, class...Args2>
struct func_swap<Ret1(*)(Args1...), Ret2(Class2::*)(Args2...)> {
	using Type1 = type::_2<Ret2(*)(Args1...), Ret1(Class2::*)(Args2...)>;
	using Type3 = type::_2<Ret1(*)(Args2...), Ret2(Class2::*)(Args1...)>;
};

template<class Ret1, class...Args1, class Ret2, class...Args2>
struct func_swap<Ret1(*)(Args1...), Ret2(*)(Args2...)> {
	using Type1 = type::_2<Ret2(*)(Args1...), Ret1(*)(Args2...)>;
	using Type3 = type::_2<Ret1(*)(Args2...), Ret2(*)(Args1...)>;
};


template<class, class>
struct func_swap_args_impl;

template<class Ret, class...Args1, template<class...> class Rep, class...Args2>
struct func_swap_args_impl<Ret(Args1...), Rep<Args2...>> :type::_1<Ret(Args2...)> { };

template<class Ret, class...Args1, template<class...> class Rep, class...Args2>
struct func_swap_args_impl<Ret(*)(Args1...), Rep<Args2...>> :type::_1<Ret(*)(Args2...)> { };

template<class Ret, class Class, class...Args1, template<class...> class Rep, class...Args2>
struct func_swap_args_impl<Ret(Class::*)(Args1...), Rep<Args2...>> :type::_1<Ret(Class::*)(Args2...)> { };

template<class Func, class Args>
using func_swap_args = func_swap_args_impl<Func, Args>::Type;

template<class T>
inline constexpr bool is_character = type::is_existing<T, type::_n<char, signed char, unsigned char, char8_t>>;


template<class T>
inline constexpr bool is_floating_point = type::is_existing <T, type::_n<float, double, long double>>;


template<class T>
inline constexpr bool is_integer = type::is_existing<T, type::_n<bool, char, signed char, unsigned char, char8_t, wchar_t, char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>>;


template<class T>
inline constexpr bool is_arithmetic = is_integer<T> || is_floating_point<T>;


template<class T>
inline constexpr bool is_basic_type = is_arithmetic<T> || is_pointer<T> || is_member_func<T> || is_nullptr<T> || is_void<T>;


template<class T>
struct pointer_impl :type::_2<T, T*> { };

template<class T>
struct pointer_impl <T*> :type::_2<T, T*> { };

template<class T>
using as_p = pointer_impl <T>::Type1;

template<class T>
using rm_p = pointer_impl <T>::Type2;


template<class T>
struct const_impl :type::_2<const T, T> { };

template<class T>
struct const_impl <const T> :type::_2<const T, T> { };

template<class T>
using as_c = const_impl<T>::Type1;

template<class T>
using rm_c = const_impl<T>::Type2;


template <class T, class V = void>
struct reference_impl :type::_3<T, T, T> { };

template <class T>
struct reference_impl<T, type::_0<T&, T&&>> :type::_3<T, T&, T&&> { };

template <class T>
struct reference_impl<T&> :type::_3<T, T&, T&&> { };

template <class T>
struct reference_impl<T&&> :type::_3<T, T&, T&&> { };

template<class T>
using as_lref = reference_impl<T>::Type2;

template<class T>
using as_rref = reference_impl<T>::Type3;

template<class T>
using rm_ref = reference_impl<T>::Type1;


template<class>
inline constexpr bool is_lref = false;

template<class T>
inline constexpr bool is_lref<T&> = true;


template<class>
inline constexpr bool is_rref = false;

template<class T>
inline constexpr bool is_rref<T&&> = true;


template<class T>
struct volatile_impl :type::_1<T> { };

template<class T>
struct volatile_impl<volatile T> :type::_1<T> { };

template<class T>
using rm_volatile = volatile_impl<T>::Type;


template<class T>
using rm_cvr = rm_volatile<rm_c<rm_ref<T>>>;

template<class This, class Other>
using enable_construct = enable<!is_same<This, rm_cvr<Other>>>;


namespace var {

	//basic template
	template<class T, T V>
	struct _1 :type::_1<T> {
		using Self = _1;
	};

	template<class T, T V1, T V2>
	struct _2 :type::_1<T> {
		using Self = _2;
	};

	template<class T, T...Vars>
	struct _n :type::_1<T> { };


	template<class Rep, uint_8 Count>
	struct move_in_rep_impl;

	template<class T, template<class, class>class Rep, template<class, T...>class Left, T...Vs, template<class, T...>class Right, T This, T...Rest, uint_8 Count>
	struct move_in_rep_impl<Rep<Left<T, Vs...>, Right<T, This, Rest...>>, Count> :type::conditional<
		Count - 1,
		move_in_rep_impl<type::_2<Left<T, Vs..., This>, Right<T, Rest...>>, Count - 1>,
		type::_2<Left<T, Vs...>, Right<T, This, Rest...>>
	> { };

	template<class Rep, uint_8 Count>
	using move_in_rep = move_in_rep_impl<Rep, Count>::Self;
	//
	//Detached
	template<class LeftRep, class RightRep, uint_8 Count>
	using move = move_in_rep<type::_2<LeftRep, RightRep>, Count>;


	template<uint_8...Vars>
	using uint8_s = _n<uint_8, Vars...>;


	//complex template
	template<uint_8 This, uint_8...Rest>
	struct make_integer_sequence :make_integer_sequence<This - 1, This, Rest...> { };

	template<uint_8...Vars>
	struct make_integer_sequence<0, Vars...> :uint8_s<0, Vars...> { };


	template<class Rep>
	using remove_first = move<_n<class Rep::Type>, Rep, 1>;


	template<uint_8 End>
	using make_index_sequence = type::conditional<bool(End), make_integer_sequence<End - 1>, uint8_s<>>;

	/*template<int_t Max, int_t...>
	struct max :var_1<Max> {};
	template<int_t Max, int_t This, int_t...Rest>
	struct max<Max, This, Rest...> :max < This<Max ? Max : This, Rest... > {};


	template<int_t Min, int_t...>
	struct min :var_1<Min> {};
	template<int_t Min, int_t This, int_t...Rest>
	struct min<Min, This, Rest...> :min < This<Min ? This : Min, Rest... > {};*/

}




namespace type {
	template<class Rep, class NewRep, uint_8 Count>
	struct as_rref_n_impl :as_rref_n_impl<remove_first<Rep>, class merge<_n<as_rref<first<Rep>>>, NewRep>::Type2, Count - 1> { };

	template<class Rep, class NewRep>
	struct as_rref_n_impl<Rep, NewRep, 0> :_1<NewRep> { };

	template<class Rep, uint_8 Count>
	using as_rref_n = as_rref_n_impl <Rep, class swap<Rep, _n<>>::Type1, Count>;
}
XSL_END
#endif//XSL_TYPE