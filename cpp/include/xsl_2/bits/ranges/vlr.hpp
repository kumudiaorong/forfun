#pragma once
#ifndef XSL_VLR  // virtual link range
#define XSL_VLR
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/list.hpp>
#include <xsl/bits/ranges/acsr.hpp>
#include <xsl/bits/ts.hpp>
#include <xsl/bits/utility.hpp>
XSL_BEGIN
namespace ranges {
  namespace impl_vlr {
    enum CMD { COPY, PINC, ADDR, EQUA };
    // cmd 0:copy
    // cmd 1:sentry
    template <class _Val>
    class vlr_node {
    public:
      // clang-format off
      typedef vlr_node                 self_type;
      typedef _Val                          val_type;
      // clang-format on
      self_type *Prev, *Next;
      constexpr vlr_node(self_type *prev, self_type *next)
        : Prev(prev)
        , Next(next){};
      constexpr vlr_node()
        : vlr_node(nullptr, nullptr) {}
      constexpr vlr_node(const vlr_node& ano) = default;
      constexpr virtual void *impl(CMD cmd = COPY, void *arg = nullptr) = 0;
      //
      constexpr virtual ~vlr_node(){};
    };
    template <class _Alloc, class _Limited_Interval>
    class vlr_node_storage : public vlr_node<typename _Alloc::val_type::val_type> {
    public:
      // clang-format off
    typedef _Limited_Interval               func_type;
    typedef _Alloc                          alloc_type;
    typedef typename alloc_type::val_type   node_type;
    typedef typename node_type::val_type    val_type;
      // clang-format on
      template <class... Args>
      vlr_node_storage(alloc_type& alc, Args&&...args)
        : node_type()
        , Alc(alc)
        , LI(forward<Args>(args)...) {}
      vlr_node_storage(const vlr_node_storage& ano) = default;
      constexpr void *impl(CMD cmd = COPY, void *arg = nullptr) override {
        switch(cmd) {
          case COPY :  // construct
            if(arg == nullptr)
              return construct_at(static_cast<vlr_node_storage *>(Alc.allocate(sizeof(*this))), *this);
            else
              Alc.deallocate(static_cast<node_type *>(arg));
            break;
          case PINC :  //++
            ++LI;
            if(!LI)
              return this->Next->impl(COPY);
            else
              return this;
          case ADDR :  //*
            return reinterpret_cast<void *>(const_cast<val_type *>(addr(*LI)));
          case EQUA :  // equal
            return static_cast<node_type *>(arg)->impl(ADDR) == nullptr
                     ? nullptr
                     : (LI == static_cast<vlr_node_storage *>(arg)->LI ? arg : nullptr);
            // default :;
        }
        return nullptr;
      }
      constexpr ~vlr_node_storage() override { destruct_at(addr(LI)); }
      alloc_type& Alc;
      func_type LI;
    };
    template <class _Alloc>
    class vlr_node_storage<_Alloc, void> : public vlr_node<typename _Alloc::val_type::val_type> {
    public:
      // clang-format off
    typedef _Alloc                          alloc_type;
    typedef typename alloc_type::val_type   node_type;
    typedef typename node_type::val_type    val_type;
      // clang-format on
      template <class... Args>
      vlr_node_storage(alloc_type& alc)
        : node_type()
        , Alc(alc) {}
      vlr_node_storage(const vlr_node_storage& ano) = default;
      constexpr void *impl(CMD cmd = COPY, void *arg = nullptr) override {
        switch(cmd) {
          case COPY :  // construct
            if(arg == nullptr)
              return construct_at(reinterpret_cast<vlr_node_storage *>(Alc.allocate(sizeof(*this))), *this);
            else
              Alc.deallocate(static_cast<node_type *>(arg));
            break;
          case PINC :  //++
            return this->Next->impl(COPY);
          case EQUA :  // equal
            return static_cast<node_type *>(arg)->impl(ADDR) == nullptr ? arg : nullptr;
          default :;
        }
        return nullptr;
      }
      constexpr ~vlr_node_storage() override {}
      alloc_type& Alc;
    };
    template <class Alloc, class... Args>
    vlr_node_storage(Alloc&&, Args&&...)
      -> vlr_node_storage<ts::rm_cvr<Alloc>, decltype(ranges::acsr(ts::eval_type<Args>()...))>;
    template <class _Val>
    class vlr_iter {
    public:
      // clang-format off
    typedef _Val               val_type;
    typedef vlr_node<val_type> node_type;
      // clang-format on
      vlr_iter(node_type *ptr)
        : Cur(static_cast<node_type *>(ptr->impl(COPY)))
        , Ptr(ptr) {}
      constexpr vlr_iter& operator++() {
        auto now = static_cast<node_type *>(Cur->impl(PINC));
        if(now != Cur) {
          Ptr = Cur->Next;
          destruct_at(Cur);
          Ptr->impl(impl_vlr::COPY, Cur);
          Cur = now;
        }
        return *this;
      }
      constexpr val_type& operator*() { return *static_cast<val_type *>(Cur->impl(ADDR)); }
      constexpr bool operator!=(const vlr_iter& ano) const {
        return Ptr != ano.Ptr || Cur->impl(EQUA, ano.Cur) == nullptr;
      }
      constexpr bool operator==(const vlr_iter& ano) const {
        return Ptr == ano.Ptr && Cur->impl(EQUA, ano.Cur) != nullptr;
      }
      constexpr ~vlr_iter() {
        destruct_at(Cur);
        Ptr->impl(impl_vlr::COPY, Cur);
      }
      node_type *Cur, *Ptr;
    };
  }  // namespace impl_vlr
  template <class _Val, class _Alloc = default_allocator<_Val>>
  class vlr {
  public:
    // clang-format off
    typedef _Val                                                  val_type;
    typedef impl_vlr::vlr_node<val_type>                node_type;
    typedef typename _Alloc::template rebind_alloc<node_type>     alloc_type;
    typedef impl_vlr::vlr_node_storage<alloc_type,void> empty_node_type;
    typedef xsl::impl_list::list<node_type,alloc_type>            storage_type;
    typedef impl_vlr::vlr_iter<val_type>                iter;
    // clang-format on
    vlr()
      : Alc()
      , Sto(Alc, xsl::construct_at(reinterpret_cast<empty_node_type *>(Alc.allocate(sizeof(empty_node_type))), Alc)) {}
    template <class Iter>
    vlr(const Iter& first, const Iter& last)
      : vlr() {
      merge(first, last);
    }
    template <class... Args, ts::enable<is_acsr<decltype(ranges::acsr(ts::eval_type<Args>()...))>> = 0>
    vlr& merge(Args&&...args) {
      typedef decltype(impl_vlr::vlr_node_storage(Alc, forward<Args>(args)...)) ins_type;
      Sto.push_back(
        xsl::construct_at(reinterpret_cast<ins_type *>(Alc.allocate(sizeof(ins_type))), Alc, forward<Args>(args)...));
      return *this;
    }
    template <class Ctr, ts::enable<!ts::is::rref<Ctr> && ts::is::traversable<Ctr>> = 0>
    vlr& merge(Ctr&& ctr) {
      return merge(ctr.begin(), ctr.end());
    }
    template <class Fn>
    vlr& map(Fn&& f) {
      for(auto& e : *this) {
        f(e);
      }
      return *this;
    }
    template <class Ctr, class Fn>
    vlr& mapped(Fn&& f, Ctr& ctr) {
      auto i = ctr.end();
      for(auto& e : *this) {
        i = ctr.insert(i, f(e));
      }
      return *this;
    }
    template <class Ctr, class Fn>
    Ctr mapped(Fn&& f) {
      Ctr ctr{};
      mapped(forward<Fn>(f), ctr);
      return as_rreference(ctr);
    }
    template <class Ctr>
    vlr& collect(Ctr& ctr) {
      auto i = ctr.end();
      for(auto& e : *this) {
        i = ++ctr.insert(i, e);
      }
      return *this;
    }
    template <class Ctr>
    Ctr collect() {
      Ctr ctr{};
      collect(ctr);
      return as_rreference(ctr);
    }
    iter begin() { return Sto.Head->Next; }
    iter end() { return Sto.Head; }

    alloc_type Alc;
    storage_type Sto;
  };
}  // namespace ranges
XSL_END
#endif  // !XSL_VLR
