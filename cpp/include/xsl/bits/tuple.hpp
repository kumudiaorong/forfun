#pragma once
#ifndef XSL_TUPLE_IMPL
#define XSL_TUPLE_IMPL
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts.hpp>
#include <xsl/bits/utility.hpp>

XSL_BEGIN
struct tag_store {};
template <typename... T>
class tuple;

template <typename This, typename... Rest>
class tuple<This, Rest...> : public tuple<Rest...> {
protected:
  // clang-format off
  typedef tuple<Rest...> 	base_type;
  typedef This 				val_type;
  // clang-format on
  val_type Val;
public:
  constexpr tuple() = default;
  constexpr tuple(tuple&&) = default;
  constexpr tuple(const tuple&) = default;
  constexpr tuple& operator=(tuple&& t) = default;
  constexpr tuple& operator=(const tuple&) = default;
  template <typename _This, typename... _Rest, ts::enable_construct<tuple, _This> = 0>
  constexpr tuple(_This&& this_param, _Rest&&...rest_params)
    : Val(forward<_This>(this_param))
    , base_type(forward<_Rest>(rest_params)...) {}
  template <typename _This, typename... _Rest, ts::enable_construct<tuple, _This> = 0>
  constexpr tuple(tag_store, _This&& this_param, _Rest&&...rest_params)
    : Val(forward<_This>(this_param))
    , base_type(forward<_Rest>(rest_params)...) {}
  template <typename T>
  constexpr T get() {
    if constexpr(ts::is::same<val_type, T>)
      return Val;
    else
      return base_type::template get<T>();
  }
  template <typename T>
  constexpr const T get() const {
    if constexpr(ts::is::same<val_type, T>)
      return Val;
    else
      return base_type::template get<T>();
  }
  template <uint_8 Id>
  constexpr decltype(auto) get() {
    if constexpr(Id == 0)
      return static_cast<ts::conditional_apply<!ts::is::rref<val_type>, ts::as_lref, val_type>>(Val);
    else
      return base_type::template get<Id - 1>();
  }
  template <uint_8 Id>
  constexpr decltype(auto) get() const {
    if constexpr(Id == 0)
      return static_cast<ts::conditional_apply<!ts::is::rref<val_type>, ts::as_lref, ts::as_c<val_type>>>(Val);
    else
      return base_type::template get<Id - 1>();
    // return ts::tp::split_by_id<tuple, Id>::Type2::Val;
  }
  constexpr uint_8 size() { return sizeof...(Rest) + 1; }
  constexpr bool operator==(const tuple& ano) { return Val == ano.Val && base_type::operator==(ano); }
};
template <>
class tuple<> {
public:
  constexpr tuple() = default;
  constexpr tuple(tuple&&) = default;
  constexpr tuple(const tuple&) = default;
  constexpr tuple& operator=(tuple&& t) = default;
  constexpr tuple& operator=(const tuple&) = default;
  constexpr tuple(tag_store) {}
  constexpr bool operator==(const tuple&) { return true; }
};
template <typename... Vals>
tuple(Vals...) -> tuple<Vals...>;
template <typename... Vals>
tuple(tag_store, Vals&&...) -> tuple<Vals&&...>;
XSL_END
#endif  // XSL_TUPLE_IMPL`