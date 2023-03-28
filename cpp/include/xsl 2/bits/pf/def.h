#ifndef XSL_DEF_PF
#define XSL_DEF_PF

#define XSL_BEGIN namespace xsl {
#define XSL_END }
#define XSL xsl::

XSL_BEGIN

using int_8 = signed char;
using int_16 = short;
using int_32 = int;
using int_64 = long long;
using uint_8 = unsigned char;
using uint_16 = unsigned short;
using uint_32 = unsigned int;
using uint_64 = unsigned long long;

using byte = unsigned char;
using size_t = uint_64;
using ptrdiff_t = int_64;
XSL_END

#endif  //! XSL_DEF_PF