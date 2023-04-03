#pragma once
#ifndef XSL_TEST
#define XSL_TEST

#include <fstream>
#include <iostream>
#include <xsl/bits/def.hpp>
#include <xsl/bits/utility.hpp>

namespace xsl::test {
  class out_steam {
  public:
    std::ostream *os;  // 文件流或标准输出流
    out_steam()
      : os(&std::cout) {
    }
    void set_out(const char *filename) {
      del_out();
      os = new std::ofstream{filename, std::ios::out | std::ios::trunc};
    }
    template <class T>
    out_steam& operator<<(T&& t) {
      (*os) << (forward<T>(t));
      return *this;
    }
    void del_out() {
      if(os != nullptr && os != &std::cout) {
        auto fs = dynamic_cast<std::ofstream *>(os);
        if(fs->is_open())
          fs->close();
        delete os;
      }
      os = nullptr;
    }
    ~out_steam() {
      del_out();
    }
  };
  static out_steam os{};
#ifdef TEST_PROC_PRINT
#define PROC_PRINT(Text) xsl::test::os << #Text << "\n"
#else
#define PROC_PRINT(Text)
#endif  // TEST_PROC_PRINT
#define TEST_SPLIT_LINE(Text) xsl::test::os << "---------------------------" #Text "---------------------------\n";

  class test_t {
  public:
    int d;
    char c;
    // void test_func00(test_t t) {
    //	PROC_PRINT(void _test_t(test_t t));
    // }
    // void test_func01(test_t t)const {
    //	PROC_PRINT(void _test_t(test_t t)const);
    // }
    /*operator int() {
            return d;
    }*/
    constexpr bool operator!=(const test_t& Ano) {
      return c != Ano.c || d != Ano.d;
    }
    constexpr bool operator==(const test_t& Ano) {
      return c == Ano.c && d == Ano.d;
    }
  };

  class test_p {
  public:
    int *Ptr;
    test_p()
      : Ptr(new int{}) {
      PROC_PRINT(test_p());
    }
    test_p(int _Val)
      : Ptr(new int(_Val)) {
      PROC_PRINT(test_p(int _Val));
    }
    test_p(const test_p& Ano)
      : Ptr(new int(*Ano.Ptr)) {
      PROC_PRINT(test_p(const test_p& Ano));
    }
    test_p(test_p&& Ano) noexcept
      : Ptr(Ano.Ptr) {
      PROC_PRINT(test_p(test_p && Ano));
      Ano.Ptr = nullptr;
    }
    test_p& operator=(const test_p& Ano) {
      PROC_PRINT(test_p & operator=(const test_p& Ano));
      Ptr = new int(*Ano.Ptr);
      return *this;
    }
    test_p& operator=(test_p&& Ano) noexcept {
      PROC_PRINT(test_p & operator=(test_p&& Ano));
      Ptr = Ano.Ptr;
      Ano.Ptr = nullptr;
      return *this;
    }
    constexpr bool operator!=(const test_p& Ano) {
      return *Ptr != *Ano.Ptr;
    }
    constexpr bool operator==(const test_p& Ano) {
      return *Ptr == *Ano.Ptr;
    }
    void test_func00(test_p) {
      PROC_PRINT(void _test_p(test_p t));
    }
    void test_func01(test_p) const {
      PROC_PRINT(void _test_p(test_p t) const);
    }
    void test_func02(test_p) & {
      PROC_PRINT(void _test_p(test_p t));
    }
    operator int() {
      return *Ptr;
    }
    ~test_p() {
      PROC_PRINT(~test_p());
      if(Ptr)
        delete Ptr;
    }
  };

#define TEST_ACT_PRINT(Text) \
xsl::test::os << #Text "\n"; \
Text;                        \
xsl::test::os << "\n"
  //
  template <template <typename...> class _Ctr, typename... _Tps>
  void print(_Ctr<char, _Tps...>& Ctr) {
    for(auto&& c : Ctr)
      xsl::test::os << c;
  }
  template <class _Ctr>
  void print(_Ctr& ctr) {
    for(auto&& e : ctr)
      xsl::test::os << e << ' ';
  }
  template <template <typename...> class _Ctr, typename... _Tps>
  void rprint(_Ctr<char, _Tps...>& Ctr) {
    for(auto b = Ctr.rbegin(), e = Ctr.rend(); b != e; ++b)
      xsl::test::os << *b;
  }
  template <template <typename...> class _Ctr, typename... _Tps>
  void print(_Ctr<char16_t, _Tps...>& Ctr) {
    for(auto&& wc : Ctr)
      xsl::test::os << wc;
  }
  template <template <typename...> class _Ctr, typename... _Tps>
  void rprint(_Ctr<char16_t, _Tps...>& Ctr) {
    for(auto b = Ctr.rbegin(), e = Ctr.rend(); b != e; ++b)
      xsl::test::os << *b;
  }
  template <template <typename...> class Ctr, typename... Tps>
  void print(Ctr<test_t, Tps...>& _Ctr) {
    for(auto&& t : _Ctr)
      xsl::test::os << t.d;
  }
  template <template <typename...> class Ctr, typename... Tps>
  void rprint(Ctr<test_t, Tps...>& _Ctr) {
    for(auto b = _Ctr.rbegin(), e = _Ctr.rend(); b != e; ++b)
      xsl::test::os << b->d;
  }
  template <template <typename...> class Ctr, typename... Tps>
  void print(Ctr<test_p, Tps...>& _Ctr) {
    for(auto&& t : _Ctr)
      xsl::test::os << *t.Ptr << ' ';
  }
  template <template <typename...> class Ctr, typename... Tps>
  void rprint(Ctr<test_p, Tps...>& _Ctr) {
    for(auto b = _Ctr.rbegin(), e = _Ctr.rend(); b != e; ++b)
      xsl::test::os << *(*b).Ptr << ' ';
  }
  void endl() {
    xsl::test::os << '\n';
  }
//
#define TEST_RET_PRINT(Text, Proc, Size) \
xsl::test::os << #Text " : ";            \
Proc(Text);                              \
xsl::test::os << " Size:" << Size << "\n\n";
  // std::cout << is_norm_func<int>;
  // std::cout << is_norm_func<int*>;
  // std::cout << is_norm_func<decltype(functor::special::f00)>;
  // std::cout << is_norm_func<decltype(&functor::special::f00)>;
  // std::cout << is_norm_func<decltype(functor::special::f01)>;
  // std::cout << is_norm_func<decltype(&functor::special::f01)>;
  // std::cout << is_norm_func<decltype(test_p::test_func00)>;
  // std::cout << is_norm_func<decltype(&test_p::test_func00)>;
  // std::cout << is_norm_func<decltype(test_p::test_func01)>;
  // std::cout << is_norm_func<decltype(&test_p::test_func01)>;
  // std::cout << is_norm_func<decltype(test_p::test_func02)>;
  // std::cout << is_norm_func<decltype(&test_p::test_func02)> << '\n';
}  // namespace xsl::test
#endif  //! XSL_TEST