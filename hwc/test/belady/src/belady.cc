#include <iostream>
#include <vector>

#include "belady.hpp"

int main() {
  std::size_t n{}, m{};

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

    if (std::cin.fail()) {
      std::abort();
    }
    
    vec.push_back(temp);
  }

  std::cout << caches::get_optimal_hits<int>(m, vec.begin(), vec.end()) << std::endl;
}