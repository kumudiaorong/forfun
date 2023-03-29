#pragma once
#ifndef TLS_BITS
#define TLS_BITS
#include"def.h"
namespace tls {
	template<uint_t Size>
	class bits {

		char Array[Size / 8 + bool(Size % 8) + 1];
		//space consumption
		constexpr uint_t spcs() {
			return Size / 8 + bool(Size % 8) + 1;
		}
	public:
		bits() {
			for (uint_t i = 0; i < spcs(); ++i)
				Array[i] = '\0';
		}
		constexpr bool get(uint_t Pos) {
			if (Pos >= Size)throw "wrong location";
			return Array[Pos / 8] & (char(1) << (Pos % 8));
		}
		constexpr void set(uint_t Pos, bool val) {
			if (Pos >= Size)throw "wrong location";
			if (val)Array[Pos / 8] |= (char(1) << (Pos % 8));
			else Array[Pos / 8] &= ((char(1) << (Pos % 8)) ^ char(255));
		}
	};
}
#endif // !TLS_BITS
