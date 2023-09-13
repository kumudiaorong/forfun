#pragma once
#ifndef XSL_BATCH
#define XSL_BATCH

#include <xsl/bits/allocator.hpp>
#include <xsl/bits/iterator.hpp>
#include <xsl/bits/pf/memory.hpp>

namespace xsl::batch {
  template <class CIter1, class CIter2, ts::enable<itor::is<CIter1> && itor::is<CIter1>> = 0>
  constexpr int_8 compare(CIter1 first1, CIter2 first2, size_t size) {
    using val_type = typename itor::traits<CIter1>::val_type;
    if constexpr(ts::is::basic_type<val_type> && ts::is::ptr<CIter1> && ts::is::ptr<CIter2>)
      return memcmp(first1, first2, size * sizeof(val_type));
    while(size-- != 0) {
      if(!(*first1 == *first2))
        return *first1 < *first2 ? -1 : 1;
      ++first1;
      ++first2;
    }
    return 0;
  }
  //
  template <class CIter1, class CIter2, ts::enable<itor::is<CIter1> && itor::is<CIter1>> = 0>
  constexpr void copy(CIter1 dest, CIter2 src, size_t count) {
    using val_type = typename itor::traits<CIter1>::val_type;
    if constexpr(ts::is::basic_type<val_type> && ts::is::ptr<CIter1> && ts::is::ptr<CIter2>)
      MCopy(dest, src, count * sizeof(val_type));
    else {
      while(count != 0) {
        *dest = *src;
        // memcpy(XSL addr(*dest), XSL addr(*src), sizeof(val_type));
        ++src;
        ++dest;
        --count;
      }
    }
  }
  //
  template <class CIter, ts::enable<itor::is<CIter>> = 0>
  constexpr void create(CIter dest, size_t count, const typename itor::traits<CIter>::val_type& val) {
    using val_type = typename itor::traits<CIter>::val_type;
    while(count != 0) {
      construct_at(addr(*dest), val);
      ++dest;
      --count;
    }
  }
  //
  template <class CIter, ts::enable<itor::is<CIter>> = 0>
  constexpr void create(CIter dest, size_t count) {
    using val_type = typename itor::traits<CIter>::val_type;
    while(count != 0) {
      construct_at(addr(*dest));
      ++dest;
      --count;
    }
  }
  //
  template <class CIter, ts::enable<itor::is<CIter>> = 0>
  constexpr void destroy(CIter dest, size_t count) {
    using val_type = typename itor::traits<CIter>::val_type;
    while(count != 0) {
      destruct_at(addr(*dest));
      ++dest;
      --count;
    }
  }
  //
  template <class CIter, ts::enable<itor::is<CIter>> = 0>
  constexpr size_t length(
    CIter first, const typename itor::traits<CIter>::val_type& end = typename itor::traits<CIter>::val_type{}) {
    size_t count{};
    while(*first != end)
      ++first, ++count;
    return count;
  }
  //
  template <class CIter, ts::enable<itor::is<CIter>> = 0>
  constexpr size_t length(CIter first, CIter last) {
    if(itor::is_random<CIter>)
      return last - first;
    size_t count{};
    while(first != last)
      ++first, ++count;
    return count;
  }
  //
  template <class CIter>
  constexpr CIter jump(CIter Iter, size_t Size) {
    if constexpr(itor::is_random<CIter>)
      Iter = Iter + Size;
    else
      while(Size != 0) {
        ++Iter;
        --Size;
      }
    return as_rreference(Iter);
  }
  //
};      // namespace xsl::batch
#endif  // !XSL_BATCH
