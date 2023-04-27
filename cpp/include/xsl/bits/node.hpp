#pragma once
#include "xsl/bits/pf/def.h"
#ifndef XSL_NODE_SUPPORT
#define XSL_NODE_SUPPORT
#include <xsl/bits/def.hpp>
namespace xsl::nd {
  template <class _Val, uint_8 N>
  class node {
  public:
    typedef _Val val_type;
    typedef size_t size_type;
    val_type Val;
    node *Next[N];
    node()
      : Val()
      , Next() {
    }
    node(const node&) = default;
  };
}  // namespace xsl::nd
#endif