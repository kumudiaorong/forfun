#pragma once
#ifndef XSL_LIST_IMPL
#define XSL_LIST_IMPL
#include "xsl_allocator.hpp"
#include "xsl_batch.hpp"
#include "xsl_container.hpp"
#include "xsl_iterator.hpp"
#include "xsl_pair.hpp"
#include "xsl_utility.hpp"
XSL_BEGIN
//
namespace impl {
  template <typename _Node, typename _Alloc = default_allocator<_Node>>
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

  template <typename _Node, typename _Alloc>
  class list {
  public:
    // clang-format off
	typedef _Node 						                    node_type;
	typedef typename node_type::val_type 						        val_type;
	typedef size_t 										    size_type;
	typedef typename _Alloc::template rebind_alloc<_Node>   alloc_type;
	typedef list_iter<list> 							    iter;
	typedef list_iter<const list> 						    citer;
	typedef reverse_iterator<iter> 						    riter;
	typedef reverse_iterator<citer> 					    criter;
    // clang-format on
    constexpr list(alloc_type& alc, node_type *head)
      : Alc(alc)
      , Head(head)
      , Size() {
      Head->Prev = Head->Next = Head;
    }
    constexpr ~list() {
      if(!invalid()) {
        erase(condition{iter{Head->Next}, Size});
        Alc.deallocate(Head);
      }
    }
    //
    constexpr list& operator=(list&& Ano) noexcept { return this->assign(as_rreference(Ano)); }
    //
    constexpr list& operator=(const list& Ano) { return this->assign(creator{iter{Ano.Head->Next}, Ano.Size}); }
    //
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
    constexpr void remove(citer dest) { this->erase(condition{iter{dest.Ptr}, 1}); }
    //
    constexpr void erase(citer first, citer last) { this->erase(condition{iter{first.Ptr}, iter{last.Ptr}}); }
    //
    constexpr void erase(citer first, size_type count) { this->erase(condition{iter{first.Ptr}, count}); }
    //
    constexpr void push_back(node_type *n) { this->connect(this->connect(Head->Prev, n), Head); }
    //
    constexpr void push_front() { this->erase(condition{iter{Head->Next}, 1}); }
    //
    constexpr void pop_back() { this->erase(condition{iter{Head->Prev}, 1}); }
    //
    constexpr void pop_front() { this->erase(condition{iter{Head->Next}, 1}); }
    //
    constexpr void resize(size_type newSize) {
      if(Size < newSize)
        this->insert(Head, creator(newSize - Size));
      else if(Size > newSize)
        this->erase(condition{jump(iter{Head->Next}, newSize), Size - newSize});
    }
    //
    constexpr void swap(list& Ano) {
      XSL swap(Alc, Ano.Alc);
      XSL swap(Head, Ano.Head);
      XSL swap(Size, Ano.Size);
    }
    //
    // default_allocator<node_type> Alc;
    alloc_type& Alc;
    node_type *Head;
    size_type Size;  // size
    //
    template <class Creator>
    constexpr void insert(node_type *where, Creator&& ctor) {
      connect(create(where->Prev, ctor), where);
      Size += ctor.Count;
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
        destruct_at(cond.First.Ptr->Prev);
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
}  // namespace impl
XSL_END
#endif  // !XSL_LIST_IMPL