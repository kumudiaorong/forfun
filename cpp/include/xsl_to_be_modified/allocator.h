#pragma once
#ifndef XSL_ALLOCATOR
#define XSL_ALLOCATOR
#include"pair.h"
//#include"memory.h"
#include"exception.h"
XSL_BEGIN
//
template<typename _Val, typename _SizeType = uint_64>
_Val* xalloc(_SizeType ByteSize) {
	return (_Val*)new char[ByteSize];
}
template<typename _Val, typename _SizeType = uint_64>
_Val* xalloc_n(_SizeType  Size) {
	return (_Val*)new char[Size * sizeof(_Val)];
}
void xfree(void* Ptr) {
	delete[](char*)Ptr;
}
//
template<typename _Val, typename..._Args>
constexpr void Construct_At(_Val* Ptr, _Args&&...Params) {
	new(Ptr)_Val(forward<_Args>(Params)...);
}
//
template<typename _Val, typename..._Args>
constexpr _Val* Construct(_Args&&...Params) {
	_Val* Ptr = xalloc_n<_Val>(1);
	Construct_At(Ptr, forward<_Args>(Params)...);
	return Ptr;
}
//
template<typename _Val>
constexpr void Destroy_At(_Val* Ptr) {
	Ptr->~_Val();
}
//
template<typename _Val>
constexpr void Destroy(_Val* Ptr) {
	Destroy_At(Ptr);
	xfree(Ptr);
}

//template<typename Val, typename SizeType, uint_8 Br>
//struct block {
//
//	Val Obj;
//	SizeType Next[Br];
//	int_8 Sign;
//	constexpr block() :Obj(), Next(), Sign() { }
//	//
//	template<typename Arg, typename...Args, enable<!is_same<block, rm_cvr<Arg>>> = 0>
//	constexpr block(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...), Next(), Sign() {}
//	//
//	constexpr block(const block& Ano) : Obj(Ano.Obj), Sign(Ano.Sign) { Copy(var::make_index_sequence<Br>{}, Ano.Next); }
//	//
//private:
//	template<uint_8...Id>
//	constexpr void Copy(var::uint8_s<Id...>, const SizeType(&AnoNext)[Br]) {
//		unfold((Next[Id] = AnoNext[Id], 0)...);
//	}
//
//};
//template<typename Val, typename SizeType>
//struct block<Val, SizeType, 0> {
//	Val Obj;
//	int_8 Sign;
//	constexpr block() :Obj(), Sign() { }
//
//	template<typename Arg, typename...Args, enable<!is_same<block, rm_cvr<Arg>>> = 0>
//	constexpr block(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...), Sign() {}
//
//	constexpr block(const block& Ano) : Obj(Ano.Obj), Sign(Ano.Sign) {}
//};


