#pragma once
#ifndef XSL_SET_SUPPORT
#define XSL_SET_SUPPORT
#include <xsl/bits/functor.hpp>
#include <xsl/bits/tree/rb.hpp>

namespace xsl::ost {  // ordered set
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
  template <class _Key, class _Comp, class _Alloc>
  class set : public tree::rb<set_traits<_Key, _Comp, _Alloc>> {
  public:
    // clang-format off
	typedef tree::rb<set_traits<_Key, _Comp, _Alloc>>       base_type;
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
      auto k = comp::get_val(ptr->Val);
      // 自定义key，无自定义key，则找公用key
      null_node_description_type nnd = this->null_upper_bound(this->Head->Next[2], k);
      if(node_type *pre_node = this->pre_node(nnd); pre_node != nullptr && this->upper_bound_equal(pre_node, k)) {
        this->Alc.deallocate(destruct_at(ptr));
        return {pre_node, false};
      }
      return {this->insert_node(nnd, ptr), true};
    }
  };
  template <class _Key, class _Comp, class _Alloc>
  class multiset : public tree::rb<set_traits<_Key, _Comp, _Alloc>> {
  public:
    // clang-format off
	typedef tree::rb<set_traits<_Key, _Comp, _Alloc>>		    base_type;
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
      return {this->insert_node(this->null_upper_bound(this->Head->Next[2], comp::get_val(ptr->Val)), ptr)};
    }
  };
}  // namespace xsl::ost
#endif  // !XSL_SET
