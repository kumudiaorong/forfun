#pragma once
#ifndef XSL_BASIC_STRING
#define XSL_BASIC_STRING
#include <initializer_list>
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/batch.hpp>
#include <xsl/bits/compare.hpp>
#include <xsl/bits/container.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/iter.hpp>
#include <xsl/bits/pf/memory.hpp>
#include <xsl/bits/utility.hpp>

XSL_BEGIN

template <class Val>
constexpr size_t strchr(const Val *str, size_t len, Val end) noexcept {
  size_t i = 0;
  for(; i < len; ++i)
    if(str[i] == end) break;
  return i == len ? npos : i;
}
//
template <class Val>
constexpr size_t strchr(const Val *str, Val end) noexcept {
  size_t i = 0;
  for(; str[i] != Val{}; ++i)
    if(str[i] == end) break;
  return str[i] == Val{} ? npos : i;
}
//
template <class Val>
constexpr size_t strstr(const Val *str, const Val *substr, size_t slen) noexcept {
  size_t i = 0;
  while(true) {
    size_t m = 0;
    for(size_t n = i; m < slen; ++m, ++n)
      if(str[n] == Val{})
        return npos;
      else if(str[n] != substr[m])
        break;
    if(m != slen) {
      Val c = str[i + slen];
      for(m = slen; m > 0; --m)
        if(substr[m - 1] == c) break;
      i += slen - m + 1;
    } else
      break;
  }
  return i;
}
template <class Val>
constexpr size_t strstr(const Val *str, const Val *substr) noexcept {
  return strstr(str, substr, batch::length(substr));
}
template <class Val>
constexpr size_t strstr(const Val *str, size_t len, const Val *substr, size_t slen) noexcept {
  size_t i = 0;
  for(; i <= len - slen;) {
    size_t m = 0;
    for(size_t n = i; m < slen; ++m, ++n)
      if(str[n] != substr[m]) break;
    if(m != slen) {
      Val c = str[i + slen];
      for(m = slen; m > 0; --m)
        if(substr[m - 1] == c) break;
      i += slen - m + 1;
    } else
      break;
  }
  return i > len - slen ? npos : i;
  // Sunday algorithm
}
template <class Val>
constexpr size_t strstr(const Val *str, size_t len, const Val *substr) noexcept {
  return strstr(str, len, substr, batch::length(substr));
}
//
template <class _Val>
class basic_string_view {
  //
public:
  typedef _Val val_type;
  typedef size_t size_type;
  typedef pointer_wrapper<const val_type> iter;
  typedef iter citer;
  typedef reverse_iterator<iter> riter;
  typedef riter criter;
  typedef std::strong_ordering comp_category;
  typedef basic_string_view comp_type;
  //
  constexpr basic_string_view()
    : Head()
    , Size() {}
  //
  constexpr basic_string_view(const basic_string_view& ano, size_type index, size_type count = npos)
    : basic_string_view(ano.Head + index, count == npos ? ano.size : count) {}
  //
  template <class Unchecked_Iter, class Size_T,
    ts::enable<is_forward_iter<ts::rm_cvr<Unchecked_Iter>> && is_random_iter<ts::rm_cvr<Unchecked_Iter>>
               && ts::has::opeartor_simple_assignment<size_t, Size_T>> = 0>
  constexpr basic_string_view(Unchecked_Iter first, Size_T count)
    : Head(XSL addr(*first))
    , Size(count) {}
  //
  constexpr basic_string_view(const val_type *first)
    : basic_string_view(first, batch::length(first)) {}
  //
  template <class Unchecked_Iter,
    ts::enable<is_forward_iter<ts::rm_cvr<Unchecked_Iter>> && is_random_iter<ts::rm_cvr<Unchecked_Iter>>> = 0>
  constexpr basic_string_view(Unchecked_Iter first, Unchecked_Iter last)
    : basic_string_view(XSL addr(*first), last - first) {}
  //
  constexpr basic_string_view(const basic_string_view&) = default;
  //
  constexpr basic_string_view(basic_string_view&&) = default;
  //
  constexpr basic_string_view& operator=(const val_type *first) { return assign(first, batch::length(first)); }
  //
  constexpr basic_string_view& operator=(basic_string_view&& ano) noexcept { return assign(ano.Head, ano.Size); }
  //
  constexpr basic_string_view& operator=(const basic_string_view& ano) { return assign(ano.Head, ano.Size); }
  //
  constexpr basic_string_view& assign(const basic_string_view& ano) { return assign(ano.Head, ano.Size); }
  //
  constexpr basic_string_view& assign(const basic_string_view& ano, size_type index, size_type count = npos) {
    return assign(ano.Head + index, count == npos ? ano.Size : count);
  }
  //
  constexpr basic_string_view& assign(basic_string_view&& ano) { return assign(ano.Head, ano.Size); }
  //
  template <class Unchecked_Iter,
    ts::enable<is_forward_iter<ts::rm_cvr<Unchecked_Iter>> && is_random_iter<ts::rm_cvr<Unchecked_Iter>>> = 0>
  constexpr basic_string_view& assign(Unchecked_Iter first, size_type count) {
    Head = XSL addr(*first);
    Size = count;
    return *this;
  }
  //
  constexpr basic_string_view& assign(const val_type *first) { return assign(first, Len<size_type>(first)); }
  //
  template <class Unchecked_Iter,
    ts::enable<is_forward_iter<ts::rm_cvr<Unchecked_Iter>> && is_random_iter<ts::rm_cvr<Unchecked_Iter>>> = 0>
  constexpr basic_string_view& assign(Unchecked_Iter first, Unchecked_Iter last) {
    return assign(XSL addr(*first), last - first);
  }

