#pragma once
#ifndef XSL_RANGES
#define XSL_RANGES
#include "xsl_allocator.hpp"
#include "xsl_compare.hpp"
#include "xsl_def.hpp"
#include "xsl_iterator.hpp"
#include "xsl_list.hpp"
#include "xsl_ts.hpp"
#include "xsl_utility.hpp"
XSL_BEGIN
namespace ranges {

  //   class counter {
  //   public:
  //     counter(size_t count = 0)
  //       : Count(count) {}
  //     constexpr void add(size_t count = 1) { Count += count; }
  //     size_t Count;
  //   };

  //   template <uint_8 Index, class T>
  //   class condition;

  //   template <>
  //   class condition<0, void> {
  //   public:
  //     constexpr condition(size_t count)
  //       : Size(count) {}
  //     constexpr operator bool() const { return Size > 0; }
  //     constexpr condition& operator++() {
  //       --Size;
  //       return *this;
  //     }
  //     size_t Size;
  //   };

  //   template <class Iter>
  //   class condition<1, Iter> {
  //   public:
  //     condition(const Iter& first, size_t count)
  //       : First(first)
  //       , Size(count) {}
  //     constexpr operator bool() const { return Size > 0; }
  //     constexpr condition& operator++() {
  //       ++First;
  //       --Size;
  //       return *this;
  //     }
  //     Iter First;
  //     size_t Size;
  //   };

  //   template <class Iter>
  //   class condition<2, Iter> {
  //   public:
  //     constexpr condition(const Iter& first, const Iter& last)
  //       : First(first)
  //       , Last(last) {}
  //     constexpr operator bool() const { return First != Last; }
  //     constexpr condition& operator++() {
  //       ++First;
  //       return *this;
  //     }
  //     Iter First, Last;
  //   };

  //   condition(size_t)->condition<0, void>;
  //   template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
  //   condition(UCIter, size_t) -> condition<1, ts::decay<UCIter>>;
  //   template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
  //   condition(UCIter, UCIter) -> condition<2, ts::decay<UCIter>>;

  //   template <class T>
  //   inline constexpr bool is_xsl_condition = false;

  //   template <uint_8 Index, class... Ts>
  //   inline constexpr bool is_xsl_condition<condition<Index, Ts...>> = true;

  //   template <class T>
  //   inline constexpr bool is_condition = is_xsl_condition<T>;

  //   template <uint_8 Index, class T>
  //   class observer;

  //   template <>
  //   class observer<0, void>
  //     : public condition<0, void>
  //     , public counter {
  //   public:
  //     typedef condition<0, void> cond_type;
  //     using cond_type::cond_type;
  //     template <class Wrap>
  //     constexpr Wrap *operator()(Wrap *Ptr) {
  //       this->add();
  //       return construct_at(Ptr);
  //     }
  //   };

  //   template <class _Val>
  //   class observer<1, _Val> : public condition<0, void> {
  //   public:
  //     // clang-format off
  //     typedef condition<0, void>  cond_type;
  //     typedef _Val                val_type;
  //     // clang-format on
  //     constexpr observer(size_t count, const val_type& ref)
  //       : cond_type(count)
  //       , Ref(ref) {}
  //     constexpr val_type& operator*() { return Ref; }
  //     template <class Wrap = val_type>
  //     Wrap *operator()(Wrap *where) {
  //       this->add();
  //       return construct_at(where, Ref);
  //     }
  //     const val_type& Ref;
  //   };

  //   template <class Iter>
  //   class observer<2, Iter>
  //     : public condition<1, Iter>
  //     , public counter {
  //   public:
  //     typedef condition<1, Iter> cond_type;
  //     using cond_type::cond_type;
  //     template <class Wrap>
  //     Wrap *operator()(Wrap *Ptr) {
  //       this->add();
  //       return construct_at(Ptr, *this->First);
  //     }
  //   };
  //   template <class Iter>
  //   class observer<3, Iter>
  //     : public condition<2, Iter>
  //     , public counter {
  //   public:
  //     typedef condition<2, Iter> cond_type;
  //     using cond_type::cond_type;
  //     template <class Wrap>
  //     Wrap *operator()(Wrap *where) {
  //       this->add();
  //       return construct_at(where, *this->First);
  //     }
  //   };

  //   template <uint_8 Index, class T>
  //   class creator;

