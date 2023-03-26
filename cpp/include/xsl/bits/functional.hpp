#pragma once
#ifndef XSL_FUNCTIONAL_IMPL
#define XSL_FUNCTIONAL_IMPL
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/tuple.hpp>

XSL_BEGIN
namespace fctl {
  //
  template <class...>
  struct function_impl;
  //
  template <class Ret, class... Args>
  struct function_impl<Ret(Args...)> {
    constexpr virtual void Copy(function_impl **) = 0;
    //
    constexpr virtual Ret call(Args...) = 0;
    //
    constexpr virtual ~function_impl(){};
  };
  //
  template <class Callee, class Ret, class... Args>
  struct function_impl<Callee, Ret(Args...)> : public function_impl<Ret(Args...)> {
    //
    Callee Res;
    //
    constexpr function_impl(const function_impl& ano) = default;
    //
    template <typename ActCallee, ts::enable_construct<function_impl, ActCallee> = 0>
    constexpr function_impl(ActCallee&& res)
      : Res(forward<ActCallee>(res)) {}
    //
    constexpr void Copy(function_impl<Ret(Args...)> **res) { *res = Construct<function_impl>(*this); }
    //
    constexpr Ret call(Args... params) { return Res(forward<Args>(params)...); }
    //
    constexpr ~function_impl() {}
  };
  //
  template <typename>
  class function;
  template <typename Ret, typename... Args>
  class function<Ret(Args...)> {
    using Tar = Ret(Args...);
    //
    function_impl<Tar> *Impl;
    //
  public:
    constexpr function()
      : Impl(nullptr) {}
    //
    template <typename Callee, ts::enable_construct<function, Callee> = 0>
    constexpr function(Callee&& res)
      : Impl(Construct<function_impl<ts::rm_ref<Callee>, Tar>>(forward<Callee>(res))) {}
    //
    constexpr function(function&& ano) {
      Impl = ano.Impl;
      ano.Impl = nullptr;
    }
    //
    constexpr function(const function& ano) { ano.Impl->Copy(&Impl); }
    //
    constexpr function& operator=(function&& ano) {
      this->~function();
      Impl = ano.Impl;
      ano.Impl = nullptr;
      return *this;
    }
    //
    constexpr function& operator=(const function& ano) {
      this->~function();
      ano.Impl->Copy(&Impl);
      return *this;
    }
    //
    template <typename... Tps>
    decltype(auto) operator()(Tps&&...params) {
      XSL_EMPTY_CHECK(Impl);
      return Impl->call(forward<Tps>(params)...);
    }
    //
    ~function() {
      if(Impl != nullptr) Destruct(Impl);
    }
  };
  //
  struct placeholder {};
  //
  template <typename Tp>
  inline constexpr bool is_placeholder = ts::is::same<Tp, placeholder>;
  //
  template <class _Callee, class... _Args>
  class binder {  // wrap bound callable object and arguments
  public:
    template <class ArgRep, class ArgSeqRep>
    class MixSeq_impl : public ts::tp::_1<ts::nt::remove_front<ArgSeqRep, 1 + ts::is::member_func_ptr<_Callee>>> {};
    template <class This, class... Rest, uint_8 Index, uint_8... ArgSeq>
    class MixSeq_impl<ts::tp::_n<This, Rest...>, ts::nt::uint8_s<Index, ArgSeq...>>
      : public MixSeq_impl<ts::tp::_n<Rest...>,
          ts::nt::uint8_s<Index + is_placeholder<ts::rm_cvr<This>>, ArgSeq..., Index>> {};
    // clang-format off
    typedef typename MixSeq_impl<ts::tp::_n<_Args...>, ts::nt::uint8_s<0>>::Type MixSeq;
    // clang-format on
    //   private:
    struct memfunc {};
    struct norfunc {};
    using FuncType = typename ts::conditional<ts::is::member_func_ptr<_Callee>, memfunc, norfunc>;
  public:
    template <class _Callee_, class _Arg_, class... _Args_>
    constexpr explicit binder(_Callee_&& callee, _Arg_&& arg, _Args_&&...args)
      : Callee(forward<_Callee_>(callee))
      , Package(forward<_Arg_>(arg), forward<_Args_>(args)...) {}
    template <class _Callee_, ts::enable_construct<binder, _Callee_> = 0>
    constexpr explicit binder(_Callee_&& callee)
      : Callee(forward<_Callee_>(callee))
      , Package() {}
    constexpr binder(const binder&) = default;
    constexpr binder(binder&&) = default;
    template <class... Unbound>
    constexpr auto operator()(Unbound&&...unbound)
    // -> decltype(Call(FuncType{},
    // ts::nt::make_index_sequence<sizeof...(_Args)>{}, MixSeq{}, tuple(tag_store{}, forward<Unbound>(unbound)...)))
    {
      return Call(FuncType{},
        ts::nt::remove_front<ts::nt::make_index_sequence<sizeof...(_Args)>, ts::is::member_func_ptr<_Callee>>{},
        MixSeq{}, tuple(tag_store{}, forward<Unbound>(unbound)...));
    }
    //   protected:
    template <typename TarRep, uint_8... Lid, uint_8... Rid>
    constexpr decltype(auto) Call(memfunc, ts::nt::uint8_s<Lid...>, ts::nt::uint8_s<Rid...>, TarRep&& tarrep) {
      return (Package.template get<0>()->*Callee)(Mixing<Lid, Rid>(tarrep)...);
    }
    //
    template <typename TarRep, uint_8... Lid, uint_8... Rid>
    constexpr decltype(auto) Call(norfunc, ts::nt::uint8_s<Lid...>, ts::nt::uint8_s<Rid...>, TarRep&& tarrep) {
      return Callee(Mixing<Lid, Rid>(tarrep)...);
    }
    //
    template <uint_8 Lid, uint_8 Rid, typename TarRep>
    constexpr decltype(auto) Mixing(TarRep& tarrep) {
      if constexpr(is_placeholder<ts::rm_cvr<ts::tp::find_by_id<Lid, _Args...>>>)
        return tarrep.template get<Rid>();
      else
        return Package.template get<Lid>();
    }
    _Callee Callee;
    tuple<_Args...> Package;
  };
  namespace impl {
    template <class Arg>
    using binder_forward_type_process = ts::conditional<is_wrraper<Arg>, ts::tp::extract<Arg>, ts::decay<Arg>>;
  }  // namespace impl
  template <class Callee, class... Args>
  binder(Callee, Args...)
    -> binder<impl::binder_forward_type_process<Callee>, impl::binder_forward_type_process<Args>...>;
  template <uint_8 Rest = 0, class Func, class... Args>
  constexpr decltype(auto) bind(Func&& func, Args&&...args) {
    typedef binder<impl::binder_forward_type_process<Func>, impl::binder_forward_type_process<Args>...> binder_type;
    return binder_type(forward<Func>(func), forward<Args>(args)...);
  }
  namespace impl {
    template <class Func, class... Args, class... Phs>  // placeholders
    constexpr decltype(auto) bind_front(ts::tp::_n<Phs...>, Func&& func, Args&&...args) {
      typedef binder<impl::binder_forward_type_process<Func>, impl::binder_forward_type_process<Args>..., Phs...>
        binder_type;
      return binder_type(forward<Func>(func), forward<Args>(args)..., Phs{}...);
    }
  }  // namespace impl
  template <uint_8 Rest = 0, class Func, class... Args>
  constexpr decltype(auto) bind_front(Func&& func, Args&&...args) {
    return impl::bind_front(ts::tp::repeat<Rest, placeholder>{}, forward<Func>(func), forward<Args>(args)...);
  }

