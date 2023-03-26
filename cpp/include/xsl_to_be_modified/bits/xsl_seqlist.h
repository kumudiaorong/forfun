#pragma once
#ifndef XSL_BASI_STRING
#define XSL_BASI_STRING
#include"xsl_def.h"
#include"xsl_allocator.h"
#include"xsl_iterator.h"
#include"xsl_batch.h"
XSL_BEGIN

template<typename _Val, typename _SizeType = uint_32, typename _Alloc = unfixed_block_allocator<_Val, _SizeType>>
class basic_string {
	//
public:
	using Val_T = _Val;
	using Alloc_T = _Alloc;
	using Addr_T = Alloc_T::Addr_T;
	using Size_T = uint_32;
	using GPtr_T = Val_T*;
	using Iter = random_forward_bilateral_iterator<basic_string>;
	using RIter = random_reverse_bilateral_iterator<basic_string>;
	//using Base_iter = typename Iter::Base;
	using Base_iter = basic_iterator<basic_string>;
	//
	constexpr basic_string(Size_T Count = 0) : Alc(), Head(Alc.alloc(Count)), Size(Count), Cap(Count) {};
	//
	constexpr basic_string(Size_T Count, const Val_T& Val) : basic_string(Count) {
		batch::Construct_n_by_1(Alc.addr(Head), Count, Val);
	}
	//
	constexpr basic_string(const Val_T* Ptr, Size_T Count) : basic_string(Count) {
		batch::Construct_n_by_n(Alc.addr(Head), Ptr, Count);
	}
	//
	explicit constexpr basic_string(const Val_T* Ptr) : basic_string(Ptr, batch::Len<Size_T>(Ptr)) {}
	//
	constexpr basic_string(basic_string&& Ano) noexcept :Alc(as_rreference(Ano.Alc)), Head(Ano.Head), Cap(Ano.Cap), Size(Ano.Size) {
		Ano.Head = Alloc_T::nulladdr;
		Ano.Cap = 0;
		Ano.Size = 0;
	}
	//
	constexpr basic_string(const basic_string& Ano) : basic_string(Ano.data(), Ano.Size) { }
	//
	constexpr basic_string& operator=(const Val_T* Ptr) {
		return assign(Ptr);
	}
	//
	constexpr basic_string& operator=(basic_string&& Ano)noexcept {
		return assign(as_rreference(Ano));
	}
	//
	constexpr basic_string& operator=(const basic_string& Ano) {
		return assign(Ano.data(), Ano.Size);
	}
	//
	constexpr basic_string& assign(const Val_T* Ptr, Size_T NewSize) {
		if (Head != Alloc_T::nulladdr)
			batch::Destroy_n(Alc.addr(Head), Size);
		Recap_without_moving(NewSize);
		batch::Construct_n_by_n(Alc.addr(Head), Ptr, NewSize);
		Size = NewSize;
		return *this;
	}
	//
	constexpr basic_string& assign(const Val_T* Ptr) {
		return assign(Ptr, batch::Len< Size_T>(Ptr));
	}
	//
	constexpr basic_string& assign(basic_string&& Ano) {
		if (Head != Alloc_T::nulladdr) {
			batch::Destroy_n(Alc.addr(Head), Size);
			Alc.free(Head);
		}
		Alc = as_rreference(Ano.Alc);
		Head = as_rreference(Ano.Head);
		Cap = Ano.Cap;
		Size = Ano.Size;
		Ano.Head = Alloc_T::nulladdr;
		Ano.Cap = 0;
		Ano.Size = 0;
		return *this;
	}
	//
	constexpr basic_string& assign(const basic_string& Ano) {
		return assign(Ano.data(), Ano.Size);
	}
	//
	constexpr Val_T& operator[](Size_T Off) {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		XSL_RANGE_CHECK(Off < Size);
		return Head[Off];
	}
	//
	constexpr Iter begin() {
		return Alc.addr(Head);
	}
	//
	constexpr Iter end() {
		return  Alc.addr(Head) + Size;
	}
	//
	constexpr RIter rbegin() {
		return  Alc.addr(Head) + Size - 1;
	}
	//
	constexpr RIter rend() {
		return  Alc.addr(Head) - 1;
	}
	//
	constexpr void clear() {
		erase_by_ptr(Alc.addr(Head), Size);
	}
	//
	constexpr void insert(Base_iter Where, Size_T Count, const Val_T& Val = Val_T{}) {
		Recap_with_moving(Size + Count);
		MMove(Where.Ptr + Count, Where.Ptr, Count * sizeof(Val_T));
		batch::Construct_n_by_1(Where.Ptr, Count, Val);
		Size += Count;
	}
	//
	constexpr void insert(Base_iter Where, const Val_T& Val = Val_T{}) {
		insert(Where, 1, Val);
	}
	//
	template<typename Checked_Iter, enable<is_iter<Checked_Iter>> = 0>
	constexpr void insert(Base_iter Where, Checked_Iter First, Checked_Iter Last) {
		//Size_T Count = OLen<Size_T>(First, Last);
		//Recap_with_moving(Size + Count);
		//MMove(Where.Ptr + Count, Where.Ptr, Count * sizeof(Val_T));
		//batch::Construct_n_by_n(Where.Ptr, Count, Val);
		//Size += Count;
	}
	//
	template<typename Checked_Iter, enable<is_iter<Checked_Iter>> = 0>
	constexpr void insert(Base_iter Where, Checked_Iter Src, Size_T Count) {

	}
	//
	constexpr basic_string& catenate(const Val_T* Ptr, Size_T Count) {
		Recap_with_moving(Size + Count);
		batch::Construct_n_by_n(Alc.addr(Head) + Size, Ptr, Count);
		Size += Count;
		return *this;
	}
	//
	constexpr basic_string& catenate(const Val_T* Ptr) {
		return catenate(Ptr, batch::Len< Size_T>(Ptr));
	}
	//
	constexpr basic_string& catenate(const basic_string& Ano) {
		return catenate(Ano.data(), Ano.Size);
	}
	//
	constexpr const Val_T* data()const {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		return Alc.addr(Head);
	}
	//
	constexpr decltype(auto) erase(Base_iter First, Base_iter Last) {
		return erase_by_ptr(First.Ptr, Last.Ptr - First.Ptr);
	}
	//
	constexpr Size_T length() {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		return Size;
	}
	//
	template<typename..._Args>
	constexpr void push_back(_Args&&... Params) {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		Recap_with_moving(Size + 1);
		Construct_At(Alc.addr(Head) + Size, forward<_Args>(Params)...);
		++Size;
	}
	//
	template<typename..._Args>
	constexpr void push_front(_Args&&... Params) {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		Recap_with_moving(Size + 1);
		MMove(Alc.addr(Head) + 1, Alc.addr(Head), Size * sizeof(Val_T));
		Construct_At(Alc.addr(Head), forward<_Args>(Params)...);
		++Size;
	}
	//
	constexpr void resize(Size_T Count, const Val_T& Val = Val_T{}) {
		Recap_with_moving(Count);
		if (Size < Count)
			batch::Construct_n_by_1(Alc.addr(Head) + Size, Count - Size, Val);
		else if (Size > Count)
			batch::Destroy_n(Alc.addr(Head) + Count, Size - Count);
		Size = Count;
	}
	//
	constexpr ~basic_string() {
		if (Head != Alloc_T::nulladdr) {
			batch::Destroy_n(Alc.addr(Head), Size);
			Alc.free(Head);
		}
	}
protected:
	//
	Alloc_T Alc;
	Addr_T Head;
	Size_T Size, Cap;
	//
#ifndef _DEBUG
#define XSL_BASIC_STRING_RECAP_MOVING(CON,COPY)											\
	 	constexpr void Recap_##CON##_moving(Size_T NewCap) {						\
			if (Head == Alloc_T::nulladdr) {										\
	 			Head = Alc.alloc(NewCap);											\
	 			Cap = NewCap;														\
	 		}																		\
			else if (Cap < NewCap) {												\
				if (Cap == 0)Cap = NewCap;											\
				else while (Cap < NewCap)Cap <<= 1;									\
				Addr_T NewHead = Alc.alloc(Cap);									\
				COPY;																\
				Alc.free(Head);														\
				Head = NewHead;														\
			}																		\
		}
	XSL_BASIC_STRING_RECAP_MOVING(with, MCopy(Alc.addr(NewHead), Alc.addr(Head), Size * sizeof(Val_T)));
	XSL_BASIC_STRING_RECAP_MOVING(without, XSL_EMPTR_MACRO);
