#pragma once
#include <cstddef>

#include "xsl/bits/pf/def.h"
#ifndef XSL_HEAP_SUPPORT
#define XSL_HEAP_SUPPORT
#include <xsl/bits/compare.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/iterator.hpp>
#include <xsl/bits/utility.hpp>
namespace xsl::hp {
  template <class _Ctr, class _Comp>
  class heap {
  public:
    // clang-format off
    typedef _Ctr ctr_type;
    typedef typename _Ctr::val_type val_type;
    typedef _Comp comp_pre;
    typedef typename ctr_type::iter iter;
    typedef typename ctr_type::diff_type diff_type;
    // clang-format on
    ctr_type Ctr;
  private:
    comp_pre Comp;
    constexpr diff_type parent(diff_type chd) {
      return (chd - 1) / 2;
    }
    constexpr diff_type lchild(diff_type par) {
      return 2 * par + 1;
    }
    constexpr diff_type rchild(diff_type par) {
      return 2 * (par + 1);
    }
    constexpr void adjust_up(iter chd) {
      auto ccmp = comp::get_val(*chd);
      auto b = Ctr.begin();
      while(chd != b) {
        iter par = b + parent(chd - b);
        auto& po = *par;
        if(!(Comp(ccmp, comp::get_val(po))))
          break;
        swap(*chd, po);
        chd = par;
      }
    }
    constexpr void adjust_down(iter par) {
      iter b = Ctr.begin();
      diff_type size = Ctr.end() - b;
      while(true) {
        diff_type pp = par - b;
        diff_type mp = lchild(pp);
        if(mp >= size)
          break;
        diff_type rp = rchild(pp);
        if(rp < size && Comp(comp::get_val(*(b + rp)), comp::get_val(*(b + mp))))
          mp = rp;
        iter mch = b + mp;
        if(Comp(comp::get_val(*par), comp::get_val(*mch)))
          break;
        swap(*par, *mch);
        par = mch;
      }
    }
  public:
    constexpr heap()
      : Ctr() {
    }
    template <class... Args>
    constexpr void push(Args&&...args) {
      Ctr.emplace_back(forward<Args>(args)...);
      adjust_up(--Ctr.end());
    }
    constexpr void pop() {
      itor::swap(Ctr.begin(), --Ctr.end());
      Ctr.pop_back();
      adjust_down(Ctr.begin());
    }
    constexpr val_type& top() {
      return Ctr.front();
    }
    constexpr bool empty() {
      return Ctr.empty();
    }
  };
}  // namespace xsl::hp
#endif  // !XSL_LIST