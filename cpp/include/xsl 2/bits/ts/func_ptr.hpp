#pragma once
#ifndef XSL_TYPE_SUPPORT_FUNC_PTR
#define XSL_TYPE_SUPPORT_FUNC_PTR
#include <xsl/bits/def.hpp>
XSL_BEGIN
namespace def::func_ptr {
  template <class>
  inline constexpr bool norm_func_ptr = false;

  template <class Ret, class... Args>
  inline constexpr bool norm_func_ptr<Ret (*)(Args...)> = true;

  template <class>
  inline constexpr bool member_func_ptr = false;

  template <class Ret, class Class>
  inline constexpr bool member_func_ptr<Ret(Class::*)> = true;
}  // namespace def::func_ptr
XSL_END
#endif  // TOT_H