#pragma once
#ifndef XSL_BASE_VIEW
#define XSL_BASE_VIEW
#include"xsl_def.h"
#include"xsl_iterator.h"
#include"xsl_batch.h"
XSL_BEGIN

template <typename _Val, typename _SizeType = uint_32>
class seq_list_view {
	//
public:
	using Val_T = _Val;
	using Size_T = _SizeType;
	using GPtr_T = const Val_T*;
	using Iter = random_forward_bilateral_iterator<seq_list_view>;
	using RIter = random_reverse_bilateral_iterator<seq_list_view>;
	using Base_Iter = Iter::Base;
	//
	constexpr seq_list_view(const Val_T* Ptr) :seq_list_view(Ptr, batch::Len<Size_T>(Ptr)) {}
	//
	constexpr seq_list_view(const Val_T* Ptr, Size_T Count) : Head(Ptr), Size(Count) {}
	//
	constexpr seq_list_view(seq_list_view&&) = default;
	//
	constexpr seq_list_view(const seq_list_view&) = default;
	//
	constexpr seq_list_view& operator=(const Val_T* Ptr) {
		return assign(Ptr);
	}
	//
	constexpr seq_list_view& operator=(seq_list_view&&) = default;
	//
	constexpr seq_list_view& operator=(const seq_list_view&) = default;
	//
	constexpr Val_T& operator[](Size_T Off) {
		XSL_EMPTY_CHECK(Head);
		XSL_RANGE_CHECK(Off < Size);
		return Head[Off];
	}
	//
	constexpr seq_list_view& assign(const Val_T* Ptr, Size_T Count) {
		Head = Ptr;
		Size = Count;
		return *this;
	}
	//
	constexpr seq_list_view& assign(const Val_T* Ptr) {
		return assign(Ptr, batch::Len<Size_T>(Ptr));
	}
	//
	constexpr seq_list_view& assign(seq_list_view&& Ano) {
		return assign(Ano.Head, Ano.Size);
	}
	//
	constexpr seq_list_view& assign(const seq_list_view& Ano) {
		return assign(Ano.Head, Ano.Size);
	}
	//
	constexpr Iter begin() {
		return  Head;
	}
	//
	constexpr Iter end() {
		return Head + Size;
	}
	//
	constexpr RIter rbegin() {
		return  Head + Size - 1;
	}
	//
	constexpr RIter rend() {
		return   Head - 1;
	}
	//
	constexpr const Val_T* data() {
		XSL_EMPTY_CHECK(Head);
		return Head;
	}
	//
	constexpr Size_T length() {
		XSL_EMPTY_CHECK(Head);
		return Size;
	}
	//
	constexpr void push_back() {
		XSL_EMPTY_CHECK(Head);
		++Size;
	}
	//
	constexpr void push_front() {
		XSL_EMPTY_CHECK(Head);
		--Head;
		++Size;
	}
	//
	constexpr void resize(Size_T Count) {
		Size = Count;
	}
private:
	//
	const Val_T* Head;
	Size_T Size;
};

