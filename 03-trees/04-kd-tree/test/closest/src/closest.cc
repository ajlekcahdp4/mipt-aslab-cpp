#include <boost/lexical_cast/bad_lexical_cast.hpp>
#include <chrono>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "kd_tree.hpp"
#include "point_n.hpp"

#include <string>
#include <cstdlib>
#include <range/v3/all.hpp>

template <typename T, std::size_t N> struct indexed_point_n : public throttle::point_n<T, N> {
  unsigned index;
};

using point4 = indexed_point_n<float, 4>;

int main(int argc, char *argv[]) {
  std::vector<point4> points;
  throttle::kd_tree<point4> kdtree;

  int n;
  std::string n_str;
  if (!(std::cin >> n_str)) {
    std::cout << "Can't read number of points\n";
    return 1;
  }

  try {
    n = boost::lexical_cast<int>(n_str);
  } catch (boost::bad_lexical_cast &) {
    std::cout << "Invalid number of points\n";
    return 1;
  }

  if (n <= 0) {
    std::cout << "Number of points can't be less or equal to 0\n";
    return 1;
  }

  points.reserve(n);
  for (int i = 0; i < n; ++i) {
    point4 point;

    if (!(std::cin >> point)) {
      std::cout << "Can't read point " << i << "\n";
      return 1;
    }

    point.index = i;
    kdtree.insert(point);
  }

  int m;
  std::string m_str;
  if (!(std::cin >> m_str)) {
    std::cout << "Can't read number of points\n";
    return 1;
  }

  try {
    m = boost::lexical_cast<int>(m_str);
  } catch (boost::bad_lexical_cast &) {
    std::cout << "Invalid number of points\n";
    return 1;
  }

  std::vector<point4> reqs;
  reqs.reserve(m);

  for (int i = 0; i < m; ++i) {
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