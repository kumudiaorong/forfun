#pragma once
#include "xsl/bits/pf/def.h"
#ifndef XSL_VECTOR_SUPPORT
#define XSL_VECTOR_SUPPORT
#include <initializer_list>
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/batch.hpp>
#include <xsl/bits/exception.hpp>
#include <xsl/bits/iterator.hpp>
#include <xsl/bits/utility.hpp>

namespace xsl::vec {
  template <class _Val, class _Alloc>
  class vector {
    //
  public:
    // clang-format off
	typedef ptrdiff_t 								        diff_type;
	typedef _Val 								              val_type;
	typedef _Alloc 								            alloc_type;
	typedef size_t 								            size_type;
	typedef itor::ptr_wrapper<val_type> 			iter;
	typedef itor::ptr_wrapper<const val_type> citer;
	typedef itor::reverse<val_type*> 		      riter;
	typedef itor::reverse<const val_type*>    criter;
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
      : vector(tag_alloc{}, 0) {
    }
    //
    constexpr vector(size_type count, const val_type& val)
      : vector(tag_alloc{}, count) {
      this->Insert(Head, count, val);
    }
    //
    constexpr vector(size_type count)
      : vector(tag_alloc{}, count) {
      this->Insert(Head, count);
    }
    //
    template <class UCFIter, ts::enable<itor::is<UCFIter>> = 0>
    constexpr vector(UCFIter first, UCFIter last)
      : vector(itor::get_unwrapped(first), batch::length(first, last)) {
    }
    //
    template <class UCFIter, ts::enable<itor::is<UCFIter>> = 0>
    constexpr vector(UCFIter first, size_type count)
      : vector(tag_alloc{}, count) {
      this->Insert(Head, itor::get_unwrapped(first), count);
    }
    //
    constexpr vector(const vector& ano)
      : vector(ano.Head, ano.Size) {
    }
    //
    constexpr vector(vector&& ano) noexcept
      : Alc(as_rreference(ano.Alc))
      , Head(ano.Head)
      , Size(ano.Size)
      , Cap(ano.Cap) {
      ano.Head = nullptr;
    }
    //
    constexpr vector(std::initializer_list<val_type> ilist)
      : vector(tag_alloc{}, ilist.size()) {
      this->Insert(Head, creator(ilist.begin(), ilist.size()));
    }
    //
    constexpr ~vector() {
      if(!this->invalid()) {
        this->erase(Head, Size);
        Alc.deallocate(Head);
      }
    }
    //
    constexpr vector& operator=(vector&& ano) noexcept {
      return this->assign(as_rreference(ano));
    }
    //
    constexpr vector& operator=(const vector& ano) {
      return this->assign(ano.Head, ano.Size);
    }
    //
    constexpr vector& operator=(std::initializer_list<val_type> ilist) {
      return this->assign(itor::get_unwrapped(ilist.begin()), ilist.size());
    }
    constexpr vector& assign(size_type count, const val_type& val) {
      return this->Assign(count, val);
    }
    //
    constexpr vector& assign(const vector& ano) {
      return this->assign(ano.Head, ano.Size);
    }
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
    template <class UCFIter, ts::enable<itor::is<UCFIter>> = 0>
    constexpr vector& assign(UCFIter first, size_type count) {
      return this->Assign(itor::get_unwrapped(first), count);
    }
    //
    template <class UCFIter, ts::enable<itor::is<UCFIter>> = 0>
    constexpr vector& assign(UCFIter first, UCFIter last) {
      return this->Assign(itor::get_unwrapped(first), batch::length(first, last));
    }
    //
    constexpr vector& assign(std::initializer_list<val_type> ilist) {
      return this->Assign(itor::get_unwrapped(ilist.begin()), ilist.size());
    }
    //
    constexpr val_type& at(size_type index) {
      ecp::check_empty_object(invalid());

      return Head[index];
    }
    //
    constexpr const val_type& at(size_type index) const {
      ecp::check_empty_object(invalid());

      return Head[index];
    }
    //
    constexpr val_type& operator[](size_type index) {
      ecp::check_empty_object(invalid());

      return Head[index];
    }
    //
    constexpr const val_type& operator[](size_type index) const {
      ecp::check_empty_object(invalid());

      return Head[index];
    }
    //
    constexpr val_type& front() {
      ecp::check_empty_object(invalid());
      return Head[0];
    }
    //
    constexpr const val_type& front() const {
      ecp::check_empty_object(invalid());
      return Head[0];
    }
    //
    constexpr val_type& back() {
      ecp::check_empty_object(invalid());
      return Head[Size - 1];
    }
    //
    constexpr const val_type& back() const {
      ecp::check_empty_object(invalid());

      return Head[Size - 1];
    }
    //
    constexpr val_type *data() {
      ecp::check_empty_object(invalid());

      return Head;
    }
    //
    constexpr const val_type *data() const {
      ecp::check_empty_object(invalid());

      return Head;
    }
    //
    constexpr iter begin() {
      return Head;
    }
    //
    constexpr citer begin() const {
      return Head;
    }
    //
    constexpr iter end() {
      return Head + Size;
    }
    //
    constexpr citer end() const {
      return Head + Size;
    }
    //
    constexpr riter rbegin() {
      return Head + Size - 1;
    }
    //
    constexpr criter rbegin() const {
      return riter{Head + Size - 1};
    }
    //
    constexpr riter rend() {
      return Head - 1;
    }
    //
    constexpr criter rend() const {
      return Head - 1;
    }
    constexpr citer cbegin() const {
      return Head;
    }
    //
    constexpr citer cend() const {
      return Head + Size;
    }
    //
    constexpr criter crbegin() const {
      return Head + Size - 1;
    }
    //
    constexpr criter crend() const {
      return Head - 1;
    }
    //
    constexpr bool invalid() const noexcept {
      return Head == nullptr;
    }
    //
    constexpr bool empty() const {
      return Size == 0;
    }
    //
    constexpr size_type size() const {
      return Size;
    }
    //
    constexpr void reserve(size_type NewCap) {
      Sus(NewCap, 0);
    }
    //
    constexpr size_type capacity() const {
      return Cap;
    }
    //
    constexpr void shrink_to_fit() {
      if(Cap != Size) {
        val_type *NewHead = Alc.allocate(Size * sizeof(val_type));
        MMove(NewHead, Head, Size * sizeof(val_type));
        Alc.deallocate(Head);
        Head = NewHead;
        Cap = Size;
      }
    }
    //
    constexpr void clear() {
      this->erase(Head, Size);
    }
    //
    constexpr iter insert(citer where, const val_type& val) {
      return this->emplace(where.Ptr, val);
    }
    //
    constexpr iter insert(citer where, val_type&& val) {
      return this->emplace(where.Ptr, as_rreference(val));
    }
    //
    constexpr iter insert(citer where, size_type count, const val_type& val) {
      return this->Insert(where.Ptr, count, val);
    }
    //
    template <class Unchecked_Iter, ts::enable<itor::is<Unchecked_Iter>> = 0>
    constexpr iter insert(citer where, Unchecked_Iter first, Unchecked_Iter last) {
      return this->Insert(where.Ptr, itor::get_unwrapped(first), batch::length(first, last));
    }
    //
    constexpr iter insert(citer where, std::initializer_list<val_type> ilist) {
      return this->Insert(where.Ptr, ilist.begin(), ilist.size());
    }
    //
    template <class Unchecked_Iter, ts::enable<itor::is<Unchecked_Iter>> = 0>
    constexpr iter insert(citer where, Unchecked_Iter first, size_type count) {
      return this->Insert(where.Ptr, itor::get_unwrapped(first), count);
    }
    //
    template <class... Args>
    constexpr iter emplace(citer where, Args&&...params) {
      auto wh = const_cast<val_type *>(where.Ptr);
      ptrdiff_t index = wh - Head;
      Sus(index, 1);
      ++Size;
      construct_at(Head + index, forward<Args>(params)...);
      return Head + index;
    }
    //
    constexpr iter erase(citer where) {
      return this->erase(where.Ptr, 1);
    }
    //
    constexpr iter erase(citer first, citer last) {
      return this->erase(first.Ptr, /*static_cast<size_type>(*/ last - first);
    }
    //
    constexpr void push_back(const val_type& val) {
      return this->emplace(Head + Size, val);
    }
    //
    constexpr void push_back(val_type&& val) {
      return this->emplace(Head + Size, as_rreference(val));
    }
    //
    template <class... Args>
    constexpr val_type& emplace_back(Args&&...params) {
      return *(this->emplace(Head + Size, forward<Args>(params)...));
    }
    //
    constexpr void pop_back() {
      this->erase(Head + Size - 1, 1);
    }
    //
    constexpr void resize(size_type newSize) {
      if(Size < newSize)
        this->Insert(Head + Size, newSize - Size);
      else if(Size > newSize)
        this->erase(Head + newSize, Size - newSize);
    }
    //
    constexpr void resize(size_type newSize, const val_type& val) {
      if(Size < newSize)
        this->Insert(Head + Size, newSize - Size, val);
      else if(Size > newSize)
        this->erase(Head + newSize, Size - newSize);
    }
    //
    constexpr void swap(vector& ano) {
      xsl::swap(Alc, ano.Alc);
      xsl::swap(Head, ano.Head);
      xsl::swap(Cap, ano.Cap);
      xsl::swap(Size, ano.Size);
    }
    //
    constexpr void swap(citer l, citer r) {
      xsl::swap(*l, *r);
    }
  protected:
    // default_allocator<_Val, _SizeType> Alc;
    alloc_type Alc;
    val_type *Head;
    size_type Size;
    size_type Cap;
    //
    template <class CIter>
    constexpr vector& Assign(CIter first, size_type count) {
      if(this->invalid()) {
        Head = Alc.allocate((count + 1) * sizeof(val_type));
        Cap = count + 1;
      } else
        erase(Head, Size);
      this->Insert(Head, first, count);
      return *this;
    }
    //
    constexpr vector& Assign(size_type count, const val_type& val) {
      if(this->invalid()) {
        Head = Alc.allocate((count + 1) * sizeof(val_type));
        Cap = count + 1;
      } else
        erase(Head, Size);
      this->Insert(Head, count, val);
      return *this;
    }
    //
    constexpr val_type *erase(const val_type *where, size_type count) {
      auto wh = const_cast<val_type *>(where);
      batch::destroy(wh, count);
      MMove(wh, wh + count, (Head + Size - (wh + count)) * sizeof(val_type));
      Size -= count;
      return wh;
    }
    //
    template <class CIter>
    constexpr val_type *Insert(const val_type *where, CIter first, size_type count) {
      auto wh = const_cast<val_type *>(where);
      ptrdiff_t index = wh - Head;
      Sus(index, count);
      Size += count;
      batch::copy(Head + index, first, count);
      return Head + count;
    }
    //
    constexpr val_type *Insert(const val_type *where, size_type count, const val_type& val) {
      auto wh = const_cast<val_type *>(where);
      ptrdiff_t index = wh - Head;
      Sus(index, count);
      Size += count;
      batch::create(Head + index, count, val);
      return Head + count;
    }
    //
    constexpr val_type *Insert(const val_type *where, size_type count) {
      auto wh = const_cast<val_type *>(where);
      ptrdiff_t index = wh - Head;
      Sus(index, count);
      Size += count;
      batch::create(Head + index, count);
      return Head + count;
    }
    //
    constexpr void Sus(size_type first, size_type count) {
      if(Cap == 0) {
        Cap = count;
      } else if(Cap < Size + count) {
        while(Cap < Size + count)
          Cap <<= 1;
        val_type *NewHead = Alc.allocate(Cap * sizeof(val_type));
        MMove(NewHead, Head, first * sizeof(val_type));
        MMove(NewHead + first + count, Head + first, (Size - first) * sizeof(val_type));
        Alc.deallocate(Head);
        Head = NewHead;
      } else {
        MMove(Head + first + count, Head + first, (Size - first) * sizeof(val_type));
      }
    }
    // set uninitialized space
  };
  //
  template <class Val, class Alloc>
  constexpr typename comp::traits<vector<Val, Alloc>>::comp_category operator<=>(
    const vector<Val, Alloc>& l, const vector<Val, Alloc>& r) {
    size_t lsize = l.size(), rsize = r.size();
    uint_8 ret = batch::compare(l.begin(), r.begin(), min(lsize, rsize));
    if(ret == 0 && lsize != rsize)
      ret = lsize < rsize ? -1 : 1;
    return ret <=> 0;
  }
  //
  template <class Val, class Alloc1, class Alloc2>
  constexpr bool operator==(const vector<Val, Alloc1>& l, const vector<Val, Alloc2>& r) {
    return l.size() == r.size() ? bool(l <=> r) : false;
  }
  //
}  // namespace xsl::vec
#endif  // !XSL_VECTOR
