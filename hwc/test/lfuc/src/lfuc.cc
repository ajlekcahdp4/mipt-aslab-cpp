#include "belady.hpp"
#include "lfu.hpp"
#include "lfuda.hpp"

struct slow_getter_t {
  std::string operator()() {
    return "123";
  }
};

int main() {
  std::vector<int> vec{};
  std::size_t n, m;

  if (!std::cin || !std::cout) {
    std::terminate();
  }

  std::cin >> m >> n;

  if (n == 0 || m == 0) {
    std::terminate();
  }

  vec.reserve(n);
  caches::lfu_t<std::string, int> cache{m};
  slow_getter_t g{};

  for (unsigned i = 0; i < n; ++i) {
    int temp{};
    std::cin >> temp;
    vec.push_back(temp);
    cache.lookup(temp, g);
  }

  std::cout << cache.get_hits() << std::endl;
  std::cout << caches::get_optimal_hits(m, vec) << std::endl;
}