  //   template <>
  //   class creator<0, void>
  //     : public condition<0, void>
  //     , public counter {
  //   public:
  //     typedef condition<0, void> cond_type;
  //     using cond_type::cond_type;
  //     template <class Wrap>
  //     constexpr Wrap *operator()(Wrap *Ptr) {
  //       this->add();
  //       return construct_at(Ptr);
  //     }
  //   };

  //   template <class Val>
  //   class creator<1, Val>
  //     : public condition<0, void>
  //     , public counter {
  //   public:
  //     typedef condition<0, void> cond_type;
  //     typedef Val val_type;
  //     creator(size_t count, const val_type& ref)
  //       : cond_type(count)
  //       , Ref(ref) {}
  //     template <class Wrap = val_type>
  //     Wrap *operator()(Wrap *where) {
  //       this->add();
  //       return construct_at(where, Ref);
  //     }
  //     const val_type& Ref;
  //   };

  //   template <class Iter>
  //   class creator<2, Iter>
  //     : public condition<1, Iter>
  //     , public counter {
  //   public:
  //     typedef condition<1, Iter> cond_type;
  //     using cond_type::cond_type;
  //     template <class Wrap>
  //     Wrap *operator()(Wrap *Ptr) {
  //       this->add();
  //       return construct_at(Ptr, *this->First);
  //     }
  //   };
  //   template <class Iter>
  //   class creator<3, Iter>
  //     : public condition<2, Iter>
  //     , public counter {
  //   public:
  //     typedef condition<2, Iter> cond_type;
  //     using cond_type::cond_type;
  //     template <class Wrap>
  //     Wrap *operator()(Wrap *where) {
  //       this->add();
  //       return construct_at(where, *this->First);
  //     }
  //   };

  //   creator(size_t)->creator<0, void>;
  //   template <class T>
  //   creator(size_t, T) -> creator<1, ts::decay<T>>;
  //   template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
  //   creator(UCIter, size_t) -> creator<2, ts::decay<UCIter>>;
  //   template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
  //   creator(UCIter, UCIter) -> creator<3, ts::decay<UCIter>>;

  //   template <class T>
  //   inline constexpr bool is_xsl_creater = false;

  //   template <uint_8 Index, class T>
  //   inline constexpr bool is_xsl_creater<creator<Index, T>> = true;

  //   template <class T>
  //   inline constexpr bool is_creater = is_xsl_creater<T>;

  // template <class Iter>
  // class sentry : basic_interval<typename iter_traits<Iter>::val_type> {
  // public:
  //   // clang-format off
  // typedef typename iter_traits<Iter>::val_type    val_type;
  // typedef basic_interval<val_type>                basic_type;
  //   // clang-format on
  //   sentry(const Iter& where)
  //     : Where(where) {}
  //   constexpr basic_type *operator++() override {
  //     if(First == Last) {
  //       return Next == nullptr ? nullptr : Next->copy();
  //     } else
  //       ++First;
  //     return this;
  //   }
  //   constexpr val_type& operator*() override { return *First; }
  //   constexpr basic_type *impl(byte cmd = 0, size_t arg = 0) override { return construct(*this); }
  //   constexpr ~sentry() override {}
  //   Iter Where;
  // };
  template <uint_8 Category, class Iter>
  class limited_interval;
  template <class _Val>
  class limited_interval<0, _Val> {
  public:
    typedef _Val val_type;
    limited_interval(size_t count, val_type& val)
      : Val(val)
      , Size(count) {}
    constexpr operator bool() const { return Size > 0; }
    constexpr limited_interval& operator++() {
      --Size;
      return *this;
    }
    constexpr val_type& operator*() { return Val; }
    constexpr bool operator!=(const limited_interval& ano) const { return Size != ano.Size || Val != ano.Val; }
    constexpr bool operator==(const limited_interval& ano) const { return Size == ano.Size && Val == ano.Val; }
    val_type& Val;
    size_t Size;
  };
  template <class _Iter>
  class limited_interval<1, _Iter> {
  public:
    // clang-format off
    typedef _Iter                                       iter_type;
    typedef typename iter_traits<iter_type>::val_type   val_type;
    // clang-format on
    limited_interval(const iter_type& first, size_t count)
      : First(first)
      , Size(count) {}
    constexpr operator bool() const { return Size > 0; }
    constexpr limited_interval& operator++() {
      ++First;
      --Size;
      return *this;
    }
    constexpr val_type& operator*() { return *First; }
    constexpr bool operator!=(const limited_interval& ano) const { return Size != ano.Size || First != ano.First; }
    constexpr bool operator==(const limited_interval& ano) const { return Size == ano.Size && First == ano.First; }
    iter_type First;
    size_t Size;
  };

