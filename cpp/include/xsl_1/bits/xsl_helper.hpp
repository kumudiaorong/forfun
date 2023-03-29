#pragma once
#ifndef XSL_HELPER
#define XSL_HELPER
#include"xsl_def.h"
#include"xsl_functor.h"
XSL_BEGIN
namespace helper {
	template<class Pred, class Ret_Pred, class...Ts>
	decltype(auto) foreach(Pred _Pred, Ret_Pred _Ret_Pred, Ts&&...Vals) {
		return _Ret_Pred(_Pred(Vals)...);
	}
}
XSL_END
#endif // !XSL_HELPER
