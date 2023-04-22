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
	typedef _Node 						            node_type;
	typedef typename node_type::val_type 	val_type;
	typedef size_t 										    size_type;
    // clang-format on
    constexpr lsm(node_type *head = nullptr)
      : Head(head) {
      if(Head != nullptr)
        Head->prev() = Head->next() = Head;
    }
    constexpr lsm(lsm&& ano)
      : Head(ano.Head) {
      ano.Head = nullptr;
    }
    //
    constexpr lsm& operator=(lsm&& ano) noexcept {
      return this->assign(static_cast<lsm&&>(ano));
    }
    //
    // constexpr lsm& operator=(const lsm& Ano) { return this->assign(creator{iter{Ano.Head->next()}, Ano.Size}); }
    //
    constexpr lsm& assign(lsm&& ano)noexcept {
      Head = ano.Head;
      ano.Head = nullptr;
      return *this;
    }
    //
    // constexpr lsm& assign(const lsm& Ano) { return assign(creator{iter{Ano.Head->next()}, Ano.Size}); }
    //
    //
    constexpr bool invalid() const {
      return Head == nullptr;
    }
    //
    constexpr void reuse(node_type *head) {
      Head->prev() = Head->next() = Head;
    }
    //
    constexpr node_type *clear() {
      XSL_EMPTY_CHECK(this->invalid());
      if(Head->next() != Head) {
        Head->prev()->next() = nullptr;
        node_type *first = Head->next();
        Head->prev() = Head->next() = Head;
        return first;
      }
      return nullptr;
    }
    //
    constexpr node_type *remove(node_type *ptr) {
      node_type *nex = ptr->next();
      this->connect(ptr->prev(), ptr->next());
      return nex;
    }
    //
    constexpr void erase(node_type *first, node_type *last) {
      this->connect(first->prev(), last);
    }
    //
    constexpr void push_back(node_type *n) {
      this->connect(this->connect(Head->prev(), n), Head);
    }
    //
    constexpr void push_front(node_type *n) {
      this->connect(n, Head->next());
      this->connect(Head, n);
    }
    //
    constexpr node_type *pop_back() {
      node_type *last = Head->prev();
      this->connect(Head->prev()->prev(), Head);
      return last;
    }
    //
    constexpr node_type *pop_front() {
      node_type *first = Head->next();
      this->connect(Head, Head->next()->next());
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
      return this->connect(this->connect(where->prev(), n), where);
    }
    constexpr node_type *insert(node_type *where, node_type *head, node_type *tail) {
      connect(where->prev(), head);
      return connect(tail, where);
    }
    constexpr node_type *connect(node_type *prev(), node_type *next()) {
      prev()->next() = next();
      next()->prev() = prev();
      return next();
    }
  };
}  // namespace xsl::ls
#endif  // !XSL_LIST_IMPL