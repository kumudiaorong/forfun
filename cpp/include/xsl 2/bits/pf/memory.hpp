#pragma once
#ifndef XSL_PF_MEMORY
#define XSL_PF_MEMORY
#include "def.h"
// #include<string.h>
#include <cstring>
XSL_BEGIN
// constexpr void  MCopy_Head_To_Tail(void* Dest, const void* Src, uint_64 ByteSize) {
//	int_32* intSrc = (int_32*)Src;
//	int_32* intEnd = (intSrc + ByteSize / sizeof(int_32));
//	int_32* intDest = (int_32*)Dest;
//	while (intSrc != intEnd)
//		*(intDest++) = *(intSrc++);
//	char* chrSrc = (char*)intEnd;
//	char* chrEnd = (char*)Src + ByteSize;
//	char* chrDest = (char*)intDest;
//	while (chrSrc != chrEnd)
//		*(chrDest++) = *(chrSrc++);
// }
// constexpr void MCopy_Tail_To_Head(void* Dest, const void* Src, uint_64 ByteSize) {
//	int_32* intRSrc = (int_32*)((char*)Src + ByteSize) - 1;
//	int_32* intREnd = (intRSrc - ByteSize / sizeof(int_32));
//	int_32* intRDest = (int_32*)((char*)Dest + ByteSize) - 1;
//	while (intRSrc != intREnd)
//		*(intRDest--) = *(intRSrc--);
//	char* chrRSrc = (char*)intREnd + sizeof(int_32) - 1;
//	char* chrREnd = (char*)Src - 1;
//	char* chrRDest = (char*)intRDest + sizeof(int_32) - 1;
//	while (chrRSrc != chrREnd)
//		*(chrRDest--) = *(chrRSrc--);
// }
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
XSL_END
#endif  // !XSL_PF_MEMORY
