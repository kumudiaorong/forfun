#pragma once
#ifndef XSL_WRAPPER
#define XSL_WRAPPER
#include"utility.h"
XSL_BEGIN
template<typename Val>
class wrapper {
	Val Obj;
public:
	template<typename...Args>
	constexpr wrapper(Args&&...params) :Obj(forward<Args>(params)...) {}
	constexpr Val* operator&() {
		return &Obj;
	}
};
XSL_END
#endif // !XSL_WRAPPER

