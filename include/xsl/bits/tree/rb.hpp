#pragma once
#ifndef XSL_RB_TREE
#define XSL_RB_TREE
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/ts/ts.hpp>
// #include <xsl/bits/compare.hpp>
// #include <xsl/bits/def.hpp>
#include <xsl/bits/exception.hpp>
#include <xsl/bits/iterator.hpp>
#include <xsl/bits/pair.hpp>
// #include <xsl/bits/pf/memory.hpp>
// #include <xsl/bits/utility.hpp>

namespace xsl::tree {
  namespace impl_rb {
    enum class color { Red, Black, Null };
    // template<class _Ctr>
    // void print_rb_tree(_Ctr& Ctr) {
    // 	for (auto&& c : Ctr)
    // 		std::cout << c->Val << ':' << (c->Color == rb_tree_color::Red ? "R\t" : "B\t");
    // 	puts("");
    // }

    template <class _Val>
    class node {
    public:
      template <class Alloc>
      static node *New(Alloc& alc) {
        node *ptr = alc.allocate(sizeof(node));
        ptr->Color = color::Red;
        ptr->Next[0] = ptr->Next[1] = ptr->Next[2] = nullptr;
        return ptr;
      }
      typedef _Val val_type;
      // neighbor
      constexpr node() = default;
      //
      template <typename Arg, typename... Args, ts::enable_construct<node, Arg> = 0>
      constexpr node(Arg&& arg, Args&&...args)
        : Color(color::Red)
        , Val(forward<Arg>(arg), forward<Args>(args)...)
        , Next() {
      }
      //
      constexpr node(const node& Ano) = default;
      //
      constexpr uint_8 rela(node *Ptr) {
        return Ptr == Next[2] ? 2 : (Ptr == Next[1] ? 1 : 0);
      }
      // first dicide parent.because if this is head,maybe same
      constexpr bool is_leaf() {
        return Next[0] == nullptr && Next[1] == nullptr;
      }
      //
      color Color;
      val_type Val;
      node *Next[3];  // Left, Right, Parent
    };
    template <class _Val>
    auto next(node<_Val> *ptr) {
      if(ptr->Next[1] == nullptr) {
        while(ptr->Next[2]->Color != color::Null && ptr->Next[2]->rela(ptr) == 1)  // 最终寻找顶层节点,即head
          ptr = ptr->Next[2];
        ptr = ptr->Next[2];
      } else {
        ptr = ptr->Next[1];
        while(ptr->Next[0] != nullptr)
          ptr = ptr->Next[0];
      }
      return ptr;
    }
    template <class _Val>
    auto prev(node<_Val> *ptr) {
      if(ptr->Next[0] == nullptr) {
        while(ptr->Next[2]->Color != color::Null && ptr->Next[2]->rela(ptr) == 0)
          ptr = ptr->Next[2];
        ptr = ptr->Next[2];
      } else {
        ptr = ptr->Next[0];
        while(ptr->Next[1] != nullptr)
          ptr = ptr->Next[1];
      }
      return ptr;
    }
    template <class _RB_Tree>
    class iter {
    public:
      // clang-format off
	    typedef itor::tag_bilateral 	        iter_category;
	    typedef typename _RB_Tree::val_type 	val_type;
	    typedef iter 							            mnr_type;
	    typedef iter 							            self_type;
	    typedef typename _RB_Tree::diff_type  diff_type;
	    typedef typename _RB_Tree::size_type 	size_type;
	    typedef typename _RB_Tree::node_type 	node_type;
      // clang-format on
      constexpr iter(node_type *ptr)
        : Ptr(ptr) {
      }
      //
      constexpr iter(iter&&) = default;
      //
      constexpr iter(const iter&) = default;
      //
      constexpr iter& operator=(iter&&) = default;
      //
      constexpr iter& operator=(const iter&) = default;
      //
      constexpr val_type& operator*() const {
        return Ptr->Val;
      }
      //
      constexpr val_type *operator->() const {
        return &(Ptr->Val);
      }
      //
      constexpr bool operator==(const iter& Ano) const {
        return Ptr == Ano.Ptr;
      }
      //
      constexpr iter& operator++() {
        Ptr = next(Ptr);
        return *this;
      }
      //
      constexpr iter& operator--() {
        Ptr = prev(Ptr);
        return *this;
      }
      node_type *Ptr;
    };
  }  // namespace impl_rb
  template <class _Traits>
  class rb {
  public:
    // clang-format off
	typedef typename _Traits::val_type 									               	    val_type;
	typedef typename _Traits::ek_type 										                  ek_type;
	typedef typename _Traits::comp_pre 										                  comp_pre;
	typedef impl_rb::node<val_type> 											              node_type;
	typedef typename _Traits::alloc_type::template rebind_alloc<node_type>  alloc_type;
	typedef ptrdiff_t 														                          diff_type;
	typedef typename alloc_type::size_type 									                size_type;
	typedef impl_rb::iter<rb> 											              iter;
	typedef const iter		 												                          citer;
	typedef itor::reverse<iter>		 									                    riter;
	typedef const riter 													                          criter;
    // clang-format on
  protected:
    // clang-format off
	typedef pair<node_type*, bool> 											                    null_node_description_type;
    // clang-format on
  public:
    constexpr rb()
      : Alc()
      , Head(Alc.allocate(sizeof(node_type)))
      , Size()
      , Ek()
      , Comp() {
      Head->Next[0] = Head->Next[1] = Head;
      Head->Next[2] = nullptr;
      Head->Color = impl_rb::color::Null;
    }
    //
    constexpr rb(const rb& ano)
      : rb() {
      Head->Next[2] = create(ano.Head->Next[2]);
      Size = ano.Size;
      if(Head->Next[2] != nullptr) {
        Head->Next[2]->Next[2] = Head;
        Head->Next[0] = pre_node(null_upper_bound(Head->Next[2], Ek(ano.Head->Next[0]->Val)));
        Head->Next[1] = next_node(null_lower_bound(Head->Next[2], Ek(ano.Head->Next[1]->Val)));
      }
    }
    //
    constexpr rb(rb&& ano)
      : Alc(as_rreference(ano.Alc))
      , Head(ano.Head)
      , Size(ano.Size)
      , Ek()
      , Comp() {
      ano.Head = nullptr;
      ano.Size = 0;
    }
    //
    constexpr ~rb() {
      if(!this->invalid()) {
        if(Head->Next[2] != nullptr)
          destroy(Head->Next[2]);
        Alc.deallocate(Head);
      }
    }
    //
    constexpr bool invalid() {
      return Head == nullptr;
    }
    //
    constexpr size_type size() {
      return Size;
    }
    //
    constexpr iter begin() {
      return {Head->Next[1]};
    }
    //
    constexpr iter end() {
      return {Head};
    }
    //
    constexpr iter rend() {
      return {Head->Next[0]};
    }
    //
    constexpr const iter begin() const {
      return {Head->Next[1]};
    }
    //
    constexpr const iter end() const {
      return {Head};
    }
    //
    template <class Key>
    constexpr iter lower_bound(const Key& key) {
      return {node_lower_bound(Head->Next[2], key)};
    }
    //
    template <class Key>
    constexpr size_type erase(const Key& key) {
      auto k = comp::get_val(key);
      node_type *ptr = node_lower_bound(Head->Next[2], k);
      size_type c{0};
      while(ptr != Head && !Comp(k, comp::get_val(Ek(ptr->Val)))) {
        ptr = erase_node(ptr);
        ++c;
      }
      return c;
    }
    //
    constexpr iter erase(citer iter) {
      return {erase_node(iter.Ptr)};
    }
    template <class Key>
    constexpr val_type& at(const Key& key) {
      node_type *bound = node_lower_bound(Head->Next[2], (key));
      ecp::check_out_of_range(bound == Head || !lower_bound_equal(bound, key));
      return bound->Val;
    }
    //
    template <class Key>
    constexpr iter find(const Key& key) {
      node_type *bound = node_lower_bound(Head->Next[2], comp::get_val(key));
      return bound != Head && lower_bound_equal(bound, key) ? bound : Head;
    }
    // protected:
    // constexpr node_type* insert_unique(const val_type& Val) {
    //	auto key = Ek(Val);
    //	null_node_description_type Nndt = null_upper_bound(Head->Next[2], key);
    //	if (upper_bound_equal(node_bound(Nndt), key))
    //		return nullptr;
    //	node_type* NewNode = Construct_At(Alc.allocate(sizeof(node_type)), Val);
    //	insert_node(Nndt, NewNode);
    //	return NewNode;
    // }
    // constexpr iter insert_equal(const val_type& Val) {
    //	node_type* NewNode = Construct_At(Alc.allocate(sizeof(node_type)), Val);
    //	insert_node(null_upper_bound(Head->Next[2], Ek(Val)), NewNode);
    //	return NewNode;
    // }
    // constexpr node_type* insert_unique(val_type&& Val) {
    //	auto key = Ek(Val);
    //	null_node_description_type Nndt = null_upper_bound(Head->Next[2], key);
    //	if (upper_bound_equal(node_bound(Nndt), key))
    //		return nullptr;
    //	node_type* NewNode = Construct_At(Alc.allocate(sizeof(node_type)), as_rreference(Val));
    //	insert_node(Nndt, NewNode);
    //	return NewNode;
    // }
    // template<class Unchecked_Iter, enable<is_iter<Unchecked_Iter>> = 0>
    // constexpr node_type* insert_unique(Unchecked_Iter First, Unchecked_Iter Last) {
    //	node_type* last_insert_node{};
    //	for (; First != Last; ++First) {
    //		node_type* tmp = insert_unique(*First);
    //		if (tmp != nullptr)
    //			last_insert_node = tmp;
    //	}
    //	return last_insert_node;
    // }
    //
    // constexpr iter insert_equal(val_type&& Val) {
    //	node_type* NewNode = Construct_At(Alc.allocate(sizeof(node_type)), as_rreference(Val));
    //	insert_node(null_upper_bound(Head->Next[2], Ek(Val)), NewNode);
    //	return NewNode;
    // }
    ////
    // template<class Unchecked_Iter, enable<is_iter<Unchecked_Iter>> = 0>
    // constexpr node_type* insert_equal(Unchecked_Iter First, Unchecked_Iter Last) {
    //	node_type* last_insert_node{};
    //	for (; First != Last; ++First)
    //		last_insert_node = insert_equal(*First);
    //	return last_insert_node;
    // }
  protected:
    template <class Key>
    constexpr node_type *node_lower_bound(node_type *start, const Key& key) {
      node_type *bound = next_node(null_lower_bound(start, key));
      return bound == nullptr ? Head : bound;
    }
    constexpr node_type *insert_node(const null_node_description_type& nndt, node_type *child) {
      ++Size;
      if(nndt.first == nullptr)
        return set_root(child);
      child->Next[2] = nndt.first;
      nndt.first->Next[nndt.second] = child;
      if(nndt.first->Color == impl_rb::color::Red)
        double_red_adjust(child, nndt.second);
      if(Head->Next[0]->Next[1] != nullptr)
        Head->Next[0] = Head->Next[0]->Next[1];
      if(Head->Next[1]->Next[0] != nullptr)
        Head->Next[1] = Head->Next[1]->Next[0];
      Head->Next[2]->Color = impl_rb::color::Black;
      return child;
    }
    constexpr node_type *erase_node(node_type *where) {
      node_type *n = next(where);
      if(where->Next[0] == nullptr) {
        if(Head->Next[0] == where)
          Head->Next[0] = where->Next[2];
        if(Head->Next[1] == where)
          Head->Next[1] = where->Next[1 + (where->Next[1] == nullptr)];
        remove_extremum_node(where, 1);
      } else {
        node_type *Repl = prev(where);
        if(Head->Next[0] == where)
          Head->Next[0] = Repl;
        remove_extremum_node(Repl, 0);
        Repl->Next[0] = where->Next[0];
        Repl->Next[1] = where->Next[1];
        Repl->Next[2] = where->Next[2];
        if(Repl->Next[0] != nullptr)
          Repl->Next[0]->Next[2] = Repl;
        if(Repl->Next[1] != nullptr)
          Repl->Next[1]->Next[2] = Repl;
        if(Repl->Next[2] != nullptr)
          Repl->Next[2]->Next[Repl->Next[2]->rela(where)] = Repl;
        Repl->Color = where->Color;
      }
      Alc.deallocate(destruct_at(where));
      --Size;
      return n;
    }
    //
    template <class Key>
    constexpr null_node_description_type null_lower_bound(node_type *start, const Key& key) {
      node_type *Parent = start;
      bool cRela{0};
      while(start != nullptr) {
        cRela = Comp(comp::get_val(Ek(start->Val)), key);
        Parent = start;
        start = start->Next[cRela];
      }
      return {Parent, cRela};
    }
    //<Ptr,[0,1]>	:Ptr is parent,num is relationship
    template <class Key>
    constexpr bool lower_bound_equal(node_type *bound, const Key& key) {
      return !Comp(key, comp::get_val(Ek(bound->Val)));
    }
    //
    template <class Key>
    constexpr null_node_description_type null_upper_bound(node_type *start, const Key& key) {
      node_type *Parent = start;
      bool CRela{1};
      while(start != nullptr) {
        CRela = !Comp(key, comp::get_val(Ek(start->Val)));
        Parent = start;
        start = start->Next[CRela];
      }
      return {Parent, CRela};
    }
    //
    template <class Key>
    constexpr bool upper_bound_equal(node_type *bound, const Key& key) {
      return !Comp(comp::get_val(Ek(bound->Val)), key);
    }
    //
    constexpr node_type *pre_node(const null_node_description_type& nnd) {
      return nnd.second ? nnd.first : (Head->Next[1] == nnd.first ? nullptr : prev(nnd.first));
    }
    //
    constexpr node_type *next_node(const null_node_description_type& nnd) {
      return nnd.second ? (Head->Next[0] == nnd.first ? nullptr : next(nnd.first)) : nnd.first;
    }
    // alc::default_allocator<node_type> Alc;
    alloc_type Alc;
    node_type *Head;
    size_type Size;
    ek_type Ek;
    comp_pre Comp;
  private:
    constexpr node_type *set_root(node_type *root) {
      Head->Next[0] = Head->Next[1] = Head->Next[2] = root;
      root->Next[0] = root->Next[1] = nullptr;
      root->Next[2] = Head;
      root->Color = impl_rb::color::Black;
      return root;
    }
    //
    constexpr void remove_extremum_node(node_type *child, uint_8 Rela) {
      node_type *Parent = child->Next[2];
      uint_8 CRela = Parent->rela(child);
      Parent->Next[CRela] = child->Next[Rela];
      if(child->Next[Rela] != nullptr) {
        Parent->Next[CRela]->Next[2] = Parent;
        Parent->Next[CRela]->Color = impl_rb::color::Black;
      } else if(child->Color == impl_rb::color::Black && Parent != Head)
        raise_branch(Parent, CRela);
    }
    //
    constexpr void raise_branch(node_type *GParent, bool PRela) {
      node_type *Parent = GParent->Next[!PRela];
      if(Parent->Color == impl_rb::color::Red) {
        single_rotate(GParent, !PRela);
        Parent->Color = impl_rb::color::Black;
        GParent->Color = impl_rb::color::Red;
        raise_branch(GParent, PRela);
      } else if(Parent->Next[!PRela] != nullptr && Parent->Next[!PRela]->Color == impl_rb::color::Red) {
        single_rotate(GParent, !PRela);
        Parent->Color = GParent->Color;
        Parent->Next[!PRela]->Color = impl_rb::color::Black;
        GParent->Color = impl_rb::color::Black;
      } else if(Parent->Next[PRela] != nullptr && Parent->Next[PRela]->Color == impl_rb::color::Red) {
        single_rotate(Parent, PRela, !PRela);
        single_rotate(GParent, !PRela);
        GParent->Next[2]->Color = GParent->Color;
        GParent->Color = impl_rb::color::Black;
      } else if(GParent->Color == impl_rb::color::Red) {
        GParent->Color = impl_rb::color::Black;
        Parent->Color = impl_rb::color::Red;
      } else {
        Parent->Color = impl_rb::color::Red;
        if(GParent->Next[2] != Head)
          raise_branch(GParent->Next[2], GParent->Next[2]->rela(GParent));
      }
    }
    //<Ptr,[0,1]>	:Ptr is parent,num is relationship
    constexpr void single_rotate(node_type *Parent, uint_8 CRtr, uint_8 PRtr) {
      node_type *GParent = Parent->Next[2], *child = Parent->Next[CRtr];
      Parent->Next[CRtr] = child->Next[!CRtr];
      if(Parent->Next[CRtr] != nullptr)  // never be head
        Parent->Next[CRtr]->Next[2] = Parent;
      Parent->Next[2] = GParent->Next[PRtr] = child;
      child->Next[!CRtr] = Parent;
      child->Next[2] = GParent;
    }
    constexpr decltype(auto) single_rotate(node_type *Parent, uint_8 CRtr) {
      return this->single_rotate(Parent, CRtr, Parent->Next[2]->rela(Parent));
    }
    constexpr void double_red_adjust(node_type *child, uint_8 CRtr) {
      node_type *Parent = child->Next[2];
      node_type *GParent = Parent->Next[2];
      uint_8 ParRtr = GParent->rela(Parent);
      node_type *Uncle = GParent->Next[!ParRtr];
      GParent->Color = impl_rb::color::Red;
      if(Uncle == nullptr || Uncle->Color == impl_rb::color::Black) {  // Scale is about n1:n2:1,n1:n2==1:1,n1 is
                                                                       // slightly larger than n2
        if(CRtr != ParRtr) {
          single_rotate(Parent, CRtr, ParRtr);
          swap(child, Parent);
        }
        single_rotate(GParent, ParRtr);
      } else {
        Uncle->Color = impl_rb::color::Black;
        if(GParent->Next[2]->Color == impl_rb::color::Red)
          double_red_adjust(GParent);
      }
      Parent->Color = impl_rb::color::Black;
    }
    constexpr decltype(auto) double_red_adjust(node_type *child) {
      return double_red_adjust(child, child->Next[2]->rela(child));
    }
    //
    constexpr node_type *create(node_type *ptr) {
      if(ptr != nullptr) {
        node_type *node = Alc.allocate(sizeof(node_type));
        node->Color = ptr->Color;
        construct_at(addr(node->Val), ptr->Val);
        if((node->Next[0] = create(ptr->Next[0])) != nullptr)
          node->Next[0]->Next[2] = node;
        if((node->Next[1] = create(ptr->Next[1])) != nullptr)
          node->Next[1]->Next[2] = node;
        return node;
      }
      return nullptr;
    }
    /*
            constexpr node_type* create(node_type* ptr) {
                    node_type* node = Alc.allocate(sizeof(node_type));
                    node->Color = ptr->Color;
                    Construct_At(addr(node->Val), ptr->Val);
                    node->Next[0] = node->Next[1] = nullptr;
                    if (ptr->Next[0] != nullptr) {
                            node->Next[0] = create(ptr->Next[0]);
                            node->Next[0]->Next[2] = node;
                    }
                    if (ptr->Next[1] != nullptr) {
                            node->Next[1] = create(ptr->Next[1]);
                            node->Next[1]->Next[2] = node;
                    }
                    return node;
            }
    */

    //
    constexpr void destroy(node_type *where) {
      if(where->Next[0] != nullptr)
        destroy(where->Next[0]);
      if(where->Next[1] != nullptr)
        destroy(where->Next[1]);
      Alc.deallocate(destruct_at(where));
    }
  };
}  // namespace xsl::tree
#endif  // !XSL_RB_TREE
