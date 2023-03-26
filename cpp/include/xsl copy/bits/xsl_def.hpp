#ifndef XSL_BASE_DEF
#define XSL_BASE_DEF
//x standard library
#include"pf/def.h"
inline void pass() {}
static int n1{}, n2{}, n3{}, n4{ }, n5{ };

XSL_BEGIN

#define XSL_EMPTR_MACRO
#define XSL_ARGS_EXPAND(...)  __VA_ARGS__
#define XSL_ADD_COMMA_(ARG)  ,ARG
#define XSL_ADD_COMMA(...)  __VA_OPT__(,)__VA_ARGS__
#define XSL_VA_ARGS_PACK(PACKAGE)  XSL_VA_ARGS_EXPAND PACKAGE


#define XSL_MACRO_ARGS_CONTER_HELPER(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_N, ...) _N
#define XSL_MACRO_ARGS_CONTER(...)  XSL_MACRO_ARGS_CONTER_HELPER(__VA_ARGS__ __VA_OPT__(,) 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define _XSL_FILL_NULL_ARGS_1(MCTR)		MCTR(
#define _XSL_FILL_NULL_ARGS_2(MCTR)		_XSL_FILL_NULL_ARGS_1(MCTR),
#define _XSL_FILL_NULL_ARGS_3(MCTR)		_XSL_FILL_NULL_ARGS_2(MCTR),
#define _XSL_FILL_NULL_ARGS_4(MCTR)		_XSL_FILL_NULL_ARGS_3(MCTR),
#define _XSL_FILL_NULL_ARGS_5(MCTR)		_XSL_FILL_NULL_ARGS_4(MCTR),
#define _XSL_FILL_NULL_ARGS_6(MCTR)		_XSL_FILL_NULL_ARGS_5(MCTR),
#define XSL_FILL_NULL_ARGS(N,MCTR)	_XSL_FILL_NULL_ARGS_##N(MCTR))
//
//
#define _XSL_REVERSE_ARGS_0(MCTR,...)			MCTR(__VA_ARGS__
#define _XSL_REVERSE_ARGS_1(MCTR,ARG,...)		_XSL_REVERSE_ARGS_0(MCTR __VA_OPT__(,)__VA_ARGS__)__VA_OPT__(,)ARG
#define _XSL_REVERSE_ARGS_2(MCTR,ARG,...)		_XSL_REVERSE_ARGS_1(MCTR,__VA_ARGS__),ARG
#define _XSL_REVERSE_ARGS_3(MCTR,ARG,...)		_XSL_REVERSE_ARGS_2(MCTR,__VA_ARGS__),ARG
#define _XSL_REVERSE_ARGS_4(MCTR,ARG,...)		_XSL_REVERSE_ARGS_3(MCTR,__VA_ARGS__),ARG
#define _XSL_REVERSE_ARGS_5(MCTR,ARG,...)		_XSL_REVERSE_ARGS_4(MCTR,__VA_ARGS__),ARG
#define _XSL_REVERSE_ARGS_6(MCTR,ARG,...)		_XSL_REVERSE_ARGS_5(MCTR,__VA_ARGS__),ARG
#define XSL_REVERSE_ARGS(N,MCTR,...)			_XSL_REVERSE_ARGS_##N(MCTR __VA_OPT__(,)__VA_ARGS__))
//
//
#define _XSL_GROUPING_ARGS_0(MCTR0,MCTR1,N,...)			MCTR0(__VA_ARGS__);XSL_REVERSE_ARGS(N,MCTR1
#define _XSL_GROUPING_ARGS_1(MCTR0,MCTR1,ARG0,ARG1,...)	_XSL_GROUPING_ARGS_0(MCTR0,MCTR1,__VA_ARGS__,ARG0),ARG1
#define _XSL_GROUPING_ARGS_2(MCTR0,MCTR1,ARG0,ARG1,...)	_XSL_GROUPING_ARGS_1(MCTR0,MCTR1,__VA_ARGS__,ARG0),ARG1
#define _XSL_GROUPING_ARGS_3(MCTR0,MCTR1,ARG0,ARG1,...)	_XSL_GROUPING_ARGS_2(MCTR0,MCTR1,__VA_ARGS__,ARG0),ARG1
#define XSL_GROUPING_ARGS(N,MCTR0,MCTR1,...)				_XSL_GROUPING_ARGS_##N(MCTR0,MCTR1 __VA_OPT__(,)__VA_ARGS__,N))

#define XSL_GROUP_TO_SAME_MCTR(N,MCTR,...)		XSL_GROUPING_ARGS(N,MCTR,MCTR __VA_OPT__(,)__VA_ARGS__)