#ifdef XSL_TEST
void xsl_test_seq_list_view() {
	TEST_SPLIT_LINE(char);
	TEST_ACT_PRINT(char cstr00[] = "123"; char cstr01[] = "123456");
	TEST_ACT_PRINT(seq_list_view<char> str00{ cstr00 });
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(seq_list_view<char> str01(cstr01, 6));
	TEST_RET_PRINT(str01, print);
	TEST_ACT_PRINT(seq_list_view<char> str02{ as_rreference(str00) });
	TEST_RET_PRINT(str02, print);
	TEST_ACT_PRINT(seq_list_view<char> str03{ str01 });
	TEST_RET_PRINT(str03, print);
	TEST_ACT_PRINT(str00 = cstr00);
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00 = as_rreference(str01));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00 = str02);
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(cstr01));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(as_rreference(str02)));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(str03));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.assign(cstr01 + 1, 3));
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.push_back());
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.push_front());
	TEST_RET_PRINT(str00, print);
	TEST_ACT_PRINT(str00.resize(6));
	TEST_RET_PRINT(str00, print);
	TEST_RET_PRINT(str00, rprint);
	TEST_SPLIT_LINE(wchar_t);
	TEST_ACT_PRINT(wchar_t cwstr00[] = L"123"; wchar_t cwstr01[] = L"123456");
	TEST_ACT_PRINT(seq_list_view<wchar_t> wstr00{ cwstr00 });
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(seq_list_view<wchar_t> wstr01(cwstr01, 6));
	TEST_RET_PRINT(wstr01, print);
	TEST_ACT_PRINT(seq_list_view<wchar_t> wstr02{ as_rreference(wstr00) });
	TEST_RET_PRINT(wstr02, print);
	TEST_ACT_PRINT(seq_list_view<wchar_t> wstr03{ wstr01 });
	TEST_RET_PRINT(wstr03, print);
	TEST_ACT_PRINT(wstr00 = cwstr00);
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00 = as_rreference(wstr01));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00 = wstr02);
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(cwstr01));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(as_rreference(wstr02)));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(wstr03));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.assign(cwstr01 + 1, 3));
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.push_back());
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.push_front());
	TEST_RET_PRINT(wstr00, print);
	TEST_ACT_PRINT(wstr00.resize(6));
	TEST_RET_PRINT(wstr00, print);
	TEST_RET_PRINT(wstr00, rprint);
	TEST_SPLIT_LINE(test_t);
	TEST_ACT_PRINT(test_t test_ts[6]{}; test_ts[0].d = 1; test_ts[1].d = 2; test_ts[2].d = 3; test_ts[3].d = 4; test_ts[4].d = 5; test_ts[5].d = 6);
	TEST_ACT_PRINT(seq_list_view<test_t> ttstr00{ test_ts });
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(seq_list_view<test_t> ttstr01(test_ts + 3, 3));
	TEST_RET_PRINT(ttstr01, print);
	TEST_ACT_PRINT(seq_list_view<test_t> ttstr02{ as_rreference(ttstr00) });
	TEST_RET_PRINT(ttstr02, print);
	TEST_ACT_PRINT(seq_list_view<test_t> ttstr03{ ttstr01 });
	TEST_RET_PRINT(ttstr03, print);
	TEST_ACT_PRINT(ttstr00 = test_ts);
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00 = as_rreference(ttstr01));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00 = ttstr02);
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(as_rreference(ttstr03)));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(test_ts));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(ttstr01));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.assign(test_ts + 1, 3));
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.push_back());
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.push_front());
	TEST_RET_PRINT(ttstr00, print);
	TEST_ACT_PRINT(ttstr00.resize(6));
	TEST_RET_PRINT(ttstr00, print);
	TEST_RET_PRINT(ttstr00, rprint);
	TEST_SPLIT_LINE(test_p);
	TEST_ACT_PRINT(test_p test_ps[7]{}; *test_ps[0].Ptr = 1; *test_ps[1].Ptr = 2; *test_ps[2].Ptr = 3; *test_ps[3].Ptr = 4; *test_ps[4].Ptr = 5; *test_ps[5].Ptr = 6);
	TEST_ACT_PRINT(seq_list_view<test_p> tpstr00{ test_ps });
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(seq_list_view<test_p> tpstr01(test_ps + 3, 3));
	TEST_RET_PRINT(tpstr01, print);
	TEST_ACT_PRINT(seq_list_view<test_p> tpstr02{ as_rreference(tpstr00) });
	TEST_RET_PRINT(tpstr02, print);
	TEST_ACT_PRINT(seq_list_view<test_p> tpstr03{ tpstr01 });
	TEST_RET_PRINT(tpstr03, print);
	TEST_ACT_PRINT(tpstr00 = test_ps);
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00 = as_rreference(tpstr01));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00 = tpstr02);
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(as_rreference(tpstr03)));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(test_ps));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(tpstr01));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.assign(test_ps + 1, 3));
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.push_back());
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.push_front());
	TEST_RET_PRINT(tpstr00, print);
	TEST_ACT_PRINT(tpstr00.resize(6));
	TEST_RET_PRINT(tpstr00, print);
	TEST_RET_PRINT(tpstr00, rprint);
}
#endif // XSL_TEST

XSL_END
#endif // !XSL_BASE_VIEW
