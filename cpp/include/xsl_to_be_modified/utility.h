#pragma once
#ifndef XSL_UTILITY
#define XSL_UTILITY
#include"xsldef.h"
#include"template.h"
XSL_BEGIN

template<class Tp>
constexpr Tp abs(const Tp Obj) {
	if (Obj < 0)return -Obj;
	return Obj;
}

// perfect forward
template <typename Tp>
constexpr Tp&& forward(rm_ref<Tp>&& Obj) { return static_cast<Tp&&>(Obj); }
template <typename Tp>
constexpr Tp&& forward(rm_ref<Tp>& Obj) { return static_cast<Tp&&>(Obj); }
//move
template<typename Tp>
constexpr decltype(auto) as_lreference(Tp&& Obj) { return static_cast<rm_ref<Tp>&>(Obj); }
template<typename Tp>
constexpr decltype(auto) as_rreference(Tp&& Obj) { return static_cast<rm_ref<Tp>&&>(Obj); }
template<typename Tp>
constexpr decltype(auto) as_const(Tp& Obj) { return  static_cast<const rm_c<Tp>&>(Obj); }
//unfold helper
template<typename Val, typename Devi = char>
constexpr Val* deviate(Val* Ptr, int_64 _Devi) { return (Val*)((Devi*)Ptr + _Devi); }

template<typename Val>
constexpr Val Max(Val Left, Val Right) {
	return Left > Right ? Left : Right;
}
template<typename Val, typename...Vals>
constexpr decltype(auto) Max(Val First, Val Second, Vals...Rest) {
	return Max(Max(First, Second), static_cast<Val>(Rest)...);
}
template<typename Val>
constexpr Val Min(Val Left, Val Right) {
	return Left < Right ? Left : Right;
}
template<typename Val>
constexpr void swap(Val& Left, Val& Right) {
	Val Tmp = as_rreference(Left);
	Left = as_rreference(Right);
	Right = as_rreference(Tmp);
}
template<typename...Vals>
constexpr void unfold(Vals&&...args) {}
XSL_END
#endif // !UTILITY
