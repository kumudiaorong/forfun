#pragma once
#ifndef XSL_LIST
#define XSL_LIST
#include"allocator.h"
#include"node.h"
#include"iterator.h"
XSL_BEGIN

template <typename _Val, typename _SizeType, typename _Alloc>
class list;

template<class _List>
struct list_gptr :public node_base_gptr<_List> {
	using Base = node_base_gptr<_List>;
	using Addr_T = typename _List::Addr_T;
	//
	using Base::Base;
	constexpr list_gptr& operator++() {
		Addr = Mgr.nbr(Addr, 0);
		return *this;
	}
	//
	constexpr list_gptr operator++(int) {
		Addr_T Tmp = Addr;
		Addr = Mgr.nbr(Addr, 0);
		return { Mgr,Tmp };
	}
	//
	constexpr list_gptr& operator--() {
		Addr = Mgr.nbr(Addr, 1);
		return *this;
	}
	//
	constexpr list_gptr operator--(int) {
		Addr_T Tmp = Addr;
		Addr = Mgr.nbr(Addr, 1);
		return { Mgr,Tmp };
	}
	using Base::Mgr, Base::Addr;
};


template <typename _Val, typename _SizeType = uint_32, typename _Alloc = fixed_block_allocator<_Val, _SizeType>>
class list {
public:
	using Val_T = _Val;
	using Size_T = _SizeType;
	using Mgr_T = node_manager<_Alloc, 2>;
	using Addr_T = Mgr_T::Addr_T;
	using GPtr_T = list_gptr<list>;
	using BIter = basic_iterator<list>;
	using Iter = forward_bilateral_iterator<list>;
	using RIter = reverse_bilateral_iterator<list>;
	using Base_iter = typename Iter::Base;


private:
	// template<typename...Args>
	// constexpr void insert_helper(_Node* node, Args&&... params) {
	// 	node->Pre = node->Pre->Next = new _Node(node->Pre, node, forward<Args>(params)...);
	// 	++Size;
	// }
	// constexpr void remove_helper(_Node* node)
	// {
	// 	(node->Pre->Next = node->Next)->Pre = node->Pre;
	// 	delete node;
	// 	--Size;
	// }
	// constexpr void erase_helper(_Node* Begin, _Node* End) {
	// 	_Node* tmp = Begin->Pre;
	// 	while (tmp->Next != End) {
	// 		remove_helper(Begin);
	// 		Begin = tmp->Next;
	// 	}
	// }
public:

