#pragma once
#ifndef XSL_FUNCTIONAL_TEST
#define XSL_FUNCTIONAL_TEST
#include <functional>
#include <xsl/bits/functional/function.hpp>
#include <xsl/bits/functional/binder.hpp>
#include <xsl/bits/test.hpp>

namespace xsl::test {
  void f00() {
    PROC_PRINT(void f00());
  }
  void f01(test_p t) {
    PROC_PRINT(void f01(test_p t));
  }
  void f02(test_p& t) {
    PROC_PRINT(void f02(test_p & t));
  }
  void f03(const test_p& t) {
    PROC_PRINT(void f03(const test_p& t));
  }
  void f04(int i, test_p t) {
    PROC_PRINT(void f04(int i, test_p t));
  }
  void xsl_test_binder() {
    TEST_ACT_PRINT(test_p tmp{}; int i = 0);
    TEST_ACT_PRINT(auto tf00 = xsl::fn::bind(&f00));
    TEST_ACT_PRINT(f00());
    TEST_ACT_PRINT(tf00());
    TEST_ACT_PRINT(auto tf11 = xsl::fn::bind_front<1>(&f01));
    TEST_ACT_PRINT(auto tf12 = xsl::fn::bind(&f01, tmp));
    TEST_ACT_PRINT(auto tf13 = xsl::fn::bind(&f01, xsl::as_const(tmp)));
    TEST_ACT_PRINT(auto tf14 = xsl::fn::bind(&f01, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(auto tf15 = xsl::fn::bind(&f01, test_p{}));
    TEST_ACT_PRINT(f01(tmp));
    TEST_ACT_PRINT(tf11(tmp));
    TEST_ACT_PRINT(tf12());
    TEST_ACT_PRINT(f01(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf11(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf13());
    TEST_ACT_PRINT(f01(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf11(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf14());
    TEST_ACT_PRINT(f01(test_p{}));
    TEST_ACT_PRINT(tf11(test_p{}));
    TEST_ACT_PRINT(tf15());
    TEST_ACT_PRINT(auto tf21 = xsl::fn::bind_front<1>(&f02));
    TEST_ACT_PRINT(auto tf22 = xsl::fn::bind(&f02, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(auto tf23 = xsl::fn::bind(&f02, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(f02(tmp));
    TEST_ACT_PRINT(tf21(tmp));
    TEST_ACT_PRINT(tf22());
    TEST_ACT_PRINT(f02(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf21(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf23());
    TEST_ACT_PRINT(auto tf31 = xsl::fn::bind_front<1>(&f03));
    TEST_ACT_PRINT(auto tf32 = xsl::fn::bind(&f03, tmp));
    TEST_ACT_PRINT(auto tf33 = xsl::fn::bind(&f03, xsl::as_const(tmp)));
    TEST_ACT_PRINT(auto tf34 = xsl::fn::bind(&f03, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(auto tf35 = xsl::fn::bind(&f03, test_p{}));
    TEST_ACT_PRINT(f03(tmp));
    TEST_ACT_PRINT(tf31(tmp));
    TEST_ACT_PRINT(tf32());
    TEST_ACT_PRINT(f03(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf31(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf33());
    TEST_ACT_PRINT(f03(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf31(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf34());
    TEST_ACT_PRINT(f03(test_p{}));
    TEST_ACT_PRINT(tf31(test_p{}));
    TEST_ACT_PRINT(tf35());
    TEST_ACT_PRINT(auto tf41 = xsl::fn::bind_front<2>(&f04));
    TEST_ACT_PRINT(auto tf42 = xsl::fn::bind_front<1>(&f04, i));
    TEST_ACT_PRINT(auto tf43 = xsl::fn::bind(&f04, xsl::fn::placeholder{}, tmp));
    TEST_ACT_PRINT(auto tf44 = xsl::fn::bind(&f04, i, tmp));
    TEST_ACT_PRINT(f04(i, tmp));
    TEST_ACT_PRINT(tf41(i, tmp));
    TEST_ACT_PRINT(tf42(tmp));
    TEST_ACT_PRINT(tf43(i));
    TEST_ACT_PRINT(tf44());
    TEST_ACT_PRINT(auto tf51 = xsl::fn::bind_front<1>(&test_p::test_func00, &tmp));
    TEST_ACT_PRINT(auto tf52 = xsl::fn::bind(&test_p::test_func00, &tmp, tmp));
    TEST_ACT_PRINT(auto tf61 = xsl::fn::bind_front<1>(&test_p::test_func01, &tmp));
    TEST_ACT_PRINT(auto tf62 = xsl::fn::bind(&test_p::test_func01, &tmp, tmp));
    TEST_ACT_PRINT(tmp.test_func00(tmp));
    TEST_ACT_PRINT(tf51(tmp));
    TEST_ACT_PRINT(tf52());
    TEST_ACT_PRINT(tmp.test_func01(tmp));
    TEST_ACT_PRINT(tf61(tmp));
    TEST_ACT_PRINT(tf62());
  }
  void std_test_binder() {
    TEST_ACT_PRINT(test_p tmp{}; int i = 0);
    TEST_ACT_PRINT(auto tf00 = std::bind(&f00));
    TEST_ACT_PRINT(f00());
    TEST_ACT_PRINT(tf00());
    TEST_ACT_PRINT(auto tf11 = std::bind_front(&f01));
    TEST_ACT_PRINT(auto tf12 = std::bind(&f01, tmp));
    TEST_ACT_PRINT(auto tf13 = std::bind(&f01, xsl::as_const(tmp)));
    TEST_ACT_PRINT(auto tf14 = std::bind(&f01, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(auto tf15 = std::bind(&f01, test_p{}));
    TEST_ACT_PRINT(f01(tmp));
    TEST_ACT_PRINT(tf11(tmp));
    TEST_ACT_PRINT(tf12());
    TEST_ACT_PRINT(f01(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf11(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf13());
    TEST_ACT_PRINT(f01(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf11(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf14());
    TEST_ACT_PRINT(f01(test_p{}));
    TEST_ACT_PRINT(tf11(test_p{}));
    TEST_ACT_PRINT(tf15());
    TEST_ACT_PRINT(auto tf21 = std::bind_front(&f02));
    TEST_ACT_PRINT(auto tf22 = std::bind(&f02, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(auto tf23 = std::bind(&f02, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(f02(tmp));
    TEST_ACT_PRINT(tf21(tmp));
    TEST_ACT_PRINT(tf22());
    TEST_ACT_PRINT(f02(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf21(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf23());
    TEST_ACT_PRINT(auto tf31 = std::bind_front(&f03));
    TEST_ACT_PRINT(auto tf32 = std::bind(&f03, tmp));
    TEST_ACT_PRINT(auto tf33 = std::bind(&f03, xsl::as_const(tmp)));
    TEST_ACT_PRINT(auto tf34 = std::bind(&f03, xsl::wrap_ref(tmp)));
    TEST_ACT_PRINT(auto tf35 = std::bind(&f03, test_p{}));
    TEST_ACT_PRINT(f03(tmp));
    TEST_ACT_PRINT(tf31(tmp));
    TEST_ACT_PRINT(tf32());
    TEST_ACT_PRINT(f03(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf31(xsl::as_const(tmp)));
    TEST_ACT_PRINT(tf33());
    TEST_ACT_PRINT(f03(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf31(xsl::as_lreference(tmp)));
    TEST_ACT_PRINT(tf34());
    TEST_ACT_PRINT(f03(test_p{}));
    TEST_ACT_PRINT(tf31(test_p{}));
    TEST_ACT_PRINT(tf35());
    TEST_ACT_PRINT(auto tf41 = std::bind_front(&f04));
    TEST_ACT_PRINT(auto tf42 = std::bind_front(&f04, i));
    TEST_ACT_PRINT(auto tf43 = std::bind(&f04, std::placeholders::_1, tmp));
    TEST_ACT_PRINT(auto tf44 = std::bind(&f04, i, tmp));
    TEST_ACT_PRINT(f04(i, tmp));
    TEST_ACT_PRINT(tf41(i, tmp));
    TEST_ACT_PRINT(tf42(tmp));
    TEST_ACT_PRINT(tf43(i));
    TEST_ACT_PRINT(tf44());
    TEST_ACT_PRINT(auto tf51 = std::bind_front(&test_p::test_func00, &tmp));
    TEST_ACT_PRINT(auto tf52 = std::bind(&test_p::test_func00, &tmp, tmp));
    TEST_ACT_PRINT(auto tf61 = std::bind_front(&test_p::test_func01, &tmp));
    TEST_ACT_PRINT(auto tf62 = std::bind(&test_p::test_func01, &tmp, tmp));
    TEST_ACT_PRINT(tmp.test_func00(tmp));
    TEST_ACT_PRINT(tf51(tmp));
    TEST_ACT_PRINT(tf52());
    TEST_ACT_PRINT(tmp.test_func01(tmp));
    TEST_ACT_PRINT(tf61(tmp));
    TEST_ACT_PRINT(tf62());
  }
}  // namespace xsl::test
#endif  // XSL_TEST
