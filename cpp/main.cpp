#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>
#include <utility>

#include "xsl/bits/test.hpp"
#define XSL_TEST
#define TEST_PROC_PRINT
// #include <xsl/bits/test.hpp>
// #include <xsl/map.hpp>
// #include <xsl/net.hpp>
// #include <xsl/ranges.hpp>
// #include <xsl/vector.hpp>
#include <xsl/bits/str/test.hpp>
using namespace xsl;
template <class T>
class tclass {
public:
  T n;
  template <class... Args>
  tclass(Args&&...args)
    : n(forward<Args>(args)...) {}
};
int main() {
  xsl_test_basic_string();
  // xsl::test::os << 1;
  // xsl::te;
  // xsl::net::http::client cli{};
  // // auto id = cli.get("http://www.baidu.com/", {});
  // xsl::net::MS2S header{};
  // header.try_emplace("User-Agent",
  //   "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36");
  // header.try_emplace("Accept",
  //   "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/"
  //   "signed-exchange;v=b3;q=0.7");
  // // header.try_emplace("Upgrade-Insecure-Requests", "1");
  // auto id = cli.get("http://www.csdn.net/", header);
  // xsl::size_t n = cli.refresh(-1);
  // auto& r = cli.resp(id);
  // std::ofstream ofs("tmp.htm", std::ios::out | std::ios::trunc);
  // ofs.write(r.Body.data(), r.Body.size());
  // ofs.close();

  // using Vec = vector<test::test_p>;
  // Vec vec{};
  // vec.insert(vec.begin(), {0, 1});
  // xsl::test::print(vec);
  // ranges::vlr<test::test_p> itv{};
  // xsl::test::print(itv);
  // itv.merge(vec.begin(), vec.end());
  // xsl::test::print(itv);
  // itv.merge(10, 1);
  // xsl::test::print(itv);
  // itv.merge(vec.begin(), 2);
  // xsl::test::print(itv);
  // itv.map([](test::test_p& t) { ++*t.Ptr; });
  // xsl::test::print(itv);
  // auto v0 = itv.collect<Vec>();
  // xsl::test::print(v0);
  // itv.collect<Vec>(v0);
  // xsl::test::print(v0);
  // auto v1 = itv.mapped<Vec>([](test::test_p& t) -> test::test_p { return {*t.Ptr + 1}; });
  // xsl::test::print(v1);
  // itv.mapped<Vec>([](test::test_p& t) -> test::test_p { return {*t.Ptr + 1}; }, v1);
  // xsl::test::print(v1);
  // itv.merge(vec);
  // xsl::test::print(itv);
}
