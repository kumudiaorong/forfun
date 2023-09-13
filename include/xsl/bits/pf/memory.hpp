#pragma once
#ifndef XSL_PF_MEMORY
#define XSL_PF_MEMORY
#include "def.h"
// #include<string.h>
#include <cstring>
namespace xsl {
  template <typename _SizeType = uint_32>
  inline void MCopy(void *Dest, const void *Src, _SizeType ByteSize) {
    memcpy(Dest, Src, ByteSize);
  }
  template <typename _SizeType = uint_32>
  inline void MMove(void *Dest, const void *Src, _SizeType ByteSize) {
    memmove(Dest, Src, ByteSize);
  }
  // construct
  template <typename _SizeType = uint_32>
  inline void MSet(void *Dest, const _SizeType Size, char Val = '\0') {
    memset(Dest, Val, Size);
  }
}  // namespace xsl
#endif  // !XSL_PF_MEMORY
