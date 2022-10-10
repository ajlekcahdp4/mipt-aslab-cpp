#include <chrono>
#include <iostream>

#include "kd_tree.hpp"
#include "point_n.hpp"
#include "vec_n.hpp"

#ifdef BOOST_FOUND__
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
namespace po = boost::program_options;
#endif

#include <cstdlib>
#include <range/v3/all.hpp>

template <typename T, std::size_t N> struct indexed_point_n : public throttle::point_n<T, N> {
  unsigned index;
};

using point4 = indexed_point_n<float, 4>;

int main(int argc, char *argv[]) {
  std::vector<point4> points;
  throttle::kd_tree<point4> kdtree;

  unsigned n;
  if (!(std::cin >> n)) {
    std::cout << "Can't read number of points\n";
    return 1;
  }

  if (n == 0) {
    std::cout << "Number of points can't be equal to 0\n";
    return 1;
  }

  for (unsigned i = 0; i < n; ++i) {
    point4 point;

    if (!(std::cin >> point)) {
      std::cout << "Can't read point " << i << "\n";
      return 1;
    }

    point.index = i;
    kdtree.insert(point);
  }

  unsigned m;
  if (!(std::cin >> m)) {
    std::cout << "Can't read number of requests\n";
    return 1;
  }

  std::vector<point4> reqs;
  for (unsigned i = 0; i < m; ++i) {
    point4 point;
    if (!(std::cin >> point)) {
      std::cout << "Can't read point " << i << "\n";
      return 1;
    }

    reqs.push_back(point);
  }

  for (const auto &q: reqs) {
    auto closest = kdtree.nearest_neighbour(q).index;
    std::cout << closest << "\n";
  }
}