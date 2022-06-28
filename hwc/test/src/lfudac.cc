#include "belady.hpp"
#include "lfu.hpp"
#include "lfuda.hpp"

int main() {
  std::vector<int> vec{1,  1, 1,  1,  1,  1,  1,  1, 1, 1,  1,  1, 1,
                       1,  2, 2,  2,  4,  4,  4,  3, 3, 3,  3,  3, 3,
                       3,  3, 44, 44, 44, 44, 44, 3, 3, 44, 44, 3, 1,
                       44, 1, 1,  1,  1,  1,  1,  1, 1, 44};

  caches::lfu_t<unsigned> cache{123};
  std::cout << caches::get_optimal_hits(2, vec) << std::endl;
}