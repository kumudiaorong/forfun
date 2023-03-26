#pragma once
#ifndef XSL_ACSR
#define XSL_ACSR
#include <xsl/bits/def.hpp>
#include <xsl/bits/iterator.hpp>
#include <xsl/bits/ts.hpp>
XSL_BEGIN
namespace ranges {

  template <uint_8 Category, class Iter>
  class acsr;//accessor
  template <class _Val>
  class acsr<0, _Val> {
  public:
    typedef _Val val_type;
    acsr(size_t count, val_type& val)
      : Val(val)
      , Size(count) {}
    constexpr operator bool() const { return Size > 0; }
    constexpr acsr& operator++() {
      --Size;
      return *this;
    }
    constexpr val_type& operator*() { return Val; }
    constexpr bool operator!=(const acsr& ano) const { return Size != ano.Size || Val != ano.Val; }
    constexpr bool operator==(const acsr& ano) const { return Size == ano.Size && Val == ano.Val; }
    val_type& Val;
    size_t Size;
  };
  template <class _Iter>
  class acsr<1, _Iter> {
  public:
    // clang-format off
    typedef _Iter                                       iter_type;
    typedef typename iter_traits<iter_type>::val_type   val_type;
    // clang-format on
    acsr(const iter_type& first, size_t count)
      : First(first)
      , Size(count) {}
    constexpr operator bool() const { return Size > 0; }
    constexpr acsr& operator++() {
      ++First;
      --Size;
      return *this;
    }
    constexpr val_type& operator*() { return *First; }
    constexpr bool operator!=(const acsr& ano) const { return Size != ano.Size || First != ano.First; }
    constexpr bool operator==(const acsr& ano) const { return Size == ano.Size && First == ano.First; }
    iter_type First;
    size_t Size;
  };

  template <class _Iter>
  class acsr<2, _Iter> {
  public:
    // clang-format off
    typedef _Iter                                       iter_type;
    typedef typename iter_traits<iter_type>::val_type   val_type;
    // clang-format on
    constexpr acsr(const iter_type& first, const iter_type& last)
      : First(first)
      , Last(last) {}
    constexpr operator bool() const { return First != Last; }
    constexpr acsr& operator++() {
      ++First;
      return *this;
    }
    constexpr val_type& operator*() { return *First; }
    constexpr bool operator!=(const acsr& ano) const { return First != ano.First || Last != ano.Last; }
    constexpr bool operator==(const acsr& ano) const { return First == ano.First && Last == ano.Last; }
    iter_type First, Last;
  };
  template <class Size_T, class T, ts::enable<ts::has::opeartor_simple_assignment<size_t, Size_T>> = 0>
  acsr(Size_T, T&&) -> acsr<0, ts::as_c<ts::decay<T>>>;
  //
  template <class Size_T, class T,
    ts::enable<ts::has::opeartor_simple_assignment<size_t, Size_T> && ts::is::lref<T>> = 0>
  acsr(Size_T, T&&) -> acsr<0, ts::decay<T>>;
  //
  template <class UCIter, class Size_T,
    ts::enable<is_iter<ts::decay<UCIter>> && ts::has::opeartor_simple_assignment<size_t, Size_T>> = 0>
  acsr(UCIter, Size_T) -> acsr<1, ts::decay<UCIter>>;
  //
  template <class UCIter, ts::enable<is_iter<ts::decay<UCIter>>> = 0>
  acsr(UCIter, UCIter) -> acsr<2, ts::decay<UCIter>>;
  //
  template <class T>
  inline constexpr bool is_xsl_acsr = false;
  //
  template <uint_8 Index, class T>
  inline constexpr bool is_xsl_acsr<acsr<Index, T>> = true;
  //
  template <class T>
  inline constexpr bool is_acsr = is_xsl_acsr<T>;
}  // namespace ranges
XSL_END
#endif  // !XSL_ACSR