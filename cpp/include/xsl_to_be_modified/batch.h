#pragma once
#ifndef XSL_MEMORY
#define XSL_MEMORY
#include"pf/memory.h"
#include"allocator.h"
#include"iterator.h"
XSL_BEGIN

template<typename Val>
constexpr void OSet_Head_To_Tail(Val* Dest, const Val* Src, uint_64 Size) {
	Val* End = Dest + Size;
	while (Dest != End)
		new(Dest++)Val(Src++);
}
template<typename Val>
constexpr void OSet_Tail_To_Head(Val* Dest, const Val* Src, uint_64 Size) {
	Val* RSrc = Src + Size - 1;
	Val* REnd = Src - 1;
	Val* RDest = Dest + Size - 1;
	while (RDest != REnd)
		new(RDest--)Val(RSrc--);
}
template<typename _Checked_Iter, typename _SizeType = uint_32>
constexpr void OSet(_Checked_Iter Dest, _SizeType Size, const typename iter_extract<_Checked_Iter>::Val_T& Val) {
	if (Size != 0)
		do { *Dest = Val, ++Dest; } while (--Size);
}
template<typename _Checked_Iter>
constexpr void OSet(_Checked_Iter First, _Checked_Iter Last, const typename iter_extract<_Checked_Iter>::Val_T& Val) {
	if (First != Last)
		do { *First = Val, ++First; } while (First != Last);
}
template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType = uint_32>
constexpr void OSet(_Checked_Iter_D Dest, _Checked_Iter_S Src, _SizeType Size) {
	if (Size != 0)
		do { *Dest = *Src, ++Dest, ++Src; } while (--Size);
}
template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType = uint_32>
constexpr void OSet(_Checked_Iter_D Dest, _Checked_Iter_S First, _Checked_Iter_S Last) {
	if (First != Last)
		do { *Dest = *First, ++Dest, ++First; } while (First != Last);
}
template<typename _SizeType, typename _Checked_Iter>
inline _SizeType OLen(_Checked_Iter Dest) {
	_SizeType Count{};
	typename iter_extract<_Checked_Iter>::Val_T End{};
	while (*Dest != End)++Count, ++Dest;
	return Count;
}
template<typename _SizeType, typename _Checked_Iter>
inline _SizeType OLen(_Checked_Iter First, _Checked_Iter Last) {
	return &*Last - &*First;
}
template<typename _SizeType, typename _Checked_Iter, enable<!is_pointer<typename iter_extract<_Checked_Iter>::GPtr_T>> = 0>
inline _SizeType OLen(_Checked_Iter First, _Checked_Iter Last) {
	return OLen_e(First, *Last);
}
template<typename _SizeType, typename _Checked_Iter>
inline _SizeType OLen_e(_Checked_Iter Dest, const typename iter_extract<_Checked_Iter>::Val_T& End) {
	_SizeType Count{};
	while (*Dest != End)++Count, ++Dest;
	return Count;
}

namespace batch {

	//template<typename _Checked_Iter, typename _SizeType>
	//struct Construct_1_to_n_helper {
	//	using Val_T = typename iter_extract<_Checked_Iter>::Val_T;
	//	static constexpr auto Proc(_Checked_Iter Dest, _SizeType Size, const Val_T& Val) {
	//		return (is_pointer<_Checked_Iter> && is_character<Val_T> ? MSet : OSet<Val_T>)(Dest, Size, Val);
	//	}
	//	static constexpr auto Proc(_Checked_Iter First, _Checked_Iter Last, const Val_T& Val) {
	//		return is_pointer<_Checked_Iter> && is_character<Val_T> ? MSet(First, Last - First, Val) : OSet<Val_T>(First, Last, Val);
	//	}
	//};
	//template<typename _Checked_Iter, typename _SizeType>
	//using Construct_1_to_n = Construct_1_to_n_helper<_Checked_Iter, _SizeType>::Proc;


