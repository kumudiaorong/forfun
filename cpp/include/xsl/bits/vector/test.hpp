#pragma once
#include "xsl/bits/allocator.hpp"
#ifndef XSL_VECTOR_TEST
#define XSL_VECTOR_TEST
#include <xsl/bits/test.hpp>
#include <xsl/bits/vector/vector.hpp>
namespace xsl::test {
  void xsl_test_vector() {
    using vec = vec::vector<test_p, alc::default_allocator<test_p>>;
    TEST_SPLIT_LINE(test_p);
    TEST_ACT_PRINT(test_p test_ps[6]{}; *test_ps[0].Ptr = 1; *test_ps[1].Ptr = 2; *test_ps[2].Ptr = 3;
                   *test_ps[3].Ptr = 4; *test_ps[4].Ptr = 5; *test_ps[5].Ptr = 6);
    TEST_ACT_PRINT(vec c00{});
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(vec c01(test_ps, 1));
    TEST_RET_PRINT(c01, print, c01.size());
    TEST_ACT_PRINT(vec c02(test_ps, test_ps + 2));
    TEST_RET_PRINT(c02, print, c02.size());
    TEST_ACT_PRINT(vec c03(3));
    TEST_RET_PRINT(c03, print, c03.size());
    TEST_ACT_PRINT(vec c04(3, test_ps[1]));
    TEST_RET_PRINT(c04, print, c04.size());
    TEST_ACT_PRINT(c00 = as_rreference(c01));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00 = c02);
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.assign(as_rreference(c03)));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.assign(test_ps, 5));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.assign(test_ps, test_ps + 6));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.assign(7, test_ps[2]));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.assign(c02));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((c00.front(), c00.back()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((as_const(c00).front(), as_const(c00).back()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((c00.begin(), c00.cbegin()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((as_const(c00).begin(), as_const(c00).cbegin()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((c00.end(), c00.cend()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((as_const(c00).end(), as_const(c00).cend()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((c00.rbegin(), c00.crbegin()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((as_const(c00).rbegin(), as_const(c00).crbegin()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((c00.rend(), c00.crend()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((as_const(c00).rend(), as_const(c00).crend()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((c00.empty(), c00.invalid()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT((as_const(c00).empty(), as_const(c00).invalid()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.clear());
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.emplace(c00.begin(), *test_ps));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.emplace(c00.begin(), as_rreference(*test_ps)));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.insert(c00.begin(), 1, test_ps[1]));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.insert(c00.begin(), test_ps + 1, test_ps + 2));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.emplace(c00.begin(), 1));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.erase(c00.begin(), ++ ++c00.begin()));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.emplace_back(test_ps[1]));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.emplace_back(as_rreference(test_ps[1])));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.emplace_back(3));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.pop_back());
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.resize(c00.size() - 3));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.resize(c00.size() + 1));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.resize(c00.size() + 1, *(test_ps + 3)));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_ACT_PRINT(c00.swap(c02));
    TEST_RET_PRINT(c00, print, c00.size());
    TEST_RET_PRINT(c00, rprint, c00.size());
  }
}  // namespace xsl::test
#endif  // XSL_TEST
