#include <chrono>
#include <cmath>
#include <iostream>
#include <set>

#include "vector.hpp"

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

int main(int argc, char *argv[]) {
  throttle::containers::vector<std::unique_ptr<int>> vec;

  for (int i = 0; i < 16; ++i) {
    vec.push_back(std::make_unique<int>(i));
  }

  for (int i = 0; i < vec.size(); ++i) {
    std::cout << *vec[i] << " ";
  }
  std::cout << "\n";

  auto vec_moved = std::move(vec);

  for (int i = 0; i < vec_moved.size(); ++i) {
    std::cout << *vec_moved[i] << " ";
  }
  std::cout << "\n";
}