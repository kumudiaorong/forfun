#pragma once
#ifndef XSL_BINDER_SUPPORT
#define XSL_BINDER_SUPPORT
#include <xsl/bits/allocator.hpp>
#include <xsl/bits/def.hpp>
#include <xsl/bits/ts/notp.hpp>
#include <xsl/bits/tuple/tuple.hpp>
// please complete the notes
namespace xsl::fn {
  struct placeholder {};
  //
  template <typename Tp>
  inline constexpr bool is_placeholder = ts::is::same<Tp, placeholder>;  // is_placeholder_v
  //
  template <class _Callee, class... _Args>
  class binder {                   // wrap bound callable object and arguments
    _Callee Callee;                // callable object
    tpl::tuple<_Args...> Package;  // arguments
    //
    struct memfunc {};  // member function
    struct norfunc {};  // normal function
    //
    template <class ArgRep, class ArgSeqRep>
    class mix_seq_impl : public ts::tp::_1<ts::nt::remove_front<ArgSeqRep, 1 + ts::is::member_func_ptr<_Callee>>> {};
    template <class This, class... Rest, uint_8 Index, uint_8... ArgSeq>
    class mix_seq_impl<ts::tp::_n<This, Rest...>, ts::nt::uint8_s<Index, ArgSeq...>>
      : public mix_seq_impl<ts::tp::_n<Rest...>,
          ts::nt::uint8_s<Index + is_placeholder<ts::rm::cvr<This>>, ArgSeq..., Index>> {};
    // clang-format off
    typedef typename mix_seq_impl<ts::tp::_n<_Args...>, ts::nt::uint8_s<0>>::type         mix_seq_type;//genenrate index sequence for required arguments
    typedef typename ts::conditional<ts::is::member_func_ptr<_Callee>, memfunc, norfunc>  func_type;
    // clang-format on
  public:
    template <class _Callee_, class _Arg_, class... _Args_>
    constexpr explicit binder(_Callee_&& callee, _Arg_&& arg, _Args_&&...args)// -> binder<_Callee_, _Arg_, _Args_...>
      : Callee(forward<_Callee_>(callee))
      , Package(forward<_Arg_>(arg), forward<_Args_>(args)...) {
    }
    template <class _Callee_, ts::enable_construct<binder, _Callee_> = 0>// -> binder<_Callee_>
    constexpr explicit binder(_Callee_&& callee)// -> binder<_Callee_>
      : Callee(forward<_Callee_>(callee))
      , Package() {
    }
    constexpr binder(const binder&) = default;
    constexpr binder(binder&&) = default;
    template <class... Unbound>
    constexpr auto operator()(Unbound&&...unbound)
    // -> decltype(Call(FuncType{},
    // ts::nt::make_index_sequence<sizeof...(_Args)>{}, MixSeq{}, tuple(tag_store{}, forward<Unbound>(unbound)...)))
    {
      return Call(func_type{},
        ts::nt::remove_front<ts::nt::make_index_sequence<sizeof...(_Args)>, ts::is::member_func_ptr<_Callee>>{},//if member function, remove the first index
        mix_seq_type{}, tpl::tuple(tag_store{}, forward<Unbound>(unbound)...));
    }
    //   protected:
    template <typename TarRep, uint_8... Lid, uint_8... Rid>
    constexpr decltype(auto) Call(memfunc, ts::nt::uint8_s<Lid...>, ts::nt::uint8_s<Rid...>, TarRep&& tarrep) {
      return (tpl::get<0>(Package)->*Callee)(Mixing<Lid, Rid>(tarrep)...);
    }
    //
    template <typename TarRep, uint_8... Lid, uint_8... Rid>
    constexpr decltype(auto) Call(norfunc, ts::nt::uint8_s<Lid...>, ts::nt::uint8_s<Rid...>, TarRep&& tarrep) {
      return Callee(Mixing<Lid, Rid>(tarrep)...);
    }
    //
    template <uint_8 Lid, uint_8 Rid, typename TarRep>
    constexpr decltype(auto) Mixing(TarRep& tarrep) {
      if constexpr(is_placeholder<ts::rm::cvr<ts::tp::find_by_id<Lid, _Args...>>>)
        return tpl::get<Rid>(tarrep);
      else
        return tpl::get<Lid>(Package);
    }
  };
  namespace impl {
    template <class Arg>
    using binder_forward_type_process = ts::conditional<is_wrraper<Arg>, ts::tp::get_type<Arg>, ts::decay<Arg>>;
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
}  // namespace xsl::fn
#endif