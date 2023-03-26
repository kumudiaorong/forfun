#ifndef XSL_DEF
#define XSL_DEF
#include"pf/xsldef.h"
//x standard library
inline void pass() {}

XSL_BEGIN
#define XSL_VA_ARGS_EXPAND(...)  __VA_ARGS__
#define XSL_EMPTR_MACRO

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

#endif