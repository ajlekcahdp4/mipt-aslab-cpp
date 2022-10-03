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

int main(int argc, char *argv[]) {
  unsigned n;
  if (!(std::cin >> n)) {
    return 1;
  }

  throttle::linmath::matrix<float> m{n, n};

  for (unsigned i = 0; i < n * n; ++i) {
    float temp;
    if (!(std::cin >> temp)) {
      return 1;
    }
    m[i / n][i % n] = temp;
  }

  auto det = m.determinant();
  std::cout << det;
}