#include <chrono>
#include <cmath>
#include <iostream>
#include <set>
#include <string>

#include "contiguous_matrix.hpp"
#include "vector.hpp"

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

template class throttle::linmath::contiguous_matrix<int>;

int main(int argc, char *argv[]) {
  throttle::linmath::contiguous_matrix<int> m{2, 2, {1, 2, 3, 4, 5, 6}};
  const throttle::linmath::contiguous_matrix<int> m2{2, 2, {1, 2, 3, 4, 5, 6}};

  m += m2;
  for (unsigned i = 0; i < m.rows(); ++i) {
    for (unsigned j = 0; j < m.cols(); ++j) {
      std::cout << m[i][j] << " ";
    }
    std::cout << "\n";
  }

  throttle::containers::vector<std::unique_ptr<int>> p;
  p.emplace_back(std::make_unique<int>(1));
  p.emplace_back(std::make_unique<int>(2));

  // for (const auto &v: p) {
  //   std::cout << *v << " ";
  // }

  // std::cout << p.capacity() << " ";

  p.emplace_back(std::make_unique<int>(1));
  p.emplace_back(std::make_unique<int>(2));
  p.emplace_back(std::make_unique<int>(1));
  p.emplace_back(std::make_unique<int>(2));
  p.emplace_back(std::make_unique<int>(1));
  p.emplace_back(std::make_unique<int>(2));

  // std::cout << p.capacity() << "\n";
  std::cout << throttle::containers::vector<int>::amortized_buffer_size(2);

}