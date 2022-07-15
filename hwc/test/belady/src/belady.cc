#include "caches.hpp"

int main() {
  std::size_t n, m;

  if (!std::cin || !std::cout) {
    std::abort();
  }

  std::cin >> m >> n;

  if (n == 0 || m == 0) {
    std::abort();
  }

  std::vector<int> vec{};
  vec.reserve(n);

  for (unsigned i = 0; i < n; ++i) {
    if (!std::cin || !std::cout) {
      std::abort();
    }

    int temp{};
    std::cin >> temp;
    vec.push_back(temp);
  }

  std::cout << caches::get_optimal_hits(m, vec) << std::endl;
}