#pragma once
#ifndef XSL_ATOMIC
#define XSL_ATOMIC
#include"xsldef.h"
#include"utility.h"
#include<mutex>
XSL_BEGIN

#define XSL_ATOMIC_LOCK(ord) Flag.lock();ord;Flag.unlock();

template<typename Val>
class atomic
{
	std::mutex Flag;
	Val Obj;
public:
	template<typename...Args>
	atomic(Args&&...params) :Obj(forward<Args>(params)...) {}
	constexpr Obj load() {
		std::lock_guard<std::mutex> lg(Flag);
		return Obj;
	}
	constexpr atomic& operator=(const Val& ano) {
		XSL_ATOMIC_LOCK(Obj = ano);
		return *this;
	}
	constexpr operator Val() {
		return Obj;
	}
	constexpr void lock() {
		Flag.lock();
	}
	constexpr void store(const Val& ano) {
		XSL_ATOMIC_LOCK(Obj = ano);
	}
	template<typename Ret, typename...Args>
	inline Ret self(Ret(Val::* f)(Args...), Args&&...params)
	{
		std::lock_guard<std::mutex> lock(Flag);
		return (Obj.*f)(toarg::forward<Args>(params)...);
	}
};
XSL_END
#endif // !XSL_ATOMIC