	//
	constexpr list() :Mgr(), Head(Mgr.alloc()), Tail(Mgr.alloc()), Size() {
		Mgr.connect(Head, Tail, 1, 0);
	}
	//
	constexpr list(list&& Another)noexcept : Mgr(as_rreference(Another.Mgr)), Head(Another.Head), Tail(Another.Tail), Size(Another.Size) {
		Another.Head = Addr_T{};
		Another.Tail = Addr_T{};
		Another.Size = 0;
	}
	//
	constexpr list(const list& Another) :Mgr(Another.Mgr), Head(Mgr.alloc()), Tail(Mgr.alloc()), Size(Another.Size) {
		Mgr.connect(Head, Tail, 1, 0);
		Insert(Tail, Another.Mgr, Another.Mgr.nbr(Another.Head, 0), Another.Mgr.nbr(Another.Tail, 1));
	}
	constexpr list& operator=(list&& Another) {
		erase_helper(Head->Next, Tail);
		Size = Another.Size;
		if (Size > 0) {
			(Head->Next = Another.Head->Next)->Pre = Head;
			(Tail->Pre = Another.Tail->Pre)->Next = Tail;
		}
		delete Another.Head;
		delete Another.Tail;
		Another.Head = nullptr;
		return *this;
	}
	constexpr list& operator=(const list& Another) {
		//erase_helper(Head->Next, Tail);
		//_Node* tmp = Another.Head->Next;
		//while (tmp != Another.Tail) {
		//	insert_helper(Tail, tmp->Obj);
		//	tmp = tmp->Next;
		//}
		return *this;
	}
	constexpr _Val& back() {
		return *Mgr.nbr_addr(Tail, 1);
	}
	//
	constexpr Iter begin() {
		return GPtr_T{ Mgr,Mgr.nbr(Head,0) };
	}
	//
	constexpr Iter end() {
		return GPtr_T{ Mgr,Tail };
	}
	//
	constexpr RIter rbegin() {
		return GPtr_T{ Mgr,Mgr.nbr(Tail,1) };
	}
	//
	constexpr RIter rend() {
		return GPtr_T{ Mgr,Head };
	}
	//
		// constexpr void clear() {
		// 	erase_helper(Head->Next, Tail);
		// }
	template<typename...Args>
	constexpr void emplace(Base_iter Pos, Args&&...Params) {
		Insert(Pos.Ptr.Addr, Mgr.construct(forward<Args>(Params)...));
	}
	template<typename...Args>
	constexpr void emplace_back(Args&&...Params) {
		Insert(Tail, Mgr.construct(forward<Args>(Params)...));
	}
	// template<typename...Args>
	// constexpr void emplace_front(Args&&...Params) {
	// 	insert_helper(Head->Next, forward<Args>(Params)...);
	// }
	// constexpr Fwd_iter end() {
	// 	return { Tail };
	// }
	constexpr void erase(Base_iter First, Base_iter Last) {

	}
	/*constexpr void remove(Base_iter Pos) {
		Mgr.connect(Pos.Ptr.Addr, 0, 1);
		Mgr.destroy(Pos.Ptr.Addr);
	}*/
	// constexpr void expand(Base_iter pos, uint_t num)
	// {
	// 	for (uint_t i = 0; i < num; ++i)
	// 		insert_helper(pos.Ptr, '\0');
	// }
	// constexpr Val& front()
	// {
	// 	return Head->Next->Obj;
	// }
	template<typename..._Vals>
	constexpr void insert(Base_iter Pos, const _Val& Val) {
		Insert(Pos.Ptr.Addr, Mgr.construct(_Val));
	}
	//
	constexpr void insert(Base_iter Pos, _Val&& Val) {
		Insert(Pos.Ptr.Addr, Mgr.construct(static_cast<_Val&&>(Val)));
	}
	//
	template<typename GIter, enable_construct<Base_iter, typename GIter::Base> = 0>
	constexpr void insert(Base_iter Pos, GIter First, GIter Last) {
		Insert(Pos.Ptr.Addr, First, Last);
	}
	//
	constexpr void insert(Base_iter Pos, Base_iter First, Base_iter Last) {
		Insert(Pos.Ptr.Addr, First.Ptr.Mgr, First.Ptr.Addr, Last.Ptr.Addr);
	}
	//
	constexpr void insert(Base_iter Pos, _SizeType Count, const _Val& Val) {
		Insert(Pos.Ptr.Addr, Mgr.construct(static_cast<_Val&&>(Val)));
	}
	//
	constexpr void push_back(_Val&& Val) {
		Insert(Tail, Mgr.construct(static_cast<_Val&&>(Val)));
	}
	//
	constexpr void push_back(const _Val& Val) {
		Insert(Tail, Mgr.construct(Val));
	}
	//
	constexpr void push_front(_Val&& Val) {
		Insert(Mgr.nbr(Head, 0), Mgr.construct(static_cast<_Val&&>(Val)));
	}
	//
	constexpr void push_front(const _Val& Val) {
		Insert(Mgr.nbr(Head, 1), Mgr.construct(Val));
	}
	// constexpr Rvs_iter rbegin() {

	// 	return { Tail->Pre };
	// }
	// constexpr Rvs_iter rend() {

	// 	return { Head };
	// }
	// constexpr void resize(uint_t size) {

