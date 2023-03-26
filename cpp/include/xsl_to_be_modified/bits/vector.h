#pragma once
#ifndef XSL_VECTOR
#define XSL_VECTOR
#include"allocator.h"
#include"pointer.h"
#include"iterator.h"
XSL_BEGIN
template<typename Val>
class vector {
public:
	using Val_T = Val;
	using Size_T = uint_32;
	using GPtr_T = Val*;
	using Iter = random_forward_bilateral_iterator<vector<Val_T>>;
	using Base_iter = typename Iter::Base;

	// inline void assert_accessor(Val* h) {
	// 	if (Head != h)throw "Different Box";
	// }
	// inline void erase_helper(Val* begin, Val* end) {
	// 	Val* t_p = begin;
	// 	while (t_p != end) {
	// 		t_p->~Val();
	// 		++t_p;
	// 	}
	// 	TLS memcpy(begin, end, (Size - (end - Head)) * sizeof(Val));
	// 	Size -= end - begin;
	// }
	// inline void expand()
	// {
	// 	Val* newHead = TLS adjustSpace(Head, Length * 2);
	// 	if (newHead);
	// 	else if (newHead = TLS getSpace<Val>(Length * 2))
	// 	{
	// 		TLS memcpy(newHead, Head, Length * sizeof(Val));
	// 		TLS freeSpace(Head);
	// 	}
	// 	else throw("memory error");
	// 	Head = newHead;
	// 	Length *= 2;
	// }
	// template<typename...Tps>
	// inline void emplace_helper(Val* pos, Tps&&...args)
	// {
	// 	if (Size == Length)expand();
	// 	TLS memcpy(pos + 1, pos, (Size - (pos - Head)) * sizeof(Val));
	// 	new(pos)Val(ARG forward<Tps>(args)...);
	// 	++Size;
	// }
	// template<typename...Tps>
	// inline void insert_helper_1(Val* pos, const Val* objs, uint_t num)
	// {
	// 	while (Size + num > Length)expand();
	// 	TLS memcpy(pos + num, pos, (Size - (pos - Head)) * sizeof(Val));
	// 	for (uint_t i = 0; i < num; ++i)
	// 		new(pos + i)Val(objs[i]);
	// 	Size += num;
	// }
	// template<typename...Tps>requires TMPLT All<TMPLT Same<typename TYPVal remove_cvr<Tps>::Type, Val>...>
	// inline void insert_helper_2(Val* pos, Tps&&...objs)
	// {
	// 	uint_t num = sizeof...(Tps);
	// 	while (Size + num > Length)expand();
	// 	TLS memcpy(pos + num, pos, (Size - (pos - Head)) * sizeof(Val));
	// 	ARG unfold((new(&Head[--num + pos])Val(ARG forward<Tps>(objs)), 0)...);
	// 	Size += num;
	// }

