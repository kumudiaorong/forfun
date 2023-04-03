#pragma once
#ifndef XSL_SET
#define XSL_SET
#include <xsl/bits/functor.hpp>
#include <xsl/bits/tree/rb.hpp>

namespace xsl {
  namespace impl {
    template <class _Key, class _Comp, class _Alloc>
    class set_traits {
    public:
      // clang-format off
	typedef _Alloc	alloc_type;
	typedef _Comp 	comp_pre;
	typedef _Key 	  val_type;
      // clang-format on
    private:
      class ek {
      public:
        constexpr const _Key& operator()(const val_type& val) {
          return val;
        }
      };
    public:
      // clang-format off
	typedef ek 		  ek_type;
      // clang-format on
    };
  }  // namespace impl
  template <class _Key, class _Comp = fc::less<_Key>, class _Alloc = alc::default_allocator<int>>
  class set : public tree::rb<impl::set_traits<_Key, _Comp, _Alloc>> {
  public:
    // clang-format off
	typedef tree::rb<impl::set_traits<_Key, _Comp, _Alloc>>        base_type;
	typedef _Key 											                      key_type;
	typedef _Key 											                      val_type;
	typedef typename base_type::node_type 					        node_type;
	typedef typename base_type::size_type 			      		  size_type;
	typedef typename base_type::alloc_type       					  alloc_type;
	typedef typename base_type::iter        					    	iter;
	typedef typename base_type::citer   				      		  citer;
	typedef typename base_type::iter 		  			  	        riter;
	typedef typename base_type::criter		        		  		criter;
	typedef pair<iter, bool> 							          	      try_iter;
    // clang-format on
  protected:
    // clang-format off
	typedef typename base_type::null_node_description_type 	null_node_description_type;
    // clang-format on
  public:
    template <class... Args>
    constexpr try_iter try_emplace(Args&&...args) {
      node_type *ptr = node_type::New(this->Alc);
      construct_at(addr(ptr->Val), forward<Args>(args)...);
      auto k = get_comp_val(ptr->Val);
      // 自定义key，无自定义key，则找公用key
      null_node_description_type nnd = this->null_upper_bound(this->Head->Next[2], k);
      if(node_type *pre_node = this->pre_node(nnd); pre_node != nullptr && this->upper_bound_equal(pre_node, k)) {
        this->Alc.deallocate(destruct_at(ptr));
        return {pre_node, false};
      }
      return {this->insert_node(nnd, ptr), true};
    }
  };
  /*
  #include <xsl/bits/xsl_test.h>
  #include <xsl/set.h>

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
    // tree::rb_color::Red ? "R\t" : "B\t");
    // }
    // puts("");
    for(auto i = Ctr.begin(); i != Ctr.end(); ++i) {
      std::cout << i.Ptr->Val.second << ':' << (i.Ptr->Color == tree::rb_color::Red ? "R\t" : "B\t");
    }
    puts("");
  }
  int main() {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uid(1, 10000000);
    set<test_p, comp> rt00{};
    std::vector<int> sto;
    int c = 2;
    while(c-- > 0) {
      for(int i = 0; i < 1000000; ++i) {
        int k = uid(e);
        if(rt00.try_emplace(k).second) sto.push_back(k);
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
  template <class _Key, class _Comp = fc::less<_Key>, class _Alloc = alc::default_allocator<int>>
  class multiset : public tree::rb<impl::set_traits<_Key, _Comp, _Alloc>> {
  public:
    // clang-format off
	typedef tree::rb<impl::set_traits<_Key, _Comp, _Alloc>>		    base_type;
	typedef _Key 											                      key_type;
	typedef _Key 											                      val_type;
	typedef typename base_type::node_type 					        node_type;
	typedef typename base_type::size_type 					        size_type;
	typedef typename base_type::alloc_type 					        alloc_type;
	typedef typename base_type::iter 						            iter;
	typedef typename base_type::citer 						          citer;
	typedef typename base_type::iter 						            riter;
	typedef typename base_type::criter 						          criter;
	typedef pair<iter, bool> 								                try_iter;
    // clang-format on
  protected:
    // clang-format off
	typedef typename base_type::null_node_description_type 	null_node_description_type;
    // clang-format on
  public:
    template <class... Args>
    constexpr iter emplace(Args&&...args) {
      node_type *ptr = node_type::New(this->Alc);
      construct_at(addr(ptr->Val), forward<Args>(args)...);
      return {this->insert_node(this->null_upper_bound(this->Head->Next[2], get_comp_val(ptr->Val)), ptr)};
    }
  };
  /*
  #include <xsl/bits/xsl_test.h>
  #include <xsl/set.h>

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
    std::uniform_int_distribution<int> uid(1, 10000000);
    multiset<test_p, comp> rt00{};
    std::vector<int> sto;
    int c = 2;
    while(c-- > 0) {
      for(int i = 0; i < 1000000; ++i) {
        int k = uid(e);
        rt00.emplace(k);
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
}  // namespace xsl
#endif  // !XSL_SET
