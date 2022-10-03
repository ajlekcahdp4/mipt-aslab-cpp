#include <chrono>
#include <cmath>
#include <iostream>
#include <set>
#include <string>

#include "contiguous_matrix.hpp"
#include "vector.hpp"
#include "matrix.hpp"

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

template class throttle::linmath::contiguous_matrix<int>;

int main(int argc, char *argv[]) {
  throttle::linmath::matrix<int> m{3, 1, {1, 2, 3}};
  const throttle::linmath::matrix<int> m2{1, 3, {1, 2, 3}};

  auto res = m * m2;

  for (unsigned i = 0; i < res.rows(); ++i) {
    for (unsigned j = 0; j < res.cols(); ++j) {
      std::cout << res[i][j] << " ";
    }
    std::cout << "\n";
  }

  std::cout << m.equal(m2, 1);
}