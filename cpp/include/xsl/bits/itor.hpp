#pragma once
#ifndef XSL_ITERATOR
#define XSL_ITERATOR
#include <xsl/bits/compare.hpp>
#include <xsl/bits/container.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts.hpp>
#include <xsl/bits/utility.hpp>

XSL_BEGIN
namespace itor {
  //

  class forward {};
  class bilateral : public forward {};
  class random : public bilateral {};

  template <class T>
  class ptr_wrapper {
  public:
    // clang-format off
	typedef random         category;
	typedef ptr_wrapper 		self_type;
	typedef T		 								val_type;
	typedef std::weak_ordering 	comp_category;
    // clang-format on
    T *Ptr;
    constexpr ptr_wrapper()
      : Ptr(nullptr) {}
    constexpr ptr_wrapper(T *ptr)
      : Ptr(ptr) {}
    template <class Other, ts::enable<ts::is::constructible<T *, Other *>> = 0>
    constexpr ptr_wrapper(ptr_wrapper<Other> ano)
      : Ptr(ano.Ptr) {}
    constexpr operator T *() { return Ptr; }
    constexpr T& operator*() const { return *Ptr; }
    constexpr T *operator->() const { return Ptr; }
    constexpr ptr_wrapper& operator++() {
      ++Ptr;
      return *this;
    }
    constexpr ptr_wrapper operator++(int) {
      T *Tmp = Ptr;
      ++Ptr;
      return {Tmp};
    }
    constexpr ptr_wrapper& operator--() {
      --Ptr;
      return *this;
    }
    constexpr ptr_wrapper operator--(int) {
      T *Tmp = Ptr;
      --Ptr;
      return {Tmp};
    }
    constexpr comp_category compare(const ptr_wrapper& ano) const { return Ptr <=> ano.Ptr; }
    constexpr bool operator==(const ptr_wrapper& ano) const { return Ptr == ano.Ptr; }
    constexpr bool operator!=(const ptr_wrapper& ano) const { return Ptr != ano.Ptr; }
  };
  template <class T>
  constexpr typename comp_traits<ptr_wrapper<T>>::comp_category operator<=>(
    const ptr_wrapper<T>& l, const ptr_wrapper<T>& r) {
    return l.compare(r) <=> 0;
  }
  // traits
  template <class T>
  using get_category = XSL get_category<T, forward>;
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
  struct traits {
    // clang-format off
	typedef get_category<CheckedIter>	category;
	typedef CheckedIter 							                    self_type;
	typedef typename get_val_type<CheckedIter>::Type 		  val_type;
    // clang-format on
  };
  template <class Val>
  struct traits<Val *> {
    // clang-format off
	typedef random	    category;
	typedef Val*            self_type;
	typedef ts::rm_cvr<Val> val_type;
    // clang-format on
  };
  //

  template <class _Iter>
  class reverse {
  public:
    // clang-format off
	typedef std::weak_ordering 						      comp_category;
	typedef reverse 			              self_type;
	typedef traits<_Iter> 									traits_type;
	typedef typename traits_type::category category;
	typedef typename traits_type::val_type 			val_type;
	typedef _Iter 															wrap_type;
    // clang-format on
    constexpr reverse() = default;
    constexpr reverse(_Iter iter)
      : Iter(as_rreference(iter)) {}
    template <class Other, ts::enable<ts::is::constructible<_Iter, Other>> = 0>
    constexpr reverse(const reverse<Other>& ano)
      : Iter(as_rreference(ano.unwrap())) {}
    template <class Other, ts::enable<ts::is::constructible<_Iter, Other>> = 0>
    constexpr reverse& operator=(const reverse<Other>& Ano) {
      Iter = Ano.Iter;
      return *this;
    }
    constexpr val_type& operator*() const { return *Iter; }
    constexpr decltype(auto) operator->() const { return XSL addr(*Iter); }
    constexpr bool operator==(const reverse& ano) const { return Iter == ano.Iter; }
    constexpr bool operator!=(const reverse& ano) const { return !(Iter == ano.Iter); }
    constexpr reverse& operator++() {
      --Iter;
      return *this;
    }
    constexpr reverse operator++(int) {
      _Iter Tmp = Iter;
      --Iter;
      return {Tmp};
    }
    constexpr reverse& operator--() {
      ++Iter;
      return *this;
    }
    constexpr reverse operator--(int) {
      _Iter Tmp = Iter;
      ++Iter;
      return {Tmp};
    }
    constexpr reverse operator+(ptrdiff_t Off) { return {Iter - Off}; }
    constexpr reverse& operator+=(ptrdiff_t Off) {
      Iter = Iter - Off;
      return *this;
    }
    constexpr reverse operator-(ptrdiff_t Off) { return {Iter + Off}; }
    constexpr reverse& operator-=(ptrdiff_t Off) {
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
  inline constexpr bool is = ts::is::pointer<UCIter> || has_iterator_operator<UCIter>;
  //
  //
  template <class UCIter>
  constexpr bool is_forward = is<UCIter> && ts::is::base<forward, get_category<UCIter>>;
  //
  template <class UCIter>
  constexpr bool is_bilateral = is<UCIter> && ts::is::base<bilateral, get_category<UCIter>>;
  //
  template <class UCIter>
  constexpr bool is_random = is<UCIter> && ts::is::base<random, get_category<UCIter>>;
  //
  template <class CIter>
  constexpr CIter jump(CIter Iter, size_t Size) {
    if constexpr(is_random<CIter>)
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
  constexpr decltype(auto) get_forward(CIter&& Iter, Size_Type Size = 0) {
    if(is_bilateral<CIter> || Size == 0) {
      if constexpr(is_random<CIter>)
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
  constexpr CIter get_tar(CIter&& Iter, SizeType Count) {
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
}  // namespace iter
XSL_END
#endif  // !XSL_ITERATOR
