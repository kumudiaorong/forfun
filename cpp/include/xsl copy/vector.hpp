#pragma once
#ifndef XSL_VECTOR
#define XSL_VECTOR
#include <initializer_list>

#include "bits/pf/memory.hpp"
#include "bits/xsl_allocator.hpp"
#include "bits/xsl_batch.hpp"
#include "bits/xsl_compare.hpp"
#include "bits/xsl_container.hpp"
#include "bits/xsl_def.hpp"
#include "bits/xsl_iterator.hpp"
#include "bits/xsl_utility.hpp"

XSL_BEGIN

template <class _Val, class _Alloc = default_allocator<_Val>>
class vector {
  //
public:
  // clang-format off
	typedef _Val 								              val_type;
	typedef _Alloc 								            alloc_type;
	typedef size_t 								            size_type;
	typedef pointer_wrapper<val_type> 			  iter;
	typedef pointer_wrapper<const val_type> 	citer;
	typedef reverse_iterator<val_type*> 		  riter;
	typedef reverse_iterator<const val_type*> criter;
	typedef std::strong_ordering 				      comp_category;
	typedef vector								            comp_type;
  // clang-format on
  //
public:
  constexpr vector(tag_alloc, size_type count)
    : Alc()
    , Head(Alc.allocate((count + 1) * sizeof(val_type)))
    , Size()
    , Cap(count + 1){};
  constexpr vector()
    : vector(tag_alloc{}, 0) {}
  //
  constexpr vector(size_type count, const val_type& val)
    : vector(tag_alloc{}, count) {
    this->insert(Head, creator(count, val));
  }
  //
  constexpr vector(size_type count)
    : vector(tag_alloc{}, count) {
    this->insert(Head, creator(count));
  }
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr vector(UCFIter first, UCFIter last)
    : vector(get_mnr_iter(first), batch::length(first, last)) {}
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr vector(UCFIter first, size_type count)
    : vector(tag_alloc{}, count) {
    this->insert(Head, creator(get_mnr_iter(first), count));
  }
  //
  constexpr vector(const vector& ano)
    : vector(ano.Head, ano.Size) {}
  //
  constexpr vector(vector&& ano) noexcept
    : Alc(as_rreference(ano.Alc))
    , Head(ano.Head)
    , Cap(ano.Cap)
    , Size(ano.Size) {
    ano.Head = nullptr;
  }
  //
  constexpr vector(std::initializer_list<val_type> ilist)
    : vector(tag_alloc{}, ilist.size()) {
    this->insert(Head, creator(get_mnr_iter(ilist.begin()), ilist.size()));
  }
  //
  constexpr ~vector() {
    if(!this->invalid()) {
      this->erase(Head, Size);
      Alc.deallocate(Head);
    }
  }
  //
  constexpr vector& operator=(vector&& ano) noexcept { return this->assign(as_rreference(ano)); }
  //
  constexpr vector& operator=(const vector& ano) { return this->assign(creator(ano.Head, ano.Size)); }
  //
  constexpr vector& operator=(std::initializer_list<val_type> ilist) {
    return this->assign(creator(get_mnr_iter(ilist.begin()), ilist.size()));
  }
  //
  //
  template <class UCreator, ts::enable<is_creater<UCreator>> = 0>
  constexpr vector& assign(UCreator&& Ctor) {
    if(this->invalid()) {
      Head = Alc.allocate((Ctor.Size + 1) * sizeof(val_type));
      Cap = Ctor.Size + 1;
    } else
      erase(Head, Size);
    this->insert(Head, Ctor);
    return *this;
  }
  constexpr vector& assign(size_type count, const val_type& val) { return this->assign(creator(count, val)); }
  //
  constexpr vector& assign(const vector& ano) { return this->assign(creator(ano.Head, ano.Size)); }
  //
  constexpr vector& assign(vector&& ano) {
    this->~vector();
    Alc = as_rreference(ano.Alc);
    Head = ano.Head;
    Cap = ano.Cap;
    Size = ano.Size;
    ano.Head = nullptr;
    return *this;
  }
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr vector& assign(UCFIter first, size_type count) {
    return this->assign(creator(get_mnr_iter(first), count));
  }
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr vector& assign(UCFIter first, UCFIter last) {
    return this->assign(creator(get_mnr_iter(first), batch::length(first, last)));
  }
  //
  constexpr vector& assign(std::initializer_list<val_type> ilist) {
    return this->assign(creator(get_mnr_iter(ilist.begin()), ilist.size()));
  }
  //
  constexpr val_type& at(size_type index) {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[index];
  }
  //
  constexpr const val_type& at(size_type index) const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[index];
  }
  //
  constexpr val_type& operator[](size_type index) {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[index];
  }
  //
  constexpr const val_type& operator[](size_type index) const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[index];
  }
  //
  constexpr val_type& front() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[0];
  }
  //
  constexpr const val_type& front() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[0];
  }
  //
  constexpr val_type& back() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[Size - 1];
  }
  //
  constexpr const val_type& back() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[Size - 1];
  }
  //
  constexpr val_type *data() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head;
  }
  //
  constexpr const val_type *data() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head;
  }
  //
  constexpr iter begin() { return Head; }
  //
  constexpr citer begin() const { return Head; }
  //
  constexpr iter end() { return Head + Size; }
  //
  constexpr citer end() const { return Head + Size; }
  //
  constexpr riter rbegin() { return Head + Size - 1; }
  //
  constexpr criter rbegin() const { return riter{Head + Size - 1}; }
  //
  constexpr riter rend() { return Head - 1; }
  //
  constexpr criter rend() const { return Head - 1; }
  constexpr citer cbegin() const { return Head; }
  //
  constexpr citer cend() const { return Head + Size; }
  //
  constexpr criter crbegin() const { return Head + Size - 1; }
  //
  constexpr criter crend() const { return Head - 1; }
  //
  constexpr bool invalid() const noexcept { return Head == nullptr; }
  //
  constexpr bool empty() const { return Size == 0; }
  //
  constexpr size_type size() const { return Size; }
  //
  constexpr void reserve(size_type NewCap) {
    if(Cap < NewCap) recap(NewCap, 0, 0);
  }
  //
  constexpr size_type capacity() const { return Cap; }
  //
  constexpr void shrink_to_fit() { recap(Size, 0, 0); }
  //
  constexpr void clear() { this->erase(Head, Size); }
  //
  constexpr iter insert(citer where, const val_type& val) { return this->emplace(where.Ptr, val); }
  //
  constexpr iter insert(citer where, val_type&& val) { return this->emplace(where.Ptr, as_rreference(val)); }
  //
  constexpr iter insert(citer where, size_type count, const val_type& val) {
    return this->insert(where.Ptr, creator(count, val));
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr iter insert(citer where, Unchecked_Iter first, Unchecked_Iter last) {
    return this->insert(where.Ptr, creator(get_mnr_iter(first), batch::length(first, last)));
  }
  //
  constexpr iter insert(citer where, std::initializer_list<val_type> ilist) {
    return this->insert(where.Ptr, creator(get_mnr_iter(ilist.begin()), ilist.size()));
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr iter insert(citer where, Unchecked_Iter first, size_type count) {
    return this->insert(where.Ptr, creator(get_mnr_iter(first), count));
  }
  //
  template <class... Args>
  constexpr iter emplace(citer where, Args&&...params) {
    auto wh = const_cast<val_type *>(where.Ptr);
    ptrdiff_t index = wh - Head;
    if(Cap == Size) {
      recap(static_cast<size_type>(index), 1);
    } else {
      MMove(wh + 1, wh, (Size - index) * sizeof(val_type));
    }
    ++Size;
    construct_at(Head + index, forward<Args>(params)...);
    return Head + index;
  }
  //
  constexpr iter erase(citer where) { return this->erase(where.Ptr, 1); }
  //
  constexpr iter erase(citer first, citer last) {
    return this->erase(first.Ptr, /*static_cast<size_type>(*/ last - first);
  }
  //
  constexpr void push_back(const val_type& val) { return this->emplace(Head + Size, val); }
  //
  constexpr void push_back(val_type&& val) { return this->emplace(Head + Size, as_rreference(val)); }
  //
  template <class... Args>
  constexpr val_type& emplace_back(Args&&...params) {
    return *(this->emplace(Head + Size, forward<Args>(params)...));
  }
  //
  constexpr void pop_back() { this->erase(Head + Size - 1, 1); }
  //
  constexpr void resize(size_type newSize) {
    if(Size < newSize)
      this->insert(Head + Size, creator(newSize - Size));
    else if(Size > newSize)
      this->erase(Head + newSize, Size - newSize);
  }
  //
  constexpr void resize(size_type newSize, const val_type& val) {
    if(Size < newSize)
      this->insert(Head + Size, creator(newSize - Size, val));
    else if(Size > newSize)
      this->erase(Head + newSize, Size - newSize);
  }
  //
  constexpr void swap(vector& ano) {
    XSL swap(Alc, ano.Alc);
    XSL swap(Head, ano.Head);
    XSL swap(Cap, ano.Cap);
    XSL swap(Size, ano.Size);
  }
protected:
  // default_allocator<_Val, _SizeType> Alc;
  alloc_type Alc;
  val_type *Head;
  size_type Size, Cap;
  //
  constexpr val_type *erase(const val_type *where, size_type count) {
    auto wh = const_cast<val_type *>(where);
    destroy_n(wh, condition(count));
    MMove(wh, wh + count, (Head + Size - (wh + count)) * sizeof(val_type));
    Size -= count;
    return wh;
  }
  //
  template <class Creator>
  constexpr val_type *insert(const val_type *where, Creator&& ctor) {
    auto wh = const_cast<val_type *>(where);
    ptrdiff_t index = wh - Head;
    if(size_type NewCap = Size + ctor.Size; Cap < NewCap) {
      if(Cap == 0)
        Cap = NewCap;
      else
        while(Cap < NewCap)
          Cap <<= 1;
      recap(static_cast<size_type>(index), ctor.Size);
    } else {
      MMove(wh + ctor.Size, wh, (Size - index) * sizeof(val_type));
    }
    Size += ctor.Size;
    create_n(Head + index, ctor);
    return Head + ctor.Count;
  }
  //
  constexpr void recap(size_type first, size_type count) {
    val_type *NewHead = Alc.allocate(Cap * sizeof(val_type));
    batch::copy(NewHead, Head, first);
    batch::copy(NewHead + first + count, Head + first, Size - first);
    Alc.deallocate(Head);
    Head = NewHead;
  }
};
//
template <class Val, class Alloc>
constexpr typename comp_traits<vector<Val, Alloc>>::comp_category operator<=>(
  const vector<Val, Alloc>& l, const vector<Val, Alloc>& r) {
  size_t lsize = l.size(), rsize = r.size();
  uint_8 ret = batch::compare(l.begin(), r.begin(), min(lsize, rsize));
  if(ret == 0 && lsize != rsize) ret = lsize < rsize ? -1 : 1;
  return ret <=> 0;
}
//
template <class Val, class Alloc1, class Alloc2>
constexpr bool operator==(const vector<Val, Alloc1>& l, const vector<Val, Alloc2>& r) {
  return l.size() == r.size() ? bool(l <=> r) : false;
}
//
#ifdef XSL_TEST
#include "bits/xsl_test.hpp"
void xsl_test_vector() {
  TEST_SPLIT_LINE(xsl::test::test_p);
  TEST_ACT_PRINT(xsl::test::test_p test_ps[6]{}; *test_ps[0].Ptr = 1; *test_ps[1].Ptr = 2; *test_ps[2].Ptr = 3;
                 *test_ps[3].Ptr = 4; *test_ps[4].Ptr = 5; *test_ps[5].Ptr = 6);
  TEST_ACT_PRINT(vector<xsl::test::test_p> c00{});
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(vector<xsl::test::test_p> c01(test_ps, 1));
  TEST_RET_PRINT(c01, print, c01.size());
  TEST_ACT_PRINT(vector<xsl::test::test_p> c02(test_ps, test_ps + 2));
  TEST_RET_PRINT(c02, print, c02.size());
  TEST_ACT_PRINT(vector<xsl::test::test_p> c03(3));
  TEST_RET_PRINT(c03, print, c03.size());
  TEST_ACT_PRINT(vector<xsl::test::test_p> c04(3, test_ps[1]));
  TEST_RET_PRINT(c04, print, c04.size());
  TEST_ACT_PRINT(c00 = as_rreference(c01));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00 = c02);
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.assign(as_rreference(c03)));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.assign(test_ps, 5));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.assign(test_ps, test_ps + 6));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.assign(7, test_ps[2]));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.assign(c02));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((c00.front(), c00.back()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((as_const(c00).front(), as_const(c00).back()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((c00.begin(), c00.cbegin()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((as_const(c00).begin(), as_const(c00).cbegin()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((c00.end(), c00.cend()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((as_const(c00).end(), as_const(c00).cend()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((c00.rbegin(), c00.crbegin()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((as_const(c00).rbegin(), as_const(c00).crbegin()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((c00.rend(), c00.crend()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((as_const(c00).rend(), as_const(c00).crend()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((c00.empty(), c00.invalid()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT((as_const(c00).empty(), as_const(c00).invalid()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.clear());
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.emplace(c00.begin(), *test_ps));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.emplace(c00.begin(), as_rreference(*test_ps)));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.insert(c00.begin(), 1, test_ps[1]));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.insert(c00.begin(), test_ps + 1, test_ps + 2));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.emplace(c00.begin(), 1));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.erase(c00.begin(), ++ ++c00.begin()));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.emplace_back(test_ps[1]));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.emplace_back(as_rreference(test_ps[1])));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.emplace_back(3));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.pop_back());
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.resize(c00.size() - 3));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.resize(c00.size() + 1));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.resize(c00.size() + 1, *(test_ps + 3)));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_ACT_PRINT(c00.swap(c02));
  TEST_RET_PRINT(c00, print, c00.size());
  TEST_RET_PRINT(c00, rprint, c00.size());
}
#endif  // XSL_TEST

XSL_END
#endif  // !XSL_VECTOR
