#pragma once
#ifndef XSL_COMPARE
#define XSL_COMPARE
#include <compare>
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/has.hpp>
#include <xsl/bits/ts/type.hpp>
#include "ts/def.hpp"
//
namespace xsl::comp {
  template <class T>
  using get_category =
    typename ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::comp_category {}), std::weak_ordering>::type;
  //
  //
  template <class T>
  using get_type =
    typename ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::comp_type {}), const T&>::type;
  //
  //
  namespace impl {
    template <class T>
    inline constexpr bool has_to_comp =
      ts::has::some<decltype([]<class U>(U) -> decltype(ts::eval_type<U>().to_comp(), 0) { return 0; }), ts::tp::_n<T>>;
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
    //
    //
  }  // namespace impl
  template <class T>
  struct traits {
    typedef get_category<T> comp_category;
    typedef get_type<T> comp_type;
  };
  //
  template <class T>
  typename traits<T>::comp_type get_val(const T& val) {
    if constexpr(impl::has_to_comp<T>)
      return val.to_comp();
    else
      return val;
  }
  // target
}  // namespace xsl::comp

#endif  // !XSL_COMPARE
