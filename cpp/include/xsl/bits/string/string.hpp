#pragma once
#include "xsl/bits/ts/has.hpp"
#include "xsl/bits/ts/unvs.hpp"
#ifndef XSL_BASIC_STRING
#define XSL_BASIC_STRING
#include <initializer_list>
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/batch.hpp>
#include <xsl/bits/exception.hpp>
#include <xsl/bits/pf/memory.hpp>

namespace xsl::str {
  template <class _Val>
  class basic_string_view {
    //
  public:
    typedef _Val val_type;
    typedef size_t size_type;
    typedef itor::ptr_wrapper<const val_type> iter;
    typedef iter citer;
    typedef itor::reverse<iter> riter;
    typedef riter criter;
    typedef std::strong_ordering comp_category;
    typedef basic_string_view comp_type;
    //
    constexpr basic_string_view()
      : Head()
      , Size() {
    }
    //
    constexpr basic_string_view(const basic_string_view& ano, size_type index, size_type count)
      : basic_string_view(ano.Head + index, count == npos ? ano.size : count) {
    }
    //
    template <class SizeType, ts::enable<ts::has::opeartor_simple_assignment<size_type, SizeType>> = 0>
    constexpr basic_string_view(const val_type *first, SizeType count)
      : Head(first)
      , Size(count) {
    }
    //
    constexpr basic_string_view(const val_type *first)
      : basic_string_view(first, batch::length(first)) {
    }
    //
    constexpr basic_string_view(const val_type *first, const val_type *last)
      : basic_string_view(first, last - first) {
    }
    //
    constexpr basic_string_view(const basic_string_view&) = default;
    //
    constexpr basic_string_view(basic_string_view&&) = default;
    //
    constexpr basic_string_view& operator=(const val_type *first) {
      return assign(first, batch::length(first));
    }
    //
    constexpr basic_string_view& operator=(basic_string_view&& ano) noexcept {
      return assign(ano.Head, ano.Size);
    }
    //
    constexpr basic_string_view& operator=(const basic_string_view& ano) {
      return assign(ano.Head, ano.Size);
    }
    //
    constexpr basic_string_view& assign(const basic_string_view& ano) {
      return assign(ano.Head, ano.Size);
    }
    //
    constexpr basic_string_view& assign(const basic_string_view& ano, size_type index, size_type count = npos) {
      return assign(ano.Head + index, count == npos ? ano.Size : count);
    }
    //
    constexpr basic_string_view& assign(basic_string_view&& ano) {
      return assign(ano.Head, ano.Size);
    }
    //
    constexpr basic_string_view& assign(const val_type *first, size_type count) {
      Head = addr(*first);
      Size = count;
      return *this;
    }
    //
    constexpr basic_string_view& assign(const val_type *first) {
      return assign(first, Len<size_type>(first));
    }
    //
    constexpr basic_string_view& assign(const val_type *first, const val_type *last) {
      return assign(first, last - first);
    }

