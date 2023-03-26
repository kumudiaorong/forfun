#ifndef XSL_TUPLE
#define XSL_TUPLE
#include"xsldef.h"
#include"utility.h"

XSL_BEGIN
namespace tuple_tag {
	struct store {};
}
template<typename...>
class tuple;

template<typename This, typename...Rest>
class tuple<This, Rest...> :public tuple<Rest...> {
protected:
	using Base = tuple<Rest...>;
	This Obj;
public:
	constexpr tuple() = default;
	constexpr tuple(tuple&&) = default;
	constexpr tuple(const tuple&) = default;
	constexpr tuple& operator=(tuple&& t) = default;
	constexpr tuple& operator=(const tuple&) = default;
	template<typename _This, typename..._Rest, enable_construct<tuple, _This> = 0>
	constexpr tuple(_This&& this_param, _Rest&&...rest_params) :Obj(forward<_This>(this_param)), Base(forward<_Rest>(rest_params)...) {}
	template<typename _This, typename..._Rest, enable_construct<tuple, _This> = 0>
	constexpr tuple(tuple_tag::store, _This&& this_param, _Rest&&...rest_params) : Obj(forward<_This>(this_param)), Base(forward<_Rest>(rest_params)...) {}
	template<typename Val, typename _Val = type::conditional<is_rref<Val>, Val, Val&>>
	constexpr decltype(auto) get() {
		if constexpr (is_same<This, Val>)return static_cast<_Val>(Obj);
		else return Base::template get<Val>();
	}
	template<typename Val, typename _Val = type::conditional<is_rref<Val>, Val, Val&>>
	constexpr decltype(auto) get()const {
		if constexpr (is_same<This, Val>)return static_cast<_Val>(Obj);
		else return Base::template get<Val>();
	}
	template<uint8_t Id, class _Val = type::find_by_id<tuple, Id>, class _Ret = type::conditional<is_rref<_Val>, _Val, _Val&>>
	constexpr _Ret get() {
		return static_cast<_Ret>(type::split_by_id<tuple, Id>::Type2::Obj);
	}
	template<uint8_t Id, class _Val = type::find_by_id<tuple, Id>, class _Ret = type::conditional<is_rref<_Val>, _Val, _Val& >>
	constexpr _Ret get()const {
		return static_cast<_Ret>(type::split_by_id<tuple, Id>::Type2::Obj);
	}
	constexpr uint8_t size() {
		return sizeof...(Rest) + 1;
	}
	constexpr bool operator==(const tuple& ano) {
		return Obj == ano.Obj && Base::operator==(ano);
	}
};
template<>
class tuple<> {
public:
	constexpr tuple() = default;
	constexpr tuple(tuple&&) = default;
	constexpr tuple(const tuple&) = default;
	constexpr tuple& operator=(tuple&& t) = default;
	constexpr tuple& operator=(const tuple&) = default;
	constexpr tuple(tuple_tag::store) {}
	constexpr bool operator==(const tuple& ano) { return true; }
};
template<typename...Vals>
tuple(Vals...)->tuple<Vals...>;
template<typename...Vals>
tuple(tuple_tag::store, Vals&&...)->tuple<Vals&&...>;
XSL_END
#endif//XSL_TUPLE`