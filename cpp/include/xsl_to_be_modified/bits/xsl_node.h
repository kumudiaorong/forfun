#pragma once
#ifndef XSL_NODE
#define XSL_NODE
#include"xsl_utility.h"
#include"xsl_allocator.h"
XSL_BEGIN


template<class _Val, uint_8 Br>
class node {
public:
	using Val_T = _Val;
	Val_T Val;
	//neighbor
	node* Nbr[Br];
	constexpr node() :Val(), Nbr() { }
	//
	template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
	constexpr node(Arg&& param, Args&&...params) : Val(forward<Arg>(param), forward<Args>(params)...), Nbr() {}
	//
	constexpr node(const node& Ano) : Val(Ano.Val) { Copy(var::make_index_sequence<Br>{}, Ano.Nbr); }
	//
	constexpr Addr_T& nbr(uint_8 Order) {
		return this->Nbr[Order];
	}
	//
	constexpr const Addr_T& nbr(uint_8 Order)const {
		return this->Nbr[Order];
		//return const_cast<node*>(this)->Nbr[Order];
	}
	//
private:
	template<uint_8...Id>
	constexpr void Copy(var::uint8_s<Id...>, const Addr_T(&AnoNbr)[Br]) {
		unfold((Nbr[Id] = AnoNbr[Id], 0)...);
	}
};
template<class _Val>
class node <_Val, 0> {
public:
	using Val_T = _Val;
	Val_T Val;
	constexpr node() :Val() { }
	//
	template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
	constexpr node(Arg&& param, Args&&...params) : Val(forward<Arg>(param), forward<Args>(params)...) {}
	//
	constexpr node(const node& Val) : Obj(Ano.Val) {}
};
template<class Ctr>
struct basic_node_gptr {
	using Mgr_T = typename Ctr::Mgr_T;
	using Val_T = typename Ctr::Val_T;
	using Addr_T = typename Ctr::Addr_T;
	using Size_T = typename Ctr::Size_T;
	//
	constexpr basic_node_gptr(const Mgr_T& _Mgr, Addr_T _Addr) :Mgr(const_cast<Mgr_T&>(_Mgr)), Addr(_Addr) {}
	//
	constexpr basic_node_gptr(basic_node_gptr&&) = default;
	//
	constexpr basic_node_gptr(const basic_node_gptr&) = default;
	//
	constexpr basic_node_gptr& operator=(basic_node_gptr&& Ano) {
		Addr = Ano.Addr;
		return *this;
	}
	//
	constexpr basic_node_gptr& operator=(const basic_node_gptr& Ano) {
		Addr = Ano.Addr;
		return *this;
	}
	//
	constexpr Val_T& operator*() {
		return *Mgr.addr(Addr);
	}
	//
	constexpr const Val_T& operator*()const {
		return *Mgr.addr(Addr);
	}
	//
	constexpr  Val_T operator->() {
		return Mgr.addr(Addr);
	}
	//
	constexpr const Val_T operator->()const {
		return Mgr.addr(Addr);
	}
	//
	constexpr bool operator==(const basic_node_gptr& Ano) {
		return Addr == Ano.Addr;
	}
	//
	constexpr bool operator!=(const basic_node_gptr& Ano) {
		return Addr != Ano.Addr;
	}
	//
	Mgr_T& Mgr;
	Addr_T Addr;
};
//0 is next,1 is pre
template<typename _Alloc, uint_8 Br>
class node_manager {
public:
	using Val_T = _Alloc::Val_T;
	using Node_T = node<_Alloc, Br>;
	using Alloc_T = Node_T::Alloc_T;//node's alloc
	using Addr_T = Alloc_T::Addr_T;
	constexpr node_manager() :Alc() {}
	constexpr node_manager(node_manager&& Ano)noexcept : Alc(as_rreference(Ano.Alc)) {}
	constexpr node_manager(const node_manager& Ano) : Alc(Ano.Alc) {}
	constexpr node_manager& operator=(node_manager&& Ano)noexcept {
		Alc = as_rreference(Ano.Alc);
		return *this;
	}
	constexpr node_manager& operator=(const node_manager& Ano) {
		Alc = Ano.Alc;
		return *this;
	}
	//
	constexpr Val_T* addr(const Addr_T& Addr) const {
		return &Alc.addr(Addr)->Obj;
	}
	//
	constexpr void connect(const Addr_T& Left, const Addr_T& Right, uint_8 L_Order, uint_8 R_Order) {
		Alc.addr(Left)->nbr(R_Order) = Right;
		Alc.addr(Right)->nbr(L_Order) = Left;
	}
	//
	constexpr void connect(const Addr_T& Addr, uint_8& L_Order, uint_8 R_Order) {
		Addr_T Left = Alc.addr(Addr)->nbr(L_Order);
		Addr_T Right = Alc.addr(Addr)->nbr(R_Order);
		Alc.addr(Left)->nbr(R_Order) = Right;
		Alc.addr(Right)->nbr(L_Order) = Left;
	}
	//
	constexpr void connect(const Addr_T& Left, const Addr_T& Right, uint_8 Order) {
		Alc.addr(Left)->nbr(Order) = Right;
	}
	//
	constexpr Addr_T alloc() {
		return Alc.alloc();
	}
	//
	template<typename...Args>
	constexpr Addr_T construct(Args&&...Params) {
		Addr_T Addr = Alc.alloc();
		XSL Construct_At(Alc.addr(Addr), forward<Args>(Params)...);
		return Addr;
	}
	//
	constexpr void destroy(Addr_T Addr) {
		XSL Destroy_At(Alc.addr(Addr));
		Alc.free(Addr);
	}
	//
	constexpr void free(Addr_T Addr) {
		Alc.free(Addr);
	}
	//
	constexpr Addr_T nbr(Addr_T& Addr, uint_8 Order) {
		return Alc.addr(Addr)->nbr(Order);
	}
	//
	constexpr const Addr_T nbr(const Addr_T& Addr, uint_8 Order)const {
		return Alc.addr(Addr)->nbr(Order);
	}
	//
	constexpr Val_T* nbr_addr(Addr_T& Addr, uint_8 Order) {
		return &Alc.addr(Alc.addr(Addr)->Nbr[Order])->Obj;
	}
	//
	constexpr const Val_T* nbr_addr(Addr_T& Addr, uint_8 Order)const {
		return &Alc.addr(Alc.addr(Addr)->Nbr[Order])->Obj;
	}
	//
private:
	Alloc_T Alc;
};
XSL_END
#endif // !XSL_NODE
