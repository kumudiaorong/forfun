#pragma once
#ifndef XSL_LIST_TEST
#define XSL_LIST_TEST
#include <xsl/bits/ls/list.hpp>
#include <xsl/bits/test.hpp>
namespace xsl::test {
  void xsl_test_list() {
    using list = xsl::ls::list<test_p, xsl::alc::default_allocator<test_p>>;
    TEST_SPLIT_LINE(test_p);
    TEST_ACT_PRINT(test_p test_ps[6]{}; *test_ps[0].Ptr = 1; *test_ps[1].Ptr = 2; *test_ps[2].Ptr = 3;
                   *test_ps[3].Ptr = 4; *test_ps[4].Ptr = 5; *test_ps[5].Ptr = 6);
    TEST_ACT_PRINT(list ls00{});
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(list ls01(test_ps, 1));
    TEST_RET_PRINT(ls01, print, ls01.size());
    TEST_ACT_PRINT(list ls02(test_ps, test_ps + 2));
    TEST_RET_PRINT(ls02, print, ls02.size());
    TEST_ACT_PRINT(list ls03(3));
    TEST_RET_PRINT(ls03, print, ls03.size());
    TEST_ACT_PRINT(list ls04(3, test_ps[1]));
    TEST_RET_PRINT(ls04, print, ls04.size());
    TEST_ACT_PRINT(ls00 = as_rreference(ls01));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00 = ls02);
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.assign(as_rreference(ls03)));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.assign(4));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.assign(test_ps, 5));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.assign(test_ps, test_ps + 6));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.assign(7, test_ps[2]));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.assign(ls02));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((ls00.front(), ls00.back()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((as_const(ls00).front(), as_const(ls00).back()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((ls00.begin(), ls00.cbegin()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((as_const(ls00).begin(), as_const(ls00).cbegin()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((ls00.end(), ls00.cend()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((as_const(ls00).end(), as_const(ls00).cend()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((ls00.rbegin(), ls00.crbegin()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((as_const(ls00).rbegin(), as_const(ls00).crbegin()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((ls00.rend(), ls00.crend()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((as_const(ls00).rend(), as_const(ls00).crend()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((ls00.empty(), ls00.invalid()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT((as_const(ls00).empty(), as_const(ls00).invalid()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.clear());
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace(ls00.begin(), *test_ps));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace(ls00.begin(), as_rreference(*test_ps)));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.insert(ls00.begin(), 1, test_ps[1]));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.insert(ls00.begin(), 2));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.insert(ls00.begin(), test_ps + 1, test_ps + 2));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace(ls00.begin(), 1));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.erase(ls00.begin(), ++ ++ls00.begin()));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace_back(test_ps[1]));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace_back(as_rreference(test_ps[1])));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace_back(3));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.pop_back());
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace_front(test_ps[2]));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace_front(as_rreference(test_ps[2])));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.emplace_front(4));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.pop_front());
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.resize(ls00.size() - 3));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.resize(ls00.size() + 1));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.resize(ls00.size() + 1, *(test_ps + 3)));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_ACT_PRINT(ls00.swap(ls02));
    TEST_RET_PRINT(ls00, print, ls00.size());
    TEST_RET_PRINT(ls00, rprint, ls00.size());
  }
}  // namespace xsl::test
#endif  // XSL_TEST
