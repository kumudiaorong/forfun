#pragma once
#ifndef XSL_UTILITY
#define XSL_UTILITY
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/as.hpp>
#include <xsl/bits/ts/rm.hpp>

namespace xsl {
  // template <class T>
  // constexpr T abs(const T Obj) {
  //   if(Obj < 0) return -Obj;
  //   return Obj;
  // }

  // perfect forward
  template <class T>
  constexpr T&& forward(ts::rm::ref<T>&& Obj) {
    return static_cast<T&&>(Obj);
  }
  template <class T>
  constexpr T&& forward(ts::rm::ref<T>& Obj) {
    return static_cast<T&&>(Obj);
  }
  template <class T>
  constexpr T *addr(T& val) {
    return __builtin_addressof(val);
  }
  // move
  template <class T>
  constexpr decltype(auto) as_lreference(T&& Obj) {
    return static_cast<ts::as::lref<T>>(Obj);
  }
  template <class T>
  constexpr decltype(auto) as_rreference(T&& Obj) {
    return static_cast<ts::as::rref<T>>(Obj);
  }
  template <class T>
  constexpr decltype(auto) as_const(T&& Obj) {
    return static_cast<ts::as::cst<T>>(Obj);
  }
  // unfold helper
  // template <class Val>
  // constexpr Val *deviate(Val *ptr, ptrdiff_t off) {
  //   return reinterpret_cast<Val *>((reinterpret_cast<char *>(ptr) + off));
  // }

  template <class T>
  class wrapper : public ts::tp::_1<T> {
  public:
    constexpr wrapper(T ref)
      : Ref(ref) {
    }
    constexpr operator T() {
      return Ref;
    }
  private:
    T Ref;
  };

  template <class T>
  inline constexpr bool is_wrraper = false;

  template <class T>
  inline constexpr bool is_wrraper<wrapper<T>> = true;
  //
  template <class T>
  constexpr wrapper<T&&> wrap_ref(T&& val) {
    return {val};
  }
#ifndef NOMINMAX
#define NOMINMAX
#endif  // NOMINMAX
  template <class Val>
  constexpr Val max(Val Left, Val Right) {
    return Left > Right ? Left : Right;
  }
  template <class Val, class... Vals>
  constexpr decltype(auto) max(Val First, Val Second, Vals... Rest) {
    return max(max(First, Second), static_cast<Val>(Rest)...);
  }
  template <class Val>
  constexpr Val min(Val Left, Val Right) {
    return Left < Right ? Left : Right;
  }
  template <class Val>
  constexpr void swap(Val& Left, Val& Right) {
    Val Tmp = as_rreference(Left);
    Left = as_rreference(Right);
    Right = as_rreference(Tmp);
  }
  template <class... Vals>
  constexpr void unfold(Vals&&...) {
  }
}  // namespace xsl
#endif  // !UTILITY