	template<typename _Checked_Iter, typename _SizeType>
	static constexpr void Set_n_by_1(_Checked_Iter Dest, _SizeType Size, const typename iter_extract<_Checked_Iter>::Val_T& Val) {
		using Val_T = typename iter_extract<_Checked_Iter>::Val_T;
		if (Size != 0) (is_pointer<_Checked_Iter> && is_character<Val_T> ? MSet<_SizeType> : OSet<_Checked_Iter, _SizeType>)(Dest, Size, Val);
	}
	template<typename _Checked_Iter, typename _SizeType>
	static constexpr void Set_n_by_1(_Checked_Iter First, _Checked_Iter Last, const typename iter_extract<_Checked_Iter>::Val_T& Val) {
		using Val_T = typename iter_extract<_Checked_Iter>::Val_T;
		if (First != Last) is_pointer<_Checked_Iter>&& is_character<Val_T> ? MSet<_SizeType>(First, Last - First, Val) : OSet<_Checked_Iter, _SizeType>(First, Last, Val);
	}

	template<typename _Checked_Iter, typename _SizeType>
	static constexpr void Construct_n_by_1(_Checked_Iter Dest, _SizeType Size, const typename iter_extract<_Checked_Iter>::Val_T& Val) {
		if (Size != 0)
			if constexpr (is_pointer<_Checked_Iter> && is_character<typename iter_extract<_Checked_Iter>::Val_T>)MSet<_SizeType>(Dest, Size, Val);
			else
				for (_SizeType i = 0; i < Size; ++i, ++Dest)
					Construct_At(&*Dest, Val);
	}
	template<typename _Checked_Iter, typename _SizeType>
	static constexpr void Construct_n_by_1(_Checked_Iter First, _Checked_Iter Last, const typename iter_extract<_Checked_Iter>::Val_T& Val) {
		if (First != Last)
			if constexpr (is_pointer<_Checked_Iter> && is_character<typename iter_extract<_Checked_Iter>::Val_T>)MSet<_SizeType>(First, Last - First, Val);
			else do { Construct_At(&*First, Val); } while (++First != Last);
	}
	//template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType>
	//struct Set_n {
	//	using Val_T = typename iter_extract<_Checked_Iter_D>::Val_T;
	//	static constexpr auto Proc(_Checked_Iter_D Dest, _Checked_Iter_S Src, _SizeType Size) {
	//		return is_pointer<_Checked_Iter_D> && is_pointer<_Checked_Iter_S> && is_basic_type<Val_T> ? MCopy(Dest, Src, Size * sizeof(Val_T)) : OSet<Val_T>(Dest, Src, Size);
	//	}
	//	static constexpr auto Proc(_Checked_Iter_D Dest, _Checked_Iter_S First, _Checked_Iter_S Last) {
	//		return is_pointer<_Checked_Iter_D> && is_pointer<_Checked_Iter_S> && is_basic_type<Val_T> ? MCopy(Dest, First, (Last - First) * sizeof(Val_T)) : OSet<Val_T>(Dest, First, Last);
	//	}
	//};