	// 	while (size < Size)
	// 		remove_helper(Tail->Pre);
	// 	while (size > Size)
	// 		insert_helper(Tail);
	// }
	constexpr _SizeType size() {
		return Size;
	}
	constexpr ~list() {
		if (Head != Addr_T{} && Tail != Addr_T{} && Size) {
			auto Tmp = Mgr.nbr(Head, 0);
			Mgr.free(Head);
			while (Tmp != Tail) {
				Head = Tmp;
				Tmp = Mgr.nbr(Tmp, 0);
				Mgr.destroy(Head);
			}
			Mgr.free(Tmp);
		}
	}
private:
	constexpr void Insert(Addr_T Addr, Addr_T Ins) {
		Mgr.connect(Mgr.nbr(Addr, 1), Ins, 1, 0);
		Mgr.connect(Ins, Addr, 1, 0);
		++Size;
	}
	constexpr void Insert(Addr_T Addr, Mgr_T Another, Addr_T First, Addr_T Last) {
		Addr_T Ins_Pre = Mgr.nbr(Addr, 1), Ins_Next = Addr;
		for (; First != Last; First = Another.nbr(First, 0)) {
			Addr_T New = Mgr.construct(*Another.addr(First));
			Mgr.connect(Ins_Pre, New, 1, 0);
			Ins_Pre = New;
		}
		Mgr.connect(Ins_Pre, Ins_Next, 1, 0);
	}
	constexpr void Insert(Addr_T Addr, Base_iter First, Base_iter Last) {
		Addr_T Ins_Pre = Mgr.nbr(Addr, 1), Ins_Next = Addr;
		for (; First != Last; ++First, ++Size) {
			Addr_T New = Mgr.construct(*First);
			Mgr.connect(Ins_Pre, New, 1, 0);
			Ins_Pre = New;
		}
		Mgr.connect(Ins_Pre, Ins_Next, 1, 0);
	}
	//
	Mgr_T Mgr;
	Addr_T Head, Tail;
	_SizeType Size;         // size
};
//template <typename Val>
//class list {
//public:
//	using Size_T = uint_32;
//	using Val_T = Val;
//	using _Alloc = node_allocator<Val, uint_32, 2>;
//	using GPtr_T = list_gptr<Val>;
//	using Iter = forward_bilateral_iterator<list>;
//	using RIter = reverse_bilateral_iterator<list>;
//	using Base_iter = typename Iter::Base;
//	//
//	constexpr list() :Alc(), Head(Alc.Construct(alloc_tag::empty{})), Tail(Alc.Construct(alloc_tag::empty{})), Size() {
//		Alc.Connect(Head, Tail, 1, 0);
//	}
//
//private:
//	// using Base_iter = Iter::Base;
//	// template<typename...Args>
//	// constexpr void insert_helper(_Node* node, Args&&... params) {
//	// 	node->Pre = node->Pre->Next = new _Node(node->Pre, node, forward<Args>(params)...);
//	// 	++Size;
//	// }
//	// constexpr void remove_helper(_Node* node)
//	// {
//	// 	(node->Pre->Next = node->Next)->Pre = node->Pre;
//	// 	delete node;
//	// 	--Size;
//	// }
//	// constexpr void erase_helper(_Node* Begin, _Node* End) {
//	// 	_Node* tmp = Begin->Pre;
//	// 	while (tmp->Next != End) {
//	// 		remove_helper(Begin);
//	// 		Begin = tmp->Next;
//	// 	}
//	// }
//public:
//
//
//	// constexpr list(list&& Another) : Head(Another.Head), Tail(Another.Tail), Size(Another.Size) {
//	// 	Another.Head = nullptr;
//	// }
//	// constexpr list(const list& Another) : list() {
//
//	// 	_Node* tmp = Another.Head->Next;
//	// 	while (tmp != Another.Tail) {
//	// 		insert_helper(Tail, tmp->Obj);
//	// 		tmp = tmp->Next;
//	// 	}
//	// }
//	// constexpr list& operator=(list&& Another) {
//
//	// 	erase_helper(Head->Next, Tail);
//	// 	Size = Another.Size;
//	// 	if (Size > 0) {
//	// 		(Head->Next = Another.Head->Next)->Pre = Head;
//	// 		(Tail->Pre = Another.Tail->Pre)->Next = Tail;
//	// 	}
//	// 	delete Another.Head;
//	// 	delete Another.Tail;
//	// 	Another.Head = nullptr;
//	// 	return *this;
//	// }
//	// constexpr list& operator=(const list& Another) {
//
//	// 	erase_helper(Head->Next, Tail);
//	// 	_Node* tmp = Another.Head->Next;
//	// 	while (tmp != Another.Tail) {
//	// 		insert_helper(Tail, tmp->Obj);
//	// 		tmp = tmp->Next;
//	// 	}
//	// 	return *this;
//	// }
//	constexpr Val& back() {
//		return Alc.Get_Val(Alc.Get_Next(Tail, 0));
//	}
//	//
//	constexpr Iter begin() {
//		return GPtr_T{ Alc,Alc.Get_Next(Head,1) };
//	}
//	//
//	constexpr Iter end() {
//		return GPtr_T{ Alc,Tail };
//	}
//	//
//	constexpr RIter rbegin() {
//		return GPtr_T{ Alc,Alc.Get_Next(Tail,0) };
//	}
//	//
//	constexpr RIter rend() {
//		return GPtr_T{ Alc,Head };
//	}
//	//
//		// constexpr void clear() {
//		// 	erase_helper(Head->Next, Tail);
//		// }
//	template<typename...Args>
//	constexpr void emplace(Base_iter Pos, Args&&...Params) {
//		Insert(Pos.Ptr.Off, Alc.Construct(forward<Args>(Params)...));
//	}
//	template<typename...Args>
//	constexpr void emplace_back(Args&&...Params) {
//		Insert(Tail, Alc.Construct(forward<Args>(Params)...));
//	}
//	// template<typename...Args>
//	// constexpr void emplace_front(Args&&...Params) {
//	// 	insert_helper(Head->Next, forward<Args>(Params)...);
//	// }
//	// constexpr Fwd_iter end() {
//	// 	return { Tail };
//	// }
//	// constexpr void erase(Base_iter Begin, Base_iter End)
//	// {
//	// 	erase_helper(Begin.Ptr, End.Ptr);
//	// }
//	constexpr void remove(Base_iter Pos) {
//		Alc.Connect(Alc.Get_Next(Pos.Ptr.Off, 0), Alc.Get_Next(Pos.Ptr.Off, 1), 1, 0);
//		Alc.Destroy(Pos.Ptr.Off);
//	}
//	// constexpr void expand(Base_iter pos, uint_t num)
//	// {
//	// 	for (uint_t i = 0; i < num; ++i)
//	// 		insert_helper(pos.Ptr, '\0');
//	// }
//	// constexpr Val& front()
//	// {
//	// 	return Head->Next->Obj;
//	// }
//	// template<typename...Vals>
//	constexpr void insert(Base_iter Pos, const Val& _Val) {
//		Insert(Pos.Ptr.Off, Alc.Construct(_Val));
//	}
//	//
//	constexpr void insert(Base_iter Pos, Val&& _Val) {
//		Insert(Pos.Ptr.Off, Alc.Construct(static_cast<Val&&>(_Val)));
//	}
//	//
//	constexpr void insert(Base_iter Pos, Size_T Count,const Val& _Val) {
//		Insert(Pos.Ptr.Off, Alc.Construct(static_cast<Val&&>(_Val)));
//	}
//	//
//	constexpr void push_back(Val&& _Val) {
//		Insert(Tail, Alc.Construct(static_cast<Val&&>(_Val)));
//	}
//	//
//	constexpr void push_back(const Val& _Val) {
//		Insert(Tail, Alc.Construct(_Val));
//	}
//	//
//	constexpr void push_front(Val&& _Val) {
//		Insert(Alc.Get_Next(Head, 1), Alc.Construct(static_cast<Val&&>(_Val)));
//	}
//	//
//	constexpr void push_front(const Val& _Val) {
//		Insert(Alc.Get_Next(Head, 1), Alc.Construct(_Val));
//	}
//	// constexpr void push_front(const Val& obj) {
//
//	// 	insert_helper(Head->Next, obj);
//	// }
//	// constexpr void pop_front() {
//
//	// 	remove_helper(Head->Next);
//	// }
//	// constexpr void pop_back() {
//
//	// 	remove_helper(Tail->Pre);
//	// }
//	// constexpr Rvs_iter rbegin() {
//
//	// 	return { Tail->Pre };
//	// }
//	// constexpr Rvs_iter rend() {
//
//	// 	return { Head };
//	// }
//	// constexpr void resize(uint_t size) {
//
//	// 	while (size < Size)
//	// 		remove_helper(Tail->Pre);
//	// 	while (size > Size)
//	// 		insert_helper(Tail);
//	// }
//	constexpr Size_T size() {
//		return Size;
//	}
//	constexpr ~list() {}
//	// 	if (Head) {
//	// 		erase_helper(Head->Next, Tail);
//	// 		delete Head;
//	// 		delete Tail;
//	// 	}
//	// }
//private:
//	constexpr void Insert(Size_T Pos, Size_T Ins) {
//		Alc.Connect(Alc.Get_Next(Pos, 0), Ins, 1, 0);
//		Alc.Connect(Ins, Pos, 1, 0);
//		++Size;
//	}
//	//
//	_Alloc Alc;
//	Size_T Head, Tail, Size;         // size
//};
#ifdef XSL_TEST
//
void xsl_test_list() {
	TEST_ACT_PRINT(list<test> ls00);
	TEST_RET_PRINT(ls00, print);
	TEST_ACT_PRINT(ls00.push_back(1));
	TEST_RET_PRINT(ls00, print);
	TEST_ACT_PRINT(ls00.push_front(0));
	TEST_RET_PRINT(ls00, print);
	TEST_ACT_PRINT(ls00.push_back(2));
	TEST_RET_PRINT(ls00, print);
	TEST_ACT_PRINT(ls00.emplace(++ls00.begin(), 3));
	TEST_RET_PRINT(ls00, print);
	TEST_ACT_PRINT(ls00.insert(++ls00.begin(), 3));
	TEST_RET_PRINT(ls00, print);
	TEST_ACT_PRINT(list<test> ls01{ ls00 });
	TEST_RET_PRINT(ls01, print);
	TEST_ACT_PRINT(list<test> ls02{ as_rreference(ls00) });
	TEST_RET_PRINT(ls02, print);
	TEST_RET_PRINT(ls02, rprint);
}
#endif
//
XSL_END
#endif // !XSL_LIST