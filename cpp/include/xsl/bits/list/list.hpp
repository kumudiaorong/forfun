#pragma once
#ifndef XSL_LIST_SUPPORT
#define XSL_LIST_SUPPORT
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/batch.hpp>
#include <xsl/bits/list/lsm.hpp>
#include <xsl/bits/ts/is.hpp>
#include <xsl/bits/ts/rm.hpp>
#include <xsl/bits/tuple/tuple.hpp>

// #include <xsl/bits/compare.hpp>
// #include <xsl/bits/def.hpp>
#include <xsl/bits/iterator.hpp>
// #include <xsl/bits/utility.hpp>
namespace xsl::ls {
  //
  template <typename _Val, typename _Alloc>
  class list;

  template <class _Val>
  class list_node {
  public:
    using val_type = _Val;
    // neighbor
    constexpr list_node()
      : Val()
      , Prev()
      , Next() {
    }
    //
    template <typename Arg, typename... Args, ts::enable_construct<list_node, Arg> = 0>
    constexpr list_node(Arg&& param, Args&&...params)
      : Val(forward<Arg>(param), forward<Args>(params)...)
      , Prev()
      , Next() {
    }
    //
    constexpr list_node(const list_node& ano)
      : Val(ano.Val)
      , Prev(ano.Prev)
      , Next(ano.Next) {
    }
    //
    constexpr val_type& operator*() {
      return Val;
    }
    //
    constexpr const val_type& operator*() const {
      return Val;
    }
    //
    val_type Val;
    list_node *Prev, *Next;
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
      Ptr = Ptr->Next;
      return *this;
    }
    //
    constexpr list_iter& operator--() {
      Ptr = Ptr->Prev;
      return *this;
    }
  private:
    friend class _List<_Node, _Alloc>;
    friend class list_iter<const _List<_Node, _Alloc>>;
    node_type *Ptr;
  };
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
      Ptr = Ptr->Next;
      return *this;
    }
    //
    constexpr list_iter& operator--() {
      Ptr = Ptr->Prev;
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
  typedef tpl::tuple<node_type*,node_type*,size_type>      list_description_type;
    // clang-format on
  protected:
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
        Erase(Mng.Head->Next, Mng.Head);
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
        this->Erase(Mng.Head->Next, Mng.Head);
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
      return Mng.Head->Next->Val;
    }
    //
    constexpr const val_type& front() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->Next->Val;
    }
    //
    constexpr val_type& back() {
      ecp::check_empty_object(invalid());
      return Mng.Head->Prev->Val;
    }
    //
    constexpr const val_type& back() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->Prev->Val;
    }
    //
    constexpr iter begin() {
      ecp::check_empty_object(invalid());
      return Mng.Head->Next;
    }
    //
    constexpr citer begin() const {
      ecp::check_empty_object(invalid());
      return Mng.Head->Next;
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
      return iter{Mng.Head->Prev};
    }
    //
    constexpr criter rbegin() const {
      ecp::check_empty_object(invalid());
      return citer{Mng.Head->Prev};
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
      return Mng.Head->Next;
    }
    //
    constexpr citer cend() const {
      ecp::check_empty_object(invalid());
      return Mng.Head;
    }
    //
    constexpr criter crbegin() const {
      ecp::check_empty_object(invalid());
      return citer{Mng.Head->Prev};
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
      this->erase(Mng.Head->Next, Mng.Head);
    }
    //
    // #ifndef _DEBUG
    //
     // #define XSL_FILL_OPT_FOR_INSERT(ARG0,ARG1) \
	constexpr void insert(citer where, size_type count XSL_ADD_COMMA(ARG0)) {	\
		this->Insert(where.GPtr.Ptr, creator(count XSL_ADD_COMMA(ARG1)));		\
	}
    // XSL_FILL_OPT(2, XSL_FILL_OPT_FOR_INSERT, const val_type& Val, Val)
    // #undef XSL_FILL_OPT_FOR_INSERT
    //
     // #define XSL_GROUP_TO_SAME_MCTR_FOR_INSERT(ARG0,ARG1)						\
	template<class UCIter, enable<itor::is<UCIter>> = 0>		\
	constexpr void insert(citer where, UCIter first, ARG0) {	\
		this->Insert(where.GPtr.Ptr, creator(itor::get_unwrapped(first), ARG1));	\
	}
    // XSL_GROUP_TO_SAME_MCTR(2, XSL_GROUP_TO_SAME_MCTR_FOR_INSERT, UCIter last, size_type count,
    // itor::get_unwrapped(last), count)
    // #undef XSL_GROUP_TO_SAME_MCTR_FOR_INSERT
    //
    // #else
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
      this->Remove(Mng.Head->Prev);
    }
    //
    template <typename... Args>
    constexpr void emplace_front(Args&&...params) {
      this->Emplace(Mng.Head->Next, forward<Args>(params)...);
    }
    //
    constexpr void pop_front() {
      this->Remove(Mng.Head->Next);
    }
    //
    constexpr void resize(size_type newSize, const val_type& val) {
      if(Size < newSize)
        this->Insert(Mng.Head, Create(newSize - Size, val));
      else if(Size > newSize)
        this->Erase(batch::jump(citer{Mng.Head->Next}, newSize).Ptr, Mng.Head);
    }
    //
    constexpr void resize(size_type newSize) {
      if(Size < newSize)
        this->Insert(Mng.Head, Create(newSize - Size));
      else if(Size > newSize)
        this->Erase(batch::jump(citer{Mng.Head->Next}, newSize).Ptr, Mng.Head);
    }
    //
    // #endif // !_DEBUG
    //
    constexpr void swap(list& ano) {
      xsl::swap(Alc, ano.Alc);
      xsl::swap(Size, ano.Size);
      Mng.swap(ano.Mng);
    }
    //
  protected:
    //
    constexpr list& Assign(const list_description_type& ldt) {
      if(invalid()) {
        Mng.reuse(Alc.allocate(sizeof(node_type)));
      } else {
        Erase(Mng.Head->Next, Mng.Head);
      }
      Insert(Mng.Head, ldt);
      return *this;
    }
    constexpr void Insert(node_type *where, const list_description_type& ldt) {
      auto [h, t, s] = ldt;
      Mng.insert(where, h, t);
      Size += s;
    }
    template <class... Args>
    constexpr void Emplace(node_type *where, Args&&...params) {
      node_type *ptr = Alc.allocate(sizeof(node_type));
      construct_at(addr(ptr->Val), forward<Args>(params)...);
      Mng.insert(where, ptr);
      ++Size;
    }
    template <class CIter>
    constexpr list_description_type Create(CIter first, CIter last) {
      node_type *head = construct_at(Alc.allocate(sizeof(node_type)), *first);
      node_type *tail = head;
      ++first;
      size_type count{1};
      while(first != last) {
        tail = Mng.connect(tail, construct_at(Alc.allocate(sizeof(node_type)), *first));
        ++first;
        ++count;
      }
      return {head, tail, count};
    }
    template <class CIter>
    constexpr list_description_type Create(CIter first, size_type count) {
      node_type *head = construct_at(Alc.allocate(sizeof(node_type)), *first);
      node_type *tail = head;
      ++first;
      size_type cond = count;
      --cond;
      while(cond) {
        tail = Mng.connect(tail, construct_at(Alc.allocate(sizeof(node_type)), *first));
        ++first;
        --cond;
      }
      return {head, tail, count};
    }
    constexpr list_description_type Create(size_type count) {
      node_type *head = construct_at(Alc.allocate(sizeof(node_type)));
      node_type *tail = head;
      size_type cond = count;
      --cond;
      while(cond) {
        tail = Mng.connect(tail, construct_at(Alc.allocate(sizeof(node_type))));
        --cond;
      }
      return {head, tail, count};
    }
    constexpr list_description_type Create(size_type count, const val_type& val) {
      node_type *head = construct_at(Alc.allocate(sizeof(node_type)), val);
      node_type *tail = head;
      size_type cond = count;
      --cond;
      while(cond) {
        tail = Mng.connect(tail, construct_at(Alc.allocate(sizeof(node_type)), val));
        --cond;
      }
      return {head, tail, count};
    }
    constexpr node_type *Erase(node_type *first, node_type *last) {
      node_type *pre = first->Prev;
      while(first != last) {
        first = first->Next;
        destruct_at(addr(first->Prev->Val));
        Alc.deallocate(first->Prev);
        --Size;
      }
      return Mng.connect(pre, last);
    }
    constexpr node_type *Remove(node_type *where) {
      node_type *nex = Mng.remove(where);
      destruct_at(addr(where->Val));
      Alc.deallocate(where);
      --Size;
      return nex;
    }
  };

#ifdef XSL_TEST
  //

#endif
  //
}  // namespace xsl::ls
#endif  // !XSL_LIST