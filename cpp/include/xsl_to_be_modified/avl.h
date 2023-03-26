#pragma once
#ifndef XSL_AVL
#define XSL_AVL
#include"allocator.h"
#include"pair.h"
#include"node.h"
#include"iterator.h"
XSL_BEGIN


template<class Key, class Val>
class avl;

template<class Key, class Val>
class avl_node_info {
public:
	using Key_T = Key;
	using Val_T = Val;
	using Pair_T = pair<Key_T, Val_T>;

	template<class _Val1, class _Val2>
	constexpr avl_node_info(_Val1&& val1, _Val2&& val2) :Res(forward<_Val1>(val1), forward<_Val2>(val2)), BF(0) {}

	constexpr avl_node_info(Pair_T&& pair) : Res(static_cast<Pair_T&&>(pair)), BF(0) {}

	constexpr Key_T& First() {
		return Res.first;
	}

	constexpr Val_T& Second() {
		return Res.second;
	}
	constexpr Pair_T& Get_Pair() {
		return Res;
	}
	constexpr int8_t& Get_BF() {
		return BF;
	}
private:

	int8_t BF;
	Pair_T Res;
};

template<class Pair>
struct avl_gptr {
public:
	using AVL = avl<Pair::Val1, Pair::Val2>;
	using Alloc = typename AVL::Alloc;
	using Val_T = typename AVL::Val_T;
	using Size_T = typename AVL::Size_T;

public:
	constexpr avl_gptr(Alloc& alc, Size_T off) :Alc(alc), Off(off) {}

	constexpr decltype(auto) operator*() {
		return Alc.Get_Val(Off).Get_Pair();
	}

	constexpr avl_gptr& operator++() {
		Get_Next(1);
		return *this;
	}

	constexpr avl_gptr operator++(int) {
		Size_T Tmp_Off = Off;
		Get_Next(1);
		return { Alc, Tmp_Off };
	}

	constexpr avl_gptr& operator--() {
		Get_Next(0);
		return *this;
	}

	constexpr avl_gptr operator--(int) {
		Size_T Tmp_Off = Off;
		Get_Next(0);
		return { Alc, Tmp_Off };
	}

	constexpr bool operator==(const avl_gptr& Ano) {
		return Off == Ano.Off;
	}

	constexpr bool operator!=(const avl_gptr& Ano) {
		return Off != Ano.Off;
	}
private:
	Alloc& Alc;
	Size_T Off;

	constexpr void Get_Next(bool LOrR) {
		if (Size_T Ch = Alc.Get_Val(Off).Next[LOrR]; Ch) {
			do Cur = Ch, Ch = Alc.Get_Val(Ch).Next[!LOrR]; while (Ch);
		}
		else if (Size_T Par = Alc.Get_Val(Off).Next[2]; Par) {
			while (Par && Alc.Get_Val(Par).Next[!LOrR] != Off)Cur = Par, Par = Alc.Get_Val(Par).Next[2];
			Cur = Par;
		}
		else Cur = 0;
	}
};


template<class Key, class Val>
class avl {
public:

	using Key_T = Key;
	using Val_T = Val;
	using Pair_T = pair<Key_T, Val_T>;
	using Crit = criterion<avl_node_info<Key_T, Val_T>>;
	using Size_T = typename Crit::Size_T;
	using Node = node<Crit, 3>;
	using Alloc = indirect_allocator<criterion<Node>>;
	using GPtr_T = avl_gptr<Val>;
	using Iter = forward_bilateral_iterator<list>;
	using RIter = reverse_bilateral_iterator<list>;
	using Base_iter = typename Iter::Base;
	constexpr avl() :Alc(), Root() {}
	//
	constexpr Iter begin() {
		Size_T Par = Root;
		if (Par) {
			Size_T LCh = Alc.Get_Next(Par, 0);
			while (LCh)Par = LCh, LCh = Alc.Get_Next(LCh, 0);
		}
		return GPtr_T{ Alc,Par };
	}
	//
	constexpr Iter end() {
		return GPtr_T{ Alc,0 };
	}
	//
	constexpr void insert(Pair_T&& kv) {
		Alc.Set_Next(Root = Insert_Find(Root, Alc.Construct(static_cast<Pair_T&&>(kv))), 2, 0);
	}
	//
	template<class _Key, class _Val, enable<is_same<rm_cvr<Key_T>, rm_cvr<_Key>>> = 0, enable<is_same<rm_cvr<Val_T>, rm_cvr<_Val>>> = 0>
	constexpr void insert(_Key&& key, _Val&& val) {
		Alc.Set_Next(Root = Insert_Find(Root, Alc.Construct(forward<_Key>(key), forward<_Val>(val))), 2, 0);
	}
	//
	constexpr void erase(const Key_T& key) {
		Alc.Set_Next(Root = Erase_Find(Root, key), 2, 0);
	}
	//
	constexpr Iter find(const Key_T& key) {
		return GPtr_T{ Find(Root,key) };
	}
	//
	constexpr Val_T& operator[](const Key_T& key) {
		Size_T Ret = Find(Root, key);
		if (!Ret)throw wrong_key{};
		return Alc.Get_Val(Ret).Second();
	}
	//
private:
	//
	Alloc Alc;
	Size_T Root;
	//
	constexpr Size_T Adjust(Size_T Par, bool LOrR) {
		if (Alc.Get_Val(Alc.Get_Next(Par, LOrR)).Get_BF() == LOrR * 2 - 1)
			return Adjust_S(Par, LOrR);
		else return Adjust_B(Par, LOrR);
	}
	//
	constexpr Size_T Adjust_S(Size_T Par, int8_t LOrR) {
		Size_T NewPar = Alc.Get_Val(Par).Next[LOrR];
		Alc.Connect(Par, LOrR, Alc.Get_Val(NewPar).Next[1 - LOrR], 2);
		// Connect(Par, Alc.Get_Next(NewPar, !LOrR), LOrR);
		Alc.Connect(NewPar, !LOrR, Par, 2);
		// Connect(NewPar, Par, !LOrR);
		Alc.Get_Val(Par).Get_BF() = 0;
		Alc.Get_Val(NewPar).Get_BF() += 1 - LOrR * 2;
		return NewPar;
	}
	//
	constexpr Size_T Adjust_B(Size_T Par, bool LOrR) {
		Alc.Connect(Par, LOrR, Adjust_S(Alc.Get_Next(Par, LOrR), !LOrR), 2);
		// Connect(Par, Adjust_S(Alc.Get_Next(Par, LOrR), !LOrR), LOrR);
		return Adjust_S(Par, LOrR);
	}
	//
	// constexpr void Connect(Size_T Par, Size_T Ch, bool LOrR) {
	// 	Alc.Set_Next(Par, LOrR, Ch);
	// 	if (Ch)Alc.Set_Next(Ch, 2, Par);
	// }

