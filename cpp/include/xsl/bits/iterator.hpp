#pragma once
#ifndef XSL_ITERATOR
#define XSL_ITERATOR
#include <xsl/bits/compare.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts.hpp>
#include <xsl/bits/utility.hpp>

XSL_BEGIN
//

class forward_iter {};
class reverse_iter {};
class bilateral_iter {};
class random_iter {};

// template<class _Ctr>
// class basic_iterator;
//
// template<template<class...>class _Ctr, class...Ts>
// class basic_iterator<_Ctr<Ts...>> {
// public:
//	using Ctr_T = _Ctr<Ts...>;
//	using GPtr_T = Ctr_T::GPtr_T;
//	using Val_T = Ctr_T::Val_T;
//	constexpr basic_iterator(GPtr_T&& gptr) : GPtr(gptr) {}
//	constexpr basic_iterator(const GPtr_T& gptr) : GPtr(gptr) {}
//	constexpr basic_iterator(basic_iterator&&) = default;
//	constexpr basic_iterator(const basic_iterator&) = default;
//	constexpr basic_iterator& operator=(basic_iterator&& Ano) {
//		GPtr = as_rreference(Ano.GPtr);
//		return *this;
//	}
//	constexpr basic_iterator& operator=(const basic_iterator& Ano) {
//		GPtr = Ano.GPtr;
//		return *this;
//	}
//	constexpr decltype(auto) operator*() const {
//		return *GPtr;
//	}
//	constexpr decltype(auto) operator->() const {
//		return &*GPtr;
//	}
//	constexpr bool operator==(const basic_iterator& iter)const {
//		return GPtr == iter.GPtr;
//	}
//	constexpr bool operator!=(const basic_iterator& iter)const {
//		return !(GPtr == iter.GPtr);
//	}
//	constexpr operator GPtr_T& () {
//		return GPtr;
//	}
//	constexpr operator const GPtr_T& ()const {
//		return GPtr;
//	}
// protected:
//	friend class _Ctr<Ts...>;
//	GPtr_T GPtr;
// };
//
//
// template<class _Ctr>
// class forward_iterator :public basic_iterator<_Ctr> {
// public:
//	using DirBase = basic_iterator<_Ctr>;
//	using Base = basic_iterator<_Ctr>;
//	using Ctr_T = DirBase::Ctr_T;
//	using GPtr_T = DirBase::GPtr_T;
//	using Val_T = DirBase::Val_T;
//	using MNR_T = GPtr_T;
//	using DirBase::DirBase;
//	//using DirBase::operator =;
//	constexpr forward_iterator& operator++() {
//		++GPtr;
//		return *this;
//	}
//	constexpr forward_iterator operator++(int) {
//		GPtr_T Tmp = GPtr;
//		++GPtr;
//		return { Tmp };
//	}
// protected:
//	using DirBase::GPtr;
// };
//
// template<class _Ctr>
// class forward_bilateral_iterator :public forward_iterator<_Ctr> {
// public:
//	using DirBase = forward_iterator<_Ctr>;
//	using Base = DirBase::Base;
//	using Ctr_T = DirBase::Ctr_T;
//	using GPtr_T = DirBase::GPtr_T;
//	using Val_T = DirBase::Val_T;
//	using MNR_T = DirBase::MNR_T;
//	using DirBase::DirBase;
//	constexpr forward_bilateral_iterator& operator--() {
//		--GPtr;
//		return *this;
//	}
//	constexpr forward_bilateral_iterator operator--(int) {
//		GPtr_T Tmp = GPtr;
//		--GPtr;
//		return { Tmp };
//	}
// protected:
//	using DirBase::GPtr;
// };
//
////iterator tag,container
// template<class _Ctr>
// class reverse_iterator :public basic_iterator<_Ctr> {
// public:
//	using DirBase = basic_iterator<_Ctr>;
//	using Base = basic_iterator<_Ctr>;
//	using Ctr_T = DirBase::Ctr_T;
//	using GPtr_T = DirBase::GPtr_T;
//	using Val_T = DirBase::Val_T;
//	using MNR_T = reverse_iterator;
//	using DirBase::DirBase;
//	constexpr reverse_iterator& operator++() {
//		--GPtr;
//		return *this;
//	}
//	constexpr reverse_iterator  operator++(int) {
//		GPtr_T Tmp = GPtr;
//		--GPtr;
//		return { Tmp };
//	}
// protected:
//	using DirBase::GPtr;
// };
//
// template<class _Ctr>
// class reverse_bilateral_iterator :public reverse_iterator<_Ctr> {
// public:
//	using DirBase = reverse_iterator<_Ctr>;
//	using Base = DirBase::Base;
//	using Ctr_T = DirBase::Ctr_T;
//	using GPtr_T = DirBase::GPtr_T;
//	using Val_T = DirBase::Val_T;
//	using MNR_T = DirBase::MNR_T;
//	using DirBase::DirBase;
//	constexpr reverse_bilateral_iterator& operator--() {
//		++GPtr;
//		return *this;
//	}
//	constexpr reverse_bilateral_iterator  operator--(int) {
//		GPtr_T Tmp = GPtr;
//		++GPtr;
//		return { Tmp };
//	}
// protected:
//	using DirBase::GPtr;
// };
//
//
// template<class _Ctr>
// class random_forward_bilateral_iterator :public
// forward_bilateral_iterator<_Ctr> {
//	//direct base;
// public:
//	using DirBase = forward_bilateral_iterator<_Ctr>;
//	using Base = DirBase::Base;
//	using Ctr_T = DirBase::Ctr_T;
//	using GPtr_T = DirBase::GPtr_T;
//	using Val_T = DirBase::Val_T;
//	using MNR_T = DirBase::MNR_T;
//	using DirBase::DirBase;
//	//
//	constexpr random_forward_bilateral_iterator operator+(int_64 Off) {
//		return{ GPtr + Off };
//	}
//	constexpr random_forward_bilateral_iterator& operator+=(int_64 Off) {
//		GPtr = GPtr + Off;
//		return *this;
//	}
//	constexpr random_forward_bilateral_iterator operator-(int_64 Off) {
//		return{ GPtr - Off };
//	}
//	constexpr random_forward_bilateral_iterator& operator-=(int_64 Off) {
//		GPtr = GPtr - Off;
//		return *this;
//	}
// protected:
//	using DirBase::GPtr;
// };
//
// template<class _Ctr>
// class random_reverse_bilateral_iterator :public reverse_bilateral_iterator
// <_Ctr> {
//	//direct base;
// public:
//	using DirBase = reverse_bilateral_iterator<_Ctr>;
//	using Base = DirBase::Base;
//	using Ctr_T = DirBase::Ctr_T;
//	using GPtr_T = DirBase::GPtr_T;
//	using Val_T = DirBase::Val_T;
//	using MNR_T = DirBase::MNR_T;
//	using DirBase::DirBase;
//	constexpr random_reverse_bilateral_iterator  operator+(int_64 Off) {
//		return{ GPtr + Off };
//	}
//	constexpr random_reverse_bilateral_iterator& operator+=(int_64 Off) {
//		GPtr = GPtr + Off;
//		return *this;
//	}
//	constexpr random_reverse_bilateral_iterator  operator-(int_64 Off) {
//		return{ GPtr - Off };
//	}
//	constexpr random_reverse_bilateral_iterator& operator-=(int_64 Off) {
//		GPtr = GPtr - Off;
//		return *this;
//	}
// protected:
//	using DirBase::GPtr;
// };