    //
    constexpr const val_type& at(size_type index) const {
      ecp::check_empty_object(invalid());
      return Head[index];
    }
    //
    constexpr const val_type& operator[](size_type index) const {
      ecp::check_empty_object(invalid());
      return Head[index];
    }
    //
    constexpr const val_type& front() const {
      ecp::check_empty_object(invalid());
      return Head[0];
    }
    //
    constexpr const val_type& back() const {
      ecp::check_empty_object(invalid());
      return Head[Size - 1];
    }
    //
    constexpr citer begin() const {
      return Head;
    }
    //
    constexpr const iter end() const {
      return Head + Size;
    }
    //
    constexpr criter rbegin() const {
      return Head + Size - 1;
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
    constexpr const val_type *data() const {
      ecp::check_empty_object(invalid());
      return Head;
    }
    //
    constexpr bool invalid() const noexcept {
      return Head == nullptr || Size == 0;
    }
    //
    constexpr bool empty() const noexcept {
      return Size == 0;
    }
    //
    constexpr size_type size() const noexcept {
      return Size;
    }
    //
    constexpr void remove_prefix(size_type count) {
      Head += count;
    }
    //
    constexpr void remove_suffix(size_type count) {
      Size -= count;
    }
    //
    constexpr void swap(basic_string_view& ano) {
      swap(*this, ano);
    }
    //
    constexpr size_type copy(val_type *dest, size_type count, size_type index = 0) const {
      size_type ret = min(count, Size - index);
      batch::copy(dest, Head, ret);
      return ret;
    }
    //
    constexpr basic_string_view substr(size_type index = 0, size_type count = npos) const {
      return {Head + index, min(count, Size - index)};
    }
    //
    constexpr int_8 compare(const val_type *ptr) const {
      return compare(ptr, batch::length(ptr));
    }
    //
    constexpr int_8 compare(const basic_string_view& ano) const {
      return compare(ano.Head, ano.Size);
    }
    //
    constexpr int_8 compare(const val_type *ptr, size_type count) const {
      int_8 ret = batch::compare(Head, ptr, min(Size, count));
      if(ret != 0)
        return ret;
      if(Size < count)
        return -1;
      else if(Size > count)
        return 1;
      return 0;
    }
    //
    constexpr size_t find(const val_type *first, size_type index = 0) const noexcept {
      size_t ret = strstr(Head + index, Size - index, first);
      return ret == npos ? npos : ret + index;
    }
    // Sunday algorithm
    constexpr size_t find(val_type val, size_type index = 0) const noexcept {
      size_t ret = strchr(Head + index, Size - index, val);
      return ret == npos ? npos : ret + index;
    }
    //
    constexpr size_t find_first_not_of(val_type val, size_type index = 0) const noexcept {
      for(; index < Size; ++index)
        if(Head[index] != val)
          return index;
      return npos;
    }
    //
  protected:
    //
    const val_type *Head;
    size_type Size;
    //
  };
  //
}  // namespace xsl::str
namespace xsl::comp {
  template <>
  struct comp::traits<const char *> {
    using comp_category = std::strong_ordering;
    using comp_type = str::basic_string_view<char>;
  };
  //
  template <size_t Size>
  struct comp::traits<char[Size]> {
    using comp_category = std::strong_ordering;
    using comp_type = str::basic_string_view<char>;
  };
}  // namespace xsl::comp
//
namespace xsl::str {
  template <class Val>
  constexpr typename comp::traits<basic_string_view<Val>>::comp_category operator<=>(
    const basic_string_view<Val>& l, const basic_string_view<Val>& r) {
    return l.compare(r.data(), r.size()) <=> 0;
  }
  //
  template <class Val>
  constexpr typename comp::traits<basic_string_view<Val>>::comp_category operator==(
    const basic_string_view<Val>& l, const basic_string_view<Val>& r) {
    return l.compare(r.data(), r.size()) == 0;
  }