#undef XSL_BASIC_STRING_RECAP_MOVING
#else
	constexpr void Recap_with_moving(Size_T NewCap) {
		if (Head == Alloc_T::nulladdr) {
			Head = Alc.alloc(NewCap);
			Cap = NewCap;
		}
		else if (Cap < NewCap) {
			if (Cap == 0)Cap = NewCap;
			else while (Cap < NewCap)Cap <<= 1;
			Addr_T NewHead = Alc.alloc(Cap);
			MCopy(Alc.addr(NewHead), Alc.addr(Head), Size * sizeof(Val_T));
			Alc.free(Head);
			Head = NewHead;
		}
	}
	constexpr void Recap_without_moving(Size_T NewCap) {
		if (Head == Alloc_T::nulladdr) {
			Head = Alc.alloc(NewCap);
			Cap = NewCap;
		}
		else if (Cap < NewCap) {
			if (Cap == 0)Cap = NewCap;
			else while (Cap < NewCap)Cap <<= 1;
			Addr_T NewHead = Alc.alloc(Cap);
			Alc.free(Head);
			Head = NewHead;
		}
	}
#endif // !DEBUG
	constexpr void erase_by_ptr(Val_T* Where, Size_T Count) {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		batch::Destroy_n(Where, Where + Count);
		MCopy(Where, Where + Count, (Alc.addr(Head) + Size - (Where + Count)) * sizeof(Val_T));
		Size -= Count;
	}
	constexpr void insert_by_ptr(Val_T* Where, Size_T Count) {
		XSL_EMPTY_CHECK(Head != Alloc_T::nulladdr);
		batch::Destroy_n(Where, Where + Count);
		MCopy(Where, Where + Count, (Alc.addr(Head) + Size - (Where + Count)) * sizeof(Val_T));
		Size -= Count;
	}
};

