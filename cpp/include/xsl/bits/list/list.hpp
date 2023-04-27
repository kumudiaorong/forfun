#pragma once
#include "xsl/bits/utility.hpp"
#ifndef XSL_LIST_SUPPORT
#define XSL_LIST_SUPPORT
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/batch.hpp>
#include <xsl/bits/exception.hpp>
#include <xsl/bits/list/lsm.hpp>
#include <xsl/bits/node.hpp>
#include <xsl/bits/pair.hpp>
#include <xsl/bits/ts/is.hpp>
#include <xsl/bits/ts/rm.hpp>
// #include <xsl/bits/compare.hpp>
// #include <xsl/bits/def.hpp>
#include <xsl/bits/iterator.hpp>
// #include <xsl/bits/utility.hpp>
namespace xsl {
  namespace ls {
    //
    template <typename _Val, typename _Alloc>
    class list;

    template <class _Val>
    class list_node {
    public:
      //clang-format off
      typedef _Val val_type;
      //clang-format on
      val_type Val;
      list_node *Next[2];

      template <class Alloc>
      static list_node *Alc(Alloc& alc) {
        list_node *ptr = alc.allocate(sizeof(list_node));
        ptr->Next[0] = ptr->Next[1] = ptr->Next[2] = nullptr;
        return ptr;
      }
      template <class Alloc, class... Args>
      static list_node *New(Alloc& alc, Args&&...args) {
        list_node *ptr = alc.allocate(sizeof(list_node));
        try {
          construct_at(addr(ptr->Val), forward<Args>(args)...);
        } catch(ecp::exception& e) {
          alc.deallocate(ptr);
          throw e;
        }
        ptr->Next[0] = ptr->Next[1] = nullptr;
        return ptr;
      }

      // neighbor
      constexpr list_node()
        : Val()
        , Next() {
      }
      //
      constexpr list_node(const list_node& ano) = default;
      //
      constexpr list_node *& prev() {
        return this->Next[0];
      }
      //
      constexpr list_node *& next() {
        return this->Next[1];
      }
      //
      constexpr val_type& operator*() {
        return this->Val;
      }
      //
      constexpr const val_type& operator*() const {
        return this->Val;
      }
      //
    };
    template <class _List>
    class list_iter;
    template <template <class, class> class _List, class _Node, class _Alloc>
    class list_iter<_List<_Node, _Alloc>> {
    public:
      // clang-format off
    typedef _List<_Node, _Alloc>          list_type;
    typedef typename list_type::val_type  val_type;
    typedef typename list_type::node_type node_type;
    typedef typename list_type::size_type size_type;
      // clang-format on
      constexpr list_iter(node_type *ptr)
        : Ptr(ptr) {
      }
      //
      constexpr list_iter(list_iter&&) = default;
      //
      constexpr list_iter(const list_iter&) = default;
      //
      constexpr list_iter& operator=(list_iter&&) = default;
      //
      constexpr list_iter& operator=(const list_iter&) = default;
      //
      constexpr val_type& operator*() const {
        return **Ptr;
      }
      //
      constexpr val_type *operator->() const {
        return addr(**Ptr);
      }
      //
      constexpr bool operator==(const list_iter& Ano) const {
        return Ptr == Ano.Ptr;
      }
      //
      constexpr list_iter& operator++() {
        Ptr = Ptr->next();
        return *this;
      }
      //
      constexpr list_iter& operator--() {
        Ptr = Ptr->prev();
        return *this;
      }
      //
    private:
      friend class _List<_Node, _Alloc>;
      friend class list_iter<const _List<_Node, _Alloc>>;
      friend void iter_swap(const list_iter&, const list_iter&);
      node_type *Ptr;
    };
  }  // namespace ls
  template <class List>
  void iter_swap(const ls::list_iter<List> l, const ls::list_iter<List> r) {
    xsl::swap(l.Ptr->prev()->next(), r.Ptr->prev()->next());
    xsl::swap(l.Ptr->next()->prev(), r.Ptr->next()->prev());
    xsl::swap(l.Ptr->prev(), r.Ptr->prev());
    xsl::swap(l.Ptr->next(), r.Ptr->next());
  }
}  // namespace xsl
namespace xsl::ls {
  template <template <class, class> class _List, class _Node, class _Alloc>
  class list_iter<const _List<_Node, _Alloc>> {
  public:
    // clang-format off
    typedef _List<_Node, _Alloc>          list_type;
    typedef typename list_type::val_type  val_type;
    typedef typename list_type::node_type node_type;
    typedef typename list_type::size_type size_type;
    // clang-format on
    constexpr list_iter(node_type *ptr)
      : Ptr(ptr) {
    }
    //
    constexpr list_iter(list_iter<list_type> ano)
      : Ptr(ano.Ptr) {
    }
    //
    constexpr list_iter(list_iter&&) = default;
    //
    constexpr list_iter(const list_iter&) = default;
    //
    constexpr list_iter& operator=(list_iter&&) = default;
    //
    constexpr list_iter& operator=(const list_iter&) = default;
    //
    constexpr const val_type& operator*() const {
      return **Ptr;
    }
    //
    constexpr const val_type *operator->() const {
      return &**Ptr;
    }
    //
    constexpr bool operator==(const list_iter& Ano) const {
      return Ptr == Ano.Ptr;
    }
    //
    constexpr list_iter& operator++() {
      Ptr = Ptr->next();
      return *this;
    }
    //
    constexpr list_iter& operator--() {
      Ptr = Ptr->prev();
      return *this;
    }
  private:
    friend class _List<_Node, _Alloc>;
    node_type *Ptr;
  };

