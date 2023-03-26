#pragma once
#ifndef XSL_ITERATOR
#define XSL_ITERATOR
#include"xsldef.h"
#include"template.h"
XSL_BEGIN

template<typename Checked_Iter>
struct iter_extract {
	using Val_T = Checked_Iter::Val_T;
	using GPtr_T = Checked_Iter::GPtr_T;
	using Ctr_T = Checked_Iter::Ctr_T;
};

template<typename Val>
struct iter_extract<Val*> {
	using Val_T = rm_cvr<Val>;
	using GPtr_T = Val_T*;
};


template<class Ctr>
class basic_iterator;

template<template<class...>class Ctr, class...Args>
class basic_iterator<Ctr<Args...>> {
public:
	using Ctr_T = Ctr<Args...>;
	using Val_T = typename Ctr_T::Val_T;
	using GPtr_T = typename Ctr_T::GPtr_T;
	constexpr basic_iterator(GPtr_T&& ptr) : Ptr(ptr) {}
	constexpr basic_iterator(const GPtr_T& ptr) : Ptr(ptr) {}
	constexpr basic_iterator(basic_iterator&&) = default;
	constexpr basic_iterator(const basic_iterator&) = default;
	constexpr basic_iterator& operator=(basic_iterator&& Ano) {
		Ptr = as_rreference(Ano.Ptr);
		return *this;
	}
	constexpr basic_iterator& operator=(const basic_iterator& Ano) {
		Ptr = Ano.Ptr;
		return *this;
	}
	constexpr decltype(auto) operator*() {
		return *Ptr;
	}
	constexpr decltype(auto) operator->() {
		return Ptr;
	}
	constexpr bool operator==(const basic_iterator& iter) {
		return Ptr == iter.Ptr;
	}
	constexpr bool operator!=(const basic_iterator& iter) {
		return Ptr != iter.Ptr;
	}
protected:
	friend class Ctr_T;
	GPtr_T Ptr;
};


template<typename Ctr>
class forward_iterator :public basic_iterator<Ctr> {
public:
	using Base = basic_iterator<Ctr>;
	using Ctr_T = Ctr;
	using Val_T = Ctr_T::Val_T;
	using Base::Base;
	using Base::operator =;
	constexpr forward_iterator& operator++() {
		++Ptr;
		return *this;
	}
	constexpr forward_iterator operator++(int) {
		return { Ptr++ };
	}
	//constexpr forward_bilateral_iterator operator+(uint_t var) {
	//	_Ptr newPtr = _Base::Ptr;
	//	while (var--)++newPtr;
	//	return{ newPtr };
	//}
protected:
	using Base::Ptr;
};

template<typename Ctr>
class forward_bilateral_iterator :public forward_iterator<Ctr> {
public:
	using DirBase = forward_iterator<Ctr>;
	using Base = DirBase::Base;
	using Ctr_T = Ctr;
	using Val_T = Ctr_T::Val_T;
	using DirBase::DirBase;
	using DirBase::operator =;
	constexpr forward_bilateral_iterator& operator--() {
		--Ptr;
		return *this;
	}
	constexpr forward_bilateral_iterator operator--(int) {
		return { Ptr-- };
	}
	//constexpr forward_bilateral_iterator operator+(uint_t var) {
	//	_Ptr newPtr = _Base::Ptr;
	//	while (var--)++newPtr;
	//	return{ newPtr };
	//}

protected:
	using Base::Ptr;
};

//iterator tag,container
template<typename Ctr>
class reverse_iterator :public basic_iterator<Ctr> {
public:
	using Base = basic_iterator<Ctr>;
	using Ctr_T = Ctr;
	using Val_T = Ctr_T::Val_T;
	using Base::Base;
	constexpr reverse_iterator& operator++() {
		--Ptr;
		return *this;
	}
	constexpr reverse_iterator  operator++(int) {
		return { Ptr-- };
	}
protected:
	using Base::Ptr;
};