//using string = basic_string<char>;


#ifdef XSL_TEST
void xsl_test_basic_string() {
	TEST_SPLIT_LINE(char);
	TEST_ACT_PRINT(char cstr00[] = "123"; char cstr01[] = "123456");
	TEST_ACT_PRINT(basic_string<char> str00{});
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(basic_string<char> str01(cstr00));
	TEST_RET_PRINT(str01, print);
	TEST_ACT_PRINT(basic_string<char> str02(cstr01, 6));
	TEST_RET_PRINT(str02, print);
	TEST_ACT_PRINT(basic_string<char> str03{ as_rreference(str01) });
	TEST_RET_PRINT(str03, print);
	TEST_ACT_PRINT(basic_string<char> str04{ str02 });
	TEST_RET_PRINT(str04, print);
	TEST_ACT_PRINT(basic_string<char> str05(3, '1'));
	TEST_RET_PRINT(str05, print);
	TEST_ACT_PRINT(str00 = cstr01);
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00 = str03);
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00 = as_rreference(str02));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(cstr00));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(str04));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(as_rreference(str03)));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(cstr01 + 1, 4));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.push_back('6'));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.push_front('1'));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.resize(7, '7'));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.catenate(cstr00, 1));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str04.catenate(cstr00));
	TEST_RET_PRINT(str04, print);
	TEST_ACT_PRINT(str00.catenate(str05));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.erase(--str00.end(), str00.end()));
	TEST_RET_PRINT(str00, print);
	TEST_RET_PRINT(str00, rprint);
	TEST_SPLIT_LINE(wchar_t);
	TEST_ACT_PRINT(wchar_t cwstr00[] = L"123"; wchar_t cwstr01[] = L"123456");
	TEST_ACT_PRINT(basic_string<wchar_t> wstr00{});
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(basic_string<wchar_t> wstr01(cwstr00));
	TEST_RET_PRINT(wstr01, print);
	TEST_ACT_PRINT(basic_string<wchar_t> wstr02(cwstr01, 6));
	TEST_RET_PRINT(wstr02, print);
	TEST_ACT_PRINT(basic_string<wchar_t> wstr03{ as_rreference(wstr01) });
	TEST_RET_PRINT(wstr03, print);
	TEST_ACT_PRINT(basic_string<wchar_t> wstr04{ wstr02 });
	TEST_RET_PRINT(wstr04, print);
	TEST_ACT_PRINT(basic_string<wchar_t> wstr05(3, '1'));
	TEST_RET_PRINT(wstr05, print);
	TEST_ACT_PRINT(wstr00 = cwstr01);
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00 = wstr03);
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00 = as_rreference(wstr02));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(cwstr00));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(wstr04));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(as_rreference(wstr03)));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(cwstr01 + 1, 4));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.push_back('6'));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.push_front('1'));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.resize(7, '7'));
	TEST_RET_PRINT(wstr00, print);
	TEST_RET_PRINT(wstr00, rprint);
	TEST_SPLIT_LINE(test_t);
	TEST_ACT_PRINT(test_t test_ts[7]{}; test_ts[0].d = 1; test_ts[1].d = 2; test_ts[2].d = 3; test_ts[3].d = 4; test_ts[4].d = 5; test_ts[5].d = 6);
	TEST_ACT_PRINT(basic_string<test_t> ttstr00{});
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(basic_string<test_t> ttstr01(test_ts + 3));
	TEST_RET_PRINT(ttstr01, print);
	TEST_ACT_PRINT(basic_string<test_t> ttstr02(test_ts, 6));
	TEST_RET_PRINT(ttstr02, print);
	TEST_ACT_PRINT(basic_string<test_t> ttstr03{ as_rreference(ttstr01) });
	TEST_RET_PRINT(ttstr03, print);
	TEST_ACT_PRINT(basic_string<test_t> ttstr04{ ttstr02 });
	TEST_RET_PRINT(ttstr04, print);
	TEST_ACT_PRINT(basic_string<test_t> ttstr05(3, test_ts[0]));
	TEST_RET_PRINT(ttstr05, print);
	TEST_ACT_PRINT(ttstr00 = test_ts);
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00 = ttstr03);
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00 = as_rreference(ttstr02));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(test_ts + 3));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(ttstr04));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(as_rreference(ttstr03)));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(test_ts + 1, 4));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.push_back(6));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.push_front(1));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.resize(7, test_ts[0]));
	TEST_RET_PRINT(ttstr00, print);
	TEST_RET_PRINT(ttstr00, rprint);
	TEST_SPLIT_LINE(test_p);
	TEST_ACT_PRINT(test_p test_ps[6]{}; *test_ps[0].Ptr = 1; *test_ps[1].Ptr = 2; *test_ps[2].Ptr = 3; *test_ps[3].Ptr = 4; *test_ps[4].Ptr = 5; *test_ps[5].Ptr = 6);
	TEST_ACT_PRINT(basic_string<test_p> tpstr00{});
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(basic_string<test_p> tpstr01(test_ps, 3));
	TEST_RET_PRINT(tpstr01, print);
	TEST_ACT_PRINT(basic_string<test_p> tpstr02(6, test_ps[0]));
	TEST_RET_PRINT(tpstr02, print);
	TEST_ACT_PRINT(basic_string<test_p> tpstr03{ as_rreference(tpstr01) });
	TEST_RET_PRINT(tpstr03, print);
	TEST_ACT_PRINT(basic_string<test_p> tpstr04{ tpstr02 });
	TEST_RET_PRINT(tpstr04, print);
	TEST_ACT_PRINT(tpstr00 = tpstr03);
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00 = as_rreference(tpstr02));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(tpstr03));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(as_rreference(tpstr04)));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(test_ps + 1, 4));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.push_back(6));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.push_front(1));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.resize(7, test_ps[0]));
	TEST_RET_PRINT(tpstr00, print);
	TEST_RET_PRINT(tpstr00, rprint);
}
#endif // XSL_TEST

XSL_END
#endif // !XSL_BASIC_STRING
