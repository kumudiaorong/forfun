#pragma once
#ifndef XSL_LIST
#define XSL_LIST
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/batch.hpp>
#include <xsl/bits/compare.hpp>
#include <xsl/bits/container.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/iterator.hpp>
#include <xsl/bits/utility.hpp>
XSL_BEGIN
//
template <typename _Val, typename _Alloc = default_allocator<_Val>>
class list;

template <class _Val>
class list_node {
public:
  using val_type = _Val;
  // neighbor
  constexpr list_node()
    : Val()
    , Prev()
    , Next() {}
  //
  template <typename Arg, typename... Args, ts::enable<!ts::is_same<list_node, ts::rm_cvr<Arg>>> = 0>
  constexpr list_node(Arg&& param, Args&&...params)
    : Val(forward<Arg>(param), forward<Args>(params)...)
    , Prev()
    , Next() {}
  //
  constexpr list_node(const list_node& Ano)
    : Val(Ano.Val)
    , Prev(Ano.Prev)
    , Next(Ano.Next) {}
  //
  val_type Val;
  list_node *Prev, *Next;
};
template <class _List>
class list_iter;
template <template <class, class> class _List, class _Val, class _Alloc>
class list_iter<_List<_Val, _Alloc>> {
public:
  // clang-format on
  typedef _List<_Val, _Alloc> list_type;
  typedef typename list_type::val_type val_type;
  typedef typename list_type::node_type node_type;
  typedef typename list_type::size_type size_type;
  typedef list_iter mnr_type;
  // clang-format on
  constexpr list_iter(node_type *ptr)
    : Ptr(ptr) {}
  //
  constexpr list_iter(list_iter&&) = default;
  //
  constexpr list_iter(const list_iter&) = default;
  //
  constexpr list_iter& operator=(list_iter&&) = default;
  //
  constexpr list_iter& operator=(const list_iter&) = default;
  //
  constexpr val_type& operator*() const { return Ptr->Val; }
  //
  constexpr val_type *operator->() const { return &(Ptr->Val); }
  //
  constexpr bool operator==(const list_iter& Ano) const { return Ptr == Ano.Ptr; }
  //
  constexpr list_iter& operator++() {
    Ptr = Ptr->Next;
    return *this;
  }
  //
  constexpr list_iter& operator--() {
    Ptr = Ptr->Prev;
    return *this;
  }
private:
  friend class _List<_Val, _Alloc>;
  friend class list_iter<const _List<_Val, _Alloc>>;
  node_type *Ptr;
};
template <template <class, class> class _List, class _Val, class _Alloc>
class list_iter<const _List<_Val, _Alloc>> {
public:
  // clang-format on
  typedef _List<_Val, _Alloc> list_type;
  typedef typename list_type::val_type val_type;
  typedef typename list_type::node_type node_type;
  typedef typename list_type::size_type size_type;
  typedef list_iter mnr_type;
  // clang-format on
  constexpr list_iter(node_type *ptr)
    : Ptr(ptr) {}
  //
  constexpr list_iter(list_iter<list_type> ano)
    : Ptr(ano.Ptr) {}
  //
  constexpr list_iter(list_iter&&) = default;
  //
  constexpr list_iter(const list_iter&) = default;
  //
  constexpr list_iter& operator=(list_iter&&) = default;
  //
  constexpr list_iter& operator=(const list_iter&) = default;
  //
  constexpr const val_type& operator*() const { return Ptr->Val; }
  //
  constexpr const val_type *operator->() const { return &(Ptr->Val); }
  //
  constexpr bool operator==(const list_iter& Ano) const { return Ptr == Ano.Ptr; }
  //
  constexpr list_iter& operator++() {
    Ptr = Ptr->Next;
    return *this;
  }
  //
  constexpr list_iter& operator--() {
    Ptr = Ptr->Prev;
    return *this;
  }
private:
  friend class _List<_Val, _Alloc>;
  node_type *Ptr;
};

