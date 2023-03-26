#pragma once
#ifndef XSL_ARRAY
#define XSL_ARRAY
#include"container.h"
#include"allocator.h"
XSL_BEGIN
//dimension
template<typename Val, uint_64 Size>
class array {
	Val Objs[Size];
	template<typename...Args, uint_8...Id>
	void init(var::uint8_s<Id...>, Args&&...params) {
		unfold((Construct_At(Objs + Id, forward<Args>(params)), 0)...);
	}

public:
	using Crit = criterion<Val>;
	using Size_T = Crit::Size_T;
	using Val_T = Val;
	using Ptr_T = Val*;
	using CPtr_T = const Val*;
	using Iter = Ptr_T;
	constexpr array() = default;
	constexpr array(array&&) = default;
	constexpr array(const array&) = default;
	template<typename Arg, typename...Args, enable_construct<array, Arg> = 0>
	constexpr array(Arg&& param, Args&&...params) {
		init(var::make_index_sequence<sizeof...(Args)>{}, forward<Args>(params)...);
	}
	constexpr array& operator=(array&&) = default;
	constexpr array& operator=(const array&) = default;
	constexpr Val_T& operator[](Size_T Off) {
		XSL_RANGE_CHECK(Off < Size);
		return Objs[Off];
	}
	constexpr Iter begin() {
		return Objs;
	}
	constexpr Iter end() {
		return Objs + Size;
	}
	constexpr CPtr_T data() {
		return Objs;
	}
	// constexpr Val* rbegin() {
	// 	TEST(Val * rbegin());
	// 	return Objs + Size - 1;
	// }
	// constexpr Val* rend() {
	// 	TEST(Val * rend());
	// 	return  Objs - 1;
	// }
	// constexpr Size_T size() {
	// 	return Size;
	// }
	constexpr ~array() = default;
};
XSL_END
#endif // !XSL_ARRAY
