#pragma once
#ifndef XSL_CONTAINER
#define XSL_CONTAINER

#include <xsl/bits/iter.hpp>
#include <xsl/bits/allocator.hpp>
XSL_BEGIN

struct tag_alloc {};

const static size_t npos = static_cast<size_t>(-1);

class counter {
public:
  counter(size_t count = 0)
    : Count(count) {}
  constexpr void add(size_t count = 1) { Count += count; }
  size_t Count;
};

template <uint_8 Index, class T>
class condition;

template <>
class condition<0, void> {
public:
  constexpr condition(size_t count)
    : Size(count) {}
  constexpr operator bool() const { return Size > 0; }
  constexpr condition& operator++() {
    --Size;
    return *this;
  }
  size_t Size;
};

template <class Iter>
class condition<1, Iter> {
public:
  condition(const Iter& first, size_t count)
    : First(first)
    , Size(count) {}
  constexpr operator bool() const { return Size > 0; }
  constexpr condition& operator++() {
    ++First;
    --Size;
    return *this;
  }
  Iter First;
  size_t Size;
};

template <class Iter>
class condition<2, Iter> {
public:
  constexpr condition(const Iter& first, const Iter& last)
    : First(first)
    , Last(last) {}
  constexpr operator bool() const { return First != Last; }
  constexpr condition& operator++() {
    ++First;
    return *this;
  }
  Iter First, Last;
};

condition(size_t)->condition<0, void>;
template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
condition(UCIter, size_t) -> condition<1, ts::decay<UCIter>>;
template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
condition(UCIter, UCIter) -> condition<2, ts::decay<UCIter>>;

template <class T>
inline constexpr bool is_xsl_condition = false;

template <uint_8 Index, class... Ts>
inline constexpr bool is_xsl_condition<condition<Index, Ts...>> = true;

template <class T>
inline constexpr bool is_condition = is_xsl_condition<T>;

template <uint_8 Index, class T>
class creator;

template <>
class creator<0, void>
  : public condition<0, void>
  , public counter {
public:
  typedef condition<0, void> cond_type;
  using cond_type::cond_type;
  template <class Wrap>
  constexpr Wrap *operator()(Wrap *Ptr) {
    this->add();
    return construct_at(Ptr);
  }
};

template <class Val>
class creator<1, Val>
  : public condition<0, void>
  , public counter {
public:
  typedef condition<0, void> cond_type;
  typedef Val val_type;
  creator(size_t count, const val_type& ref)
    : cond_type(count)
    , Ref(ref) {}
  template <class Wrap = val_type>
  Wrap *operator()(Wrap *where) {
    this->add();
    return construct_at(where, Ref);
  }
  const val_type& Ref;
};

template <class Iter>
class creator<2, Iter>
  : public condition<1, Iter>
  , public counter {
public:
  typedef condition<1, Iter> cond_type;
  using cond_type::cond_type;
  template <class Wrap>
  Wrap *operator()(Wrap *Ptr) {
    this->add();
    return construct_at(Ptr, *this->First);
  }
};
template <class Iter>
class creator<3, Iter>
  : public condition<2, Iter>
  , public counter {
public:
  typedef condition<2, Iter> cond_type;
  using cond_type::cond_type;
  template <class Wrap>
  Wrap *operator()(Wrap *where) {
    this->add();
    return construct_at(where, *this->First);
  }
};

creator(size_t)->creator<0, void>;
template <class T>
creator(size_t, T) -> creator<1, ts::decay<T>>;
template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
creator(UCIter, size_t) -> creator<2, ts::decay<UCIter>>;
template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
creator(UCIter, UCIter) -> creator<3, ts::decay<UCIter>>;

template <class T>
inline constexpr bool is_xsl_creater = false;

template <uint_8 Index, class T>
inline constexpr bool is_xsl_creater<creator<Index, T>> = true;

template <class T>
inline constexpr bool is_creater = is_xsl_creater<T>;

template <class T, class Creator, ts::enable<is_xsl_creater<ts::rm_ref<Creator>>> = 0>
void create_n(T *first, Creator&& ctor) {
  while(ctor) {
    ctor(first);
    ++first;
    ++ctor;
  }
}
template <class CIter, class Creator, ts::enable<!ts::is::pointer<CIter> && is_creater<ts::rm_ref<Creator>>> = 0>
void create_n(CIter first, Creator&& ctor) {
  while(ctor) {
    ctor(addr(*first));
    ++first;
    ++ctor;
  }
}
template <class T, class Cond, ts::enable<is_condition<ts::rm_ref<Cond>>> = 0>
void destroy_n(T *first, Cond&& cond) {
  while(cond) {
    destruct_at(first);
    ++first;
    ++cond;
  }
}
template <class CIter, class Cond, ts::enable<!ts::is::pointer<CIter> && is_condition<ts::rm_ref<Cond>>> = 0>
void destroy_n(CIter first, Cond&& cond) {
  while(cond) {
    destruct_at(addr(*first));
    ++first;
    ++cond;
  }
}
XSL_END
#endif  // XSL_ALLOCATOR