	constexpr vector() :Head(Allocate<Val_T>(1)), Cap(1), Size() {}
	//
	constexpr vector(Size_T Count) : Head(Allocate(Count)), Cap(Count), Size(Count) {
		MemOpr::Init_Cont(Head, Count);
	};
	//
	constexpr vector(vector&& Ano)noexcept :Head(Ano.Head), Cap(Ano.Cap), Size(Ano.Size) {
		Ano.Head = nullptr;
		Ano.Cap = 0;
		Ano.Size = 0;
	}
	//
	constexpr vector(const vector& Ano) :Head(Allocate<Val_T>(Ano.Size)), Cap(Ano.Size), Size(Ano.Size) {
		for (Size_T Off = 0; Off < Size; ++Off)
			Construct_At(Head + Off, Ano.Head[Off]);
	}
	//
	constexpr Val& operator[](Size_T Off) {
		return Head[Off];
	}
	// constexpr vector(const vector& Ano) : Cap(Ano.Cap), Size(Ano.Size) {
	// 	//trace("vector(const vector& v)");
	// 	erase_helper(Head, Head + Size);
	// 	insert_helper_1(Head, v.Head, v.Size);
	// }
	// constexpr Val& at(uint_t id) {
	// 	return Head[id];
	// }
	// constexpr void clear() {
	// 	//trace("clear()");
	// 	erase_helper(Head, Head + Size);
	// }
	constexpr Iter begin() {
		return { Head };
	}
	//
	constexpr Iter end() {
		return { Head + Size };
	}
	//
	constexpr bool empty() {
		return !Size;
	}
	//
	constexpr Val& back() {
		return Head[Size - 1];
	}
	template<typename...Args>
	constexpr void emplace_back(Args&&...Params) {
		Recap_with_moving(Size + 1, Size);
		Construct_At<Val_T>(Head + Size, forward<Args>(Params)...);
		++Size;
	}
	//
	constexpr void push_back(const Val& Val) {
		emplace_back(Val);
	}
	//
	constexpr void push_back(Val_T&& Val) {
		emplace_back(static_cast<Val_T&&>(Val));
	}
	//
	constexpr void pop_back() {
		--Size;
		Destroy_At(Head + Size);
	}
	//
	// //args for one obj or objs
	// template<typename...Tps>
	// constexpr void emplace_front(Tps&&...args)
	// {
	// 	//trace("void emplace_front(Tps&&...args)");
	// 	emplace_helper(Head, ARG forward<Tps>(args)...);
	// }

	// constexpr Val& front() {
	// 	//trace("Val& front()");
	// 	return Head[0];
	// }
	// constexpr accessor find(const Val& obj) {
	// 	uint_t i = 0;
	// 	for (; i < Size; ++i)
	// 		if (Head[i] == obj)break;
	// 	return { Head + i };
	// }
	// template<typename...Tps>
	// constexpr void insert(accessor cur, Tps&&...objs)
	// {
	// 	//trace("void insert(accessor cur, Tps&&...objs)");
	// 	insert_helper_2(cur.pos, ARG forward<Tps>(objs)...);
	// }
	// //args for one obj or objs
	// //args for one obj or objs
	// template<typename...Tps>
	// constexpr void push_front(Tps&&...objs)
	// {
	// 	//trace("void push_front(Tps&&...objs)");
	// 	insert_helper_2(Head, ARG forward<Tps>(objs)...);
	// }
	// constexpr void pop_back() {
	// 	//trace("void pop_back()");
	// 	Head[--Size].~Val();
	// }
	// constexpr void pop_front()
	// {
	// 	//trace("void pop_front()");
	// 	erase_helper(Head, Head + 1);
	// }
	constexpr Size_T size() {
		return Size;
	}
	constexpr ~vector() {
		if (Head && Cap) {
			for (Size_T Off = 0; Off != Size; ++Off)
				Destroy_At(Head + Off);
			Deallocate(Head);
		}
	}
private:
	//
	constexpr void Recap_with_moving(Size_T NewCap, Size_T InsOff) {
		if (Head == nullptr) {
			Head = Allocate<Val_T>(NewCap);
			Cap = NewCap;
		}
		else if (Cap < NewCap) {
			while (Cap < NewCap)Cap <<= 1;
			Val* NewHead = Allocate<Val_T>(Cap);
			if (InsOff)MemOpr::Copy_Cont(NewHead, Head, InsOff);
			if (InsOff != Size)MemOpr::Copy_Cont(NewHead, Head, Size - InsOff);
			Deallocate(Head);
			Head = NewHead;
		}
	}
	using MemOpr = basic_operations<Val, Size_T>;

	Val* Head;
	Size_T Size, Cap;
};
//
#ifdef XSL_TEST
void xsl_test_vector() {
	TEST_ACT_PRINT(vector<test> vt00);
	TEST_RET_PRINT(vt00);
	TEST_ACT_PRINT(vt00.push_back(test{ 1 }));
	TEST_RET_PRINT(vt00);
	TEST_ACT_PRINT(vt00.emplace_back(2));
	TEST_RET_PRINT(vt00);
	TEST_ACT_PRINT(vt00.pop_back());
	TEST_RET_PRINT(vt00);
}
#endif // XSL_TEST


XSL_END
#endif // !XSL_VECTOR
