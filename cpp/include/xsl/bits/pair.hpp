#pragma once
#ifndef XSL_PAIR_IMPL
#define XSL_PAIR_IMPL
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/utility.hpp>
XSL_BEGIN

template<class _First, class _Second>
struct pair {
  // clang-format off
	typedef _First 	first_type;
	typedef _Second second_type;
  // clang-format on
	first_type first;
	second_type second;
	constexpr pair():first(), second() {}
	constexpr pair(const pair&) = default;
	template<class _First_, class _Second_, ts::enable_construct<pair, _First_> = 0>
	constexpr pair(_First_&& _first, _Second_&& _second): first(forward<_First_>(_first)), second(forward<_Second_>(_second)) {}
};
template<class _Pair>
class set_pair {
public:
	_Pair*Pair;
	set_pair(_Pair*pair):Pair(pair) {}
	template<class...Args>
	constexpr set_pair& _1(Args&&...args) {
		construct_at(const_cast<ts::rm_c<typename _Pair::first_type>*>(addr(Pair->first)), forward<Args>(args)...);
		return *this;
	}
	template<class...Args>
	constexpr set_pair& _2(Args&&...args) {
		construct_at(const_cast<ts::rm_c<typename _Pair::second_type>*>(addr(Pair->second)), forward<Args>(args)...);
		return *this;
	}
};
XSL_END
#endif // !XSL_PAIR_IMPL
