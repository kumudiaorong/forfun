#pragma once
#ifndef XSL_LIST_MANAGER
#define XSL_LIST_MANAGER
#include <xsl/bits/def.hpp>
//
namespace xsl::ls {
  template <typename _Node>
  class lsm {  // list manager
  public:
    // clang-format off
	typedef _Node 						                    node_type;
	typedef typename node_type::val_type 						        val_type;
	typedef size_t 										    size_type;
    // clang-format on
    constexpr lsm(node_type *head = nullptr)
      : Head(head) {
      Head->Prev = Head->Next = Head;
    }
    constexpr lsm(lsm&& ano)
      : Head(ano.Head) {
      ano.Head = nullptr;
    }
    //
    constexpr lsm& operator=(lsm&& ano) noexcept {
      return this->assign(as_rreference(ano));
    }
    //
    // constexpr lsm& operator=(const lsm& Ano) { return this->assign(creator{iter{Ano.Head->Next}, Ano.Size}); }
    //
    constexpr lsm& assign(lsm&& ano) {
      Head = ano.Head;
      ano.Head = nullptr;
      return *this;
    }
    //
    // constexpr lsm& assign(const lsm& Ano) { return assign(creator{iter{Ano.Head->Next}, Ano.Size}); }
    //
    //
    constexpr bool invalid() const {
      return Head == nullptr;
    }
    //
    constexpr void reuse(node_type *head) {
      Head->Prev = Head->Next = Head;
    }
    //
    constexpr node_type *clear() {
      XSL_EMPTY_CHECK(this->invalid());
      if(Head->Next != Head) {
        Head->Prev->Next = nullptr;
        node_type *first = Head->Next;
        Head->Prev = Head->Next = Head;
        return first;
      }
      return nullptr;
    }
    //
    constexpr node_type *remove(node_type *ptr) {
      node_type *nex = ptr->Next;
      this->connect(ptr->Prev, ptr->Next);
      return nex;
    }
    //
    constexpr void erase(node_type *first, node_type *last) {
      this->connect(first->Prev, last);
    }
    //
    constexpr void push_back(node_type *n) {
      this->connect(this->connect(Head->Prev, n), Head);
    }
    //
    constexpr void push_front(node_type *n) {
      this->connect(n, Head->Next);
      this->connect(Head, n);
    }
    //
    constexpr node_type *pop_back() {
      node_type *last = Head->Prev;
      this->connect(Head->Prev->Prev, Head);
      return last;
    }
    //
    constexpr node_type *pop_front() {
      node_type *first = Head->Next;
      this->connect(Head, Head->Next->Next);
      return first;
    }
    //
    constexpr void swap(lsm& ano) {
      node_type *tmp = Head;
      Head = ano.Head;
      ano.Head = tmp;
    }
    //
    node_type *Head;
    //
    constexpr node_type *insert(node_type *where, node_type *n) {
      return this->connect(this->connect(where->Prev, n), where);
    }
    constexpr node_type *insert(node_type *where, node_type *head, node_type *tail) {
      connect(where->Prev, head);
      return connect(tail, where);
    }
    constexpr node_type *connect(node_type *Prev, node_type *Next) {
      Prev->Next = Next;
      Next->Prev = Prev;
      return Next;
    }
  };
}  // namespace xsl::ls
#endif  // !XSL_LIST_IMPL