// template<class Unchecked_Iter>
// inline constexpr bool is_xsl_iter = false;
////
// template<template<class>class XSL_Iter, class Ctr>
// inline constexpr bool is_xsl_iter<XSL_Iter<Ctr>> =
// type::is_existing<XSL_Iter<Ctr>, 	type::_n < 	basic_iterator<Ctr>,
//	forward_iterator<Ctr>,
//	forward_bilateral_iterator<Ctr>,
//	random_forward_bilateral_iterator<Ctr>,
//	reverse_iterator<Ctr>,
//	reverse_bilateral_iterator<Ctr>,
//	random_reverse_bilateral_iterator<Ctr>
//	>
//>;
// template<class Unchecked_Iter>
// inline constexpr bool is_xsl_random_iter = false;
////
// template<template<class>class XSL_Iter, class Ctr>
// inline constexpr bool is_xsl_random_iter <XSL_Iter<Ctr>> =
// type::is_existing<XSL_Iter<Ctr>, 	type::_n <
//	random_forward_bilateral_iterator<Ctr>,
//	random_reverse_bilateral_iterator<Ctr>
//	>
//>;
// template<class Unchecked_Iter>
// inline constexpr bool is_random_iter = false;
////
// template<template<class>class XSL_Iter, class Ctr>
// inline constexpr bool is_random_iter<XSL_Iter<Ctr>> =
// is_xsl_random_iter<XSL_Iter<Ctr>> && type::is_existing<XSL_Iter<Ctr>,
//	type::_n <
//	random_forward_bilateral_iterator<Ctr>,
//	random_reverse_bilateral_iterator<Ctr>
//	>
//>;
//
//

