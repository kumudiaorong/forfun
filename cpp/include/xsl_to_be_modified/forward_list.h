#pragma once
#ifndef XSL_FORWARD_LIST
#define XSL_FORWARD_LIST
#include"allocator.h"
#include"node.h"
#include"iterator.h"
XSL_BEGIN

template<typename _Val, typename _SizeType, typename _Alloc>
class forward_list;

template<class Fwd_List >
struct fwd_list_gptr :public node_base_gptr<Fwd_List > {
	using Base = node_base_gptr<Fwd_List>;
	using Addr_T = typename Fwd_List::Addr_T;
	//
	using Base::Base;
	//
	constexpr fwd_list_gptr& operator++() {
		Addr = Mgr.nbr(Addr, 0);
		return *this;
	}
	//
	constexpr fwd_list_gptr operator++(int) {
		Addr_T Tmp = Addr;
		Addr = Mgr.nbr(Addr, 0);
		return { Mgr,Tmp };
	}
	using Base::Mgr, Base::Addr;
};


template<typename _Val, typename _SizeType = uint_32, typename _Alloc = fixed_block_allocator<_Val, _SizeType>>
class forward_list {

public:
	using Val_T = _Val;
	using Size_T = uint_32;

	using Mgr_T = node_manager<_Alloc, 1>;
	using Addr_T = Mgr_T::Addr_T;
	using GPtr_T = fwd_list_gptr<forward_list>;
	using BIter = basic_iterator<forward_list>;
	using Iter = forward_iterator<forward_list>;
	using Base_iter = typename Iter::Base;
	//
	//
	constexpr forward_list() :Mgr(), Head(Addr_T{}) {}
	//
	//constexpr forward_list(forward_list&& Ano)noexcept : Alc(as_rreference(Ano.Alc)), Head(Ano.Head) { Ano.Head = 0; }
	//
	//constexpr forward_list(const forward_list& Ano) : Alc(Ano.Alc), Head(Ano.Head) {}
	//
	constexpr Iter begin() {
		return GPtr_T{ Mgr,Head };
	}
	//
	constexpr Iter end() {
		return GPtr_T{ Mgr,Addr_T{} };
	}
	//
	template<class...Args>
	constexpr void emplace_front(Args&&...Params) {
		Renew_Head(Mgr.construct(forward<Args>(Params)...));
	}
	//
	constexpr bool empty() {
		return !Head;
	}
	constexpr _Val& front() {
		XSL_EMPTY_CHECK(Head);
		return *Mgr.addr(Head);
	}
	constexpr void push_front(_Val&& Val) {
		Renew_Head(Mgr.construct(static_cast<_Val&&>(Val)));
	}

	constexpr void push_front(const _Val& Val) {
		Renew_Head(Mgr.construct(Val));
	}

	constexpr void pop_front() {
		XSL_EMPTY_CHECK(Head);
		Addr_T NewHead = Mgr.nbr(Head, 0);
		Mgr.destroy(Head);
		Head = NewHead;
	}

	constexpr ~forward_list() {
		while (Head) {
			Addr_T Tmp = Mgr.nbr(Head, 0);
			Mgr.destroy(Head);
			Head = Tmp;
		}
	}
private:
	Mgr_T Mgr;
	Addr_T Head;
	constexpr void Renew_Head(Addr_T NewHead) {
		if (Head)Mgr.connect(NewHead, Head, 0);
		Head = NewHead;
	}
};
#ifdef XSL_TEST
void xsl_test_fwd_list() {
	TEST_ACT_PRINT(test t00(2); forward_list<test> fl00);
	TEST_RET_PRINT(fl00, print);
	TEST_ACT_PRINT(fl00.emplace_front(0));
	TEST_RET_PRINT(fl00, print);
	TEST_ACT_PRINT(fl00.push_front(test(1)));
	TEST_RET_PRINT(fl00, print);
	TEST_ACT_PRINT(fl00.push_front(t00));
	TEST_RET_PRINT(fl00, print);
	TEST_ACT_PRINT(fl00.pop_front());
	TEST_RET_PRINT(fl00, print);
}
#endif // XSL_TEST
XSL_END
#endif // !XSL_FORWARD_LIST
