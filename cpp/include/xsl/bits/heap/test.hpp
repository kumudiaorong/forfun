#pragma once
#ifndef XSL_HEAP_TEST
#define XSL_HEAP_TEST
#include <random>
#include <xsl/bits/functor.hpp>
#include <xsl/bits/heap/heap.hpp>
#include <xsl/bits/test.hpp>
#include <xsl/bits/ts/is.hpp>
#include <xsl/bits/ts/ts.hpp>
#include <xsl/vector.hpp>
namespace xsl::test {
  template <class Ctr, class Comp, class Seq, ts::enable<ts::is::same<typename Ctr::val_type, int>> = 0>
  void print(hp::heap<Ctr, Comp>& hp, Seq s = "") {
    auto nhp = hp;
    typename hp::heap<Ctr, Comp>::val_type last = nhp.top();
    Comp pre{};
    while(!nhp.empty()) {
      os << nhp.top() << s;
      if(pre(nhp.top(), last)) {
        os << "wrong sort";
        break;
      }
      nhp.pop();
    }
  }
  void xsl_test_heap() {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uid(1, 1000000);
    xsl::hp::heap<xsl::vector<int>, xsl::fc::less<int>> hp{};
    for(int i = 0; i < 10000; ++i) {
      hp.push(uid(e));
    }
    xsl::test::print(hp, ' ');
    xsl::test::endl();
  }
}  // namespace xsl::test
#endif  // XSL_TEST