  template <typename _Val, typename _Alloc>
  class list {
  public:
    // clang-format off
  	typedef _Val 												                      val_type;
  	typedef size_t 												                    size_type;
  	typedef list_node<val_type> 								              node_type;
  	typedef typename _Alloc::template rebind_alloc<node_type>	alloc_type;
  	typedef lsm<node_type> 							                      mng_type;
  	typedef list_iter<list> 								                  iter;
	  typedef list_iter<const list> 								            citer;
	  typedef itor::reverse<iter> 								              riter;
	  typedef itor::reverse<citer> 							                criter;
    // clang-format on
  protected:
    // clang-format off
    typedef pair<node_type*, size_type>      list_decription_type;//head, size
    // clang-format on
    alloc_type Alc;
    mng_type Mng;
    size_type Size;
  public:
    constexpr list()
      : Alc()
      , Mng(Alc.allocate(sizeof(node_type)))
      , Size() {
    }
    constexpr list(size_type count)
      : list() {
      this->Insert(Mng.Head, Create(count));
    }
    //
    constexpr list(size_type count, const val_type& val)
      : list() {
      this->Insert(Mng.Head, Create(count, val));
    }
    //
    template <class UCIter, ts::enable<itor::is<UCIter>> = 0>
    constexpr list(UCIter first, UCIter last)
      : list() {
      this->Insert(Mng.Head, Create(itor::get_unwrapped(first), itor::get_unwrapped(last)));
    }
    //
    template <class UCIter, ts::enable<itor::is<UCIter>> = 0>
    constexpr list(UCIter first, size_type count)
      : list() {
      this->Insert(Mng.Head, Create(itor::get_unwrapped(first), count));
    }
    //
    constexpr list(const list& ano)
      : list() {
      this->Insert(Mng.Head, Create(ano.begin(), ano.Size));
    }
    //
    constexpr list(list&& ano)
      : Alc(as_rreference(ano.Alc))
      , Mng(as_rreference(ano.Mng))
      , Size(ano.Size) {
      ano.Mng.Head = nullptr;
    }
    //
    constexpr ~list() {
      if(!invalid()) {
        Erase(Mng.Head->next(), Mng.Head);
        Alc.deallocate(Mng.Head);
      }
    }
    //
    // #endif // _DEBUG
    constexpr list& operator=(list&& ano) noexcept {
      return this->assign(as_rreference(ano));
    }
    constexpr list& operator=(const list& ano) {
      return Assign(Create(ano.begin(), ano.Size));
    }
    //
    constexpr list& assign(size_type count) {
      return Assign(Create(count));
    }
    //
    constexpr list& assign(size_type count, const val_type& val) {
      return Assign(Create(count, val));
    }
    //
    template <class UCIter, ts::enable<itor::is<UCIter>> = 0>
    constexpr list& assign(UCIter first, UCIter last) {
      return Assign(Create(itor::get_unwrapped(first), itor::get_unwrapped(last)));
    }
    // Ring-linked list
    template <class UCIter, ts::enable<itor::is<UCIter>> = 0>
    constexpr list& assign(UCIter first, size_type count) {
      return Assign(Create(itor::get_unwrapped(first), count));
    }
    //
    // #endif // _DEBUG
    constexpr list& assign(list&& ano) {
      if(!this->invalid()) {
        this->Erase(Mng.Head->next(), Mng.Head);
        Alc.deallocate(Mng.Head);
      }
      Alc = as_rreference(ano.Alc);
      Mng = as_rreference(ano.Mng);
      Size = ano.Size;
      return *this;
    }
    //
    constexpr list& assign(const list& ano) {
      return Assign(Create(ano.begin(), ano.Size));
    }
    //
    constexpr val_type& front() {
      ecp::check_empty_object(invalid());
      return Mng.Head->next()->Val;
    }
    //
    constexpr const val_type& front() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->next()->Val;
    }
    //
    constexpr val_type& back() {
      ecp::check_empty_object(invalid());
      return Mng.Head->prev()->Val;
    }
    //
    constexpr const val_type& back() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->prev()->Val;
    }
    //
    constexpr iter begin() {
      ecp::check_empty_object(invalid());
      return Mng.Head->next();
    }
    //
    constexpr citer begin() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->next();
    }
    //
    constexpr iter end() {
      ecp::check_empty_object(invalid());
      return Mng.Head;
    }
    //
    constexpr citer end() const {
      ecp::check_empty_object(invalid());
      return Mng.Head;
    }
    //
    constexpr riter rbegin() {
      ecp::check_empty_object(invalid());
      return iter{Mng.Head->prev()};
    }
    //
    constexpr criter rbegin() const {
      ecp::check_empty_object(invalid());
      return citer{Mng.Head->prev()};
    }
    //
    constexpr riter rend() {
      ecp::check_empty_object(invalid());
      return iter{Mng.Head};
    }
    //
    constexpr criter rend() const {
      ecp::check_empty_object(invalid());
      return citer{Mng.Head};
    }
    //
    // #endif//_DEBUG
    constexpr citer cbegin() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->next();
    }
    //
    constexpr citer cend() const {
      ecp::check_empty_object(invalid());
      return Mng.Head;
    }
    //
    constexpr criter crbegin() const {
      ecp::check_empty_object(invalid());
      return citer{Mng.Head->prev()};
    }
    //
    constexpr criter crend() const {
      ecp::check_empty_object(invalid());
      return citer{Mng.Head};
    }
    //
    constexpr bool empty() const {
      ecp::check_empty_object(invalid());
      return Size == 0;
    }
    //
    constexpr bool invalid() const {
      return Mng.Head == nullptr;
    }
    //
    constexpr size_type size() const {
      return Size;
    }
    //
    constexpr void clear() {
      ecp::check_empty_object(invalid());
      this->erase(Mng.Head->next(), Mng.Head);
    }
    //
    constexpr void insert(citer where, size_type count) {
      this->Insert(where.Ptr, Create(count));
    }
    //
    constexpr void insert(citer where, size_type count, const val_type& val) {
      this->Insert(where.Ptr, Create(count, val));
    }
    //
    template <class UCIter, ts::enable<itor::is<UCIter>> = 0>
    constexpr void insert(citer where, UCIter first, UCIter last) {
      this->Insert(where.Ptr, Create(itor::get_unwrapped(first), itor::get_unwrapped(last)));
    }
    //
    template <class UCIter, ts::enable<itor::is<UCIter>> = 0>
    constexpr void insert(citer where, UCIter first, size_type count) {
      this->Insert(where.Ptr, Create(itor::get_unwrapped(first), count));
    }
    //
    // #endif // _DEBUG
    //
    template <class... Args>
    constexpr void emplace(citer where, Args&&...params) {
      this->Emplace(where.Ptr, forward<Args>(params)...);
    }
    //
    constexpr citer remove(citer dest) {
      return this->Remove(dest.Ptr);
    }
    //
    constexpr node_type *erase(citer first, citer last) {
      return this->Erase(first.Ptr, last.Ptr);
    }
    //
    template <typename... Args>
    constexpr void emplace_back(Args&&...params) {
      this->Emplace(Mng.Head, forward<Args>(params)...);
    }
    //
    constexpr void pop_back() {
      this->Remove(Mng.Head->prev());
    }
    //
    template <typename... Args>
    constexpr void emplace_front(Args&&...params) {
      this->Emplace(Mng.Head->next(), forward<Args>(params)...);
    }
    //
    constexpr void pop_front() {
      this->Remove(Mng.Head->next());
    }
    //
    constexpr void resize(size_type newSize, const val_type& val) {
      if(Size < newSize)
        this->Insert(Mng.Head, Create(newSize - Size, val));
      else if(Size > newSize)
        this->Erase(batch::jump(citer{Mng.Head->next()}, newSize).Ptr, Mng.Head);
    }
    //
    constexpr void resize(size_type newSize) {
      if(Size < newSize)
        this->Insert(Mng.Head, Create(newSize - Size));
      else if(Size > newSize)
        this->Erase(batch::jump(citer{Mng.Head->next()}, newSize).Ptr, Mng.Head);
    }
    //
    constexpr void swap(list& ano) {
      xsl::swap(Alc, ano.Alc);
      xsl::swap(Size, ano.Size);
      Mng.swap(ano.Mng);
    }
    //
  // protected:
    //
    constexpr list& Assign(const list_decription_type& l) {
      if(invalid()) {
        safe_throw([this] { Mng.reuse(Alc.allocate(sizeof(node_type))); }, l.first, l.first->prev());
      } else {
        Erase(Mng.Head->next(), Mng.Head);
      }
      Insert(Mng.Head, l);
      return *this;
    }
    constexpr void Insert(node_type *where, const list_decription_type& l) {
      auto [h, s] = l;
      Mng.insert(where, h, h->prev());
      Size += s;
    }
    template <class... Args>
    constexpr void Emplace(node_type *where, Args&&...params) {
      Mng.insert(where, node_type::New(Alc, forward<Args>(params)...));
      ++Size;
    }
    template <class CIter>
    constexpr list_decription_type Create(CIter first, CIter last) {
      node_type *head = node_type::New(Alc, *first);
      node_type *tail = head;
      ++first;
      size_type count{1};
      while(first != last) {
        safe_throw([this, &first, &tail] { tail = Mng.connect(tail, node_type::New(Alc, *first)); }, head, nullptr);
        ++first;
        ++count;
      }
      head->prev() = tail;
      return {head, count};
    }
    template <class CIter>
    constexpr list_decription_type Create(CIter first, size_type count) {
      node_type *head = node_type::New(Alc, *first);
      node_type *tail = head;
      ++first;
      size_type cond = count;
      --cond;
      while(cond) {
        safe_throw([this, &first, &tail] { tail = Mng.connect(tail, node_type::New(Alc, *first)); }, head, nullptr);
        ++first;
        --cond;
      }
      head->prev() = tail;
      return {head, count};
    }
    constexpr list_decription_type Create(size_type count) {
      node_type *head = node_type::New(Alc);
      node_type *tail = head;
      size_type cond = count;
      --cond;
      while(cond) {
        safe_throw([this, &tail] { tail = Mng.connect(tail, node_type::New(Alc)); }, head, nullptr);
        --cond;
      }
      head->prev() = tail;
      return {head, count};
    }
    constexpr list_decription_type Create(size_type count, const val_type& val) {
      node_type *head = node_type::New(Alc, val);
      node_type *tail = head;
      size_type cond = count;
      --cond;
      while(cond) {
        safe_throw([this, &val, &tail] { tail = Mng.connect(tail, node_type::New(Alc, val)); }, head, nullptr);
        --cond;
      }
      head->prev() = tail;
      return {head, count};
    }
    constexpr size_type Destroy(node_type *first, node_type *last) {
      size_type c = 0;
      while(first != last) {
        node_type *dp = first;
        first = first->next();
        Alc.deallocate(destruct_at(addr(*dp)));
        ++c;
      }
      return c;
    }
    constexpr node_type *Erase(node_type *first, node_type *last) {
      node_type *pre = first->prev();
      Size -= this->Destroy(first, last);
      return Mng.connect(pre, last);
    }
    constexpr node_type *Remove(node_type *where) {
      node_type *nex = Mng.remove(where);
      destruct_at(addr(where->Val));
      Alc.deallocate(where);
      --Size;
      return nex;
    }
    template <class Fn>
    constexpr void safe_throw(Fn&& fn, node_type *first, node_type *last) {
      try {
        fn();
      } catch(ecp::exception e) {
        this->Destroy(first, last);
        throw e;
      }
    }
  };
}  // namespace xsl::ls
#endif  // !XSL_LIST