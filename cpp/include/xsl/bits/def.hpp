#ifndef XSL_BASE_DEF
#define XSL_BASE_DEF
// x standard library
#include <xsl/bits/pf/def.h>
namespace xsl {
  template <typename... Ts>
  inline constexpr void pass(Ts&&...) noexcept {
  }
  static const size_t npos = static_cast<size_t>(-1);
  struct tag_store {};
}  // namespace xsl

#endif  // XSL_BASE_DEF