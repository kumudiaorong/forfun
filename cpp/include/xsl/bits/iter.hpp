#pragma once
#include "xsl/bits/pf/def.h"
#ifndef XSL_ITERATOR
#define XSL_ITERATOR
#include <xsl/bits/compare.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts.hpp>
#include <xsl/bits/utility.hpp>

XSL_BEGIN
//

class forward_iter {};
class bilateral_iter {};
class random_iter {};

template <class T>
class pointer_wrapper {
public:
  // clang-format off
	typedef random_iter         iter_category;
	typedef pointer_wrapper 		self_type;
	typedef T		 								val_type;
	typedef std::weak_ordering 	comp_category;
  // clang-format on
  T *Ptr;
  constexpr pointer_wrapper()
    : Ptr(nullptr) {}
  constexpr pointer_wrapper(T *ptr)
    : Ptr(ptr) {}
  template <class Other, ts::enable<ts::is::constructible<T *, Other *>> = 0>
  constexpr pointer_wrapper(pointer_wrapper<Other> ano)
    : Ptr(ano.Ptr) {}
  constexpr operator T *() { return Ptr; }
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
  : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::iter_category {}), forward_iter>::Self {};
// gcc would not think using statement is a type,if using typedef,it would
// become int
//
template <class T>
struct get_val_type : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::val_type {}), T>::Self {};
//
//
template <class T>
struct get_mnr_type : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::mnr_type {}), T>::Self {};
//
//
// minimum requirements
template <class CheckedIter>
struct iter_traits {
  // clang-format off
	typedef typename get_iter_category<CheckedIter>::Type	iter_category;
	typedef CheckedIter 							                    self_type;
	typedef typename get_val_type<CheckedIter>::Type 		  val_type;
  // clang-format on
};
template <class Val>
struct iter_traits<Val *> {
  // clang-format off
	typedef random_iter	    iter_category;
	typedef Val*            self_type;
	typedef ts::rm_cvr<Val> val_type;
  // clang-format on
};
//

template <class _Iter>
class reverse_iterator {
public:
  // clang-format off
	typedef std::weak_ordering 						      comp_category;
	typedef reverse_iterator 			              self_type;
	typedef iter_traits<_Iter> 									traits_type;
	typedef typename traits_type::iter_category iter_category;
	typedef typename traits_type::val_type 			val_type;
	typedef _Iter 															wrap_type;
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
  constexpr reverse_iterator operator+(ptrdiff_t Off) { return {Iter - Off}; }
  constexpr reverse_iterator& operator+=(ptrdiff_t Off) {
    Iter = Iter - Off;
    return *this;
  }
  constexpr reverse_iterator operator-(ptrdiff_t Off) { return {Iter + Off}; }
  constexpr reverse_iterator& operator-=(ptrdiff_t Off) {
    Iter = Iter + Off;
    return *this;
  }
  constexpr wrap_type _unwrap() const { return Iter; }
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
constexpr bool is_forward_iter = is_iter<UCIter> && ts::is::same<get_iter_category<UCIter>, forward_iter>;
//
template <class UCIter>
constexpr bool is_bilateral_iter = is_iter<UCIter> && ts::is::same<get_iter_category<UCIter>, bilateral_iter>;
//
template <class UCIter>
constexpr bool is_random_iter = is_iter<UCIter> && ts::is::same<get_iter_category<UCIter>, random_iter>;
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
