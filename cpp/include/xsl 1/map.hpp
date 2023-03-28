#pragma once
#ifndef XSL_MAP
#define XSL_MAP
#include <xsl/bits/functor.hpp>
#include <xsl/bits/rb_tree.hpp>
XSL_BEGIN
template <class _Key, class _Val, class _Comp, class _Alloc>
class map_traits {
public:
  // clang-format off
  typedef _Alloc                  alloc_type;
  typedef _Comp                   comp_pre;
  typedef pair<const _Key, _Val>  val_type;
  // clang-format on
private:
  class ek {
  public:
    constexpr const typename val_type::first_type& operator()(const val_type& val) { return val.first; }
  };
public:
  // clang-format off
  typedef ek                      ek_type;
  // clang-format on
};

template <class _Key, class _Val, class _Comp = fctor::less<_Key>, class _Alloc = default_allocator<_Val>>
class map : public rb_tree<map_traits<_Key, _Val, _Comp, _Alloc>> {
public:
  // clang-format off
  typedef rb_tree<map_traits<_Key, _Val, _Comp, _Alloc>>  base_type;
  typedef _Key                                            key_type;
  typedef _Val                                            val_type;
  typedef typename base_type::node_type                   node_type;
  typedef typename base_type::size_type                   size_type;
  typedef typename base_type::alloc_type                  alloc_type;
  typedef typename base_type::iter                        iter;
  typedef typename base_type::citer                       citer;
  typedef typename base_type::iter                        riter;
  typedef typename base_type::criter                      criter;
  typedef pair<iter, bool>                                try_iter;
  // clang-format on
protected:
  // clang-format off
  typedef typename base_type::null_node_description_type  null_node_description_type;
  // clang-format on
public:
  template <class Key, class... Args>
  constexpr try_iter try_emplace(Key&& key, Args&&...args) {
    auto k = get_comp_val(key);
    // 自定义key，无自定义key，则找公用key
    null_node_description_type nnd = this->null_upper_bound(this->Head->Next[2], k);
    if(node_type *pre_node = this->pre_node(nnd); pre_node != nullptr && this->upper_bound_equal(pre_node, k))
      return {pre_node, false};
    node_type *ptr = node_type::New(this->Alc);
    set_pair(&ptr->Val)._1(forward<Key>(key))._2(forward<Args>(args)...);
    return {this->insert_node(nnd, ptr), true};
  }
};
/*
#include <xsl/bits/xsl_test.h>
#include <xsl/map.h>

#include <iostream>
#include <random>
#include <vector>

using namespace xsl;
struct comp {
  constexpr bool operator()(const test_p& l, const test_p& r) { return *l.Ptr < *r.Ptr; }
};
template <class _Ctr>
void print_map(_Ctr& Ctr) {
  // for (auto && c : Ctr) {
  // std::cout << *c.first.Ptr << '\t'; //<< ':' << (c->Color ==
  // rb_tree_color::Red ? "R\t" : "B\t");
  // }
  // puts("");
  for(auto i = Ctr.begin(); i != Ctr.end(); ++i) {
    std::cout << i.Ptr->Val.second << ':' << (i.Ptr->Color == rb_tree_color::Red ? "R\t" : "B\t");
  }
  puts("");
}
int main() {
  std::random_device r;
  std::default_random_engine e(r());
  std::uniform_int_distribution<int> uid(1, 1000000);
  map<test_p, int, comp> rt00{};
  std::vector<int> sto;
  int c = 2;
  while(c-- > 0) {
    for(int i = 0; i < 100000; ++i) {
      int k = uid(e);
      if(rt00.try_emplace(k, k).second) sto.push_back(k);
      // if (i < 10)
      // print_map(rt00);
    }
    std::cout << rt00.size() << " begin" << std::endl;
    while(rt00.size())
      rt00.erase(sto[rt00.size() - 1]);
    sto.clear();
    std::cout << rt00.size() << " end" << std::endl;
  }
}
*/
template <class _Key, class _Val, class _Comp = fctor::less<_Key>, class _Alloc = default_allocator<int>>
class multimap : public rb_tree<map_traits<_Key, _Val, _Comp, _Alloc>> {
public:
  // clang-format off
  typedef rb_tree<map_traits<_Key, _Val, _Comp, _Alloc>>  base_type;
  typedef _Key                                            key_type;
  typedef _Val                                            val_type;
  typedef typename base_type::node_type                   node_type;
  typedef typename base_type::size_type                   size_type;
  typedef typename base_type::alloc_type                  alloc_type;
  typedef typename base_type::iter                        iter;
  typedef typename base_type::citer                       citer;
  typedef typename base_type::iter                        riter;
  typedef typename base_type::criter                      criter;
  typedef pair<iter, bool>                                try_iter;
  // clang-format on
protected:
  // clang-format off
  typedef typename base_type::null_node_description_type  null_node_description_type;
  // clang-format on
public:
  template <class Key, class... Args>
  constexpr iter emplace(Key&& key, Args&&...args) {
    node_type *ptr = node_type::New(this->Alc);
    auto k = get_comp_val(key);
    set_pair(&ptr->Val)._1(forward<Key>(key))._2(forward<Args>(args)...);
    return {this->insert_node(this->null_upper_bound(this->Head->Next[2], k), ptr)};
  }
};
/*
#include <xsl/bits/xsl_test.h>
#include <xsl/map.h>

#include <iostream>
#include <random>
#include <vector>
using namespace xsl;
struct comp {
  constexpr bool operator()(int l, const test_p& r) { return l < *r.Ptr; }
  constexpr bool operator()(const test_p& l, int r) { return *l.Ptr < r; }
  constexpr bool operator()(const test_p& l, const test_p& r) { return *l.Ptr < *r.Ptr; }
};
template <class _Ctr>
void print_map(_Ctr& Ctr) {
  // for (auto && c : Ctr) {
  // std::cout << *c.first.Ptr << '\t'; //<< ':' << (c->Color ==
  // rb_tree_color::Red ? "R\t" : "B\t");
  // }
  // puts("");
  for(auto i = Ctr.begin(); i != Ctr.end(); ++i) {
    std::cout << i.Ptr->Val.second << ':' << (i.Ptr->Color == rb_tree_color::Red ? "R\t" : "B\t");
  }
  puts("");
}
int main() {
  std::random_device r;
  std::default_random_engine e(r());
  std::uniform_int_distribution<int> uid(1, 1000);
  multimap<test_p, int, comp> rt00{};
  std::vector<int> sto;
  int c = 2;
  while(c-- > 0) {
    for(int i = 0; i < 1000000; ++i) {
      int k = uid(e);
      rt00.emplace(k, k);
      sto.push_back(k);
      // if (i < 10)
      // print_map(rt00);
    }
    std::cout << rt00.size() << " begin" << std::endl;
    while(rt00.size()) {
      rt00.erase(sto.back());
      sto.pop_back();
    }
    sto.clear();
    std::cout << rt00.size() << " end" << std::endl;
  }
}
*/
XSL_END
#endif  // !XSL_MAP
