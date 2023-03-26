#pragma once
#ifndef XSL_POINTER
#define XSL_POINTER
#include"xsldef.h"
XSL_BEGIN


template<typename Crit>
class default_pointer {
public:
	//
	using Val_T = typename Crit::Val_T;
	using Ptr_T = typename Crit::Ptr_T;
	//
	constexpr default_pointer() :Ptr(nullptr) {}
	//
	constexpr default_pointer(Ptr_T _Ptr) : Ptr(_Ptr) {}
	//
	constexpr Ptr_T& Get() {
		return Ptr;
	}
	//
	constexpr const Ptr_T& Get()const {
		return Ptr;
	}
	//
	constexpr Val_T& operator*() {
		return *Ptr;
	}
	//
	constexpr const Val_T& operator* ()const {
		return *Ptr;
	}
	//
	constexpr bool operator==(const default_pointer& Ano) {
		return Ptr == Ano.Ptr;
	}
	//
	constexpr bool operator!=(const default_pointer& Ano) {
		return Ptr != Ano.Ptr;
	}
	//
	constexpr default_pointer& operator++() {
		++Ptr;
		return *this;
	}
	//
	constexpr default_pointer operator++(int) {
		return { Ptr++ };
	}
	//
	constexpr default_pointer& operator--() {
		--Ptr;
		return *this;
	}
	//
	constexpr default_pointer operator--(int) {
		return { Ptr-- };
	}
	//
	constexpr default_pointer operator+(int64_t Off) {
		return{ Ptr + Off };
	}
	//
	constexpr default_pointer& operator+=(int64_t Off) {
		Ptr += Off;
		return *this;
	}
	//
	constexpr default_pointer operator-(int64_t Off) {
		return{ Ptr - Off };
	}
	//
	constexpr default_pointer& operator-=(int64_t Off) {
		Ptr -= Off;
		return *this;
	}
	//
private:
	Ptr_T Ptr;
};




//namespace tag_ptr {
//	struct baseptr {};
//	//sequential list
//	struct seql {};
//	//linked list
//	struct likl {};
//	//deque
//	struct deque;
//}
//template<typename, typename...>
//class pointer;

//template<typename Val>
//class pointer<tag_ptr::baseptr, Val> {
//protected:
//	Val* Ptr;
//public:
//	constexpr pointer(Val* ptr) :Ptr(ptr) {}
//	constexpr pointer(pointer&&) = default;
//	constexpr pointer(const pointer&) = default;
//	constexpr pointer& operator=(pointer&&) = default;
//	constexpr pointer& operator=(const pointer&) = default;
//	constexpr virtual operator Val* () {
//		return Ptr;
//	}
//	constexpr virtual bool operator==(const pointer& ptr) {
//		return Ptr == ptr.Ptr;
//	}
//	constexpr virtual bool operator!=(const pointer& ptr) {
//		return Ptr != ptr.Ptr;
//	}
//};
//template<typename Node>
//class pointer<tag_ptr::likl, Node> :public pointer<tag_ptr::baseptr, Node> {
//	using Base = pointer<tag_ptr::baseptr, Node>;
//	using _Val = Node::_Val;
//public:
//	using Base::Base;
//	constexpr _Val& operator*() {
//		return Base::Ptr->Obj;
//	}
//	constexpr pointer& operator++() {
//		Base::Ptr = Base::Ptr->Next;
//		return *this;
//	}
//	constexpr pointer operator++(int)
//	{
//		Node* newPtr = Base::Ptr;
//		Base::Ptr = Base::Ptr->Next;
//		return { newPtr };
//	}
//	constexpr pointer operator+(uint_t val) {
//		Node* newPtr = Base::Ptr;
//		while (val--)newPtr = newPtr->Next;
//		return{ newPtr };
//	}
//	constexpr pointer& operator--() {
//		Base::Ptr = Base::Ptr->Pre;
//		return *this;
//	}
//	constexpr pointer operator--(int)
//	{
//		Node* newPtr = Base::Ptr;
//		Base::Ptr = Base::Ptr->Pre;
//		return { newPtr };
//	}
//	constexpr pointer operator-(uint_t val) {
//		Node* newPtr = Base::Ptr;
//		while (val--)newPtr = newPtr->Pre;
//		return{ newPtr };
//	}
//};
//template<typename Deque>
//class pointer<tag_ptr::deque, Deque> :public pointer<tag_ptr::baseptr, Deque> {
//	uint_t Pos;
//public:
//	using _Base = pointer<tag_ptr::baseptr, Deque>;
//	using _Val = Deque::_Val;
//	pointer(Deque* ptr, uint_t pos) :_Base::Ptr(ptr), Pos(pos) {}
//	constexpr virtual operator _Val* () {
//		return &_Base::Ptr->at(Pos);
//	}
//	constexpr virtual bool operator==(const pointer& ptr) {
//		return _Base::Ptr == ptr.Ptr && Pos == ptr.Pos;
//	}
//	constexpr virtual bool operator!=(const pointer& ptr) {
//		return _Base::Ptr != ptr.Ptr || Pos == ptr.Pos;
//	}
//	constexpr _Val& operator*() {
//		return _Base::Ptr->at(Pos);
//	}
//	constexpr pointer& operator++() {
//		++Pos;
//		return *this;
//	}
//	constexpr pointer operator++(int)
//	{
//		return { _Base::Ptr,Pos++ };
//	}
//	constexpr pointer operator+(uint_t val) {
//		return{ _Base::Ptr,Pos + val };
//	}
//	constexpr pointer& operator--() {
//		--Pos;
//		return *this;
//	}
//	constexpr pointer operator--(int)
//	{
//		return { _Base::Ptr,Pos-- };
//	}
//	constexpr pointer operator-(uint_t val) {
//		return{ _Base::Ptr,Pos - val };
//	}
//};
XSL_END

#endif // !XSL_POINTER
