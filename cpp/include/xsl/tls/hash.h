#pragma once
#ifndef XSL_HASH
#define XSL_HASH
#include"xsldef.h"
#include"allocator.h"
#include"iterator.h"
#include"tuple.h"
XSL_BEGIN

template<class Val1, class Val2>
class pair {
	tuple<Val1, Val2> Res;
public:
	using First_T = Val1;
	using Second_T = Val2;

	template<class _Val1, class _Val2>
	pair(_Val1&& val1, _Val2&& val2) :Res(forward<_Val1>(val1), forward<_Val2>(val2)) {}
	First_T& First() {
		return Res.get<0>();
	}
	Second_T& Second() {
		return Res.get<1>();
	}
};

#define XSL_HASH_SIZE ((uint32_t)1 << (sizeof(int32_t) * 8 - 1))

template<class Key>//prime
constexpr uint64_t hash_size = 1152921504606846883ULL;

template<>//prime
constexpr uint64_t hash_size<uint64_t> = 1152921504606846883ULL;

template<>//prime
constexpr uint64_t hash_size<int64_t> = 1152921504606846883ULL;

template<>//prime
constexpr uint64_t hash_size<uint32_t> = 2147483647ULL;

template<>//prime
constexpr uint64_t hash_size<int32_t> = 2147483647ULL;

template<>//prime
constexpr uint64_t hash_size<uint16_t> = 32749ULL;

template<>//prime
constexpr uint64_t hash_size<int16_t> = 32749ULL;

template<>//prime
constexpr uint64_t hash_size<uint8_t> = 127ULL;

template<>//prime
constexpr uint64_t hash_size<int8_t> = 127ULL;


template<class Key, enable<is_integer<Key>> = 0>
struct hash_func {
	constexpr int64_t operator()(const Key key) {
		return (key % hash_size<Key>+hash_size<Key>) % hash_size<Key>;
	}
};


template<class Alloc>
struct hash_ptr {
public:
	using Size_T = Alloc::Size_T;
private:
	Alloc& Alc;
	Size_T Off;

public:
	constexpr hash_ptr(Alloc& alc, Size_T off) :Alc(alc), Off(off) {}

	constexpr Alloc::Val_T& operator*() {
		return Alc.Get_Val(Off);
	}

	constexpr hash_ptr& operator++() {
		Off = Alc.Get_Next(Off, 0);
		return *this;
	}

	constexpr hash_ptr operator++(int) {
		Size_T Tmp = Off;
		Off = Alc.Get_Next(Off, 0);
		return { Tmp };
	}

	constexpr bool operator==(const hash_ptr& Ano) {
		return Off == Ano.Off;
	}

	constexpr bool operator!=(const hash_ptr& Ano) {
		return Off != Ano.Off;
	}
};



template<class Key, class Val>
class hash {
public:
	using Pair_T = pair<Key, Val>;

	using Crit = criterion<Pair_T>;

	using Key_T = Key;
	using Val_T = Val;

	using Size_T = Crit::Size_T;

	using Alloc = indirect_allocator<Crit, 1>;

	using GPtr_T = hash_ptr<Alloc>;

	using Iter = forward_iterator<GPtr_T>;
private:
	hash_func<Key_T> HF;
	Alloc Alc;
	Size_T* Head;


	constexpr Size_T& Find(const Key_T& key, const Size_T& Off) {
		if ((~Off) || key == (Alc.Get_Ptr(Off)->Obj).First())return Off;
		return Find(key, Alc.Get_Next(Off));
	}

public:
	constexpr hash() :Alc(), Head(Alc.Allocate<Size_T>(sizeof(Size_T)* hash_size<Key_T>)) { };

	constexpr hash(hash&& Ano) :Alc(as_rreference(Ano.Alc)), Head(Ano.Head) { Ano.Head = nullptr; };

	constexpr hash(const hash& Ano) :Alc(Ano.Alc), Head(Alc.Allocate<Size_T>(sizeof(Size_T)* hash_size<Key_T>)) { MemCopy<Size_T>(Head, Ano.Head, hash_size<Key_T>); };

	constexpr ~hash() { if (Head != nullptr)Alc.Deallocate(Head); };

	constexpr Iter operator[](const Key_T& key) {
		GPtr_T{ Find(key,Head[HF(key)]) };
	}

	constexpr void insert(Pair_T&& pair) {
		Find(pair.First(), Head[HF(pair.First())]) = Alc.Construct_Off(static_cast<Pair_T&&>(pair));
	}

	constexpr void insert(const Pair_T& pair) {
		Find(pair.First(), Head[HF(pair.First())]) = Alc.Construct_Off(pair);
	}
};

XSL_END

#endif // !XSL_HASH