template <class T>
class pointer_wrapper {
public:
  // clang-format off
	typedef ts::tp::_n<forward_iter, bilateral_iter, random_iter> iter_category;
	typedef T* 													                        mnr_type;
	typedef pointer_wrapper 									                  self_type;
	typedef T		 											                          val_type;
	typedef std::weak_ordering 									                comp_category;
  // clang-format on
  T *Ptr;
  constexpr pointer_wrapper()
    : Ptr(nullptr) {}
  constexpr pointer_wrapper(T *ptr)
    : Ptr(ptr) {}
  template <class Other, ts::enable<ts::is::constructible<T *, Other *>> = 0>
  constexpr pointer_wrapper(pointer_wrapper<Other> ano)
    : Ptr(ano.Ptr) {}
  constexpr operator mnr_type() { return Ptr; }
  constexpr T& operator*() const { return *Ptr; }
  constexpr T *operator->() const { return Ptr; }
  constexpr pointer_wrapper& operator++() {
    ++Ptr;
    return *this;
  }
  constexpr pointer_wrapper operator++(int) {
    T *Tmp = Ptr;
    ++Ptr;
    return {Tmp};
  }
  constexpr pointer_wrapper& operator--() {
    --Ptr;
    return *this;
  }
  constexpr pointer_wrapper operator--(int) {
    T *Tmp = Ptr;
    --Ptr;
    return {Tmp};
  }
  constexpr comp_category compare(const pointer_wrapper& ano) const { return Ptr <=> ano.Ptr; }
  constexpr bool operator==(const pointer_wrapper& ano) const { return Ptr == ano.Ptr; }
  constexpr bool operator!=(const pointer_wrapper& ano) const { return Ptr != ano.Ptr; }
};
template <class T>
constexpr typename comp_traits<pointer_wrapper<T>>::comp_category operator<=>(
  const pointer_wrapper<T>& l, const pointer_wrapper<T>& r) {
  return l.compare(r) <=> 0;
}
// iter_traits
template <class T>
struct get_iter_category
  : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::iter_category {}),
      ts::tp::_n<>>::Self {};
// gcc would not think using statement is a type,if using typedef,it would
// become int
//
template <class T>
struct get_val_type
  : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::val_type {}), T>::Self {};
//
//
template <class T>
struct get_mnr_type
  : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::mnr_type {}), T>::Self {};
//
//
// minimum requirements
template <class CheckedIter>
struct iter_traits {
  // clang-format off
	typedef typename get_iter_category<CheckedIter>::Type	iter_category;
	typedef typename get_mnr_type<CheckedIter>::Type 		  mnr_type;
	typedef CheckedIter 							                    self_type;
	typedef typename get_val_type<CheckedIter>::Type 		  val_type;
  // clang-format on
};
template <class Val>
struct iter_traits<Val *> {
  // clang-format off
	typedef ts::tp::_n<forward_iter, bilateral_iter, random_iter>	iter_category;
	typedef Val* 												                        mnr_type;
	typedef Val* 												                        self_type;
	typedef ts::rm_cvr<Val> 										                    val_type;
  // clang-format on
};
//