	template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType>
	static constexpr void Construct_n_by_n(_Checked_Iter_D Dest, _Checked_Iter_S Src, _SizeType Size) {
		using Val_T = typename iter_extract<_Checked_Iter_D>::Val_T;
		if (Size != 0)
			if constexpr (is_pointer<_Checked_Iter_D> && is_pointer<_Checked_Iter_S> && is_basic_type<Val_T>)MCopy<_SizeType>(Dest, Src, Size * sizeof(Val_T));
			else
				for (_SizeType i = 0; i < Size; ++i, ++Dest, ++Src)
					Construct_At(&*Dest, *Src);
	}
	template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType>
	static constexpr void Construct_n_by_n(_Checked_Iter_D Dest, _Checked_Iter_S First, _Checked_Iter_S Last) {
		using Val_T = typename iter_extract<_Checked_Iter_D>::Val_T;
		if (First != Last)
			if constexpr (is_pointer<_Checked_Iter_D> && is_pointer<_Checked_Iter_S> && is_basic_type<Val_T>)MCopy<_SizeType>(Dest, First, (Last - First) * sizeof(Val_T));
			else do { Construct_At(&*Dest, *First); } while (++Dest, ++First != ++Last);
	}
	template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType>
	static constexpr void Set_n_by_n(_Checked_Iter_D Dest, _Checked_Iter_S Src, _SizeType Size) {
		using Val_T = typename iter_extract<_Checked_Iter_D>::Val_T;
		if (Size != 0) is_pointer<_Checked_Iter_D>&& is_pointer<_Checked_Iter_S>&& is_basic_type<Val_T> ? MCopy<_SizeType>(Dest, Src, Size * sizeof(Val_T)) : OSet<_Checked_Iter_D, _Checked_Iter_S, _SizeType>(Dest, Src, Size);
	}
	template<typename _Checked_Iter_D, typename _Checked_Iter_S, typename _SizeType>
	static constexpr void Set_n_by_n(_Checked_Iter_D Dest, _Checked_Iter_S First, _Checked_Iter_S Last) {
		using Val_T = typename iter_extract<_Checked_Iter_D>::Val_T;
		if (First != Last) is_pointer<_Checked_Iter_D>&& is_pointer<_Checked_Iter_S>&& is_basic_type<Val_T> ? MCopy<_SizeType>(Dest, First, (Last - First) * sizeof(Val_T)) : OSet<_Checked_Iter_D, _Checked_Iter_S, _SizeType>(Dest, First, Last);
	}
	template<typename _Checked_Iter, typename _SizeType>
	static constexpr void Destroy_n(_Checked_Iter Dest, _SizeType Size) {
		if constexpr (!is_basic_type<typename iter_extract<_Checked_Iter>::Val_T>)
			for (_SizeType i = 0; i < Size; ++i, ++Dest)
				Destroy_At(&*Dest);
	}
	template<typename _Checked_Iter>
	static constexpr void Destroy_n(_Checked_Iter First, _Checked_Iter Last) {
		if constexpr (!is_basic_type<typename iter_extract<_Checked_Iter>::Val_T>)
			if (First != Last)
				do { Destroy_At(&*First); } while (++First != Last);

	}
	template<typename _SizeType, typename _Checked_Iter>
	static constexpr decltype(auto) Len(_Checked_Iter Dest) {
		return is_pointer<_Checked_Iter> && is_character<typename iter_extract<_Checked_Iter>::Val_T> ?
			MLen<_SizeType>(Dest) : OLen<_SizeType>(Dest);
	}
}

//template<typename Val, typename SizeType, bool MorO = is_trivial<Val>>
//class Mopr;
//
//template<typename Val, typename SizeType>
//class Mopr<Val, SizeType, true> {
//
//	using Val_T = Val;
//	using Size_T = SizeType;
//
//public:
//	//static constexpr auto Copy_Head_To_Tail(Val* Dest, const Val* Src, uint_64 Size) {
//	//	return Copy_Head_To_Tail(Dest, Src, Size * sizeof(Val));
//	//}
//	////
//	//static constexpr auto Copy_Head_To_Tail(Val* Dest, const Val* First, const Val* Last) {
//	//	return Copy_Head_To_Tail(Dest, First, (Last - First) * sizeof(Val));
//	//}
//	////
//	//static constexpr auto Copy_Tail_To_Head(Val* Dest, const Val* Src, uint_64 Size) {
//	//	return Copy_Tail_To_Head(Dest, Src, Size * sizeof(Val));
//	//}
//	////
//	//static constexpr auto Copy_Tail_To_Head(Val* Dest, const Val* First, const Val* Last) {
//	//	return Copy_Tail_To_Head(Dest, First, (Last - First) * sizeof(Val));
//	//}
//	//
//	static inline auto Copy(Val* Dest, const Val* Src, SizeType Size) {
//		return  XSL MCopy(Dest, Src, Size * sizeof(Val));
//	}
//	//
//	static inline auto Copy(Val* Dest, const Val* First, const Val* Last) {
//		return XSL MCopy(Dest, First, (Last - First) * sizeof(Val));
//	}
//	//
//
//	//多参
//	template<bool is_character<Val> = true>
//	static inline auto Set(Val* First, Val* Last, const Val _Val) {
//		return XSL MSet(First, Last - First, _Val);
//	}
//	//
//	template<bool is_character<Val> = true>
//	static inline auto Set(Val* First, SizeType Size, const Val _Val) {
//		return XSL MSet(First, Size, _Val);
//	}
//	//
//	template<bool is_character<Val> = false>
//	static inline auto Set(Val* First, Val* Last, const Val& _Val) {
//		return XSL OSet(First, Last - First, _Val);
//	}
//	//
//	template<bool is_character<Val> = false>
//	static inline auto Set(Val* First, SizeType Size, const Val& _Val) {
//		return XSL OSet(First, Size, _Val);
//	}
//	//
//};
XSL_END
#endif // !XSL_MEMORY
