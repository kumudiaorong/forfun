#pragma once
#ifndef XSL_TREE
#define XSL_TREE
#include"allocator.h"
#include"iterator.h"
XSL_BEGIN

template <typename Val, uint_8 Br>
class tree {
public:
    using Crit = criterion<Val>;

    using Size_T = Crit::Size_T;
    using Val_T = Val;

    using Node = node<Crit, Br>;

    using Alloc = indirect_allocator<criterion<Node>>;


    constexpr tree() :Alc(), Root(), Size() {}
protected:
    constexpr void set_next(Size_T Par, uint_8 Id, Size_T Ch) {
        Alc.Get_Val(In(Par)).Next[Id] = Ch;
        Alc.Get_Val(In(Ch)).Next[0] = Par;
    }
private:
    constexpr Size_T In(Size_T Off) {
        return Off - 1;
    }
    constexpr Size_T Out(Size_T Off) {
        return Off + 1;
    }

protected:
    Alloc Alc;
    Size_T Root, Size;         // size
};

XSL_END
#endif // !TLS_BT_LIST