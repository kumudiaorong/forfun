#pragma once
#ifndef XSL_BASIC_STRING_TEST
#define XSL_BASIC_STRING_TEST
#include <xsl/bits/string/string.hpp>
#include <xsl/bits/test.hpp>
namespace xsl::test {
  void xsl_test_basic_string() {
    using str = xsl::str::basic_string<char, xsl::alc::default_allocator<char>>;
    TEST_SPLIT_LINE(char);
    TEST_ACT_PRINT(char cstr00[] = "123"; char cstr01[] = "123456");
    TEST_ACT_PRINT(str str00{});
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str str01(cstr00));
    TEST_RET_PRINT(str01, xsl::test::print, str01.size());
    TEST_ACT_PRINT(str str02(cstr01, 6));
    TEST_RET_PRINT(str02, xsl::test::print, str02.size());
    TEST_ACT_PRINT(str str03{xsl::as_rreference(str01)});
    TEST_RET_PRINT(str03, xsl::test::print, str03.size());
    TEST_ACT_PRINT(str str04{str02});
    TEST_RET_PRINT(str04, xsl::test::print, str04.size());
    TEST_ACT_PRINT(str str05(3, '1'));
    TEST_RET_PRINT(str05, xsl::test::print, str05.size());
    TEST_ACT_PRINT(str00 = cstr01);
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = str03);
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = xsl::as_rreference(str02));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.assign(cstr00));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.assign(str04));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.assign(xsl::as_rreference(str03)));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.assign(cstr01 + 1, 4));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.append({'6'}));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.append(cstr00, 1));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.resize(7, '7'));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str04.append(cstr00));
    TEST_RET_PRINT(str04, xsl::test::print, str04.size());
    TEST_ACT_PRINT(str00.append(str05));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00.erase(--str00.end(), str00.end()));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = str00 + str04);
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = str00 + cstr00);
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = str00 + 'a');
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = xsl::as_rreference(str00) + xsl::as_rreference(str04));
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = xsl::as_rreference(str00) + cstr00);
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_ACT_PRINT(str00 = xsl::as_rreference(str00) + 'b');
    TEST_RET_PRINT(str00, xsl::test::print, str00.size());
    TEST_RET_PRINT(str00, xsl::test::rprint, str00.size());
    // TEST_SPLIT_LINE(char16_t);
    // TEST_ACT_PRINT(char16_t cwstr00[] = L"123"; char16_t cwstr01[] = L"123456");
    // TEST_ACT_PRINT(xsl::str::basic_string<char16_t> wstr00{});
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(xsl::str::basic_string<char16_t> wstr01(cwstr00));
    // TEST_RET_PRINT(wstr01, xsl::test::print, wstr01.size());
    // TEST_ACT_PRINT(xsl::str::basic_string<char16_t> wstr02(cwstr01, 6));
    // TEST_RET_PRINT(wstr02, xsl::test::print, wstr02.size());
    // TEST_ACT_PRINT(xsl::str::basic_string<char16_t> wstr03{xsl::as_rreference(wstr01)});
    // TEST_RET_PRINT(wstr03, xsl::test::print, wstr03.size());
    // TEST_ACT_PRINT(xsl::str::basic_string<char16_t> wstr04{wstr02});
    // TEST_RET_PRINT(wstr04, xsl::test::print, wstr04.size());
    // TEST_ACT_PRINT(xsl::str::basic_string<char16_t> wstr05(3, '1'));
    // TEST_RET_PRINT(wstr05, xsl::test::print, wstr05.size());
    // TEST_ACT_PRINT(wstr00 = cwstr01);
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = wstr03);
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = xsl::as_rreference(wstr02));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00.assign(cwstr00));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00.assign(wstr04));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00.assign(xsl::as_rreference(wstr03)));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00.assign(cwstr01 + 1, 4));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00.append({'6'}));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00.resize(7, '7'));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = wstr00 + wstr04);
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = wstr00 + cwstr00);
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = wstr00 + L'a');
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = xsl::as_rreference(wstr00) + xsl::as_rreference(wstr04));
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = xsl::as_rreference(wstr00) + cwstr00);
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_ACT_PRINT(wstr00 = xsl::as_rreference(wstr00) + L'b');
    // TEST_RET_PRINT(wstr00, xsl::test::print, wstr00.size());
    // TEST_RET_PRINT(wstr00, xsl::test::rprint, wstr00.size());
  }
}  // namespace xsl::test
#endif  // XSL_TEST
