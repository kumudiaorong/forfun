#ifndef XSL_FUNCTION
#define XSL_FUNCTION
#include"tuple.h"
#include"allocator.h"

XSL_BEGIN
//
template<class...>
struct function_impl;
//
template<class Ret, class...Args>
struct function_impl<Ret(Args...)> {
	constexpr virtual void Copy(function_impl**) = 0;
	//
	constexpr virtual Ret call(Args...) = 0;
	//
	constexpr virtual ~function_impl() {};
};
//
template<class Callee, class Ret, class...Args>
struct function_impl<Callee, Ret(Args...)> :public function_impl<Ret(Args...)> {
	//
	Callee Res;
	//
	constexpr function_impl(const function_impl& ano) = default;
	//
	template<typename ActCallee, enable_construct<function_impl, ActCallee> = 0>
	constexpr function_impl(ActCallee&& res) :Res(forward<ActCallee>(res)) {}
	//
	constexpr void Copy(function_impl<Ret(Args...)>** res) {
		*res = Construct<function_impl>(*this);
	}
	//
	constexpr Ret call(Args...params) {
		return Res(forward<Args>(params)...);
	}
	//
	constexpr ~function_impl() {}
};
//
template<typename>
class function;
template<typename Ret, typename...Args>
class function<Ret(Args...)> {
	using Tar = Ret(Args...);
	//
	function_impl<Tar>* Impl;
	//
public:
	constexpr function() :Impl(nullptr) {}
	//
	template<typename Callee, enable_construct<function, Callee> = 0>
	constexpr function(Callee&& res) : Impl(Construct<function_impl<rm_ref<Callee>, Tar>>(forward<Callee>(res))) {}
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
	template<typename...Tps>
	decltype(auto) operator()(Tps&&...params) {
		XSL_EMPTY_CHECK(Impl);
		return Impl->call(forward<Tps>(params)...);
	}
	//
	~function() {
		if (Impl != nullptr)Destruct(Impl);
	}
};
//
struct placeholder {};
//
template<typename Tp>
inline constexpr bool is_placeholder = is_same<Tp, placeholder>;
//
template<typename, typename...>
struct adapter_proc;
//
template<typename Ret, typename...SrcArgs, typename...CurActArgs>
struct adapter_proc<Ret(*)(SrcArgs...), CurActArgs...> {
	//
	static_assert(sizeof...(SrcArgs) >= sizeof...(CurActArgs), "too many parameters");
	//
	using Tuple = type::fill_back<tuple<CurActArgs...>, type::_n<placeholder>, sizeof...(SrcArgs) - sizeof...(CurActArgs)>;
	//
	template<typename, typename, typename ArgsSeqRep>
	struct Proc_impl :type::_1< var::remove_first<ArgsSeqRep>> {};
	//
	template<typename SrcThis, typename...SrcRest, typename CurThis, typename...CurRest, uint8_t TarId, uint8_t...ArgsSeq>
	struct Proc_impl<type::_n<SrcThis, SrcRest...>, tuple<CurThis, CurRest...>, var::uint8_s<TarId, ArgsSeq...>> :
		Proc_impl<type::_n<SrcRest...>, tuple<CurRest...>, type::conditional< is_placeholder<CurThis>, var::uint8_s<TarId + 1, ArgsSeq..., TarId>, var::uint8_s<TarId, ArgsSeq..., 0>> > {
		static_assert(is_placeholder<CurThis> || is_constructible<SrcThis, CurThis>, "wrong parameter");
	};
	using ProcRet = type::_3<void, Tuple, typename Proc_impl<type::_n<SrcArgs...>, Tuple, var::uint8_s<0>>::Type>;
};
//
#define XSL_ADAPTER_PROC(ATTRIB)\
template<typename Ret, typename Class, typename ActClass, typename...SrcArgs, typename...CurActArgs>\
struct adapter_proc<Ret(Class::*)(SrcArgs...)ATTRIB, ActClass, CurActArgs...> {	\
	using MidRet =typename adapter_proc < Ret(*)(SrcArgs...), CurActArgs...>::ProcRet;	\
	using ProcRet = type::_3<ActClass, typename MidRet::Type2, typename MidRet::Type3>;	\
};
XSL_OPT_CONST_0(XSL_ADAPTER_PROC);
#undef XSL_ADAPTER_PROC
//
template<typename Src, typename Tuple, typename ClassPtr>
struct adapter_store {
	Src Ptr;
	ClassPtr Obj;
	Tuple CurRep;
	template<typename...CurActArgs>
	adapter_store(Src ptr, ClassPtr obj, CurActArgs&&...curparams) :Ptr(ptr), Obj(obj), CurRep(forward<CurActArgs&&>(curparams)...) {}
};
//
template<typename Src, typename Tuple>
struct adapter_store<Src, Tuple, void> {
	Src Ptr;
	Tuple CurRep;
	template<typename...CurActArgs>
	adapter_store(Src ptr, CurActArgs&&...curparams) :Ptr(ptr), CurRep(forward<CurActArgs&&>(curparams)...) {}
};
//
template<typename Src, typename...CurActArgs>
class adapter {
public:
	using ProcRet = typename adapter_proc<Src, CurActArgs...>::ProcRet;
	using Tuple = typename ProcRet::Type2;
	using ArgsSeq = typename ProcRet::Type3;
	//
	struct memfunc {};
	struct norfunc {};
	//
	using FuncType = type::conditional<is_member_func<Src>, memfunc, norfunc>;
	using Storage = adapter_store<Src, Tuple, typename ProcRet::Type1>;
	//
	Storage Res;
	//
	template<uint8_t Lid, uint8_t Rid, typename TarRep>
	constexpr decltype(auto) Mixing(TarRep& tarrep) {
		using CurArg = type::find_by_id<Tuple, Lid>;
		if constexpr (is_placeholder<CurArg>)return tarrep.template get<Rid>();
		else return static_cast<type::conditional<is_same<CurArg, rm_cvr<CurArg>>, CurArg&&, CurArg>>(Res.CurRep.template get<Lid>());
	}
	//
	template<typename TarRep, uint8_t...Lid, uint8_t...Rid>
	constexpr decltype(auto) Call(memfunc, var::uint8_s<Lid...>, var::uint8_s<Rid...>, TarRep&& tarrep) {
		return (Res.Obj->*Res.Ptr)(Mixing<Lid, Rid>(tarrep)...);
	}
	//
	template<typename TarRep, uint8_t...Lid, uint8_t...Rid>
	constexpr decltype(auto) Call(norfunc, var::uint8_s<Lid...>, var::uint8_s<Rid...>, TarRep&& tarrep) {
		return (Res.Ptr)(Mixing<Lid, Rid>(tarrep)...);
	}
	//
public:
	constexpr adapter(adapter&& ano) = default;
	constexpr adapter(const adapter& ano) = default;
	constexpr adapter& operator=(const adapter& ano) = default;
	constexpr adapter& operator=(adapter&& ano) = default;
	//
	constexpr adapter(Src ptr, CurActArgs&&...params) :Res(ptr, forward<CurActArgs&&>(params)...) {}
	//
	template<typename...TarActArgs>
	constexpr decltype(auto) operator()(TarActArgs&&...tarparams) {
		return Call(FuncType{}, var::make_index_sequence<type::size<Tuple>>{}, ArgsSeq{}, tuple(tuple_tag::store{}, static_cast<TarActArgs&&>(tarparams)...));
	}
};
//
template<typename Src, typename...CurActArgs>
adapter(Src, CurActArgs&&...) -> adapter<Src, CurActArgs...>;
//
#ifdef XSL_TEST
void f00() {
	PROC_PRINT(void f00());
}
void f01(test t) {
	PROC_PRINT(void f01(test t));
}
void f02(test& t) {
	PROC_PRINT(void f02(test & t));
}
void f03(const test& t) {
	PROC_PRINT(void f03(const test & t));
}
void f04(int i, test t) {
	PROC_PRINT(void f04(int i, test t));
}
void xsl_test_adapter() {
	TEST_ACT_PRINT(test tmp{}; int i = 0);
	TEST_ACT_PRINT(auto tf00 = adapter(&f00));
	TEST_ACT_PRINT(f00());
	TEST_ACT_PRINT(tf00());
	TEST_ACT_PRINT(auto tf11 = adapter(&f01));
	TEST_ACT_PRINT(auto tf12 = adapter(&f01, tmp));
	TEST_ACT_PRINT(auto tf13 = adapter(&f01, as_const(tmp)));
	TEST_ACT_PRINT(auto tf14 = adapter(&f01, as_lreference(tmp)));
	TEST_ACT_PRINT(auto tf15 = adapter(&f01, test{}));
	TEST_ACT_PRINT(f01(tmp));
	TEST_ACT_PRINT(tf11(tmp));
	TEST_ACT_PRINT(tf12());
	TEST_ACT_PRINT(f01(as_const(tmp)));
	TEST_ACT_PRINT(tf11(as_const(tmp)));
	TEST_ACT_PRINT(tf13());
	TEST_ACT_PRINT(f01(as_lreference(tmp)));
	TEST_ACT_PRINT(tf11(as_lreference(tmp)));
	TEST_ACT_PRINT(tf14());
	TEST_ACT_PRINT(f01(test{}));
	TEST_ACT_PRINT(tf11(test{}));
	TEST_ACT_PRINT(tf15());
	TEST_ACT_PRINT(auto tf21 = adapter(&f02));
	TEST_ACT_PRINT(auto tf22 = adapter(&f02, tmp));
	TEST_ACT_PRINT(auto tf23 = adapter(&f02, as_lreference(tmp)));
	TEST_ACT_PRINT(f02(tmp));
	TEST_ACT_PRINT(tf21(tmp));
	TEST_ACT_PRINT(tf22());
	TEST_ACT_PRINT(f02(as_lreference(tmp)));
	TEST_ACT_PRINT(tf21(as_lreference(tmp)));
	TEST_ACT_PRINT(tf23());
	TEST_ACT_PRINT(auto tf31 = adapter(&f03));
	TEST_ACT_PRINT(auto tf32 = adapter(&f03, tmp));
	TEST_ACT_PRINT(auto tf33 = adapter(&f03, as_const(tmp)));
	TEST_ACT_PRINT(auto tf34 = adapter(&f03, as_lreference(tmp)));
	TEST_ACT_PRINT(auto tf35 = adapter(&f03, test{}));
	TEST_ACT_PRINT(f03(tmp));
	TEST_ACT_PRINT(tf31(tmp));
	TEST_ACT_PRINT(tf32());
	TEST_ACT_PRINT(f03(as_const(tmp)));
	TEST_ACT_PRINT(tf31(as_const(tmp)));
	TEST_ACT_PRINT(tf33());
	TEST_ACT_PRINT(f03(as_lreference(tmp)));
	TEST_ACT_PRINT(tf31(as_lreference(tmp)));
	TEST_ACT_PRINT(tf34());
	TEST_ACT_PRINT(f03(test{}));
	TEST_ACT_PRINT(tf31(test{}));
	TEST_ACT_PRINT(tf35());
	TEST_ACT_PRINT(auto tf41 = adapter(&f04));
	TEST_ACT_PRINT(auto tf42 = adapter(&f04, i));
	TEST_ACT_PRINT(auto tf43 = adapter(&f04, placeholder{}, tmp));
	TEST_ACT_PRINT(auto tf44 = adapter(&f04, i, tmp));
	TEST_ACT_PRINT(f04(i, tmp));
	TEST_ACT_PRINT(tf41(i, tmp));
	TEST_ACT_PRINT(tf42(tmp));
	TEST_ACT_PRINT(tf43(i));
	TEST_ACT_PRINT(tf44());
	TEST_ACT_PRINT(auto tf51 = adapter(&test::test_func00, &tmp));
	TEST_ACT_PRINT(auto tf52 = adapter(&test::test_func00, &tmp, tmp));
	TEST_ACT_PRINT(auto tf61 = adapter(&test::test_func01, &tmp));
	TEST_ACT_PRINT(auto tf62 = adapter(&test::test_func01, &tmp, tmp));
	TEST_ACT_PRINT(tmp.test_func00(tmp));
	TEST_ACT_PRINT(tf51(tmp));
	TEST_ACT_PRINT(tf52());
	TEST_ACT_PRINT(tmp.test_func01(tmp));
	TEST_ACT_PRINT(tf61(tmp));
	TEST_ACT_PRINT(tf62());
}
#endif // !XSL_TEST
XSL_END
#endif // !XSL_FUNCTION