	constexpr Size_T Find(Size_T Par, const Key_T& key) {
		Key_T& ParKey = Alc.Get_Val(Par).First();
		if (!Par || ParKey == key)return Par;
		return Find(Alc.Get_Next(Par, ParKey < key), key);
	}


	constexpr Size_T Erase_Find(Size_T Par, const Key_T& key) {
		if (!Par)throw wrong_key{};
		Key_T& ParKey = Alc.Get_Val(Par).First();
		if (ParKey == key) {
			Size_T Rpl, LCh = Alc.Get_Next(Par, 0), RCh = Alc.Get_Next(Par, 1);
			int8_t BF = Alc.Get_Val(Par).Get_BF();
			Alc.Destruct(Par);
			if (LCh + RCh) {
				bool LOrR = RCh;
				Size_T Ch = Erase_Bdry(LOrR ? RCh : LCh, !LOrR, Rpl);
				Connect(Rpl, LOrR ? LCh : RCh, !LOrR);

				Alc.Get_Val(Rpl).Get_BF() = BF;
				return Erase_Check(Rpl, Ch, LOrR);
			}
			return 0;
		}
		bool LOrR = ParKey < key;
		return Erase_Check(Par, Erase_Find(Alc.Get_Next(Par, LOrR), key), LOrR);
	}

	constexpr Size_T Erase_Bdry(Size_T Par, bool LOrR, Size_T& Tar) {
		Size_T Ch = Alc.Get_Next(Par, LOrR);
		if (Ch) return Erase_Check(Par, Erase_Bdry(Ch, LOrR, Tar), LOrR);
		Tar = Par;
		return Alc.Get_Next(Par, !LOrR);
	}

	constexpr Size_T Erase_Check(Size_T Par, Size_T Ch, bool LOrR) {
		Connect(Par, Ch, LOrR);
		if (!Ch || !Alc.Get_Val(Ch).Get_BF()) {
			int8_t ParBF = Alc.Get_Val(Par).Get_BF() += (!LOrR) * 2 - 1;
			if (abs(ParBF) > 1)Par = Adjust(Par, !LOrR);
		}
		return Par;
	}
	constexpr Size_T Insert_Find(Size_T Par, Size_T Ins) {
		if (!Par)return Ins;
		bool LOrR = Alc.Get_Val(Ins).Obj.First() >= Alc.Get_Val(Par).Obj.First();
		return Insert_Check(Par, Insert_Find(Alc.Get_Val(Par).Next[LOrR], Ins), LOrR);
	}
	constexpr Size_T Insert_Check(Size_T Par, Size_T Ch, bool LOrR) {
		if(Alc.Get_Val(Par). Alc.Connect(Par, LOrR, Ch, 2);
		// Connect(Par, Ch, LOrR);
		if ((!Alc.Get_Next(Ch, 0) && !Alc.Get_Next(Ch, 1)) || abs(Alc.Get_Val(Ch).Get_BF()) == 1) {

			int8_t ParBF = Alc.Get_Val(Par).Get_BF() += LOrR * 2 - 1;
			if (abs(ParBF) > 1)Par = Adjust(Par, LOrR);
		}
		return Par;
	}

};

XSL_END
#endif 