  //
  constexpr const val_type& at(size_type index) const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[index];
  }
  //
  constexpr const val_type& operator[](size_type index) const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[index];
  }
  //
  constexpr const val_type& front() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[0];
  }
  //
  constexpr const val_type& back() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head[Size - 1];
  }
  //
  constexpr citer begin() const { return Head; }
  //
  constexpr const iter end() const { return Head + Size; }
  //
  constexpr criter rbegin() const { return Head + Size - 1; }
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
  constexpr const val_type *data() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head;
  }
  //
  constexpr bool invalid() const noexcept { return Head == nullptr || Size == 0; }
  //
  constexpr bool empty() const noexcept { return Size == 0; }
  //
  constexpr size_type size() const noexcept { return Size; }
  //
  constexpr void remove_prefix(size_type count) { Head += count; }
  //
  constexpr void remove_suffix(size_type count) { Size -= count; }
  //
  constexpr void swap(basic_string_view& ano) { XSL swap(*this, ano); }
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
  constexpr int_8 compare(const val_type *ptr) const { return compare(ptr, batch::length(ptr)); }
  //
  constexpr int_8 compare(const basic_string_view& ano) const { return compare(ano.Head, ano.Size); }
  //
  constexpr int_8 compare(const val_type *ptr, size_type count) const {
    int_8 ret = batch::compare(Head, ptr, min(Size, count));
    if(ret != 0) return ret;
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
      if(Head[index] != val) return index;
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
template <class T>
struct comp_traits<const T *> {
  using comp_category = std::strong_ordering;
  using comp_type = basic_string_view<T>;
};
//
template <class T, size_t Size>
struct comp_traits<T[Size]> {
  using comp_category = std::strong_ordering;
  using comp_type = basic_string_view<T>;
};
//
template <class Val>
constexpr typename comp_traits<basic_string_view<Val>>::comp_category operator<=>(
  const basic_string_view<Val>& l, const basic_string_view<Val>& r) {
  return l.compare(r.data(), r.size()) <=> 0;
}
//
template <class Val>
constexpr typename comp_traits<basic_string_view<Val>>::comp_category operator==(
  const basic_string_view<Val>& l, const basic_string_view<Val>& r) {
  return l.compare(r.data(), r.size()) == 0;
}

template <class _Val, class _Alloc = default_allocator<_Val>>
class basic_string {
  //
public:
  // clang-format off
  typedef _Val                                val_type;
  typedef _Alloc                              alloc_type;
  typedef size_t                              size_type;
  typedef pointer_wrapper<val_type>           iter;
  typedef pointer_wrapper<const val_type>     citer;
  typedef reverse_iterator<val_type *>        riter;
  typedef reverse_iterator<const val_type *>  criter;
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
    this->insert(Head, creator(count, Val));
  }
  //
  constexpr basic_string(const basic_string& ano, size_type index, size_type count)
    : basic_string(ano.Head + index, count > ano.Size - index ? ano.size : count) {}
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr basic_string(UCFIter first, size_type count)
    : basic_string(tag_alloc{}, count) {
    this->insert(Head, get_mnr_iter(first), count);
  }
  //
  constexpr basic_string(const val_type *first)
    : basic_string(first, batch::length(first)) {}
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr basic_string(UCFIter first, UCFIter last)
    : basic_string(get_mnr_iter(first), batch::length(first, last)) {}
  //
  constexpr basic_string(const basic_string& ano)
    : basic_string(ano.Head, ano.Size) {}
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
    : basic_string(ano.data(), ano.size()) {}
  //
  constexpr basic_string(const basic_string_view<val_type>& ano, size_type index, size_type count)
    : basic_string(ano.data() + index, count > ano.size() - index ? ano.size() : count) {}
  //
  constexpr ~basic_string() {
    if(!this->invalid()) {
      this->erase(Head, Size);
      Alc.deallocate(Head);
    }
  }
  //
  constexpr basic_string& operator=(const val_type *first) { return this->Assign(first, batch::length(first)); }
  //
  constexpr basic_string& operator=(basic_string&& ano) noexcept { return this->assign(as_rreference(ano)); }
  //
  constexpr basic_string& operator=(const basic_string& ano) { return this->Assign(ano.Head, ano.Size); }
  //
  constexpr basic_string& assign(size_type count, val_type Val = val_type{}) {
    return this->Assign(creator(count, Val));
  }
  //
  constexpr basic_string& assign(const basic_string& ano) { return this->Assign(ano.Head, ano.Size); }
  //
  constexpr basic_string& assign(const basic_string& ano, size_type index, size_type count = npos) {
    return this->Assign(ano.Head + index, count == npos ? ano.Size : count);
  }
  //
  constexpr basic_string& assign(basic_string&& ano) {
    if(!this->invalid()) Alc.deallocate(Head);
    Alc = as_rreference(ano.Alc);
    Head = ano.Head;
    Cap = ano.Cap;
    Size = ano.Size;
    ano.Head = nullptr;
    return *this;
  }
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr basic_string& assign(UCFIter first, size_type count) {
    return this->Assign(get_mnr_iter(first), count);
  }
  //
  constexpr basic_string& assign(const val_type *first) { return this->Assign(first, batch::length(first)); }
  //
  template <class UCFIter, ts::enable<is_forward_iter<UCFIter>> = 0>
  constexpr basic_string& assign(UCFIter first, UCFIter last) {
    return this->Assign(get_mnr_iter(first), batch::length(first, last));
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
  constexpr const riter rbegin() const { return iter(Head + Size - 1); }
  //
  constexpr riter rend() { return Head - 1; }
  //
  constexpr criter rend() const { return Head - 1; }
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
  constexpr const iter cbegin() const { return Head; }
  //
  constexpr citer cend() const { return Head + Size; }
  //
  constexpr const riter crbegin() const { return Head + Size - 1; }
  //
  constexpr criter crend() const { return Head - 1; }
  //
  constexpr bool invalid() const noexcept { return Head == nullptr; }
  //
  constexpr bool empty() const noexcept { return Size == 0; }
  //
  constexpr size_type size() const noexcept { return Size; }
  //
  constexpr void reserve(size_type NewCap) {
    if(Cap < NewCap + 1) recap(NewCap + 1, 0, 0);
  }
  //
  constexpr size_type capacity() const { return Cap ? Cap - 1 : 0; }
  //
  constexpr void shrink_to_fit() { recap(Size + 1, 0, 0); }
  //
  constexpr void clear() { this->erase(Head, Size); }
  //
  constexpr iter insert(size_type index, size_type count, val_type Val = val_type{}) {
    return this->insert(Head + index, creator(count, Val));
  }

  constexpr iter insert(size_type index, const val_type *first, size_type count = npos) {
    return this->insert(Head + index, first, count == npos ? batch::length(first) : count);
  }
  //
  constexpr iter insert(size_type index, const basic_string& ano) {
    return this->insert(Head + index, ano.Head, ano.Size);
  }
  //
  constexpr iter insert(size_type index, const basic_string& ano, size_type ins_index, size_type count = npos) {
    return this->insert(Head + index, &ano[ins_index], count == npos ? ano.Size - ins_index : count);
  }
  //
  constexpr iter insert(citer where, val_type Val) { return this->insert(where, &Val, 1); }
  //
  constexpr iter insert(citer where, size_type count, val_type val = val_type{}) {
    return this->insert(where, creator(count, val));
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr iter insert(citer where, Unchecked_Iter first, size_type count) {
    return this->insert(where, get_mnr_iter(first), count);
  }
  //
  constexpr iter insert(citer where, const val_type *ptr) { return this->insert(where, ptr, batch::length(ptr)); }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr iter insert(citer where, Unchecked_Iter first, Unchecked_Iter last) {
    return this->insert(where, get_mnr_iter(first), batch::length(first, last));
  }
  //
  constexpr iter erase(size_type index = 0, size_type count = npos) {
    return this->erase(Head + index, count == npos ? Size : count);
  }
  //
  constexpr iter erase(citer where) { return this->erase(where, 1); }
  //
  constexpr iter erase(citer first, citer last) {
    return this->erase(const_cast<val_type *>((const val_type *)first), static_cast<size_type>(last - first));
  }
  //
  constexpr void push_back(val_type Val) { return this->insert(Head + Size, &Val, 1); }
  //
  constexpr void pop_back() { return this->erase(Head + Size - 1, 1); }
  //
  constexpr basic_string& append(size_type count, val_type Val) {
    XSL_EMPTY_CHECK(this->invalid());
    this->insert(Head + Size, creator(count, Val));
    return *this;
  }
  //
  constexpr basic_string& append(const basic_string& ano) {
    this->insert(Head + Size, ano.Head, ano.Size);
    return *this;
  }
  //
  constexpr basic_string& append(const basic_string& ano, size_type index, size_type count = npos) {
    this->insert(Head + Size, ano.Head + index, count == npos ? ano.Size : count);
    return *this;
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr basic_string& append(Unchecked_Iter first, Unchecked_Iter last) {
    this->insert(Head + Size, get_mnr_iter(first), batch::length(first, last));
    return *this;
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr basic_string& append(Unchecked_Iter first, size_type count) {
    this->insert(Head + Size, get_mnr_iter(first), count);
    return *this;
  }
  //
  constexpr basic_string& append(const val_type *ptr) {
    this->insert(Head + Size, ptr, batch::length(ptr));
    return *this;
  }
  //
  constexpr basic_string& append(std::initializer_list<val_type> ilist) {
    this->insert(Head + Size, ilist.begin(), ilist.size());
    return *this;
  }
  //
  constexpr basic_string& append(const basic_string_view<val_type>& ano) {
    this->insert(Head + Size, ano.Head, ano.Size);
    return *this;
  }
  //
  constexpr basic_string& append(const basic_string_view<val_type>& ano, size_type index, size_type count) {
    this->insert(Head + Size, ano.data() + index, count > ano.size() - index ? ano.size() : count);
    return *this;
  }
  //
  constexpr basic_string& operator+=(val_type Val) {
    XSL_EMPTY_CHECK(this->invalid());
    this->insert(Head + Size, &Val, 1);
    return *this;
  }
  //
  constexpr basic_string& operator+=(const basic_string& ano) {
    this->insert(Head + Size, ano.Head, ano.Size);
    return *this;
  }
  //
  constexpr basic_string& operator+=(const val_type *first) {
    this->insert(Head + Size, first, batch::length(first));
    return *this;
  }
  //
  constexpr basic_string& operator+=(const basic_string_view<val_type> *vano) {
    this->insert(Head + Size, vano->data(), vano->size());
    return *this;
  }
  //
  constexpr int_8 compare(const basic_string_view<val_type>& ano) const { return compare(ano.data(), ano.size()); }
  //
  constexpr int_8 compare(const val_type *first) const { return compare(first, batch::length(first)); }
  //
  constexpr int_8 compare(const val_type *first, size_type count) const {
    int_8 ret = batch::compare(Head, first, min(Size, count));
    if(ret != 0) return ret;
    if(Size < count)
      return -1;
    else if(Size > count)
      return 1;
    return 0;
  }
  //
  constexpr void resize(size_type NewSize, val_type Val = val_type{}) {
    if(Size < NewSize)
      this->insert(Head + Size, creator(NewSize - Size, Val));
    else if(Size > NewSize)
      this->erase(Head + NewSize, Size - NewSize);
    Head[NewSize] = val_type{};
    Size = NewSize;
  }
  //
  constexpr size_t find(const val_type *first, size_type index = 0) const noexcept { return strstr(Head, Size, first); }
  // Sunday algorithm
  constexpr operator basic_string_view<val_type>() const { return {Head, Size}; }
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
    this->insert(Head, first, count);
    return *this;
  }
  //
  template <class Creator>
  constexpr basic_string& Assign(Creator&& Ctor) {
    if(this->invalid()) {
      Head = Alc.allocate((Ctor.count + 1) * sizeof(val_type));
      Cap = Ctor.count + 1;
    } else
      erase(Head, Size);
    this->insert(Head, Ctor);
    return *this;
  }
  //
  constexpr iter erase(val_type *where, size_type count) {
    MMove(where, where + count, (Head + Size + 1 - (where + count)) * sizeof(val_type));
    Size -= count;
    return where;
  }
  //
  template <class Creator>
  constexpr val_type *insert(val_type *where, Creator&& ctor) {
    expand_with_moving(where, ctor.Size);
    create_n(where, ctor);
    return Head + ctor.Count;
  }
  //
  template <class CIter>
  constexpr iter insert(val_type *where, CIter first, size_type count) {
    if constexpr(!ts::is::pointer<CIter>) return this->insert(where, creator(first, count));
    ptrdiff_t index = where - Head;
    expand_with_moving(where, count);  // head maybe has changed
    batch::copy(Head + index, first, count);
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

using string = basic_string<char>;

template <class Val>
constexpr basic_string<Val> operator+(const basic_string_view<Val>& l, const basic_string_view<Val>& r) {
  basic_string<Val> str = l;
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
template <class Val, class Alloc = default_allocator<Val>>
constexpr basic_string<Val, Alloc> operator+(const basic_string_view<Val>& l, Val val) {
  return as_rreference(basic_string<Val, Alloc>{l} += val);
}
//
template <class Val, class Alloc = default_allocator<Val>>
constexpr basic_string<Val, Alloc> operator+(Val val, const basic_string_view<Val>& r) {
  return as_rreference(basic_string<Val, Alloc>{1, val} += r);
}
//
template <class Val, class Alloc1, class Alloc2>
constexpr typename comp_traits<basic_string<Val, Alloc1>>::comp_category operator<=>(
  const basic_string<Val, Alloc1>& l, const basic_string<Val, Alloc2>& r) {
  return l.compare(r.data(), r.size()) <=> 0;
}
//
template <class Val, class Alloc>
constexpr typename comp_traits<basic_string<Val, Alloc>>::comp_category operator<=>(
  const basic_string<Val, Alloc>& l, const Val *r) {
  return l.compare(r) <=> 0;
}
//
template <class Val, class Alloc>
constexpr typename comp_traits<basic_string_view<Val>>::comp_category operator<=>(
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
#ifdef XSL_TEST
#include <xsl/bits/test.hpp>
void xsl_test_basic_string() {
  TEST_SPLIT_LINE(char);
  TEST_ACT_PRINT(char cstr00[] = "123"; char cstr01[] = "123456");
  TEST_ACT_PRINT(basic_string<char> str00{});
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(basic_string<char> str01(cstr00));
  TEST_RET_PRINT(str01, xsl::test::print, str01.size());
  TEST_ACT_PRINT(basic_string<char> str02(cstr01, 6));
  TEST_RET_PRINT(str02, xsl::test::print, str02.size());
  TEST_ACT_PRINT(basic_string<char> str03{as_rreference(str01)});
  TEST_RET_PRINT(str03, xsl::test::print, str03.size());
  TEST_ACT_PRINT(basic_string<char> str04{str02});
  TEST_RET_PRINT(str04, xsl::test::print, str04.size());
  TEST_ACT_PRINT(basic_string<char> str05(3, '1'));
  TEST_RET_PRINT(str05, xsl::test::print, str05.size());
  TEST_ACT_PRINT(str00 = cstr01);
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = str03);
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = as_rreference(str02));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.assign(cstr00));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.assign(str04));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.assign(as_rreference(str03)));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.assign(cstr01 + 1, 4));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.append({'6'}));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.append(cstr00, 1));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.resize(7, '7'));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str04.append(cstr00));
  TEST_RET_PRINT(str04, xsl::test::print, str04.size());
  TEST_ACT_PRINT(str00.append(str05));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00.erase(--str00.end(), str00.end()));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = str00 + str04);
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = str00 + cstr00);
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = str00 + 'a');
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = as_rreference(str00) + as_rreference(str04));
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = as_rreference(str00) + cstr00);
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_ACT_PRINT(str00 = as_rreference(str00) + 'b');
  TEST_RET_PRINT(str00, xsl::test::print, str00.size());
  TEST_RET_PRINT(str00, xsl::test::rprint, str00.size());
  // TEST_SPLIT_LINE(char16_t);
  // TEST_ACT_PRINT(char16_t cwstr00[] = L"123"; char16_t cwstr01[] = L"123456");
  // TEST_ACT_PRINT(basic_string<char16_t> wstr00{});
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(basic_string<char16_t> wstr01(cwstr00));
  // TEST_RET_PRINT(wstr01, xsl::test::print, wstr01.size());
  // TEST_ACT_PRINT(basic_string<char16_t> wstr02(cwstr01, 6));
  // TEST_RET_PRINT(wstr02, xsl::test::print, wstr02.size());
  // TEST_ACT_PRINT(basic_string<char16_t> wstr03{as_rreference(wstr01)});
  // TEST_RET_PRINT(wstr03, xsl::test::print, wstr03.size());
  // TEST_ACT_PRINT(basic_string<char16_t> wstr04{wstr02});
  // TEST_RET_PRINT(wstr04, xsl::test::print, wstr04.size());
  // TEST_ACT_PRINT(basic_string<char16_t> wstr05(3, '1'));
  // TEST_RET_PRINT(wstr05, xsl::test::print, wstr05.size());
  // TEST_ACT_PRINT(wstr00 = cwstr01);
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = wstr03);
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = as_rreference(wstr02));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00.assign(cwstr00));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00.assign(wstr04));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00.assign(as_rreference(wstr03)));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00.assign(cwstr01 + 1, 4));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00.append({'6'}));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00.resize(7, '7'));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = wstr00 + wstr04);
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = wstr00 + cwstr00);
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = wstr00 + L'a');
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = as_rreference(wstr00) + as_rreference(wstr04));
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = as_rreference(wstr00) + cwstr00);
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_ACT_PRINT(wstr00 = as_rreference(wstr00) + L'b');
  // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
  // TEST_RET_PRINT(wstr00, xsl::test::rprint, wstr00.size());
}
#endif  // XSL_TEST

XSL_END
#endif  // !XSL_BASIC_STRING
