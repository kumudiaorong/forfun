#pragma once
#ifndef XSL_ITERATOR
#define XSL_ITERATOR
#include <xsl/bits/compare.hpp>
// #include <xsl/bits/container.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/functor.hpp>
#include <xsl/bits/ts/as.hpp>
#include <xsl/bits/ts/is.hpp>
#include <xsl/bits/ts/rm.hpp>
#include <xsl/bits/ts/ts.hpp>
#include <xsl/bits/utility.hpp>

namespace xsl {
  //
  namespace itor {
    struct tag_forward {};
    struct tag_bilateral : tag_forward {};
    struct tag_random : tag_bilateral {};
    template <class T>
    using get_unwrap_type =
      typename ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::unwrap_type {}), T&>::type;
    template <class T>
    class ptr_wrapper {
    public:
      // clang-format off
	typedef tag_random          iter_category;
	typedef ptr_wrapper 		    self_type;
	typedef T		 								val_type;
	typedef ptrdiff_t		 				diff_type;
	typedef std::weak_ordering  comp_category;
	typedef T* 	                unwrap_type;
      // clang-format on
      T *Ptr;
      constexpr ptr_wrapper()
        : Ptr(nullptr) {
      }
      constexpr ptr_wrapper(T *ptr)
        : Ptr(ptr) {
      }
      template <class Other, ts::enable<ts::has::constructible<T *, Other *>> = 0>
      constexpr ptr_wrapper(ptr_wrapper<Other> ano)
        : Ptr(ano.Ptr) {
      }
      constexpr T *to_unwrap() {
        return Ptr;
      }
      constexpr operator T *() {
        return Ptr;
      }
      constexpr T& operator*() const {
        return *Ptr;
      }
      constexpr T *operator->() const {
        return Ptr;
      }
      constexpr ptr_wrapper& operator++() {
        ++Ptr;
        return *this;
      }
      constexpr ptr_wrapper operator++(int) {
        T *Tmp = Ptr;
        ++Ptr;
        return {Tmp};
      }
      constexpr ptr_wrapper operator+(diff_type off) {
        return {Ptr + off};
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
      constexpr ptr_wrapper operator-(diff_type off) {
        return {Ptr - off};
      }
      constexpr diff_type operator-(const ptr_wrapper& ano) {
        return Ptr - ano.Ptr;
      }
      constexpr comp_category compare(const ptr_wrapper& ano) const {
        return Ptr <=> ano.Ptr;
      }
      constexpr bool operator==(const ptr_wrapper& ano) const {
        return Ptr == ano.Ptr;
      }
      constexpr bool operator!=(const ptr_wrapper& ano) const {
        return Ptr != ano.Ptr;
      }
    };
    template <class T>
    constexpr typename comp::traits<ptr_wrapper<T>>::comp_category operator<=>(
      const ptr_wrapper<T>& l, const ptr_wrapper<T>& r) {
      return l.compare(r) <=> 0;
    }
    // traits
    template <class T>
    using get_category =
      typename ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::iter_category {}), tag_forward>::type;
    // gcc would not think using statement is a type,if using typedef,it would
    // become int
    //
    template <class CheckedIter>
    struct traits {
      // clang-format off
	typedef get_category<CheckedIter>	        iter_category;
	typedef CheckedIter 							        self_type;
	typedef ts::tp::get_val_type<CheckedIter> val_type;
      // clang-format on
    };
    template <class Val>
    struct traits<Val *> {
      // clang-format off
	typedef tag_random	    iter_category;
	typedef Val*            self_type;
	typedef ts::rm::cvr<Val> val_type;
      // clang-format on
    };
    //