  template <class _Val, class _Alloc>
  class basic_string {
    //
  public:
    // clang-format off
  typedef _Val                                val_type;
  typedef _Alloc                              alloc_type;
  typedef size_t                              size_type;
  typedef itor::ptr_wrapper<val_type>           iter;
  typedef itor::ptr_wrapper<const val_type>     citer;
  typedef itor::reverse<val_type *>        riter;
  typedef itor::reverse<const val_type *>  criter;
  typedef std::strong_ordering                comp_category;
  typedef const basic_string&                 comp_type;
    // clang-format on
    //
  public:
    constexpr basic_string(tag_alloc, size_type count)
      : Alc()
      , Head(Alc.allocate((count + 1) * sizeof(val_type)))
      , Size()
      , Cap(count + 1){};
    constexpr basic_string()
      : basic_string(tag_alloc{}, 0) {
      Head[0] = val_type{};
    }
    //
    constexpr basic_string(size_type count)
      : basic_string(tag_alloc{}, count) {
      Size = count;
    }
    //
    constexpr basic_string(size_type count, val_type Val)
      : basic_string(tag_alloc{}, count) {
      this->Set(Head, count, Val);
    }
    //
    constexpr basic_string(const basic_string& ano, size_type index, size_type count)
      : basic_string(ano.Head + index, count > ano.Size - index ? ano.size : count) {
    }
    //
    template <class UCFIter, ts::enable<itor::is_forward<UCFIter>> = 0>
    constexpr basic_string(UCFIter first, size_type count)
      : basic_string(tag_alloc{}, count) {
      this->Copy(Head, itor::get_unwrapped(first), count);
    }
    //
    constexpr basic_string(const val_type *first)
      : basic_string(first, batch::length(first)) {
    }
    //
    template <class UCFIter, ts::enable<itor::is_forward<UCFIter>> = 0>
    constexpr basic_string(UCFIter first, UCFIter last)
      : basic_string(itor::get_unwrapped(first), batch::length(first, last)) {
    }
    //
    constexpr basic_string(const basic_string& ano)
      : basic_string(ano.Head, ano.Size) {
    }
    //
    constexpr basic_string(basic_string&& ano) noexcept
      : Alc(as_rreference(ano.Alc))
      , Head(ano.Head)
      , Cap(ano.Cap)
      , Size(ano.Size) {
      ano.Head = nullptr;
    }
    //
    // template<class Similar_String_View, enable<type::is_constructible<basic_string_view<val_type>,
    // type::_n<decay<Similar_String_View>>>> = 0>
    constexpr basic_string(const basic_string_view<val_type>& ano)
      : basic_string(ano.data(), ano.size()) {
    }
    //
    constexpr basic_string(const basic_string_view<val_type>& ano, size_type index, size_type count)
      : basic_string(ano.data() + index, count > ano.size() - index ? ano.size() : count) {
    }
    //
    constexpr ~basic_string() {
      if(!this->invalid()) {
        this->erase(Head, Size);
        Alc.deallocate(Head);
      }
    }
    //
    constexpr basic_string& operator=(const val_type *first) {
      return this->Assign(first, batch::length(first));
    }
    //
    constexpr basic_string& operator=(basic_string&& ano) noexcept {
      return this->assign(as_rreference(ano));
    }
    //
    constexpr basic_string& operator=(const basic_string& ano) {
      return this->Assign(ano.Head, ano.Size);
    }
    //
    constexpr basic_string& assign(size_type count, val_type Val = val_type{}) {
      return this->Assign(count, Val);
    }
    //
    constexpr basic_string& assign(const basic_string& ano) {
      return this->Assign(ano.Head, ano.Size);
    }
    //
    constexpr basic_string& assign(const basic_string& ano, size_type index, size_type count = npos) {
      return this->Assign(ano.Head + index, count == npos ? ano.Size : count);
    }
    //
    constexpr basic_string& assign(basic_string&& ano) {
      if(!this->invalid())
        Alc.deallocate(Head);
      Alc = as_rreference(ano.Alc);
      Head = ano.Head;
      Cap = ano.Cap;
      Size = ano.Size;
      ano.Head = nullptr;
      return *this;
    }
    //
    template <class UCFIter, ts::enable<itor::is_forward<UCFIter>> = 0>
    constexpr basic_string& assign(UCFIter first, size_type count) {
      return this->Assign(itor::get_unwrapped(first), count);
    }
    //
    constexpr basic_string& assign(const val_type *first) {
      return this->Assign(first, batch::length(first));
    }
    //
    template <class UCFIter, ts::enable<itor::is_forward<UCFIter>> = 0>
    constexpr basic_string& assign(UCFIter first, UCFIter last) {
      return this->Assign(itor::get_unwrapped(first), batch::length(first, last));
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
    constexpr const riter rbegin() const {
      return iter(Head + Size - 1);
    }
    //
    constexpr riter rend() {
      return Head - 1;
    }
    //
    constexpr criter rend() const {
      return Head - 1;
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
    constexpr const iter cbegin() const {
      return Head;
    }
    //
    constexpr citer cend() const {
      return Head + Size;
    }
    //
    constexpr const riter crbegin() const {
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
    constexpr bool empty() const noexcept {
      return Size == 0;
    }
    //
    constexpr size_type size() const noexcept {
      return Size;
    }
    //
    constexpr void reserve(size_type NewCap) {
      if(Cap < NewCap + 1)
        recap(NewCap + 1, 0, 0);
    }
    //
    constexpr size_type capacity() const {
      return Cap ? Cap - 1 : 0;
    }
    //
    constexpr void shrink_to_fit() {
      recap(Size + 1, 0, 0);
    }
    //
    constexpr void clear() {
      this->erase(Head, Size);
    }
    //
    constexpr iter insert(size_type index, size_type count, val_type Val = val_type{}) {
      return this->Set(Head + index, count, Val);
    }

    constexpr iter insert(size_type index, const val_type *first, size_type count = npos) {
      return this->Copy(Head + index, first, count == npos ? batch::length(first) : count);
    }
    //
    constexpr iter insert(size_type index, const basic_string& ano) {
      return this->Copy(Head + index, ano.Head, ano.Size);
    }
    //
    constexpr iter insert(size_type index, const basic_string& ano, size_type ins_index, size_type count = npos) {
      return this->Copy(Head + index, &ano[ins_index], count == npos ? ano.Size - ins_index : count);
    }
    //
    constexpr iter insert(citer where, val_type val) {
      return this->Set(where, 1, val);
    }
    //
    constexpr iter insert(citer where, size_type count, val_type val = val_type{}) {
      return this->Set(where, count, val);
    }
    //
    template <class Unchecked_Iter, ts::enable<itor::is<Unchecked_Iter>> = 0>
    constexpr iter insert(citer where, Unchecked_Iter first, size_type count) {
      return this->Copy(where, itor::get_unwrapped(first), count);
    }
    //
    constexpr iter insert(citer where, const val_type *ptr) {
      return this->Copy(where, ptr, batch::length(ptr));
    }
    //
    template <class Unchecked_Iter, ts::enable<itor::is<Unchecked_Iter>> = 0>
    constexpr iter insert(citer where, Unchecked_Iter first, Unchecked_Iter last) {
      return this->Copy(where, itor::get_unwrapped(first), batch::length(first, last));
    }
    //
    constexpr iter erase(size_type index = 0, size_type count = npos) {
      return this->erase(Head + index, count == npos ? Size : count);
    }
    //
    constexpr iter erase(citer where) {
      return this->erase(where, 1);
    }
    //
    constexpr iter erase(citer first, citer last) {
      return this->erase(const_cast<val_type *>((const val_type *)first), static_cast<size_type>(last - first));
    }
    //
    constexpr void push_back(val_type val) {
      return this->Set(Head + Size, 1, val);
    }
    //
    constexpr void pop_back() {
      return this->erase(Head + Size - 1, 1);
    }
    //
    constexpr basic_string& append(size_type count, val_type val) {
      ecp::check_empty_object(invalid());
      this->Set(Head + Size, count, val);
      return *this;
    }
    //
    constexpr basic_string& append(const basic_string& ano) {
      this->Copy(Head + Size, ano.Head, ano.Size);
      return *this;
    }
    //
    constexpr basic_string& append(const basic_string& ano, size_type index, size_type count = npos) {
      this->Copy(Head + Size, ano.Head + index, count == npos ? ano.Size : count);
      return *this;
    }
    //
    template <class Unchecked_Iter, ts::enable<itor::is<Unchecked_Iter>> = 0>
    constexpr basic_string& append(Unchecked_Iter first, Unchecked_Iter last) {
      this->Copy(Head + Size, itor::get_unwrapped(first), batch::length(first, last));
      return *this;
    }
    //
    template <class Unchecked_Iter, ts::enable<itor::is<Unchecked_Iter>> = 0>
    constexpr basic_string& append(Unchecked_Iter first, size_type count) {
      this->Copy(Head + Size, itor::get_unwrapped(first), count);
      return *this;
    }
    //
    constexpr basic_string& append(const val_type *ptr) {
      this->Copy(Head + Size, ptr, batch::length(ptr));
      return *this;
    }
    //
    constexpr basic_string& append(std::initializer_list<val_type> ilist) {
      this->Copy(Head + Size, ilist.begin(), ilist.size());
      return *this;
    }
    //
    constexpr basic_string& append(const basic_string_view<val_type>& ano) {
      this->Copy(Head + Size, ano.Head, ano.Size);
      return *this;
    }
    //
    constexpr basic_string& append(const basic_string_view<val_type>& ano, size_type index, size_type count) {
      this->Copy(Head + Size, ano.data() + index, count > ano.size() - index ? ano.size() : count);
      return *this;
    }
    //
    constexpr basic_string& operator+=(val_type val) {
      ecp::check_empty_object(invalid());
      this->Set(Head + Size, 1, val);
      return *this;
    }
    //
    constexpr basic_string& operator+=(const basic_string& ano) {
      this->Copy(Head + Size, ano.Head, ano.Size);
      return *this;
    }
    //
    constexpr basic_string& operator+=(const val_type *first) {
      this->Copy(Head + Size, first, batch::length(first));
      return *this;
    }
    //
    constexpr basic_string& operator+=(const basic_string_view<val_type> *vano) {
      this->Copy(Head + Size, vano->data(), vano->size());
      return *this;
    }
    //
    constexpr int_8 compare(const basic_string_view<val_type>& ano) const {
      return compare(ano.data(), ano.size());
    }
    //
    constexpr int_8 compare(const val_type *first) const {
      return compare(first, batch::length(first));
    }
    //
    constexpr int_8 compare(const val_type *first, size_type count) const {
      int_8 ret = batch::compare(Head, first, min(Size, count));
      if(ret != 0)
        return ret;
      if(Size < count)
        return -1;
      else if(Size > count)
        return 1;
      return 0;
    }
    //
    constexpr void resize(size_type NewSize, val_type Val = val_type{}) {
      if(Size < NewSize)
        this->Set(Head + Size, NewSize - Size, Val);
      else if(Size > NewSize)
        this->erase(Head + NewSize, Size - NewSize);
      Head[NewSize] = val_type{};
      Size = NewSize;
    }
    //
    constexpr size_t find(const val_type *first, size_type index = 0) const noexcept {
      return strstr(Head, Size, first);
    }
    // Sunday algorithm
    constexpr operator basic_string_view<val_type>() const {
      return {Head, Size};
    }
  protected:
    //
    // default_allocator<_Val, _SizeType> Alc;
    alloc_type Alc;
    val_type *Head;
    size_type Size, Cap;
    //
    template <class Checked_Iter>
    constexpr basic_string& Assign(Checked_Iter first, size_type count) {
      if(this->invalid()) {
        Head = Alc.allocate((count + 1) * sizeof(val_type));
        Cap = count + 1;
      } else
        erase(Head, Size);
      this->Copy(Head, first, count);
      return *this;
    }
    //
    template <class Creator>
    constexpr basic_string& Assign(size_type count, val_type val) {
      if(this->invalid()) {
        Head = Alc.allocate((count + 1) * sizeof(val_type));
        Cap = count + 1;
      } else
        erase(Head, Size);
      this->Set(Head, count, val);
      return *this;
    }
    //
    constexpr iter erase(val_type *where, size_type count) {
      MMove(where, where + count, (Head + Size + 1 - (where + count)) * sizeof(val_type));
      Size -= count;
      return where;
    }
    //
    template <class CIter>
    constexpr iter Copy(val_type *where, CIter first, size_type count) {
      ptrdiff_t index = where - Head;
      expand_with_moving(where, count);  // head maybe has changed
      batch::copy(Head + index, first, count);
      return Head + index + count;
    }
    //
    constexpr iter Set(val_type *where, size_type count, val_type val) {
      ptrdiff_t index = where - Head;
      expand_with_moving(where, count);  // head maybe has changed
      batch::create(Head + index, count, val);
      return Head + index + count;
    }
    //
    constexpr void expand_with_moving(val_type *where, size_type count) {
      if(size_type NewCap = Size + 1 + count; Cap < NewCap) {
        if(Cap == 0)
          Cap = NewCap;
        else
          while(Cap < NewCap)
            Cap <<= 1;
        recap(Cap, static_cast<size_type>(where - Head), count);
      } else {
        MMove(where + count, where, (Size - (where - Head)) * sizeof(val_type));
      }
      Size += count;
      Head[Size] = val_type{};
    }
    constexpr void recap(size_type NewCap, size_type first, size_type count) {
      val_type *NewHead = Alc.allocate(NewCap * sizeof(val_type));
      batch::copy(NewHead, Head, first);
      batch::copy(NewHead + first + count, Head + first, Size - first);
      Alc.deallocate(Head);
      Head = NewHead;
    }
  };

  template <class Val>
  constexpr basic_string<Val, alc::default_allocator<Val>> operator+(
    const basic_string_view<Val>& l, const basic_string_view<Val>& r) {
    basic_string<Val, alc::default_allocator<Val>> str = l;
    str += r;
    return as_rreference(str);
  }
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(const basic_string<Val, Alloc>& l, const basic_string_view<Val>& r) {
    basic_string<Val, Alloc> str = l;
    str += r;
    return as_rreference(str);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(const basic_string<Val, Alloc>& l, const basic_string<Val, Alloc>& r) {
    return as_rreference(basic_string<Val, Alloc>{l} += r);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(const basic_string<Val, Alloc>& l, const Val *r) {
    return as_rreference(basic_string<Val, Alloc>{l} += r);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(const basic_string<Val, Alloc>& l, Val val) {
    return as_rreference(basic_string<Val, Alloc>{l} += val);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(const Val *l, basic_string<Val, Alloc>& r) {
    return as_rreference(basic_string<Val, Alloc>{l} += r);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(Val val, const basic_string<Val, Alloc>& r) {
    return as_rreference(basic_string<Val, Alloc>{1, val} += r);
  }
  //
  template <class Val, class Alloc1, class Alloc2>
  constexpr basic_string<Val, Alloc1> operator+(basic_string<Val, Alloc1>&& l, basic_string<Val, Alloc2>&& r) {
    return as_rreference(l.capacity() > r.capacity() ? l += r : r += l);
  }
  //
  template <class Val, class Alloc1, class Alloc2>
  constexpr basic_string<Val, Alloc1> operator+(basic_string<Val, Alloc1>&& l, basic_string<Val, Alloc2>& r) {
    return as_rreference(l += r);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(basic_string<Val, Alloc>&& l, const Val *r) {
    return as_rreference(l += r);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(basic_string<Val, Alloc>&& l, Val val) {
    return as_rreference(l += val);
  }
  //
  template <class Val, class Alloc1, class Alloc2>
  constexpr basic_string<Val, Alloc1> operator+(const basic_string<Val, Alloc1>& l, basic_string<Val, Alloc2>&& r) {
    return as_rreference(r += l);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(const Val *l, basic_string<Val, Alloc>&& r) {
    return as_rreference(r += l);
  }
  //
  template <class Val, class Alloc>
  constexpr basic_string<Val, Alloc> operator+(Val val, basic_string<Val, Alloc>&& r) {
    return as_rreference(r += val);
  }
  //
  template <class Val, class Alloc = alc::default_allocator<Val>>
  constexpr basic_string<Val, Alloc> operator+(const basic_string_view<Val>& l, Val val) {
    return as_rreference(basic_string<Val, Alloc>{l} += val);
  }
  //
  template <class Val, class Alloc = alc::default_allocator<Val>>
  constexpr basic_string<Val, Alloc> operator+(Val val, const basic_string_view<Val>& r) {
    return as_rreference(basic_string<Val, Alloc>{1, val} += r);
  }
  //
  template <class Val, class Alloc1, class Alloc2>
  constexpr typename comp::traits<basic_string<Val, Alloc1>>::comp_category operator<=>(
    const basic_string<Val, Alloc1>& l, const basic_string<Val, Alloc2>& r) {
    return l.compare(r.data(), r.size()) <=> 0;
  }
  //
  template <class Val, class Alloc>
  constexpr typename comp::traits<basic_string<Val, Alloc>>::comp_category operator<=>(
    const basic_string<Val, Alloc>& l, const Val *r) {
    return l.compare(r) <=> 0;
  }
  //
  template <class Val, class Alloc>
  constexpr typename comp::traits<basic_string_view<Val>>::comp_category operator<=>(
    const basic_string_view<Val>& l, const basic_string<Val, Alloc>& r) {
    return l.compare(r.data(), r.size()) <=> 0;
  }
  //
  template <class Val, class Alloc1, class Alloc2>
  constexpr bool operator==(const basic_string<Val, Alloc1>& l, const basic_string<Val, Alloc2>& r) {
    return l.compare(r.data(), r.size()) == 0;
  }
  //
  template <class Val, class Alloc>
  constexpr bool operator==(const basic_string<Val, Alloc>& l, const Val *r) {
    return l.compare(r) == 0;
  }
}  // namespace xsl::str
#endif  // !XSL_BASIC_STRING
