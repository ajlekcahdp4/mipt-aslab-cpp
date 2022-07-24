#include <iostream>

#include "caches.hpp"

struct slow_getter_t {
  int operator()(int p_key) {
    // The answer to “the Ultimate Question of Life, the Universe, and
    // Everything.”
    return 42;
  }
};

int main() {
  std::size_t n, m;

  if (!std::cin || !std::cout) {
    std::abort();
  }

  std::cin >> m >> n;

  if (n == 0 || m == 0) {
    std::abort();
  }

  caches::lfu_t<int, int> cache{m};
  slow_getter_t g{};

  for (unsigned i = 0; i < n; ++i) {
    if (!std::cin || !std::cout) {
      std::abort();
    }

    int temp{};
    std::cin >> temp;
    cache.lookup(temp, g);
  }

  std::cout << cache.get_hits() << std::endl;
}