#pragma once
#ifndef XSL_STRING_ALGO
#define XSL_STRING_ALGO
#include <xsl/bits/batch.hpp>
#include <xsl/bits/def.hpp>

namespace xsl {
  template <class Val>
  constexpr size_t strchr(const Val *str, size_t len, Val end) noexcept {
    size_t i = 0;
    for(; i < len; ++i)
      if(str[i] == end)
        break;
    return i == len ? npos : i;
  }
  //
  template <class Val>
  constexpr size_t strchr(const Val *str, Val end) noexcept {
    size_t i = 0;
    for(; str[i] != Val{}; ++i)
      if(str[i] == end)
        break;
    return str[i] == Val{} ? npos : i;
  }
  //
  template <class Val>
  constexpr size_t strstr(const Val *str, const Val *substr, size_t slen) noexcept {
    size_t i = 0;
    while(true) {
      size_t m = 0;
      for(size_t n = i; m < slen; ++m, ++n)
        if(str[n] == Val{})
          return npos;
        else if(str[n] != substr[m])
          break;
      if(m != slen) {
        Val c = str[i + slen];
        for(m = slen; m > 0; --m)
          if(substr[m - 1] == c)
            break;
        i += slen - m + 1;
      } else
        break;
    }
    return i;
  }
  template <class Val>
  constexpr size_t strstr(const Val *str, const Val *substr) noexcept {
    return strstr(str, substr, batch::length(substr));
  }
  template <class Val>
  constexpr size_t strstr(const Val *str, size_t len, const Val *substr, size_t slen) noexcept {
    size_t i = 0;
    for(; i <= len - slen;) {
      size_t m = 0;
      for(size_t n = i; m < slen; ++m, ++n)
        if(str[n] != substr[m])
          break;
      if(m != slen) {
        Val c = str[i + slen];
        for(m = slen; m > 0; --m)
          if(substr[m - 1] == c)
            break;
        i += slen - m + 1;
      } else
        break;
    }
    return i > len - slen ? npos : i;
    // Sunday algorithm
  }
  template <class Val>
  constexpr size_t strstr(const Val *str, size_t len, const Val *substr) noexcept {
    return strstr(str, len, substr, batch::length(substr));
  }
  //
}  // namespace xsl
#endif  // !XSL_BASIC_STRING
