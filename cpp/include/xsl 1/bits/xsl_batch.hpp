#pragma once
#ifndef XSL_BATCH
#define XSL_BATCH
#include "xsl_allocator.hpp"
#include "xsl_iterator.hpp"
XSL_BEGIN
namespace batch {

  template <class CIter1, class CIter2>
  constexpr int_8 compare(CIter1 first1, CIter2 first2, size_t size) {
    using val_type = typename iter_traits<CIter1>::val_type;
    if constexpr(ts::is_basic_type<val_type> && ts::is_pointer<CIter1> && ts::is_pointer<CIter2>)
      return memcmp(first1, first2, size * sizeof(val_type));
    while(size-- != 0) {
      if(!(*first1 == *first2)) return *first1 < *first2 ? -1 : 1;
      ++first1;
      ++first2;
    }
    return 0;
  }
  //
  template <class CIter1, class CIter2>
  constexpr int_8 compare(CIter1 first1, CIter1 last1, CIter2 first2, CIter1 last2) {
    return 0;
  }
  //
  template <class CIter1, class CIter2>
  constexpr void copy(CIter1 dest, CIter2 src, size_t size) {
    using val_type = typename iter_traits<CIter1>::val_type;
    if constexpr(ts::is_basic_type<val_type> && ts::is_pointer<CIter1> && ts::is_pointer<CIter2>)
      memcpy(dest, src, size * sizeof(val_type));
    else {
      while(size-- != 0) {
        memcpy(XSL addr(*dest), XSL addr(*src), sizeof(val_type));
        ++src;
        ++dest;
      }
    }
  }
  //
  template <class CIter>
  constexpr size_t length(CIter first,
    const typename iter_traits<CIter>::val_type& end = typename iter_traits<CIter>::val_type{}) {
    size_t count{};
    while(*first != end)
      ++first, ++count;
    return count;
  }
  //
  template <class CIter>
  constexpr size_t length(CIter first, CIter last) {
    if(is_random_iter<CIter>) return last - first;
    size_t count{};
    while(first != last)
      ++first, ++count;
    return count;
  }
  //
};  // namespace batch
// template<class  Checked_Iter, class  SizeType>
// static constexpr void Set_n_by_1(Checked_Iter Dest, SizeType Size, const typename
// iter_extract<Checked_Iter>::Val_T& Val) { 	for (; Size > 0; ++Dest, --Size) 		*Dest =
// Val;
//
// }
////
// template<class  Checked_Iter>
// static constexpr void Set_n_by_1(Checked_Iter First, Checked_Iter Last, const typename
// iter_extract<Checked_Iter>::Val_T& Val) { 	for (; First != Last; ++First) 		*First =
// Val;
// }
////
// template<class  Checked_Iter, class  SizeType>
// static constexpr void Construct_n_by_1(Checked_Iter Dest, SizeType Size, const typename
// iter_extract<Checked_Iter>::Val_T& Val) { 	if constexpr (is_basic_type<class
// iter_extract<Checked_Iter>::Val_T>) 		Set_n_by_1(Dest, Size, Val); 	else
// for (; Size > 0; ++Dest,
//--Size) 			Construct_At(&*Dest, Val);
// }
////
// template<class  Checked_Iter, class  SizeType>
// static constexpr void Construct_n_by_1(Checked_Iter First, Checked_Iter Last, const typename
// iter_extract<Checked_Iter>::Val_T& Val) { 	if constexpr (is_basic_type<class
// iter_extract<Checked_Iter>::Val_T>)Set_n_by_1(First, Last, Val); 	else 		for (; First
// != Last;
//++First) 			Construct_At(&*First, Val);
// }
////
// template<class  Checked_Iter_D, class  Checked_Iter_S, class  SizeType>
// constexpr void Set_n_by_n(Checked_Iter_D Dest, Checked_Iter_S Src, SizeType Size) {
//	for (; Size > 0; ++Dest, ++Src, --Size)
//		*Dest = *Src;
// }
////
// template<class  Checked_Iter_D, class  Checked_Iter_S>
// constexpr void Set_n_by_n(Checked_Iter_D Dest, Checked_Iter_S First, Checked_Iter_S Last) {
//	for (; First != Last; ++Dest, ++First)
//		*Dest = *First;
// }
////
// template<class Checked_Iter_D, class  Checked_Iter_S, class  SizeType>
// constexpr void Construct_n_by_n(Checked_Iter_D Dest, Checked_Iter_S Src, SizeType Size) {
//	using Val_T = class  iter_extract<Checked_Iter_D>::Val_T;
//	if constexpr (is_basic_type<class  iter_extract<Checked_Iter_D>::Val_T>)
//		Set_n_by_n(Dest, Src, Size);
//	else
//		for (; Size > 0; ++Dest, ++Src, --Size)
//			Construct_At(&*Dest, *Src);
// }
////
// template<class  Checked_Iter_D, class  Checked_Iter_S, class  SizeType>
// constexpr void Construct_n_by_n(Checked_Iter_D Dest, Checked_Iter_S First, Checked_Iter_S Last) {
//	if constexpr (is_basic_type<class  iter_extract<Checked_Iter_D>::Val_T>)
//		Set_n_by_n(Dest, First, Last);
//	else
//		for (; First != Last; ++Dest, ++First)
//			Construct_At(&*Dest, *First);
// }
////
// template<class  Checked_Iter_D, class  Checked_Iter_S, class  SizeType>
// constexpr void Set_n_by_n_r(Checked_Iter_D Dest, Checked_Iter_S Src, SizeType Size) {
//	for (; Size > 0; --Dest, --Src, --Size)
//		*Dest = *Src;
// }
////
// template<class  Checked_Iter_D, class  Checked_Iter_S>
// constexpr void Set_n_by_n_r(Checked_Iter_D Dest, Checked_Iter_S First, Checked_Iter_S Last) {
//	for (; First != Last; --Dest, --First)
//		*Dest = *First;
// }
////
// template<class Checked_Iter_D, class  Checked_Iter_S, class  SizeType>
// constexpr void Construct_n_by_n_r(Checked_Iter_D Dest, Checked_Iter_S Src, SizeType Size) {
//	using Val_T = class  iter_extract<Checked_Iter_D>::Val_T;
//	if constexpr (is_basic_type<class  iter_extract<Checked_Iter_D>::Val_T>)
//		Set_n_by_n(Dest, Src, Size);
//	else
//		for (; Size > 0; --Dest, --Src, --Size)
//			Construct_At(&*Dest, *Src);
// }
////
// template<class  Checked_Iter_D, class  Checked_Iter_S, class  SizeType>
// constexpr void Construct_n_by_n_r(Checked_Iter_D Dest, Checked_Iter_S First, Checked_Iter_S Last)
// { 	if constexpr (is_basic_type<class  iter_extract<Checked_Iter_D>::Val_T>)
// Set_n_by_n(Dest,
// First, Last); 	else 		for (; First != Last; --Dest, --First)
// Construct_At(&*Dest, *First);
// }
////
// template<class  Checked_Iter, class  SizeType>
// constexpr void Destroy_n(Checked_Iter Dest, SizeType Size) {
//	if constexpr (!is_basic_type<class  iter_extract<Checked_Iter>::Val_T>)
//		for (SizeType i = 0; i < Size; ++i, ++Dest)
//			Destroy_At(&*Dest);
// }
////
// template<class  Checked_Iter>
// constexpr void Destroy_n(Checked_Iter First, Checked_Iter Last) {
//	if constexpr (!is_basic_type<class  iter_extract<Checked_Iter>::Val_T>)
//		for (; First != Last; ++First)
//			Destroy_At(&*First);
// }
//
//
// template<class  Val, class  SizeType, bool MorO = is_trivial<Val>>
// class Mopr;
//
// template<class  Val, class  SizeType>
// class Mopr<Val, SizeType, true> {
//
//	using Val_T = Val;
//	using Size_T = SizeType;
//
// public:
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
// };
XSL_END
#endif  // !XSL_BATCH