    template <class _Iter>
    class reverse {
    public:
      // clang-format off
	typedef std::weak_ordering 						      comp_category;
	typedef reverse 			                      self_type;
	typedef traits<_Iter> 									    traits_type;
	typedef typename traits_type::iter_category iter_category;
	typedef typename traits_type::val_type 			val_type;
	typedef self_type 										      unwrap_type;
      // clang-format on
      constexpr reverse() = default;
      constexpr reverse(_Iter iter)
        : Iter(as_rreference(iter)) {
      }
      template <class Other, ts::enable<ts::has::constructible<_Iter, Other>> = 0>
      constexpr reverse(const reverse<Other>& ano)
        : Iter(as_rreference(ano.unwrap())) {
      }
      template <class Other, ts::enable<ts::has::constructible<_Iter, Other>> = 0>
      constexpr reverse& operator=(const reverse<Other>& Ano) {
        Iter = Ano.Iter;
        return *this;
      }
      constexpr val_type& operator*() const {
        return *Iter;
      }
      constexpr decltype(auto) operator->() const {
        return addr(*Iter);
      }
      constexpr bool operator==(const reverse& ano) const {
        return Iter == ano.Iter;
      }
      constexpr bool operator!=(const reverse& ano) const {
        return !(Iter == ano.Iter);
      }
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
      constexpr reverse operator+(ptrdiff_t Off) {
        return {Iter - Off};
      }
      constexpr reverse& operator+=(ptrdiff_t Off) {
        Iter = Iter - Off;
        return *this;
      }
      constexpr reverse operator-(ptrdiff_t Off) {
        return {Iter + Off};
      }
      constexpr reverse& operator-=(ptrdiff_t Off) {
        Iter = Iter + Off;
        return *this;
      }
    protected:
      _Iter Iter;
    };

    template <class UCIter>
    inline constexpr bool has_iterator_operator =
      ts::has::opeartor_pre_increment<UCIter> && ts::has::opeartor_pre_increment<UCIter>
      && ts::has::opeartor_pre_increment<UCIter>;
    //

    template <class CIter>
    inline constexpr bool has_to_unwrap =
      ts::has::some<decltype([]<class T>(T) -> decltype(eval_type<T>().to_unwrap(), 0) { return 0; }),
        ts::tp::_n<CIter>>;
    //
    template <class UCIter>
    inline constexpr bool is = ts::is::ptr<UCIter> || has_iterator_operator<UCIter>;
    //
    //
    template <class UCIter>
    constexpr bool is_forward = is<UCIter> && ts::has::base<tag_forward, get_category<UCIter>>;
    //
    template <class UCIter>
    constexpr bool is_bilateral = is<UCIter> && ts::has::base<tag_bilateral, get_category<UCIter>>;
    //
    template <class UCIter>
    constexpr bool is_random = is<UCIter> && ts::has::base<tag_random, get_category<UCIter>>;
    //
    template <class CIter>
    constexpr get_unwrap_type<CIter> get_unwrapped(CIter& iter) {
      if constexpr(has_to_unwrap<CIter>)
        return iter.to_unwrap();
      else
        return iter;
    }
  }  // namespace itor
  template <class Iter1, class Iter2,
    ts::enable<ts::is::same<typename  itor::traits<Iter1>::val_type, typename itor::traits<Iter2>::val_type>> = 0>
  constexpr void iter_swap(const Iter1& l, const Iter2& r) {
    xsl::swap(*l, *r);
  }
  // template <class Iter, class Pre>
  // constexpr void max(const Iter& l, const Iter& r, Pre pre = fc::less<typename traits<Iter>::val_type>{}) {
  //   pre(*l, *r) ? l : r;
  // }

  //
  // template <class CIter, class SizeType>
  // constexpr decltype(auto) get_forward(CIter&& Iter, SizeType Size = 0) {
  //   if(is_bilateral<CIter> || Size == 0) {
  //     if constexpr(is_random<CIter>)
  //       return (Iter - Size).base();
  //     else {
  //       while(Size-- != 0)
  //         --Iter;
  //       return Iter.base();
  //     }
  //   } else
  //     throw "error";
  //   return Iter;
  // }
  //
  // template <class CIter, class SizeType>
  // constexpr CIter get_tar(CIter&& Iter, SizeType Count) {
  //   if constexpr(ts::has::opeartor_addition<ts::rm::cvr<CIter>, SizeType>)
  //     return Iter + Count;
  //   else if constexpr(ts::has::opeartor_addition_assignment<ts::rm::cvr<CIter>, SizeType>)
  //     return Iter += Count;
  //   else {
  //     while(Count-- > 0)
  //       ++Iter;
  //     return Iter;
  //   }
  // }
  // target
}  // namespace xsl
#endif  // !XSL_ITERATOR
