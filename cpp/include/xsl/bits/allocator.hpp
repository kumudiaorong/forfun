#pragma once
#ifndef XSL_ALLOCATOR
#define XSL_ALLOCATOR
#include <cstdlib>
#include <xsl/bits/def.hpp>
#include <xsl/bits/exception.hpp>
#include <xsl/bits/utility.hpp>

//
namespace xsl {
  struct tag_alloc {};
  template <typename T>
  constexpr T *xalloc(size_t byteSize) {
    return static_cast<T *>(std::malloc(byteSize));
  }
  template <typename T>
  T *xalloc_n(size_t count) {
    return static_cast<T *>(std::malloc(count * sizeof(T)));
  }
  void xfree(void *ptr) {
    std::free(ptr);
  }
  //
  template <typename T, typename... Args>
  constexpr T *construct_at(T *ptr, Args&&...args) {
    return new(ptr) T(forward<Args>(args)...);
  }
  //
  template <typename T, typename... _Args>
  constexpr T *construct(_Args&&...Params) {
    T *Ptr = xalloc_n<T>(1);
    construct_at(Ptr, forward<_Args>(Params)...);
    return Ptr;
  }
  //
  template <typename T>
  constexpr T *destruct_at(T *Ptr) {
    Ptr->~T();
    return Ptr;
  }
  //
  template <typename T>
  constexpr void destruct(T *Ptr) {
    destruct_at(Ptr);
    xfree(Ptr);
  }
}  // namespace xsl
namespace xsl::alc {
  constexpr uint_16 XSL_ALLOC_MINISIZE = 16;

  template <typename _Val>
  class default_allocator {
  public:
    // clang-format off
  typedef _Val    val_type;
  typedef size_t  size_type;
    // clang-format on
    template <class NewVal>
    using rebind_alloc = default_allocator<NewVal>;
  public:
    //
    constexpr default_allocator() = default;
    //
    constexpr default_allocator(default_allocator&&) = default;
    //
    constexpr default_allocator(const default_allocator&) = default;
    //
    constexpr default_allocator& operator=(const default_allocator&) = default;
    //
    constexpr default_allocator& operator=(default_allocator&&) = default;
    //
    constexpr val_type *allocate(size_type byteSize) {
      return xalloc<val_type>(byteSize);
    }
    //
    constexpr void deallocate(val_type *ptr) {
      return xfree(ptr);
    }
  };
}  // namespace xsl::alc
#endif  // XSL_ALLOCATOR