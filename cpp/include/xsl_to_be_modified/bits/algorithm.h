#pragma once
#ifndef XSL_ALGORITHM
#define XSL_ALGORITHM
#include"xsldef.h"
XSL_BEGIN
//typename container
template<typename Iter, typename Cond>
constexpr  Iter find_if(Iter Begin, Iter End, Cond Proc) {
	for (; Begin != End; ++Begin)
		if (Proc(*Begin))break;
	return Begin;
}
template<typename Iter, typename Cond, typename Val>
constexpr  Iter upper_bound(Iter Begin, Iter End, const Val& _Val) {
	for (; Begin != End; ++Begin)
		if (Proc(*Begin))break;
	return Begin;
}



XSL_END

#endif //TLS_ALGORITHM