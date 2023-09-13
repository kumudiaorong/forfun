#pragma once
#ifndef XSL_FUNCTION_SUPPORT
#define XSL_FUNCTION_SUPPORT
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/tuple/tuple.hpp>
#include <xsl/bits/ts/notp.hpp>

namespace xsl::fn {
  //
  namespace impl {
    template <class...>
    struct function;
    //
    template <class Ret, class... Args>
    struct function<Ret(Args...)> {
      constexpr virtual void Copy(function **) = 0;
      //
      constexpr virtual Ret call(Args...) = 0;
      //
      constexpr virtual ~function(){};
    };
    //
    template <class Callee, class Ret, class... Args>
    struct function<Callee, Ret(Args...)> : public function<Ret(Args...)> {
      //
      Callee Res;
      //
      constexpr function(const function& ano) = default;
      //
      template <typename ActCallee, ts::enable_construct<function, ActCallee> = 0>
      constexpr function(ActCallee&& res)
        : Res(forward<ActCallee>(res)) {
      }
      //
      constexpr void Copy(function<Ret(Args...)> **res) {
        *res = Construct<function>(*this);
      }
      //
      constexpr Ret call(Args... params) {
        return Res(forward<Args>(params)...);
      }
      //
      constexpr ~function() {
      }
    };
  }  // namespace impl
  //
  template <typename>
  class function;
  template <typename Ret, typename... Args>
  class function<Ret(Args...)> {
    using Tar = Ret(Args...);
    //
    impl::function<Tar> *Impl;
    //
  public:
    constexpr function()
      : Impl(nullptr) {
    }
    //
    template <typename Callee, ts::enable_construct<function, Callee> = 0>
    constexpr function(Callee&& res)
      : Impl(Construct<impl::function<ts::rm::ref<Callee>, Tar>>(forward<Callee>(res))) {
    }
    //
    constexpr function(function&& ano) {
      Impl = ano.Impl;
      ano.Impl = nullptr;
    }
    //
    constexpr function(const function& ano) {
      ano.Impl->Copy(&Impl);
    }
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
      if(Impl != nullptr)
        Destruct(Impl);
    }
  };
  //
}  // namespace xsl::fn
#endif  // !XSL_FUNCTIONAL_IMPL
