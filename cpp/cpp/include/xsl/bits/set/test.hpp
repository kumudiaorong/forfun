#pragma once
#ifndef XSL_SET_TEST
#define XSL_SET_TEST
#include <random>
#include <vector>
#include <xsl/bits/set/set.hpp>
#include <xsl/bits/test.hpp>
namespace xsl::test {
  void xsl_test_set() {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uid(1, 10000000);
    ost::set<test_p, fc::less<int>> s00{};
    std::vector<int> v00;
    auto f = [&uid, &e, &s00, &v00]() {
      for(int i = 0; i < 1000000; ++i) {
        int k = uid(e);
        if(s00.try_emplace(k).second)
          v00.push_back(k);
      }
      while(s00.size()) {
        s00.erase(v00.back());
        v00.pop_back();
      }
    };
    f();
    f();
  }
  void xsl_test_multiset() {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uid(1, 10000000);
    ost::multiset<test_p, fc::less<int>> s00{};
    std::vector<int> v00;
    auto f = [&uid, &e, &s00, &v00]() {
      for(int i = 0; i < 1000000; ++i) {
        int k = uid(e);
        s00.emplace(k);
        v00.push_back(k);
      }
      while(s00.size()) {
        s00.erase(v00.back());
        v00.pop_back();
      }
    };
    f();
    f();
  }
}  // namespace xsl::test
#endif  // !XSL_SET