template<typename Ctr>
class reverse_bilateral_iterator :public reverse_iterator<Ctr> {
	using DirBase = reverse_iterator<Ctr>;
public:
	using Base = DirBase::Base;
	using Ctr_T = Ctr;
	using Val_T = Ctr_T::Val_T;
	using DirBase::DirBase;
	constexpr reverse_bilateral_iterator& operator--() {
		++Ptr;
		return *this;
	}
	constexpr reverse_bilateral_iterator  operator--(int) {
		return { Ptr++ };
	}
	//constexpr iterator operator-(uint_t var) {
	//	_Ptr newPtr = _Base::Ptr;
	//	while (var--)--newPtr;
	//	return{ newPtr };
	//}
protected:
	using Base::Ptr;
};


template<typename Ctr>
class random_forward_bilateral_iterator :public forward_bilateral_iterator<Ctr> {
	//direct base;
	using DirBase = forward_bilateral_iterator<Ctr>;
public:
	using Base = DirBase::Base;
	using Ctr_T = Ctr;
	using Val_T = Ctr_T::Val_T;
	using DirBase::DirBase;


	constexpr random_forward_bilateral_iterator operator+(int_64 Off) {
		return{ Ptr + Off };
	}
	constexpr random_forward_bilateral_iterator& operator+=(int_64 Off) {
		Ptr += Off;
		return *this;
	}
	constexpr random_forward_bilateral_iterator operator-(int_64 Off) {
		return{ Ptr - Off };
	}
	constexpr random_forward_bilateral_iterator& operator-=(int_64 Off) {
		Ptr -= Off;
		return *this;
	}
protected:
	using Base::Ptr;
};

template<typename Ctr>
class random_reverse_bilateral_iterator :public reverse_bilateral_iterator <Ctr> {
	//direct base;
	using DirBase = reverse_bilateral_iterator<Ctr>;
public:
	using Base = DirBase::Base;
	using Ctr_T = Ctr;
	using Val_T = Ctr_T::Val_T;
	using DirBase::DirBase;

	constexpr random_reverse_bilateral_iterator  operator+(int_64 Off) {
		return{ Ptr + Off };
	}
	constexpr random_reverse_bilateral_iterator& operator+=(int_64 Off) {
		Ptr += Off;
		return *this;
	}
	constexpr random_reverse_bilateral_iterator  operator-(int_64 Off) {
		return{ Ptr - Off };
	}
	constexpr random_reverse_bilateral_iterator& operator-=(int_64 Off) {
		Ptr -= Off;
		return *this;
	}
private:
	using Base::Ptr;
};

template<typename Unchecked_Iter>
inline constexpr bool is_xsl_iter = false;

template<template<class>class XSL_Iter, typename Ctr>
inline constexpr bool is_xsl_iter<XSL_Iter<Ctr>> = is_existing<XSL_Iter<Ctr>,
	type::_n <
	basic_iterator<Ctr>,
	forward_iterator<Ctr>,
	forward_bilateral_iterator<Ctr>,
	random_forward_bilateral_iterator<Ctr>,
	reverse_iterator<Ctr>,
	reverse_bilateral_iterator<Ctr>,
	random_reverse_bilateral_iterator<Ctr>
	>
>;


template<typename Unchecked_Iter>
inline constexpr bool is_other_iter = is_existing< rm_cvr<Unchecked_Iter>, type::_n <>>;


template<typename Unchecked_Iter>
inline constexpr bool is_iter = is_pointer<Unchecked_Iter> || is_xsl_iter<Unchecked_Iter> || is_other_iter<Unchecked_Iter>;

template<typename Checked_Iter>
constexpr decltype(auto) get_unwraped(Checked_Iter&& Iter) {
	if constexpr (is_pointer<Checked_Iter>)
		return Iter;
	else if constexpr (is_xsl_iter<Checked_Iter> && is_pointer<typename iter_extract<Checked_Iter>::GPtr_T>)
		return &*Iter;
	else return Iter;
}
XSL_END
#endif // !XSL_ITERATOR
