#pragma once
#ifndef XSL_TYPE_SUPPORT_TYPE
#define XSL_TYPE_SUPPORT_TYPE
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/def.hpp>
XSL_BEGIN
namespace ts {
  namespace tp {
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
}  // namespace ts
XSL_END
#endif  // TOT_H