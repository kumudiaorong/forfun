#pragma once
#ifndef XSL_NODE
#define XSL_NODE
#include"utility.h"
XSL_BEGIN

template<typename Alloc, uint_8 Br>
struct node {};
template<template<typename, typename>class Alloc, typename Val, typename SizeType, uint_8 Br>
struct node<Alloc<Val, SizeType>, Br> {
	using Alloc_T = Alloc<node, SizeType>;
	using Addr_T = Alloc_T::Addr_T;
	Val Obj;
	//neighbor
	Addr_T Nbr[Br];
	constexpr node() :Obj(), Nbr() { }
	//
	template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
	constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...), Nbr() {}
	//
	constexpr node(const node& Ano) : Obj(Ano.Obj) { Copy(var::make_index_sequence<Br>{}, Ano.Nbr); }
	//
private:
	template<uint_8...Id>
	constexpr void Copy(var::uint8_s<Id...>, const Addr_T(&AnoNbr)[Br]) {
		unfold((Nbr[Id] = AnoNbr[Id], 0)...);
	}
};
template<template<typename, typename>class Alloc, typename Val, typename SizeType>
struct node<Alloc<Val, SizeType>, 0> {
	Val Obj;
	constexpr node() :Obj() { }
	//
	template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
	constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...) {}
	//
	constexpr node(const node& Ano) : Obj(Ano.Obj) {}
};
template<class Ctr>
struct node_base_gptr {
	using Mgr_T = typename Ctr::Mgr_T;
	using Val_T = typename Ctr::Val_T;
	using Addr_T = typename Ctr::Addr_T;
	using Size_T = typename Ctr::Size_T;
	//
	constexpr node_base_gptr(Mgr_T& _Mgr, Addr_T _Addr) :Mgr(_Mgr), Addr(_Addr) {}
	//
	constexpr node_base_gptr(node_base_gptr&&) = default;
	//
	constexpr node_base_gptr(const node_base_gptr&) = default;
	//
	constexpr node_base_gptr& operator=(node_base_gptr&& Ano) {
		Addr = Ano.Addr;
		return *this;
	}
	//
	constexpr node_base_gptr& operator=(const node_base_gptr& Ano) {
		Addr = Ano.Addr;
		return *this;
	}
	//
	constexpr Val_T& operator*() {
		return *Mgr.addr(Addr);
	}
	//
	constexpr Val_T operator->() {
		return Mgr.addr(Addr);
	}
	//
	constexpr bool operator==(const node_base_gptr& Ano) {
		return Addr == Ano.Addr;
	}
	//
	constexpr bool operator!=(const node_base_gptr& Ano) {
		return Addr != Ano.Addr;
	}
	//
	Mgr_T& Mgr;
	Addr_T Addr;
};
//template<typename Val, typename SizeType, uint_8 Br>
//struct node {
//
//	Val Obj;
//	//neighbor
//	SizeType Nbr[Br];
//	constexpr node() :Obj(), Nbr() { }
//	//
//	template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
//	constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...), Nbr() {}
//	//
//	constexpr node(const node& Ano) : Obj(Ano.Obj) { Copy(var::make_index_sequence<Br>{}, Ano.Nbr); }
//	//
//private:
//	template<uint_8...Id>
//	constexpr void Copy(var::uint8_s<Id...>, const SizeType(&AnoNbr)[Br]) {
//		unfold((Nbr[Id] = AnoNbr[Id], 0)...);
//	}
//
//};
//template<typename Val, typename SizeType>
//struct node<Val, SizeType, 0> {
//	Val Obj;
//	constexpr node() :Obj() { }
//
//	template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
//	constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...) {}
//
//	constexpr node(const node& Ano) : Obj(Ano.Obj) {}
//};
template<typename Alloc, uint_8 Br>
class node_manager {
public:
	using Val_T = Alloc::Val_T;
	using Node_T = node<Alloc, Br>;
	using Alloc_T = Node_T::Alloc_T;//node's alloc
	using Addr_T = Alloc_T::Addr_T;
	constexpr node_manager() :Alc() {}
	constexpr node_manager(node_manager&& Another) : Alc(as_rreference(Another.Alc)) {}
	constexpr node_manager(const node_manager& Another) : Alc(Another.Alc) {}
	constexpr Val_T* addr(Addr_T Addr) {
		return &Alc.addr(Addr)->Obj;
	}
	constexpr Val_T* addr(Addr_T Addr) const {
		return &Alc.addr(Addr)->Obj;
	}
	constexpr void connect(Addr_T Left, Addr_T Right, uint_8 L_Order, uint_8 R_Order) {
		Alc.addr(Left)->Nbr[R_Order] = Right;
		Alc.addr(Right)->Nbr[L_Order] = Left;
	}
	constexpr void connect(Addr_T Addr, uint_8 L_Order, uint_8 R_Order) {
		Addr_T Left = Alc.addr(Addr)->Nbr[L_Order];
		Addr_T Right = Alc.addr(Addr)->Nbr[R_Order];
		Alc.addr(Left)->Nbr[R_Order] = Right;
		Alc.addr(Right)->Nbr[L_Order] = Left;
	}
	constexpr void connect(Addr_T Left, Addr_T Right, uint_8 Order) {
		Alc.addr(Left)->Nbr[Order] = Right;
	}
	constexpr Addr_T alloc() {
		return Alc.alloc();
	}
	template<typename...Args>
	constexpr Addr_T construct(Args&&...Params) {
		return Alc.construct(forward<Args>(Params)...);
	}
	constexpr void destroy(Addr_T Addr) {
		Alc.destroy(Addr);
	}
	constexpr void free(Addr_T Addr) {
		Alc.free(Addr);
	}
	constexpr Addr_T nbr(Addr_T Addr, uint_8 Order) {
		return Alc.addr(Addr)->Nbr[Order];
	}
	constexpr Addr_T nbr(Addr_T Addr, uint_8 Order)const {
		return Alc.addr(Addr)->Nbr[Order];
	}
	constexpr Val_T* nbr_addr(Addr_T Addr, uint_8 Order) {
		return &Alc.addr(Alc.addr(Addr)->Nbr[Order])->Obj;
	}
private:
	Alloc_T Alc;
};
XSL_END
#endif // !XSL_NODE