//constexpr uint_32 dynamic_block_minisize = 16;
//template<typename Val, typename SizeType = uint_32>
//class dynamic_block {
//public:
//	constexpr dynamic_block() :dynamic_block(dynamic_block_minisize) {}
//	//
//	constexpr dynamic_block(SizeType Count) : Head(xalloc_n<Val>(Count)), Cap(Count) {};
//	//
//	constexpr dynamic_block(dynamic_block&& Ano)noexcept :Head(Ano.Head), Cap(Ano.Cap) {
//		Ano.Head = nullptr;
//		Ano.Cap = 0;
//	}
//	//
//	constexpr dynamic_block(const dynamic_block& Ano, SizeType Size) : Head(xalloc_n<Val>(Ano.Cap)), Cap(Ano.Cap) {
//		Copy_Inde(Head, Ano.Head, Size);
//	}
//	//
//	constexpr Val& operator[](SizeType Off) {
//		return Head[Off];
//	}
//	//
//	constexpr void Recap_with_moving(SizeType NewCap, SizeType Size) {
//		if (Head == nullptr) {
//			Head = xalloc_n<Val>(NewCap);
//			Cap = NewCap;
//		}
//		else if (Cap < NewCap) {
//			SizeType TarCap = Cap;
//			do TarCap <<= 1; while (TarCap < NewCap);
//			Val* NewHead = xalloc_n<Val>(TarCap);
//			XSL_ALLOC_CHECK(NewHead);
//			Copy_Cont(NewHead, Head, Size);
//			xfreeate(Head);
//			Head = NewHead;
//			Cap = TarCap;
//		}
//	}
//	//
//	constexpr ~dynamic_block() {
//		if (Head && Cap) {
//			xfree(Head);
//		}
//	}
//private:
//	//
//	Val* Head;
//	SizeType Cap;
//};
//template<typename Val, typename SizeType = uint_32>
//class dynamic_array {
//	dynamic_array(SizeType _Cap) :Head(xalloc_n<Val>(_Cap)), Cap(_Cap) {}
//	dynamic_array(SizeType _Cap, const Val& Init_Val) :dynamic_array(_Cap) {
//		Set_Inde(Head, _Cap, Init_Val);
//	}
//	Val& operator[](uint_64 Off) {
//		if (Off >= Cap) {
//			uint_64 NewCap = Cap;
//			while (NewCap <= Off)NewCap <<= 1;
//			Val* NewHead = xalloc_n<Val>(NewCap);
//			XSL_ALLOC_CHECK(NewHead);
//			Copy_Cont(NewHead, Head, sizeof(Val), Cap);
//			xfree(Head);
//			Head = NewHead;
//			Cap = NewCap;
//		}
//		return Head[Off];
//	}
//	~dynamic_array() {
//
//	}
//private:
//	Val* Head;
//	SizeType Cap;
//};
//Idle block management
template<typename _SizeType = uint_32>
struct IBmgr {
	pair<_SizeType, _SizeType>* Head;
	_SizeType Cap, Size;
	constexpr IBmgr(_SizeType _Cap) :Head(xalloc_n<pair<_SizeType, _SizeType>>(_Cap)), Cap(_Cap), Size(1) {
		Head[0] = { 0,_Cap };
	}
	//Section
	constexpr void EXPA(pair<_SizeType, _SizeType> Sect) {
		Head[Size] = Sect;
	}
	constexpr _SizeType POP() {
		if (!Size)return -1;
		_SizeType Ret = Head[Size - 1].second - 1;
		if (Head[Size - 1].first == Ret)--Size;
		else --Head[Size - 1].second;
		return Ret;
	}
	constexpr void PUSH(_SizeType Off) {
		uint_32 Left = 0, Right = Size - 1, Mid;
		while (Left < Right) {
			Mid = (Left + Right + 1) >> 1;
			if (Head[Mid].second < Off)Left = Mid;
			else if (Head[Mid].second == Off)break;
			else Right = Mid - 1;
		}
		if (Head[Left].second == Off) {
			++Head[Left].second;
			if (Left < Size - 1 && Head[Left + 1].first - 1 == Off) {
				Head[Left].second = Head[Left + 1].second;
				Copy_Cont(Head + Left + 1, Head + Left + 2, sizeof(pair<_SizeType, _SizeType>) * (Size - Left - 2));
				--Size;
			}
		}
		else if (Left < Size - 1 && Head[Left + 1].first - 1 == Off) {
			--Head[Left + 1].first;
		}
		else {
			if (Size == Cap) {
				pair<_SizeType, _SizeType>* NewHead = xalloc_n<pair<_SizeType, _SizeType>>(static_cast<uint_64>(Cap) << 1);
				XSL_ALLOC_CHECK(NewHead);
				Copy_Cont(NewHead, Head, sizeof(pair<_SizeType, _SizeType>) * (static_cast<uint_64>(Left) + 1));
				new(NewHead + Left + 1)pair<_SizeType, _SizeType>{Off, Off + 1};
				Copy_Cont(NewHead + Left + 2, Head + Left + 2, sizeof(pair<_SizeType, _SizeType>) * (Size - Left - 2));
				XSL xfree(Head);
				Head = NewHead;
				Cap <<= 1;
			}
			else {
				Copy_Cont(Head + Left + 1, Head + Left + 2, sizeof(pair<_SizeType, _SizeType>) * (Size - Left - 1));
				new(Head + Left + 1)pair<_SizeType, _SizeType>{Off, Off + 1};
			}
			++Size;
		}
	}
	~IBmgr() { XSL xfree(Head); };
};
//
//
#ifdef XSL_TEST
//
//void xsl_test_IBmgr() {
//	xsl::IBmgr<> IBmgr00(32);
//	xsl_test_IBmgr_print(IBmgr00, "origin");
//	IBmgr00.POP();
//	IBmgr00.POP();
//	IBmgr00.POP();
//	IBmgr00.POP();
//	IBmgr00.PUSH(31);
//	xsl_test_IBmgr_print(IBmgr00, "pre connect start");
//	IBmgr00.PUSH(28);
//	xsl_test_IBmgr_print(IBmgr00, "pre connect over");
//	xsl_test_IBmgr_print(IBmgr00, "next connect start");
//	IBmgr00.PUSH(30);
//	xsl_test_IBmgr_print(IBmgr00, "next connect over");
//	xsl_test_IBmgr_print(IBmgr00, "mid connect start");
//	IBmgr00.PUSH(29);
//	xsl_test_IBmgr_print(IBmgr00, "mid connect over");
//}
//origin: (0, 32)
//pre connect start : (0, 28)(31, 32)
//pre connect over : (0, 29)(31, 32)
//next connect start : (0, 29)(31, 32)
//next connect over : (0, 29)(30, 32)
//mid connect start : (0, 29)(30, 32)
//mid connect over : (0, 32)
#endif

