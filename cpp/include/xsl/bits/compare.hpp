#pragma once
#ifndef XSL_COMPARE
#define XSL_COMPARE
#include <compare>
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/has.hpp>
#include <xsl/bits/ts/type.hpp>

XSL_BEGIN
//
namespace comp {
  template <class T>
  struct get_category
    : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::comp_category {}), std::weak_ordering>::Self {};
  //
  //
  template <class T>
  struct get_type : ts::tp::type_traits<T, decltype([]<class U>(U) -> typename U::comp_type {}), const T&>::Self {};
  //
  //
  namespace impl {
    template <class T>
    inline constexpr bool has_to_comp =
      ts::has::some<decltype([]<class U>(U) -> decltype(eval_type<U>().to_comp(), 0) { return 0; }), ts::tp::_n<T>>;
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
    typedef typename get_category<T>::Type comp_category;
    typedef typename get_type<T>::Type comp_type;
  };
  //
  template <class T>
  typename traits<T>::comp_type get_comp_val(const T& val) {
    if constexpr(impl::has_to_comp<T>)
      return val.to_comp();
    else
      return val;
  }
  // target
}  // namespace comp
XSL_END
#endif  // !XSL_COMPARE
