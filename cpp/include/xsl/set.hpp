#pragma once
#ifndef XSL_SET
#define XSL_SET
#include <xsl/bits/set/set.hpp>

namespace xsl {
  template <class Key, class Comp = fc::less<Key>, class Alloc = alc::default_allocator<Key>>
  using set = ost::set<Key, Comp, Alloc>;
  template <class Key, class Comp = fc::less<Key>, class Alloc = alc::default_allocator<Key>>
  using multiset = ost::multiset<Key, Comp, Alloc>;
}  // namespace xsl
#endif  // !XSL_SET