//namespace alloc_tag {
//	struct empty {};
//}
//
constexpr uint_16 XSL_ALLOC_MINISIZE = 16;
//template<typename Val, typename SizeType, uint_8 Br>
//class node_allocator {
//public:
//	using Block = block<Val, SizeType, Br>;
//	//
//	constexpr node_allocator(SizeType _Devi = 0) :ILM(XSL_ALLOC_MINISIZE), Head(XSL xalloc_n<Block>(XSL_ALLOC_MINISIZE)), Cap(XSL_ALLOC_MINISIZE), Devi(_Devi + 1) {}
//	//
//	// constexpr block_allocator (block_allocator && Ano)noexcept : Head((this->~block_allocator (), Ano.Head)), Cap(Ano.Cap) {
//	//     Ano.Head = nullptr;
//	//     Ano.Cap = 0;
//	// }
//	// //
//	// constexpr block_allocator (const block_allocator & Ano) : Head(xalloc_n<Ptr_T>(sizeof(Ptr_T)* Ano.Cap)), Cap(Ano.Cap) {
//	//     SizeType Array_Off = 0;
//	//     for (; Array_Off < Ano.Cap && Ano.Head[Array_Off]; ++Array_Off) {
//	//         Head[Array_Off] = xalloc_n<Val>((sizeof(Val) << 5) + 4);
//	//         if (int Doc = *(int*)Head[Array_Off] = *(int*)Ano.Head[Array_Off]; Doc)
//	//             for (uint_8 Ptr_Off = 0; !(Ptr_Off >> 5); ++Ptr_Off)
//	//                 if (Doc & (1 << Ptr_Off))
//	//                     Construct_At(Get_Ptr(Array_Off, Ptr_Off), *deviate(Ano.Head[Array_Off] + Ptr_Off, 4));
//	//     }
//	//     Init_Cont(Head + Array_Off, sizeof(Ptr_T) * (Cap - Array_Off));
//	// }
//	//
//	template<typename...Args>
//	constexpr auto Construct(Args&&...Params) {
//		SizeType Off = ILM.POP();
//		if (!~Off) {
//			Expand();
//			Off = ILM.POP();
//		}
//		new(Head + Off)Block(forward<Args>(Params)...);
//		return Deviate(Off);
//	}
//	//
//	constexpr auto Construct(alloc_tag::empty) {
//		SizeType Off = ILM.POP();
//		if (!~Off) {
//			Expand();
//			Off = ILM.POP();
//		}
//		Head[Off].Sign = 1;
//		return Deviate(Off);
//	}
//	//
//	constexpr void Destroy(SizeType Off) {
//		XSL	Destroy_At(Head + Regress(Off));
//		ILM.PUSH(Regress(Off));
//	}
//	//
//	constexpr void Destroy(alloc_tag::empty, SizeType Off) {
//		ILM.PUSH(Regress(Off));
//	}
//	//
//	constexpr Val& Get_Val(SizeType Off) {
//		return Head[Regress(Off)].Obj;
//	}
//	//
//	constexpr auto Get_Next(SizeType Off, SizeType Next) {
//		return Head[Regress(Off)].Next[Next];
//	}
//	//
//	constexpr void Set_Next(SizeType Off, SizeType Next_Off, SizeType Next) {
//		Head[Regress(Off)].Next[Next] = Next_Off;
//	}
//	//
//	constexpr void Connect(SizeType Left_Off, SizeType Right_Off, SizeType Left_Next, SizeType Right_Next) {
//		Head[Regress(Left_Off)].Next[Left_Next] = Right_Off;
//		Head[Regress(Right_Off)].Next[Right_Next] = Left_Off;
//	}
//	//
//	~node_allocator() {
//		if (Head != nullptr) {
//			SizeType ILM_Off = 0;
//			for (SizeType Off = 0; Off < Cap; ++Off) {
//				if (ILM_Off != ILM.Size && Off == ILM.Head[ILM_Off].first)
//					Off = ILM.Head[ILM_Off++].second;
//				if (!Head[Off].Sign)
//					XSL Destroy_At(Head + Off);
//			}
//			XSL xfree(Head);
//		}
//	}
//protected:
//	constexpr void Expand() {
//		Block* NewHead = XSL xalloc_n<Block>(Cap << 1);
//		XSL_ALLOC_CHECK(NewHead);
//		Copy_Cont(NewHead, Head, sizeof(Block) * Cap);
//		XSL xfree(Head);
//		ILM.EXPA({ Cap,Cap << 1 });
//		Head = NewHead;
//		Cap <<= 1;
//	}
//	constexpr auto Deviate(SizeType Off) {
//		return Off + Devi;
//	}
//	constexpr auto Regress(SizeType Off) {
//		return Off - Devi;
//	}
//	IBmgr<SizeType> ILM;
//	Block* Head;
//	//deviation 
//	SizeType Cap, Devi;
//	//Next store
//};

