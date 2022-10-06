#include <chrono>
#include <iostream>

#include "vec_n.hpp"
#include "point_n.hpp"

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

#include <cstdlib>

using point4 = throttle::point_n<float, 4>;
using vec4 = throttle::vec_n<float, 4>;

int main(int argc, char *argv[]) {
  point4 a{1, 2};
  point4 b{3, 4};
  
  auto c = b - a;
  for (const auto &v: c.arr) {
    std::cout << v << " ";
  }

  std::cout << "\n";
}