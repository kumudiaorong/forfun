#pragma once
#ifndef XSL_GRAPH
#define XSL_GRAPH
#include"allocator.h"
#include"node.h"
XSL_BEGIN

// template <typename, uint_8>
// class graph;

// template<class Val>
// struct ls_ptr {
//     using List = branch_list<Val, 2>;
//     using Alloc = List::Alloc;
//     using Val_T = List::Val_T;
//     using Size_T = List::Size_T;

//     constexpr ls_ptr(Alloc& alc, Size_T off) :Alc(alc), Off(off) {}

//     constexpr Val_T& operator*() {
//         return Alc.Get_Val(Off).Obj;
//     }

//     constexpr ls_ptr& operator++() {
//         Off = Alc.Get_Val(Off).Next[1];
//         return *this;
//     }

//     constexpr ls_ptr operator++(int) {
//         Size_T Tmp = Off;
//         Off = Alc.Get_Val(Off).Next[1];
//         return { Alc,Tmp };
//     }

//     constexpr ls_ptr& operator--() {
//         Off = Alc.Get_Val(Off).Next[0];
//         return *this;
//     }

//     constexpr ls_ptr operator--(int) {
//         Size_T Tmp = Off;
//         Off = Alc.Get_Val(Off).Next[0];
//         return { Alc,Tmp };
//     }

//     constexpr bool operator==(const ls_ptr& Ano) {
//         return Off == Ano.Off;
//     }

//     constexpr bool operator!=(const ls_ptr& Ano) {
//         return Off != Ano.Off;
//     }
//     Alloc& Alc;
//     Size_T Off;
// };



// template<typename Crit, uint_8 Br>
// struct node {
//     using Val_T = typename Crit::Val_T;
//     using Size_T = typename Crit::Size_T;

//     Val_T Obj;
//     Size_T Next[Br];

//     constexpr node() :Obj(), Next() { }

//     template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
//     constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...), Next() {}

//     constexpr node(const node& Ano) : Obj(Ano.Obj) { Copy(var::make_index_sequence<Br>{}, Ano.Next); }
// private:
//     template<uint_8...Id>
//     constexpr void Copy(var::uint8_s<Id...>, const Size_T(&AnoNext)[Br]) {
//         unfold((Next[Id] = AnoNext[Id], 0)...);
//     }

// };
// template<typename Crit>
// struct node<Crit, 2> {
//     using Val_T = typename Crit::Val_T;
//     using Size_T = typename Crit::Size_T;

//     Val_T Obj;
//     Size_T Next[2];

//     constexpr node() :Obj(), Next() { }

//     template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
//     constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...), Next() {}

//     constexpr node(const node& Ano) : Obj(Ano.Obj) { Next[0] = Ano.Next[0], Next[1] = Ano.Next[1]; }

// };
// template<typename Crit>
// struct node<Crit, 0> {
//     using Val_T = typename Crit::Val_T;
//     using Size_T = typename Crit::Size_T;

//     Val_T Obj;

//     constexpr node() :Obj() { }

//     template<typename Arg, typename...Args, enable<!is_same<node, rm_cvr<Arg>>> = 0>
//     constexpr node(Arg&& param, Args&&...params) : Obj(forward<Arg>(param), forward<Args>(params)...) {}

//     constexpr node(const node& Ano) : Obj(Ano.Obj) {}

// };

template<class ET>
class edge {
public:

};





template <class Val, uint_8 Br>
class graph {
public:
    using Crit = criterion<Val>;


    using Size_T = Crit::Size_T;
    using Val_T = Val;

    using Node = node<Crit, Br>;

    using Alloc = indirect_allocator<criterion<Node>>;

    constexpr graph() :Alc(), Root(), Size() {}
    
    
    template<class...Args>
    constexpr void set_next(Size_T Off,uint_8 Id,Args&&...params) {

    }
private:
    // using Base_iter = Iter::Base;
    // template<typename...Args>
    // constexpr void insert_helper(_Node* node, Args&&... params) {
    // 	node->Pre = node->Pre->Next = new _Node(node->Pre, node, forward<Args>(params)...);
    // 	++Size;
    // }
    // constexpr void remove_helper(_Node* node)
    // {
    // 	(node->Pre->Next = node->Next)->Pre = node->Pre;
    // 	delete node;
    // 	--Size;
    // }
    // constexpr void erase_helper(_Node* Begin, _Node* End) {
    // 	_Node* tmp = Begin->Pre;
    // 	while (tmp->Next != End) {
    // 		remove_helper(Begin);
    // 		Begin = tmp->Next;
    // 	}
    // }
public:


    constexpr void push_back(const Val_T& obj) {
        Connect(Tail, Alc.Construct(obj));
    }

private:
    void Connect(Size_T Cur, Size_T Ins) {
        Alc.Get_Val(Alc.Get_Val(Ins).Next[0] = Alc.Get_Val(Cur).Next[0]).Next[1] = Ins;
        Alc.Get_Val(Alc.Get_Val(Ins).Next[1] = Cur).Next[0] = Ins;
    }

    Alloc Alc;
    Size_T Head, Tail, Size;         // size
};

template<class Val>
using list = branch_list<Val, 2>;
XSL_END
#endif // !XSL_GRAPH