template<typename _Val, typename _SizeType>
class unfixed_block_allocator {
public:
	using Val_T = _Val;
	using Size_T = _SizeType;
	//
protected:
	struct block {
		Size_T Size;
		union {
			block* Next;
			Val_T Val;
		}Block_Info;
		constexpr block*& next() {
			return Block_Info.Next;
		}
		constexpr Val_T* ptr() {
			return &Block_Info.Val;
		}
	};
	//
public:
	using Addr_T = block*;
	static constexpr Addr_T nulladdr = nullptr;
	//
		//template<typename Other>
		//using rebind_allocator = block_allocator<Other, _SizeType>;
		//
	constexpr unfixed_block_allocator() :Head(xalloc_n<block>(1)) {
		Head->Size = 0;
		Head->next() = nulladdr;
	}
	//
	constexpr unfixed_block_allocator(unfixed_block_allocator&& Ano)noexcept : Head(Ano.Head) {
		Ano.Head = nulladdr;
	}
	//
	constexpr unfixed_block_allocator(const unfixed_block_allocator& Ano) : unfixed_block_allocator() {}
	//
	constexpr unfixed_block_allocator& operator =(const unfixed_block_allocator&) = default;
	constexpr unfixed_block_allocator& operator =(unfixed_block_allocator&& Ano)noexcept {
		Addr_T Tmp = Head;
		while (Tmp->next() != nulladdr)
			Tmp = Tmp->next();
		Tmp->next() = Ano.Head;
		Ano.Head = nulladdr;
		return *this;
	}
	constexpr Val_T* addr(Addr_T Addr)const {
		if (Addr != nulladdr) return Addr->ptr();
		else return nullptr;
	}
	//
	constexpr Addr_T alloc(Size_T Count) {
		Addr_T Addr = Head;
		while (Addr->next() != nulladdr && Addr->next()->Size < Count)
			Addr = Addr->next();
		if (Addr->next() == nulladdr) {
			Addr = xalloc<block>(Max(Count * sizeof(Val_T), sizeof(Addr_T)) - sizeof(Addr_T) + sizeof(block));
			Addr->Size = Count;
		}
		else {
			Addr_T Tmp = Addr->next();
			Addr->next() = Tmp->next();
			Addr = Tmp;
		}
		return Addr;
	}
	constexpr void free(Addr_T Addr) {
		Addr->next() = Head->next();
		Head->next() = Addr;
	}
	//
	~unfixed_block_allocator() {
		if (Head != nulladdr) {
			Addr_T Addr = Head;
			while (Head != nulladdr) {
				Head = Addr->next();
				xfree(Addr);
				Addr = Head;
			}
		}
	}
protected:
	Addr_T Head;
};