template <class _Iter>
class reverse_iterator {
public:
  // clang-format off
	typedef std::weak_ordering 														                          comp_category;
	typedef reverse_iterator 														                            self_type;
	typedef iter_traits<_Iter> 														                          traits_type;
	typedef ts::tp::replace_first<typename traits_type::iter_category, reverse_iter> 	iter_category;
	typedef reverse_iterator<typename traits_type::mnr_type> 						            mnr_type;
	typedef typename traits_type::val_type 													                val_type;
	typedef _Iter 																	                                wrap_type;
  // clang-format on
  constexpr reverse_iterator() = default;
  constexpr reverse_iterator(_Iter iter)
    : Iter(as_rreference(iter)) {}
  template <class Other, ts::enable<ts::is::constructible<_Iter, Other>> = 0>
  constexpr reverse_iterator(const reverse_iterator<Other>& ano)
    : Iter(as_rreference(ano.unwrap())) {}
  template <class Other, ts::enable<ts::is::constructible<_Iter, Other>> = 0>
  constexpr reverse_iterator& operator=(const reverse_iterator<Other>& Ano) {
    Iter = Ano.Iter;
    return *this;
  }
  constexpr val_type& operator*() const { return *Iter; }
  constexpr decltype(auto) operator->() const { return XSL addr(*Iter); }
  constexpr bool operator==(const reverse_iterator& ano) const { return Iter == ano.Iter; }
  constexpr bool operator!=(const reverse_iterator& ano) const { return !(Iter == ano.Iter); }
  constexpr reverse_iterator& operator++() {
    --Iter;
    return *this;
  }
  constexpr reverse_iterator operator++(int) {
    _Iter Tmp = Iter;
    --Iter;
    return {Tmp};
  }
  constexpr reverse_iterator& operator--() {
    ++Iter;
    return *this;
  }
  constexpr reverse_iterator operator--(int) {
    _Iter Tmp = Iter;
    ++Iter;
    return {Tmp};
  }
  constexpr reverse_iterator operator+(int_64 Off) { return {Iter - Off}; }
  constexpr reverse_iterator& operator+=(int_64 Off) {
    Iter = Iter - Off;
    return *this;
  }
  constexpr reverse_iterator operator-(int_64 Off) { return {Iter + Off}; }
  constexpr reverse_iterator& operator-=(int_64 Off) {
    Iter = Iter + Off;
    return *this;
  }
  constexpr wrap_type unwrap() const { return Iter; }
protected:
  _Iter Iter;
};

template <class UCIter>
inline constexpr bool has_iterator_operator =
  ts::has::opeartor_pre_increment<UCIter> && ts::has::opeartor_pre_increment<UCIter>
  && ts::has::opeartor_pre_increment<UCIter>;
//
template <class UCIter>
inline constexpr bool is_iter = ts::is::pointer<UCIter> || has_iterator_operator<UCIter>;
//
//
template <class UCIter>
constexpr bool is_forward_iter =
  is_iter<UCIter> && ts::is::existing<forward_iter, typename iter_traits<UCIter>::iter_category>;
//
template <class UCIter>
constexpr bool is_reverse_iter =
  is_iter<UCIter> && ts::is::existing<reverse_iter, typename iter_traits<UCIter>::iter_category>;
//
template <class UCIter>
constexpr bool is_bilateral_iter =
  is_iter<UCIter> && ts::is::existing<bilateral_iter, typename iter_traits<UCIter>::iter_category>;
//
template <class UCIter>
constexpr bool is_random_iter =
  is_iter<UCIter> && ts::is::existing<random_iter, typename iter_traits<UCIter>::iter_category>;
//
template <class CIter>
constexpr typename iter_traits<ts::rm_cvr<CIter>>::mnr_type get_mnr_iter(CIter Iter) {
  return Iter;
}
//
template <class CIter>
constexpr CIter jump(CIter Iter, size_t Size) {
  if constexpr(is_random_iter<CIter>)
    Iter = Iter + Size;
  else
    while(Size != 0) {
      ++Iter;
      --Size;
    }
  return as_rreference(Iter);
}
//
template <class CIter, class Size_Type>
constexpr decltype(auto) get_forward_iter(CIter&& Iter, Size_Type Size = 0) {
  if(is_bilateral_iter<CIter> || Size == 0) {
    if constexpr(is_random_iter<CIter>)
      return (Iter - Size).base();
    else {
      while(Size-- != 0)
        --Iter;
      return Iter.base();
    }
  } else
    throw "error";
  return Iter;
}
//
template <class CIter, class SizeType>
constexpr CIter get_tar_iter(CIter&& Iter, SizeType Count) {
  if constexpr(ts::has::opeartor_addition<ts::rm_vr<CIter>, SizeType>)
    return Iter + Count;
  else if constexpr(ts::has::opeartor_addition_assignment<ts::rm_vr<CIter>, SizeType>)
    return Iter += Count;
  else {
    while(Count-- > 0)
      ++Iter;
    return Iter;
  }
}
// target
XSL_END
#endif  // !XSL_ITERATOR