  namespace impl {
    template <class Func, class... Args, class... Phs>  // placeholders
    constexpr decltype(auto) bind_back(ts::tp::_n<Phs...>, Func&& func, Args&&...args) {
      typedef binder<impl::binder_forward_type_process<Func>, Phs..., impl::binder_forward_type_process<Args>...>
        binder_type;
      return binder_type(func, Phs{}..., args...);
    }
  }  // namespace impl
  template <uint_8 Rest = 0, class Func, class... Args>
  constexpr decltype(auto) bind_back(Func&& func, Args&&...args) {
    return impl::bind_back(ts::tp::repeat<Rest, placeholder>{}, forward<Func>(func), forward<Args>(args)...);
  }
}  // namespace fctl
XSL_END
#include <functional>

#include <xsl/bits/test.hpp>

XSL_BEGIN
namespace test {
  namespace binder {
    void f00() { PROC_PRINT(void f00()); }
    void f01(test_p t) { PROC_PRINT(void f01(test_p t)); }
    void f02(test_p& t) { PROC_PRINT(void f02(test_p & t)); }
    void f03(const test_p& t) { PROC_PRINT(void f03(const test_p& t)); }
    void f04(int i, test_p t) { PROC_PRINT(void f04(int i, test_p t)); }
    void xsl_test_binder() {
      TEST_ACT_PRINT(test_p tmp{}; int i = 0);
      TEST_ACT_PRINT(auto tf00 = xsl::fctl::bind(&f00));
      TEST_ACT_PRINT(f00());
      TEST_ACT_PRINT(tf00());
      TEST_ACT_PRINT(auto tf11 = xsl::fctl::bind_front<1>(&f01));
      TEST_ACT_PRINT(auto tf12 = xsl::fctl::bind(&f01, tmp));
      TEST_ACT_PRINT(auto tf13 = xsl::fctl::bind(&f01, xsl::as_const(tmp)));
      TEST_ACT_PRINT(auto tf14 = xsl::fctl::bind(&f01, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(auto tf15 = xsl::fctl::bind(&f01, test_p{}));
      TEST_ACT_PRINT(f01(tmp));
      TEST_ACT_PRINT(tf11(tmp));
      TEST_ACT_PRINT(tf12());
      TEST_ACT_PRINT(f01(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf11(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf13());
      TEST_ACT_PRINT(f01(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf11(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf14());
      TEST_ACT_PRINT(f01(test_p{}));
      TEST_ACT_PRINT(tf11(test_p{}));
      TEST_ACT_PRINT(tf15());
      TEST_ACT_PRINT(auto tf21 = xsl::fctl::bind_front<1>(&f02));
      TEST_ACT_PRINT(auto tf22 = xsl::fctl::bind(&f02, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(auto tf23 = xsl::fctl::bind(&f02, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(f02(tmp));
      TEST_ACT_PRINT(tf21(tmp));
      TEST_ACT_PRINT(tf22());
      TEST_ACT_PRINT(f02(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf21(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf23());
      TEST_ACT_PRINT(auto tf31 = xsl::fctl::bind_front<1>(&f03));
      TEST_ACT_PRINT(auto tf32 = xsl::fctl::bind(&f03, tmp));
      TEST_ACT_PRINT(auto tf33 = xsl::fctl::bind(&f03, xsl::as_const(tmp)));
      TEST_ACT_PRINT(auto tf34 = xsl::fctl::bind(&f03, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(auto tf35 = xsl::fctl::bind(&f03, test_p{}));
      TEST_ACT_PRINT(f03(tmp));
      TEST_ACT_PRINT(tf31(tmp));
      TEST_ACT_PRINT(tf32());
      TEST_ACT_PRINT(f03(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf31(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf33());
      TEST_ACT_PRINT(f03(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf31(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf34());
      TEST_ACT_PRINT(f03(test_p{}));
      TEST_ACT_PRINT(tf31(test_p{}));
      TEST_ACT_PRINT(tf35());
      TEST_ACT_PRINT(auto tf41 = xsl::fctl::bind_front<2>(&f04));
      TEST_ACT_PRINT(auto tf42 = xsl::fctl::bind_front<1>(&f04, i));
      TEST_ACT_PRINT(auto tf43 = xsl::fctl::bind(&f04, xsl::fctl::placeholder{}, tmp));
      TEST_ACT_PRINT(auto tf44 = xsl::fctl::bind(&f04, i, tmp));
      TEST_ACT_PRINT(f04(i, tmp));
      TEST_ACT_PRINT(tf41(i, tmp));
      TEST_ACT_PRINT(tf42(tmp));
      TEST_ACT_PRINT(tf43(i));
      TEST_ACT_PRINT(tf44());
      TEST_ACT_PRINT(auto tf51 = xsl::fctl::bind_front<1>(&test_p::test_func00, &tmp));
      TEST_ACT_PRINT(auto tf52 = xsl::fctl::bind(&test_p::test_func00, &tmp, tmp));
      TEST_ACT_PRINT(auto tf61 = xsl::fctl::bind_front<1>(&test_p::test_func01, &tmp));
      TEST_ACT_PRINT(auto tf62 = xsl::fctl::bind(&test_p::test_func01, &tmp, tmp));
      TEST_ACT_PRINT(tmp.test_func00(tmp));
      TEST_ACT_PRINT(tf51(tmp));
      TEST_ACT_PRINT(tf52());
      TEST_ACT_PRINT(tmp.test_func01(tmp));
      TEST_ACT_PRINT(tf61(tmp));
      TEST_ACT_PRINT(tf62());
    }
    void std_test_binder() {
      TEST_ACT_PRINT(test_p tmp{}; int i = 0);
      TEST_ACT_PRINT(auto tf00 = std::bind(&f00));
      TEST_ACT_PRINT(f00());
      TEST_ACT_PRINT(tf00());
      TEST_ACT_PRINT(auto tf11 = std::bind_front(&f01));
      TEST_ACT_PRINT(auto tf12 = std::bind(&f01, tmp));
      TEST_ACT_PRINT(auto tf13 = std::bind(&f01, xsl::as_const(tmp)));
      TEST_ACT_PRINT(auto tf14 = std::bind(&f01, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(auto tf15 = std::bind(&f01, test_p{}));
      TEST_ACT_PRINT(f01(tmp));
      TEST_ACT_PRINT(tf11(tmp));
      TEST_ACT_PRINT(tf12());
      TEST_ACT_PRINT(f01(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf11(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf13());
      TEST_ACT_PRINT(f01(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf11(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf14());
      TEST_ACT_PRINT(f01(test_p{}));
      TEST_ACT_PRINT(tf11(test_p{}));
      TEST_ACT_PRINT(tf15());
      TEST_ACT_PRINT(auto tf21 = std::bind_front(&f02));
      TEST_ACT_PRINT(auto tf22 = std::bind(&f02, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(auto tf23 = std::bind(&f02, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(f02(tmp));
      TEST_ACT_PRINT(tf21(tmp));
      TEST_ACT_PRINT(tf22());
      TEST_ACT_PRINT(f02(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf21(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf23());
      TEST_ACT_PRINT(auto tf31 = std::bind_front(&f03));
      TEST_ACT_PRINT(auto tf32 = std::bind(&f03, tmp));
      TEST_ACT_PRINT(auto tf33 = std::bind(&f03, xsl::as_const(tmp)));
      TEST_ACT_PRINT(auto tf34 = std::bind(&f03, xsl::wrap_ref(tmp)));
      TEST_ACT_PRINT(auto tf35 = std::bind(&f03, test_p{}));
      TEST_ACT_PRINT(f03(tmp));
      TEST_ACT_PRINT(tf31(tmp));
      TEST_ACT_PRINT(tf32());
      TEST_ACT_PRINT(f03(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf31(xsl::as_const(tmp)));
      TEST_ACT_PRINT(tf33());
      TEST_ACT_PRINT(f03(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf31(xsl::as_lreference(tmp)));
      TEST_ACT_PRINT(tf34());
      TEST_ACT_PRINT(f03(test_p{}));
      TEST_ACT_PRINT(tf31(test_p{}));
      TEST_ACT_PRINT(tf35());
      TEST_ACT_PRINT(auto tf41 = std::bind_front(&f04));
      TEST_ACT_PRINT(auto tf42 = std::bind_front(&f04, i));
      TEST_ACT_PRINT(auto tf43 = std::bind(&f04, std::placeholders::_1, tmp));
      TEST_ACT_PRINT(auto tf44 = std::bind(&f04, i, tmp));
      TEST_ACT_PRINT(f04(i, tmp));
      TEST_ACT_PRINT(tf41(i, tmp));
      TEST_ACT_PRINT(tf42(tmp));
      TEST_ACT_PRINT(tf43(i));
      TEST_ACT_PRINT(tf44());
      TEST_ACT_PRINT(auto tf51 = std::bind_front(&test_p::test_func00, &tmp));
      TEST_ACT_PRINT(auto tf52 = std::bind(&test_p::test_func00, &tmp, tmp));
      TEST_ACT_PRINT(auto tf61 = std::bind_front(&test_p::test_func01, &tmp));
      TEST_ACT_PRINT(auto tf62 = std::bind(&test_p::test_func01, &tmp, tmp));
      TEST_ACT_PRINT(tmp.test_func00(tmp));
      TEST_ACT_PRINT(tf51(tmp));
      TEST_ACT_PRINT(tf52());
      TEST_ACT_PRINT(tmp.test_func01(tmp));
      TEST_ACT_PRINT(tf61(tmp));
      TEST_ACT_PRINT(tf62());
    }
  }  // namespace binder
}  // namespace test
XSL_END
#endif  // !XSL_FUNCTIONAL_IMPL