template<typename _Val, typename _SizeType>
class fixed_block_allocator {
public:
	using Val_T = _Val;
	using Size_T = _SizeType;
	//
protected:
	struct block {
		uint_8 S;
		union {
			block* Next;
			Val_T Val;
		}Block_Info;
		constexpr block*& next() {
			return Block_Info.Next;
		}
		constexpr Val_T& val() {
			return Block_Info.Val;
		}
	};
public:
	using Addr_T = block*;
	static constexpr Addr_T nulladdr = nullptr;
	//
	//template<typename Other>
	//using rebind_allocator = block_allocator<Other, _SizeType>;
	//
	constexpr fixed_block_allocator() :Head(XSL xalloc_n<block>(XSL_ALLOC_MINISIZE)), Cap(XSL_ALLOC_MINISIZE - 1) {
		Head->S = 1;
		Head->next() = nulladdr;
	}
	//
	constexpr fixed_block_allocator(fixed_block_allocator&& Ano)noexcept : Head(Ano.Head), Cap(Ano.Cap) {
		Ano.Head = nulladdr;
		Ano.Cap = 0;
	}
	//
	constexpr fixed_block_allocator(const fixed_block_allocator& Ano) : fixed_block_allocator() {}
	//
	constexpr Val_T* addr(Addr_T Addr) {
		return &Addr->val();
	}
	//
	constexpr Val_T* addr(Addr_T Addr)const {
		return &Addr->val();
	}
	//
	constexpr Addr_T alloc() {
		Addr_T Addr = Head;
		if (Head->next() == nulladdr) {
			if (!Cap) {
				Head = XSL xalloc_n<block>(XSL_ALLOC_MINISIZE);
				Head->S = 1;
				Cap = XSL_ALLOC_MINISIZE - 1;
			}
			else {
				++Head;
				Head->S = 0;
				--Cap;
			}
			Head->next() = nulladdr;
		}
		else Head = Head->next();
		return Addr;
	}
	constexpr void free(Addr_T Addr) {
		Addr->next() = Head;
		Head = Addr;
	}
	template<typename..._Args>
	constexpr Addr_T construct(_Args&&...Params) {
		Addr_T Addr = alloc();
		XSL Construct_At(&Addr->val(), forward<_Args>(Params)...);
		return Addr;
	}
	constexpr void destroy(Addr_T Addr) {
		XSL Destroy_At(&Addr->val());
		free(Addr);
	}
	//
	~fixed_block_allocator() {
		if (Head && Cap) {
			Addr_T Addr = Head;
			while (true) {
				while (Addr->next() != nulladdr && Addr->next()->S == 0)
					Addr->next() = Addr->next()->next();
				if (Addr->next() != nulladdr)
					Addr = Addr->next();
				else break;
			}
			if (Head->S == 0)Head = Head->next();
			while (Head) {
				Addr = Head->next();
				XSL xfree(Head);
				Head = Addr;
			}
		}
	}
protected:
	Addr_T Head;
	Size_T Cap;
};

XSL_END
#endif//XSL_ALLOCATOR