#include <bitset>
#include <boost/type_index.hpp>
#include <cstddef>
#include <iostream>

#include "xsl/bits/ranges/vlr.hpp"
#include "xsl/bits/rb_tree.hpp"

#define TEST_PROC_PRINT
#include <xsl/bits/test.hpp>
#include <xsl/map.hpp>
// #include <xsl/net.hpp>
#include <xsl/ranges.hpp>
#include <xsl/vector.hpp>
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
  // xsl::http::client cli{};
  // auto id = cli.get("http://www.baidu.com/", {});
  // xsl::size_t n = cli.refresh(-1);
  // auto& r = cli.resp(id);
  // std::ofstream ofs("tmp.htm", std::ios::out | std::ios::trunc);
  // ofs.write(r.Body.data(), r.Body.size());
  // ofs.close();
  using Vec = vector<test::test_p>;
  Vec vec{};
  vec.insert(vec.begin(), {0, 1});
  xsl::test::print(vec);
  ranges::vlr<test::test_p> itv{};
  xsl::test::print(itv);
  itv.merge(vec.begin(), vec.end());
  xsl::test::print(itv);
  // itv.merge(10, 1);
  // xsl::test::print(itv);
  itv.merge(vec.begin(), 2);
  xsl::test::print(itv);
  itv.map([](test::test_p& t) { ++*t.Ptr; });
  xsl::test::print(itv);
  auto v0 = itv.collect<Vec>();
  xsl::test::print(v0);
  itv.collect<Vec>(v0);
  xsl::test::print(v0);
  auto v1 = itv.mapped<Vec>([](test::test_p& t) -> test::test_p { return {*t.Ptr + 1}; });
  xsl::test::print(v1);
  itv.mapped<Vec>([](test::test_p& t) -> test::test_p { return {*t.Ptr + 1}; }, v1);
  xsl::test::print(v1);
  // itv.merge(vec);
  // xsl::test::print(itv);
}