  template <class _Iter>
  class limited_interval<2, _Iter> {
  public:
    // clang-format off
    typedef _Iter                                       iter_type;
    typedef typename iter_traits<iter_type>::val_type   val_type;
    // clang-format on
    constexpr limited_interval(const iter_type& first, const iter_type& last)
      : First(first)
      , Last(last) {}
    constexpr operator bool() const { return First != Last; }
    constexpr limited_interval& operator++() {
      ++First;
      return *this;
    }
    constexpr val_type& operator*() { return *First; }
    constexpr bool operator!=(const limited_interval& ano) const { return First != ano.First || Last != ano.Last; }
    constexpr bool operator==(const limited_interval& ano) const { return First == ano.First && Last == ano.Last; }
    iter_type First, Last;
  };
  template <class Size_T, class T, ts::enable<ts::tp::has_opeartor_simple_assignment<size_t, Size_T>> = 0>
  limited_interval(Size_T, T&&) -> limited_interval<0, ts::as_c<ts::decay<T>>>;
  //
  template <class Size_T, class T,
    ts::enable<ts::tp::has_opeartor_simple_assignment<size_t, Size_T> && ts::is_lref<T>> = 0>
  limited_interval(Size_T, T&&) -> limited_interval<0, ts::decay<T>>;
  //
  template <class UCIter, class Size_T,
    ts::enable<is_iter<ts::decay<UCIter>> && ts::tp::has_opeartor_simple_assignment<size_t, Size_T>> = 0>
  limited_interval(UCIter, Size_T) -> limited_interval<1, ts::decay<UCIter>>;
  //
  template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
  limited_interval(UCIter, UCIter) -> limited_interval<2, ts::decay<UCIter>>;
  //
  template <class T>
  inline constexpr bool is_xsl_limited_interval = false;
  //
  template <uint_8 Index, class T>
  inline constexpr bool is_xsl_limited_interval<limited_interval<Index, T>> = true;
  //
  template <class T>
  inline constexpr bool is_limited_interval = is_xsl_limited_interval<T>;
  //
  namespace impl {
    enum CMD { COPY, PINC, ADDR, EQUA };
    // cmd 0:copy
    // cmd 1:sentry
    template <class _Val>
    class interval_node {
    public:
      // clang-format off
      typedef interval_node                 self_type;
      typedef _Val                          val_type;
      // clang-format on
      self_type *Prev, *Next;
      constexpr interval_node(self_type *prev, self_type *next)
        : Prev(prev)
        , Next(next){};
      constexpr interval_node()
        : interval_node(nullptr, nullptr) {}
      constexpr interval_node(const interval_node& ano) = default;
      constexpr virtual void *impl(CMD cmd = COPY, void *arg = nullptr) { return nullptr; }
      //
      constexpr virtual ~interval_node(){};
    };
    template <class _Alloc, class _Limited_Interval>
    class interval_node_storage : public interval_node<typename _Alloc::val_type::val_type> {
    public:
      // clang-format off
    typedef _Limited_Interval               func_type;
    typedef _Alloc                          alloc_type;
    typedef typename alloc_type::val_type   node_type;
    typedef typename node_type::val_type    val_type;
      // clang-format on
      template <class... Args>
      interval_node_storage(alloc_type& alc, Args&&...args)
        : node_type()
        , Alc(alc)
        , LI(forward<Args>(args)...) {}
      interval_node_storage(const interval_node_storage& ano) = default;
      constexpr void *impl(CMD cmd = COPY, void *arg = nullptr) override {
        switch(cmd) {
          case COPY :  // construct
            return construct_at(static_cast<interval_node_storage *>(Alc.allocate(sizeof(*this))), *this);
          case PINC :  //++
            ++LI;
            if(!LI) {
              return this->Next == nullptr ? nullptr : this->Next->impl(COPY);
            } else
              return this;
          case ADDR :  //*
            return reinterpret_cast<void *>(const_cast<val_type *>(addr(*LI)));
          case EQUA :  // equal
            auto ano = static_cast<interval_node_storage *>(arg);
            return ano->impl(ADDR) == nullptr ? nullptr : (LI == ano->LI ? arg : nullptr);
            // default :;
        }
        return nullptr;
      }
      constexpr ~interval_node_storage() override {
        destruct_at(addr(LI));
        Alc.deallocate(this);
      }
      alloc_type& Alc;
      func_type LI;
    };
    template <class _Alloc>
    class interval_node_storage<_Alloc, void> : public interval_node<typename _Alloc::val_type::val_type> {
    public:
      // clang-format off
    typedef _Alloc                          alloc_type;
    typedef typename alloc_type::val_type   node_type;
    typedef typename node_type::val_type    val_type;
      // clang-format on
      template <class... Args>
      interval_node_storage(alloc_type& alc)
        : node_type()
        , Alc(alc) {}
      interval_node_storage(const interval_node_storage& ano) = default;
      constexpr void *impl(CMD cmd = COPY, void *arg = nullptr) override {
        switch(cmd) {
          case COPY :  // construct
            return construct_at(reinterpret_cast<interval_node_storage *>(Alc.allocate(sizeof(*this))), *this);
          case PINC :  //++
            return this->Next == nullptr ? nullptr : this->Next->impl(COPY);
          case EQUA :  // equal
            return static_cast<interval_node_storage *>(arg)->impl(ADDR) == nullptr ? arg : nullptr;
          default :;
        }
        return nullptr;
      }
      constexpr ~interval_node_storage() override { Alc.deallocate(this); }
      alloc_type& Alc;
    };
    template <class Alloc, class... Args>
    interval_node_storage(Alloc&&, Args&&...)
      -> interval_node_storage<ts::rm_cvr<Alloc>, decltype(ranges::limited_interval(ts::tp::eval_type<Args>()...))>;
    template <class _Val>
    class interval_iter {
    public:
      // clang-format off
    typedef _Val                    val_type;
    typedef interval_node<val_type> node_type;
      // clang-format on
      interval_iter(node_type *ptr)
        : Cur(static_cast<node_type *>(ptr->impl(COPY)))
        , Ptr(ptr) {}
      constexpr interval_iter& operator++() {
        auto now = static_cast<node_type *>(Cur->impl(PINC));
        if(now != Cur) {
          Ptr = Cur->Next;
          destruct_at(Cur);
          Cur = now;
        }
        return *this;
      }
      constexpr val_type& operator*() { return *static_cast<val_type *>(Cur->impl(ADDR)); }
      constexpr bool operator!=(const interval_iter& ano) const {
        return Ptr != ano.Ptr || Cur->impl(EQUA, ano.Cur) == nullptr;
      }
      constexpr bool operator==(const interval_iter& ano) const {
        return Ptr == ano.Ptr && Cur->impl(EQUA, ano.Cur) != nullptr;
      }
      constexpr ~interval_iter() { destruct_at(Cur); }
      node_type *Cur, *Ptr;
    };
  }  // namespace impl
  template <class _Val, class _Alloc = default_allocator<_Val>>
  class interval {
  public:
    // clang-format off
    typedef _Val                                              val_type;
    typedef impl::interval_node<val_type>                     node_type;
    typedef typename _Alloc::template rebind_alloc<node_type> alloc_type;
    typedef impl::interval_node_storage<alloc_type,void>      empty_node_type;
    typedef xsl::impl::list<node_type,alloc_type>             storage_type;
    typedef impl::interval_iter<val_type>                     iter;
    // clang-format on
    template <class Iter>
    interval(const Iter& first, const Iter& last)
      : Alc()
      , Sto(Alc, xsl::construct_at(reinterpret_cast<empty_node_type *>(Alc.allocate(sizeof(empty_node_type))), Alc)) {
      merge(first, last);
    }
    template <class... Args,
      ts::enable<is_limited_interval<decltype(ranges::limited_interval(ts::tp::eval_type<Args>()...))>> = 0>
    interval& merge(Args&&...args) {
      typedef decltype(impl::interval_node_storage(Alc, forward<Args>(args)...)) ins_type;
      Sto.push_back(
        xsl::construct_at(reinterpret_cast<ins_type *>(Alc.allocate(sizeof(ins_type))), Alc, forward<Args>(args)...));
      return *this;
    }
    template <class Fn>
    interval& map(Fn&& f) {
      for(auto& e : *this) {
        f(e);
      }
      return *this;
    }
    iter begin() { return Sto.Head->Next; }
    iter end() { return Sto.Head; }

    alloc_type Alc;
    storage_type Sto;
  };
}  // namespace ranges
XSL_END
#endif  // !XSL_RANGES
