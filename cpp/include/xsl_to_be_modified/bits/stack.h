#pragma once
#ifndef XSL_STACK
#define XSL_STACK
#include"vector.h"
XSL_BEGIN
template<typename _Val>
class stack {
public:
    using Storage = vector<_Val>;
    using Crit = criterion<_Val>;
    using Val_T = _Val;
    using Ptr_T = _Val*;
    using Ref_T = _Val&;
    using CRef_T =const _Val&;
    using Size_T = Crit::Size_T;

    constexpr stack() :Res() {}
    // constexpr vector(const vector& Ano) : Cap(Ano.Cap), Size(Ano.Size) {
    // 	//trace("vector(const vector& v)");
    // 	erase_helper(Head, Head + Size);
    // 	insert_helper_1(Head, v.Head, v.Size);
    // }
    // constexpr Val& at(uint_t id) {
    // 	return Head[id];
    // }
    // constexpr void clear() {
    // 	//trace("clear()");
    // 	erase_helper(Head, Head + Size);
    // }
    constexpr bool empty() {
        return Res.empty();
    }
    template<typename...Args>
    constexpr void emplace(Args&&...params) {
        Res.emplace_back(forward<Args>(params)...);
    }
    constexpr void push(CRef_T Val) {
        Res.emplace_back(Val);
    }
    constexpr void pop() {
        Res.pop_back();
    }
    constexpr Ref_T top() {
        return Res.back();
    }
    constexpr Size_T size() {
        return Res.size();
    }
private:
    Storage Res;
};
XSL_END
#endif // !XSL_STACK
