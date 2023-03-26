#pragma once
#ifndef XSL_COMPARE
#define XSL_COMPARE
#include <compare>
#include "xsl_def.hpp"
#include "xsl_ts.hpp"
XSL_BEGIN
//
template<class T>
struct get_comp_category:ts::tp::type_traits < T, decltype([]<class U>(U)->typename U::comp_category {}), std::weak_ordering > ::Self {};
//
//
template<class T>
struct get_comp_type:ts::tp::type_traits < T, decltype([]<class U>(U)->typename U::comp_type {}), T > ::Self {};
//
//
template <class T>
struct comp_traits {
	typedef typename get_comp_category<T>::Type comp_category;
	typedef typename get_comp_type<T>::Type comp_type;
};
//
template <class T>
typename comp_traits<T>::comp_type get_comp_val(const T& val) {
	return val;
}

// target
XSL_END
#endif // !XSL_COMPARE
