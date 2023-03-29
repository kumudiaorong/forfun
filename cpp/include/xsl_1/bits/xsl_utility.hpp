#pragma once
#ifndef XSL_UTILITY
#define XSL_UTILITY
#include "xsl_def.hpp"
#include "xsl_ts.hpp"
XSL_BEGIN
class conjunction {
public:
  template <class T, class... Ts>
  constexpr bool operator()(const T& First, Ts&&...Rest) {
    return First && *this(Rest...);
  }
};
template <class T>
constexpr T abs(const T Obj) {
  if(Obj < 0) return -Obj;
  return Obj;
}

// perfect forward
template <class T>
constexpr T&& forward(ts::rm_ref<T>&& Obj) {
  return static_cast<T&&>(Obj);
}
template <class T>
constexpr T&& forward(ts::rm_ref<T>& Obj) {
  return static_cast<T&&>(Obj);
}
template <class T>
constexpr T *addr(T& val) {
  return __builtin_addressof(val);
}
// move
template <class T>
constexpr decltype(auto) as_lreference(T&& Obj) {
  return static_cast<ts::as_lref<T>>(Obj);
}
template <class T>
constexpr decltype(auto) as_rreference(T&& Obj) {
  return static_cast<ts::as_rref<T>>(Obj);
}
template <class T>
constexpr decltype(auto) as_const(T& Obj) {
  return static_cast<ts::as_c<T>&>(Obj);
}
// unfold helper
template <class Val, class Devi = char>
constexpr Val *deviate(Val *Ptr, int_64 _Devi) {
  return (Val *)((Devi *)Ptr + _Devi);
}

template <class T>
class wrapper : public ts::tp::_1<T> {
public:
  constexpr wrapper(T ref)
    : Ref(ref) {}
  constexpr operator T() { return Ref; }
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
template <class T>
struct wrap_reference {};
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
constexpr void unfold(Vals&&...args) {}
template <class T, class... Ts, class Ret_Pred = conjunction>
constexpr bool equal(const T& This, const T& Second, Ts&&...Rest, Ret_Pred RP) {
  return RP(equal<T>(This, Second), equal<T>(This, Rest...));
}
template <class T>
constexpr bool equal(const T& Left, const T& Right) {
  return Left == Right;
}
// template <bool Con, class T1, class T2>
// constexpr bool conditional(T1&& l, T2&& r) {
//   // if(Con) ? l : r;
// }
XSL_END
#endif  // !UTILITY
