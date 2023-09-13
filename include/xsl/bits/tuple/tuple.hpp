#pragma once
#ifndef XSL_TUPLE_SUPPORT
#define XSL_TUPLE_SUPPORT
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/as.hpp>
#include <xsl/bits/ts/type.hpp>
#include <xsl/bits/ts/ts.hpp>
#include <xsl/bits/utility.hpp>

namespace xsl::tpl {
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
      : base_type(forward<_Rest>(rest_params)...)
      , Val(forward<_This>(this_param)) {
    }
    template <typename _This, typename... _Rest, ts::enable_construct<tuple, _This> = 0>
    constexpr tuple(tag_store, _This&& this_param, _Rest&&...rest_params)
      : base_type(forward<_Rest>(rest_params)...)
      , Val(forward<_This>(this_param)) {
    }
    constexpr decltype(auto) to_unwrap() {
      return static_cast<ts::conditional_apply<!ts::is::rref<val_type>, ts::as::lref, val_type>>(Val);
    }
    constexpr decltype(auto) to_unwrap() const {
      return static_cast<ts::conditional_apply<!ts::is::rref<val_type>, ts::as::lref, ts::as::cst<val_type>>>(Val);
    }
    // template <typename T>
    // constexpr T get() {
    //   if constexpr(ts::is::same<val_type, T>)
    //     return Val;
    //   else
    //     return base_type::template get<T>();
    // }
    // template <typename T>
    // constexpr const T get() const {
    //   if constexpr(ts::is::same<val_type, T>)
    //     return Val;
    //   else
    //     return base_type::template get<T>();
    // }
    // template <uint_8 Id>
    // constexpr decltype(auto) get() {
    //   if constexpr(Id == 0)
    //     return static_cast<ts::conditional_apply<!ts::is::rref<val_type>, ts::as::lref, val_type>>(Val);
    //   else
    //     return base_type::template get<Id - 1>();
    // }
    // template <uint_8 Id>
    // constexpr decltype(auto) get() const {
    //   if constexpr(Id == 0)
    //     return static_cast<ts::conditional_apply<!ts::is::rref<val_type>, ts::as::lref, ts::as::cst<val_type>>>(Val);
    //   else
    //     return base_type::template get<Id - 1>();
    //   // return ts::tp::split_by_id<tuple, Id>::Type2::Val;
    // }
    // constexpr uint_8 size() { return sizeof...(Rest) + 1; }
    constexpr bool operator==(const tuple& ano) {
      return Val == ano.Val && base_type::operator==(ano);
    }
  };
  template <>
  class tuple<> {
  public:
    constexpr tuple() = default;
    constexpr tuple(tuple&&) = default;
    constexpr tuple(const tuple&) = default;
    constexpr tuple& operator=(tuple&& t) = default;
    constexpr tuple& operator=(const tuple&) = default;
    constexpr tuple(tag_store) {
    }
    constexpr bool operator==(const tuple&) {
      return true;
    }
  };
  template <typename... Vals>
  tuple(Vals...) -> tuple<Vals...>;
  template <typename... Vals>
  tuple(tag_store, Vals&&...) -> tuple<Vals&&...>;

  template <uint_8 Id, class Tpl>
  constexpr decltype(auto) get(Tpl& t) {
    return static_cast<typename ts::tp::split_by_id<Tpl, Id>::type2&>(t).to_unwrap();
    // return t.template tail<Id>().head();
  }
  template <uint_8 Id, class Tpl>
  constexpr decltype(auto) get(Tpl&& t) {
    return as_rreference(static_cast<typename ts::tp::split_by_id<Tpl, Id>::type2&>(t).to_unwrap());
    // return t.template tail<Id>().head();
  }
  template <uint_8 Id, class Tpl>
  constexpr decltype(auto) get(const Tpl& t) {
    return static_cast<const typename ts::tp::split_by_id<Tpl, Id>::type2&>(t).to_unwrap();
    // return t.template tail<Id>().head();
  }
}  // namespace xsl::tpl
#include <tuple>
namespace std {
  template <std::size_t i, typename... Args>
  struct tuple_element<i, xsl::tpl::tuple<Args...>> {
    typedef xsl::ts::tp::find_by_id_with_rep<xsl::tpl::tuple<Args...>, i> type;
  };
  template <typename... Args>
  struct tuple_size<xsl::tpl::tuple<Args...>> : xsl::ts::var::_1<sizeof...(Args)> {};
}  // namespace std
#endif  // XSL_TUPLE_IMPL`