#ifndef XSL_TEST
#define XSL_TEST
#include"xsldef.h"
#include<iostream>

#ifdef TEST_PROC_PRINT
#define PROC_PRINT(Text) printf(#Text);puts("")
#else
#define PROC_PRINT(Text)
#endif // TEST_PROC_PRINT

class test_t {
public:
	int d;
	char c;
	//void test_func00(test_t t) {
	//	PROC_PRINT(void _test_t(test_t t));
	//}
	//void test_func01(test_t t)const {
	//	PROC_PRINT(void _test_t(test_t t)const);
	//}
	/*operator int() {
		return d;
	}*/
	constexpr bool operator !=(const test_t& Ano) {
		return c != Ano.c || d != Ano.d;
	}
	constexpr bool operator ==(const test_t& Ano) {
		return c == Ano.c && d == Ano.d;
	}
};

class test_p {
public:
	int* Ptr;
	test_p() :Ptr(new int{}) { PROC_PRINT(test_p()); }
	test_p(int _Val) :Ptr(new int(_Val)) { PROC_PRINT(test_p(int _Val)); }
	test_p(const test_p& Ano) :Ptr(new int(*Ano.Ptr)) { PROC_PRINT(test_p(const test_p & Ano)); }
	test_p(test_p&& Ano) noexcept :Ptr(Ano.Ptr) {
		PROC_PRINT(test_p(test_p && Ano));
		Ano.Ptr = nullptr;
	}
	test_p& operator=(const test_p& Ano) {
		PROC_PRINT(test_p & operator=(const test_p & Ano));
		Ptr = new int(*Ano.Ptr);
		return *this;
	}
	test_p& operator=(test_p&& Ano)noexcept {
		PROC_PRINT(test_p & operator=(test_p && Ano));
		Ptr = Ano.Ptr;
		Ano.Ptr = nullptr;
		return *this;
	}
	constexpr bool operator !=(const test_p& Ano) {
		return *Ptr != *Ano.Ptr;
	}
	constexpr bool operator ==(const test_p& Ano) {
		return *Ptr == *Ano.Ptr;
	}
	void test_func00(test_p t) {
		PROC_PRINT(void _test_p(test_p t));
	}
	void test_func01(test_p t)const {
		PROC_PRINT(void _test_p(test_p t)const);
	}
	operator int() {
		return *Ptr;
	}
	~test_p() {
		PROC_PRINT(~test_p());
		if (Ptr)delete Ptr;
	}
};

#define TEST_SPLIT_LINE(Text) puts("---------------------------"#Text"---------------------------");

#define TEST_ACT_PRINT(Text) puts(#Text);Text;puts("")
//
// template<typename Ctr>
// void print(Ctr&& _Ctr) {
// 	for (auto&& t : _Ctr)
// 		std::cout << t;
// 	puts("");
// }
template<template<typename...>class _Ctr, typename..._Tps>
void print(_Ctr<char, _Tps...>& Ctr) {
	for (auto&& c : Ctr)
		printf("%c ", c);
	puts("");
}
template<template<typename...>class _Ctr, typename..._Tps>
void rprint(_Ctr<char, _Tps...>& Ctr) {
	for (auto b = Ctr.rbegin(), e = Ctr.rend(); b != e; ++b)
		printf("%c ", *b);
	puts("");
}
template<template<typename...>class _Ctr, typename..._Tps>
void print(_Ctr<wchar_t, _Tps...>& Ctr) {
	for (auto&& wc : Ctr)
		printf("%wc ", wc);
	puts("");
}
template<template<typename...>class _Ctr, typename..._Tps>
void rprint(_Ctr<wchar_t, _Tps...>& Ctr) {
	for (auto b = Ctr.rbegin(), e = Ctr.rend(); b != e; ++b)
		printf("%wc ", *b);
	puts("");
}
template<template<typename...>class Ctr, typename...Tps>
void print(Ctr<test_t, Tps...>& _Ctr) {
	for (auto&& t : _Ctr)
		printf("%d ", t.d);
	puts("");
}
template<template<typename...>class Ctr, typename...Tps>
void rprint(Ctr<test_t, Tps...>& _Ctr) {
	for (auto b = _Ctr.rbegin(), e = _Ctr.rend(); b != e; ++b)
		printf("%d ", (*b).d);
	puts("");
}
template<template<typename...>class Ctr, typename...Tps>
void print(Ctr<test_p, Tps...>& _Ctr) {
	for (auto&& t : _Ctr)
		printf("%d ", *t.Ptr);
	puts("");
}
template<template<typename...>class Ctr, typename...Tps>
void rprint(Ctr<test_p, Tps...>& _Ctr) {
	for (auto b = _Ctr.rbegin(), e = _Ctr.rend(); b != e; ++b)
		printf("%d ", *(*b).Ptr);
	puts("");
}
//
#define TEST_RET_PRINT(Text,Proc) printf(#Text); printf(" : "); Proc(Text);puts("")

class test02 {
public:
};
#endif//!XSL_TEST