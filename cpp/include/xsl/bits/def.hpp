#ifndef XSL_BASE_DEF
#define XSL_BASE_DEF
// x standard library
#include "pf/def.h"
inline void pass() {
}
static int n1{}, n2{}, n3{}, n4{}, n5{};

namespace xsl {
  static const size_t npos = static_cast<size_t>(-1);
  struct tag_store {};
}  // namespace xsl

#endif  // XSL_BASE_DEF