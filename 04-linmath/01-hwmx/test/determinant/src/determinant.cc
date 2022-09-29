#include <chrono>
#include <cmath>
#include <iostream>
#include <set>
#include <string>

#include "vector.hpp"

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

int main(int argc, char *argv[]) {
  throttle::containers::vector<std::string> vec;

  for (int i = 0; i < 2 << 16; ++i) {
    vec.push_back(std::to_string(i));
  }

  for (int i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";

  auto vec_moved = vec;

  for (int i = 0; i < vec_moved.size(); ++i) {
    std::cout << vec_moved[i] << " ";
  }
  std::cout << "\n";
}