template <typename _Val, typename _Alloc>
class list {
public:
  // clang-format off
	typedef _Val 												val_type;
	typedef size_t 												size_type;
	typedef list_node<val_type> 								node_type;
	typedef typename _Alloc::template rebind_alloc<node_type>	alloc_type;
	typedef list_iter<list> 									iter;
	typedef list_iter<const list> 								citer;
	typedef reverse_iterator<iter> 								riter;
	typedef reverse_iterator<citer> 							criter;
  // clang-format on
  // #ifndef _DEBUG
  //
   // #define XSL_FILL_OPT_FOR_CONSTRUCTOR(ARG0,ARG1)					\
	constexpr list(size_type Count XSL_ADD_COMMA(ARG0)) : list() {	\
		this->Insert(Head, creator(Count XSL_ADD_COMMA(ARG1)));	\
	}
  // XSL_FILL_OPT(2, XSL_FILL_OPT_FOR_CONSTRUCTOR, const val_type& Val, Val)
  // #undef XSL_FILL_OPT_FOR_CONSTRUCTOR
  //
   // #define XSL_GROUP_TO_SAME_MCTR_FOR_CONSTRUCTOR(ARG0,ARG1)				\
	template<class Unchecked_Iter, enable<is_iter<Unchecked_Iter>> = 0>	\
	constexpr list(Unchecked_Iter First, ARG0) : list() {				\
		this->Insert(Head, creator(get_mnr_iter(First), ARG1));			\
	}
  // XSL_GROUP_TO_SAME_MCTR(2, XSL_GROUP_TO_SAME_MCTR_FOR_CONSTRUCTOR, Unchecked_Iter Last, size_type Count,
  // get_mnr_iter(Last), Count)
  // #undef XSL_GROUP_TO_SAME_MCTR_FOR_CONSTRUCTOR
  //
  // #else
  //
  constexpr list()
    : Alc()
    , Head(Alc.allocate(sizeof(node_type)))
    , Size() {
    Head->Prev = Head->Next = Head;
  }
  constexpr list(size_type Count)
    : list() {
    this->insert(Head, creator{Count});
  }
  //
  constexpr list(size_type Count, const val_type& Val)
    : list() {
    this->insert(Head, creator{Count, Val});
  }
  //
  template <class Unchecked_Iter, ts::enable<is_forward_iter<Unchecked_Iter>> = 0>
  constexpr list(Unchecked_Iter First, Unchecked_Iter Last)
    : list() {
    this->insert(Head, creator{get_mnr_iter(First), get_mnr_iter(Last)});
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr list(Unchecked_Iter First, size_type Count)
    : list() {
    this->insert(Head, creator{get_mnr_iter(First), Count});
  }
  //
  constexpr ~list() {
    if(!invalid()) {
      erase(condition{iter{Head->Next}, Size});
      Alc.deallocate(Head);
    }
  }
  //
  // #endif // _DEBUG
  constexpr list& operator=(list&& Ano) noexcept { return this->assign(as_rreference(Ano)); }
  constexpr list& operator=(const list& Ano) { return this->assign(creator{iter{Ano.Head->Next}, Ano.Size}); }
  //
  // #ifndef _DEBUG
  //
   // #define XSL_FILL_OPT_FOR_ASSIGN(ARG0,ARG1)						\
	constexpr list& assign(size_type Count XSL_ADD_COMMA(ARG0)) {	\
		return assign(creator(Count XSL_ADD_COMMA(ARG1)));		\
	}
  // XSL_FILL_OPT(2, XSL_FILL_OPT_FOR_ASSIGN, const val_type& Val, Val)
  // #undef XSL_FILL_OPT_FOR_ASSIGN
  //
   // #define XSL_GROUP_TO_SAME_MCTR_FOR_ASSIGN(ARG0,ARG1)					\
	template<class Unchecked_Iter, enable<is_iter<Unchecked_Iter>> = 0>	\
	constexpr list& assign(Unchecked_Iter First, ARG0) {				\
		return assign(creator(get_mnr_iter(First), ARG1));				\
	}
  // XSL_GROUP_TO_SAME_MCTR(2, XSL_GROUP_TO_SAME_MCTR_FOR_ASSIGN, Unchecked_Iter Last, size_type Count,
  // get_mnr_iter(Last), Count);
  // #undef XSL_GROUP_TO_SAME_MCTR_FOR_ASSIGN
  //
  // #else
  //
  constexpr list& assign(size_type Count) { return assign(creator{Count}); }
  //
  constexpr list& assign(size_type Count, const val_type& Val) { return assign(creator{Count, Val}); }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr list& assign(Unchecked_Iter First, Unchecked_Iter Last) {
    return assign(creator{get_mnr_iter(First), get_mnr_iter(Last)});
  }
  // Ring-linked list
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr list& assign(Unchecked_Iter First, size_type Count) {
    return assign(creator{get_mnr_iter(First), Count});
  }
  //
  // #endif // _DEBUG
  constexpr list& assign(list&& Ano) {
    if(!this->invalid()) {
      this->erase(condition{iter{Head->Next}, Size});
      Alc.deallocate(Head);
    }
    Alc = as_rreference(Ano.Alc);
    Head = Ano.Head;
    Size = Ano.Size;
    Ano.Head = nullptr;
    Ano.Size = 0;
    return *this;
  }
  //
  constexpr list& assign(const list& Ano) { return assign(creator{iter{Ano.Head->Next}, Ano.Size}); }
  //
  constexpr val_type& front() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Next->Val;
  }
  //
  constexpr const val_type& front() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Next->Val;
  }
  //
  constexpr val_type& back() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Prev->Val;
  }
  //
  constexpr const val_type& back() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Prev->Val;
  }
  //
  constexpr iter begin() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Next;
  }
  //
  constexpr citer begin() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Next;
  }
  //
  constexpr iter end() {
    XSL_EMPTY_CHECK(this->invalid());
    return Head;
  }
  //
  constexpr citer end() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head;
  }
  //
  constexpr riter rbegin() {
    XSL_EMPTY_CHECK(this->invalid());
    return iter{Head->Prev};
  }
  //
  constexpr criter rbegin() const {
    XSL_EMPTY_CHECK(this->invalid());
    return citer{Head->Prev};
  }
  //
  constexpr riter rend() {
    XSL_EMPTY_CHECK(this->invalid());
    return iter{Head};
  }
  //
  constexpr criter rend() const {
    XSL_EMPTY_CHECK(this->invalid());
    return citer{Head};
  }
  //
  // #endif//_DEBUG
  constexpr citer cbegin() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head->Next;
  }
  //
  constexpr citer cend() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Head;
  }
  //
  constexpr criter crbegin() const {
    XSL_EMPTY_CHECK(this->invalid());
    return citer{Head->Prev};
  }
  //
  constexpr criter crend() const {
    XSL_EMPTY_CHECK(this->invalid());
    return citer{Head};
  }
  //
  constexpr bool empty() const {
    XSL_EMPTY_CHECK(this->invalid());
    return Size == 0;
  }
  //
  constexpr bool invalid() const { return Head == nullptr; }
  //
  constexpr size_type size() const { return Size; }
  //
  constexpr void clear() {
    XSL_EMPTY_CHECK(this->invalid());
    this->erase(condition{iter{Head->Next}, Size});
  }
  //
  // #ifndef _DEBUG
  //
   // #define XSL_FILL_OPT_FOR_INSERT(ARG0,ARG1) \
	constexpr void insert(citer where, size_type Count XSL_ADD_COMMA(ARG0)) {	\
		this->Insert(where.GPtr.Ptr, creator(Count XSL_ADD_COMMA(ARG1)));		\
	}
  // XSL_FILL_OPT(2, XSL_FILL_OPT_FOR_INSERT, const val_type& Val, Val)
  // #undef XSL_FILL_OPT_FOR_INSERT
  //
   // #define XSL_GROUP_TO_SAME_MCTR_FOR_INSERT(ARG0,ARG1)						\
	template<class Unchecked_Iter, enable<is_iter<Unchecked_Iter>> = 0>		\
	constexpr void insert(citer where, Unchecked_Iter First, ARG0) {	\
		this->Insert(where.GPtr.Ptr, creator(get_mnr_iter(First), ARG1));	\
	}
  // XSL_GROUP_TO_SAME_MCTR(2, XSL_GROUP_TO_SAME_MCTR_FOR_INSERT, Unchecked_Iter Last, size_type Count,
  // get_mnr_iter(Last), Count)
  // #undef XSL_GROUP_TO_SAME_MCTR_FOR_INSERT
  //
  // #else
  //
  constexpr void insert(citer where, size_type Count) { this->insert(where.Ptr, creator{Count}); }
  //
  constexpr void insert(citer where, size_type count, const val_type& val) {
    this->insert(where.Ptr, creator{count, val});
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr void insert(citer where, Unchecked_Iter First, Unchecked_Iter Last) {
    this->insert(where.Ptr, creator{get_mnr_iter(First), get_mnr_iter(Last)});
  }
  //
  template <class Unchecked_Iter, ts::enable<is_iter<Unchecked_Iter>> = 0>
  constexpr void insert(citer where, Unchecked_Iter First, size_type Count) {
    this->insert(where.Ptr, creator{get_mnr_iter(First), Count});
  }
  //
  // #endif // _DEBUG
  //
  template <class... Args>
  constexpr void emplace(citer where, Args&&...params) {
    this->emplace(where.Ptr, forward<Args>(params)...);
  }
  //
  constexpr void remove(citer dest) { this->erase(condition{iter{dest.Ptr}, 1}); }
  //
  constexpr void erase(citer first, citer last) { this->erase(condition{iter{first.Ptr}, iter{last.Ptr}}); }
  //
  constexpr void erase(citer first, size_type count) { this->erase(condition{iter{first.Ptr}, count}); }
  //
  template <typename... Args>
  constexpr void emplace_back(Args&&...params) {
    this->emplace(Head, forward<Args>(params)...);
  }
  //
  constexpr void pop_back() { this->erase(condition{iter{Head->Prev}, 1}); }
  //
  template <typename... Args>
  constexpr void emplace_front(Args&&...params) {
    this->emplace(Head->Next, forward<Args>(params)...);
  }
  //
  constexpr void pop_front() { this->erase(condition{iter{Head->Next}, 1}); }
  //
  constexpr void resize(size_type newSize, const val_type& val) {
    if(Size < newSize)
      this->insert(Head, creator(newSize - Size, val));
    else if(Size > newSize)
      this->erase(condition{jump(iter{Head->Next}, newSize), Size - newSize});
  }
  //
  constexpr void resize(size_type newSize) {
    if(Size < newSize)
      this->insert(Head, creator(newSize - Size));
    else if(Size > newSize)
      this->erase(condition{jump(iter{Head->Next}, newSize), Size - newSize});
  }
  //
  // #endif // !_DEBUG
  //
  constexpr void swap(list& Ano) {
    XSL swap(Alc, Ano.Alc);
    XSL swap(Head, Ano.Head);
    XSL swap(Size, Ano.Size);
  }
  //
protected:
  // default_allocator<node_type> Alc;
  alloc_type Alc;
  node_type *Head;
  size_type Size;  // size
  //
  template <class Creator, ts::enable<is_xsl_creater<ts::rm_cvr<Creator>>> = 0>
  constexpr list& assign(Creator&& ctor) {
    if(this->invalid()) {
      Head = Alc.allocate(sizeof(node_type));
      this->connect(Head, Head);
    } else {
      size_type size = Size;
      if(ctor) insert(Head, as_rreference(ctor));
      this->erase(condition{iter{Head->Next}, size});
    }
    return *this;
  }
  template <class Creator>
  constexpr void insert(node_type *where, Creator&& ctor) {
    connect(create(where->Prev, ctor), where);
    Size += ctor.Count;
  }
  template <class... Args>
  constexpr void emplace(node_type *where, Args&&...params) {
    node_type *ptr = Alc.allocate(sizeof(node_type));
    construct_at(addr(ptr->Val), forward<Args>(params)...);
    connect(connect(where->Prev, ptr), where);
    ++Size;
  }
  template <class Creator>
  constexpr node_type *create(node_type *head, Creator& ctor) {
    while(ctor) {
      head = connect(head, Alc.allocate(sizeof(node_type)));
      ctor(addr(head->Val));
      ++ctor;
    }
    return head;
  }
  template <class Cond>
  constexpr void erase(Cond&& cond) {
    node_type *pre = cond.First.Ptr->Prev;
    while(cond) {
      ++cond;
      destroy_at(addr(cond.First.Ptr->Prev->Val));
      Alc.deallocate(cond.First.Ptr->Prev);
      --Size;
    }
    pre->Next = cond.First.Ptr;
    cond.First.Ptr->Prev = pre;
  }
  constexpr node_type *connect(node_type *Prev, node_type *Next) {
    Prev->Next = Next;
    Next->Prev = Prev;
    return Next;
  }
};

