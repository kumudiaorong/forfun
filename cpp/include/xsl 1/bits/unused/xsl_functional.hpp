namespace special {
  template <typename, typename...>
  struct binder_proc;
  //
  template <typename Ret, typename... SrcArgs, typename... CurActArgs>
  struct binder_proc<Ret (*)(SrcArgs...), CurActArgs...> {
    //
    static_assert(sizeof...(SrcArgs) >= sizeof...(CurActArgs), "too many parameters");
    //
    using Tuple = typename ts::tp::merge<tuple<CurActArgs...>,
      ts::tp::repeat<sizeof...(SrcArgs) - sizeof...(CurActArgs), placeholder>>::Type2;
    // using Tuple = ts::tp::fill_back<tuple<CurActArgs...>, sizeof...(SrcArgs) - sizeof...(CurActArgs),
    // placeholder>;
    //
    template <typename, typename, typename ArgsSeqRep>
    struct Proc_impl : ts::tp::_1<ts::tp::remove_first<ArgsSeqRep>> {};
    //
    template <typename SrcThis, typename... SrcRest, typename CurThis, typename... CurRest, uint_8 TarId,
      uint_8... ArgsSeq>
    struct Proc_impl<ts::tp::_n<SrcThis, SrcRest...>, tuple<CurThis, CurRest...>, ts::nt::uint8_s<TarId, ArgsSeq...>>
      : Proc_impl<ts::tp::_n<SrcRest...>, tuple<CurRest...>,
          ts::conditional<is_placeholder<CurThis>, ts::nt::uint8_s<TarId + 1, ArgsSeq..., TarId>,
            ts::nt::uint8_s<TarId, ArgsSeq..., 0>>> {
      static_assert(is_placeholder<CurThis> || ts::tp::is_constructible<SrcThis, CurThis>, "wrong parameter");
    };
    using ProcRet =
      ts::tp::_3<void, Tuple, typename Proc_impl<ts::tp::_n<SrcArgs...>, Tuple, ts::nt::uint8_s<0>>::Type>;
  };
  //
#define XSL_FILL_OPT_CONST(ATTRIB)                                                                      \
template <typename Ret, typename Class, typename ActClass, typename... SrcArgs, typename... CurActArgs> \
struct binder_proc<Ret (Class::*)(SrcArgs...) ATTRIB, ActClass, CurActArgs...> {                        \
using MidRet = typename binder_proc<Ret (*)(SrcArgs...), CurActArgs...>::ProcRet;                       \
using ProcRet = ts::tp::_3<ActClass, typename MidRet::Type2, typename MidRet::Type3>;                   \
};
  XSL_FILL_OPT(1, XSL_FILL_OPT_CONST, const);
#undef XSL_FILL_OPT_CONST
  //
  template <typename Src, typename Tuple, typename ClassPtr>
  struct binder_store {
    Src Ptr;
    ClassPtr Obj;
    Tuple CurRep;
    template <typename... CurActArgs>
    binder_store(Src ptr, ClassPtr obj, CurActArgs&&...curparams)
      : Ptr(ptr)
      , Obj(obj)
      , CurRep(forward<CurActArgs&&>(curparams)...) {}
  };
  //
  template <typename Src, typename Tuple>
  struct binder_store<Src, Tuple, void> {
    Src Ptr;
    Tuple CurRep;
    template <typename... CurActArgs>
    binder_store(Src ptr, CurActArgs&&...curparams)
      : Ptr(ptr)
      , CurRep(forward<CurActArgs&&>(curparams)...) {}
  };
  //
  template <typename Src, typename... CurActArgs>
  class binder {
  public:
    using ProcRet = typename binder_proc<Src, CurActArgs...>::ProcRet;
    using Tuple = typename ProcRet::Type2;
    using ArgsSeq = typename ProcRet::Type3;
    //
    struct memfunc {};
    struct norfunc {};
    //
    using FuncType = ts::conditional<ts::is_member_func_ptr<Src>, memfunc, norfunc>;
    using Storage = binder_store<Src, Tuple, typename ProcRet::Type1>;
    //
    Storage Res;
    //
    template <uint_8 Lid, uint_8 Rid, typename TarRep>
    constexpr decltype(auto) Mixing(TarRep& tarrep) {
      using CurArg = ts::tp::find_by_id_with_rep<Tuple, Lid>;
      if constexpr(is_placeholder<CurArg>)
        return tarrep.template get<Rid>();
      else
        return static_cast<ts::conditional<ts::is_same<CurArg, ts::rm_cvr<CurArg>>, CurArg&&, CurArg>>(
          Res.CurRep.template get<Lid>());
    }
    //
    template <typename TarRep, uint_8... Lid, uint_8... Rid>
    constexpr decltype(auto) Call(memfunc, ts::nt::uint8_s<Lid...>, ts::nt::uint8_s<Rid...>, TarRep&& tarrep) {
      return (Res.Obj->*Res.Ptr)(Mixing<Lid, Rid>(tarrep)...);
    }
    //
    template <typename TarRep, uint_8... Lid, uint_8... Rid>
    constexpr decltype(auto) Call(norfunc, ts::nt::uint8_s<Lid...>, ts::nt::uint8_s<Rid...>, TarRep&& tarrep) {
      return (Res.Ptr)(Mixing<Lid, Rid>(tarrep)...);
    }
    //
  public:
    constexpr binder(binder&& ano) = default;
    constexpr binder(const binder& ano) = default;
    constexpr binder& operator=(const binder& ano) = default;
    constexpr binder& operator=(binder&& ano) = default;
    //
    constexpr binder(Src ptr, CurActArgs&&...params)
      : Res(ptr, forward<CurActArgs&&>(params)...) {}
    //
    template <typename... TarActArgs>
    constexpr decltype(auto) operator()(TarActArgs&&...tarparams) {
      return Call(FuncType{}, ts::nt::make_index_sequence<ts::tp::size<Tuple>>{}, ArgsSeq{},
        tuple(tag_store{}, static_cast<TarActArgs&&>(tarparams)...));
    }
  };
  //
  template <typename Src, typename... CurActArgs>
  binder(Src, CurActArgs&&...) -> binder<Src, CurActArgs...>;
  //
#ifdef XSL_TEST
  void f00() { PROC_PRINT(void f00()); }
  void f01(test_p t) { PROC_PRINT(void f01(test_p t)); }
  void f02(test_p& t) { PROC_PRINT(void f02(test_p & t)); }
  void f03(const test_p& t) { PROC_PRINT(void f03(const test_p& t)); }
  void f04(int i, test_p t) { PROC_PRINT(void f04(int i, test_p t)); }
  void xsl_test_binder() {
    TEST_ACT_PRINT(test_p tmp{}; int i = 0);
    TEST_ACT_PRINT(auto tf00 = binder(&f00));
    TEST_ACT_PRINT(f00());
    TEST_ACT_PRINT(tf00());
    TEST_ACT_PRINT(auto tf11 = binder(&f01));
    TEST_ACT_PRINT(auto tf12 = binder(&f01, tmp));
    TEST_ACT_PRINT(auto tf13 = binder(&f01, as_const(tmp)));
    TEST_ACT_PRINT(auto tf14 = binder(&f01, as_lreference(tmp)));
    TEST_ACT_PRINT(auto tf15 = binder(&f01, test_p{}));
    TEST_ACT_PRINT(f01(tmp));
    TEST_ACT_PRINT(tf11(tmp));
    TEST_ACT_PRINT(tf12());
    TEST_ACT_PRINT(f01(as_const(tmp)));
    TEST_ACT_PRINT(tf11(as_const(tmp)));
    TEST_ACT_PRINT(tf13());
    TEST_ACT_PRINT(f01(as_lreference(tmp)));
    TEST_ACT_PRINT(tf11(as_lreference(tmp)));
    TEST_ACT_PRINT(tf14());
    TEST_ACT_PRINT(f01(test_p{}));
    TEST_ACT_PRINT(tf11(test_p{}));
    TEST_ACT_PRINT(tf15());
    TEST_ACT_PRINT(auto tf21 = binder(&f02));
    TEST_ACT_PRINT(auto tf22 = binder(&f02, tmp));
    TEST_ACT_PRINT(auto tf23 = binder(&f02, as_lreference(tmp)));
    TEST_ACT_PRINT(f02(tmp));
    TEST_ACT_PRINT(tf21(tmp));
    TEST_ACT_PRINT(tf22());
    TEST_ACT_PRINT(f02(as_lreference(tmp)));
    TEST_ACT_PRINT(tf21(as_lreference(tmp)));
    TEST_ACT_PRINT(tf23());
    TEST_ACT_PRINT(auto tf31 = binder(&f03));
    TEST_ACT_PRINT(auto tf32 = binder(&f03, tmp));
    TEST_ACT_PRINT(auto tf33 = binder(&f03, as_const(tmp)));
    TEST_ACT_PRINT(auto tf34 = binder(&f03, as_lreference(tmp)));
    TEST_ACT_PRINT(auto tf35 = binder(&f03, test_p{}));
    TEST_ACT_PRINT(f03(tmp));
    TEST_ACT_PRINT(tf31(tmp));
    TEST_ACT_PRINT(tf32());
    TEST_ACT_PRINT(f03(as_const(tmp)));
    TEST_ACT_PRINT(tf31(as_const(tmp)));
    TEST_ACT_PRINT(tf33());
    TEST_ACT_PRINT(f03(as_lreference(tmp)));
    TEST_ACT_PRINT(tf31(as_lreference(tmp)));
    TEST_ACT_PRINT(tf34());
    TEST_ACT_PRINT(f03(test_p{}));
    TEST_ACT_PRINT(tf31(test_p{}));
    TEST_ACT_PRINT(tf35());
    TEST_ACT_PRINT(auto tf41 = binder(&f04));
    TEST_ACT_PRINT(auto tf42 = binder(&f04, i));
    TEST_ACT_PRINT(auto tf43 = binder(&f04, placeholder{}, tmp));
    TEST_ACT_PRINT(auto tf44 = binder(&f04, i, tmp));
    TEST_ACT_PRINT(f04(i, tmp));
    TEST_ACT_PRINT(tf41(i, tmp));
    TEST_ACT_PRINT(tf42(tmp));
    TEST_ACT_PRINT(tf43(i));
    TEST_ACT_PRINT(tf44());
    TEST_ACT_PRINT(auto tf51 = binder(&test_p::test_func00, &tmp));
    TEST_ACT_PRINT(auto tf52 = binder(&test_p::test_func00, &tmp, tmp));
    TEST_ACT_PRINT(auto tf61 = binder(&test_p::test_func01, &tmp));
    TEST_ACT_PRINT(auto tf62 = binder(&test_p::test_func01, &tmp, tmp));
    TEST_ACT_PRINT(tmp.test_func00(tmp));
    TEST_ACT_PRINT(tf51(tmp));
    TEST_ACT_PRINT(tf52());
    TEST_ACT_PRINT(tmp.test_func01(tmp));
    TEST_ACT_PRINT(tf61(tmp));
    TEST_ACT_PRINT(tf62());
  }
#endif  // !XSL_TEST
}  // namespace special