//Parity grouping
//MCTR:macro container
//Description:size(ARGS...)=2*N

#define XSL_GENERATE_INDEX_ARG(INDEX) ARG##INDEX
#define XSL_GENERATE_INDEX_ARG_GLUE_0(INDEX) XSL_GENERATE_INDEX_ARG(INDEX)##_0
#define XSL_GENERATE_INDEX_ARG_GLUE_1(INDEX) XSL_GENERATE_INDEX_ARG(INDEX)##_1

#define XSL_FILL_OPT_1(STAT,ARG0_0,ARG0_1,...)	\
STAT(XSL_REPEAT_MACRO_BY_INDEX_N( XSL_MACRO_ARGS_CONTER(__VA_ARGS__)/2  ARG0_0 __VA_OPT__(,)__VA_ARGS__)			\
STAT(ARG0_1 __VA_OPT__(,)__VA_ARGS__)

#define XSL_FILL_OPT(N,MACRO,...)	\
XSL_FILL_NULL_ARGS(N,MACRO)			\
MACRO(__VA_ARGS__)

#define XSL_FILL_OPT_2_(STAT,ARG0_0,ARG0_1,ARG1_0,ARG1_1,...)\
STAT(ARG0_0,ARG1_0 __VA_OPT__(,)__VA_ARGS__)				\
STAT(ARG0_1,ARG1_1 __VA_OPT__(,)__VA_ARGS__)





#define XSL_FILL_OPT_3(STAT,ARG0_0,ARG0_1,ARG1_0,ARG1_1,ARG2_0,ARG2_1)	\
XSL_FILL_OPT_2(STAT, ARG0_0,ARG0_1,ARG1_0,ARG1_1,ARG2_0,ARG2_0)			\
XSL_FILL_OPT_2(STAT, ARG0_0,ARG0_1,ARG1_0,ARG1_1,ARG2_1,ARG2_1)




#ifdef XSL_TEST
#define TEST(arg)  puts(#arg)
#else 
#define TEST(arg)
#endif // XSL_TEST
//
#define XSL_OPT_CONST_0(MACRO)	\
MACRO(XSL_EMPTR_MACRO)			\
MACRO(const)

#define XSL_OPT_BIND_1(OPT)


#define XSL_OPT_ATTRIB(STATE,OPT0,...)\
STATE(__VA_ARGS__,)\
STATE(__VA_ARGS__,OPT0)



#define XSL_OPT_ATTRIB_2(STATE,OPT0,OPT1)\
STATE()\
STATE(OPT0)


#define XSL_OPT_CONST_N(MACRO,...)	\
XSL_VA_ARGS_EXPAND(					\
MACRO(XSL_EMPTR_MACRO,##__VA_ARGS__)\
MACRO(const,##__VA_ARGS__)			\
)

//#define PRIVATE_MACRO_VAR_ARGS_IMPL_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9, N, ...) N
//#define PRIVATE_MACRO_VAR_ARGS_IMPL(args)  PRIVATE_MACRO_VAR_ARGS_IMPL_COUNT args
//#define COUNT_MACRO_VAR_ARGS(...)  PRIVATE_MACRO_VAR_ARGS_IMPL((__VA_ARGS__,10,9,8,7,6,5 4,3,2,1,0))
//
//#define PRIVATE_MACRO_CHOOSE_HELPER2(M, count)  M##count
//#define PRIVATE_MACRO_CHOOSE_HELPER1(M, count)  PRIVATE_MACRO_CHOOSE_HELPER2(M, count)
//#define PRIVATE_MACRO_CHOOSE_HELPER(M, count)   PRIVATE_MACRO_CHOOSE_HELPER1(M, count)
//
//
//#ifdef WIN32
////EXPAND ���ڽ��windows�ϱ�������__VA_ARGS__չ����һ������������
//#define EXPAND(...) __VA_ARGS__
//#define REG_DYNCREATE(...) EXPAND(PRIVATE_MACRO_CHOOSE_HELPER(PRIVATE_REG_DYNCREATE_, COUNT_MACRO_VAR_ARGS( __VA_ARGS__))( __VA_ARGS__))
//#else
//#define REG_DYNCREATE(...) PRIVATE_MACRO_CHOOSE_HELPER(PRIVATE_REG_DYNCREATE_, COUNT_MACRO_VAR_ARGS( __VA_ARGS__))( __VA_ARGS__)
//#endif
//
//REG_DYNCREATE(ABC,DEF)

XSL_END

#endif//XSL_BASE_DEF