#ifdef XSL_TEST
//
void xsl_test_list() {
  TEST_SPLIT_LINE(test_p);
  TEST_ACT_PRINT(test_p test_ps[6]{}; *test_ps[0].Ptr = 1; *test_ps[1].Ptr = 2; *test_ps[2].Ptr = 3;
                 *test_ps[3].Ptr = 4; *test_ps[4].Ptr = 5; *test_ps[5].Ptr = 6);
  TEST_ACT_PRINT(list<test_p> ls00{});
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(list<test_p> ls01(test_ps, 1));
  TEST_RET_PRINT(ls01, print, ls01.size());
  TEST_ACT_PRINT(list<test_p> ls02(test_ps, test_ps + 2));
  TEST_RET_PRINT(ls02, print, ls02.size());
  TEST_ACT_PRINT(list<test_p> ls03(3));
  TEST_RET_PRINT(ls03, print, ls03.size());
  TEST_ACT_PRINT(list<test_p> ls04(3, test_ps[1]));
  TEST_RET_PRINT(ls04, print, ls04.size());
  TEST_ACT_PRINT(ls00 = as_rreference(ls01));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00 = ls02);
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.assign(as_rreference(ls03)));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.assign(4));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.assign(test_ps, 5));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.assign(test_ps, test_ps + 6));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.assign(7, test_ps[2]));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.assign(ls02));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((ls00.front(), ls00.back()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((as_const(ls00).front(), as_const(ls00).back()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((ls00.begin(), ls00.cbegin()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((as_const(ls00).begin(), as_const(ls00).cbegin()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((ls00.end(), ls00.cend()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((as_const(ls00).end(), as_const(ls00).cend()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((ls00.rbegin(), ls00.crbegin()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((as_const(ls00).rbegin(), as_const(ls00).crbegin()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((ls00.rend(), ls00.crend()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((as_const(ls00).rend(), as_const(ls00).crend()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((ls00.empty(), ls00.invalid()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT((as_const(ls00).empty(), as_const(ls00).invalid()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.clear());
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace(ls00.begin(), *test_ps));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace(ls00.begin(), as_rreference(*test_ps)));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.insert(ls00.begin(), 1, test_ps[1]));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.insert(ls00.begin(), 2));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.insert(ls00.begin(), test_ps + 1, test_ps + 2));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace(ls00.begin(), 1));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.erase(ls00.begin(), ++ ++ls00.begin()));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace_back(test_ps[1]));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace_back(as_rreference(test_ps[1])));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace_back(3));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.pop_back());
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace_front(test_ps[2]));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace_front(as_rreference(test_ps[2])));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.emplace_front(4));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.pop_front());
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.resize(ls00.size() - 3));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.resize(ls00.size() + 1));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.resize(ls00.size() + 1, *(test_ps + 3)));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_ACT_PRINT(ls00.swap(ls02));
  TEST_RET_PRINT(ls00, print, ls00.size());
  TEST_RET_PRINT(ls00, rprint, ls00.size());
}
#endif
//
XSL_END
#endif  // !XSL_LIST