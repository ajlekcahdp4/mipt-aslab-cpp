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
  throttle::containers::vector<int> vec;
}