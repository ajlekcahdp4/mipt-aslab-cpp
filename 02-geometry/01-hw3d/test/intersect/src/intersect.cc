#include <chrono>
#include <iostream>

#include "broadphase/broadphase_structure.hpp"
#include "broadphase/bruteforce.hpp"
#include "broadphase/octree.hpp"

#include "narrowphase/collision_shape.hpp"
#include "primitives/plane.hpp"
#include "primitives/triangle3.hpp"
#include "vec3.hpp"

#include <cmath>
#include <set>
#include <vector>

struct indexed_geom : public throttle::geometry::collision_shape<float> {
  unsigned index;
  indexed_geom(unsigned idx, auto &&base) : collision_shape{base}, index{idx} {};
};

using throttle::geometry::collision_shape;
using throttle::geometry::is_roughly_equal;
using throttle::geometry::point3;
using throttle::geometry::segment3;
using throttle::geometry::triangle3;
using throttle::geometry::vec3;

template <typename T>
throttle::geometry::collision_shape<T> shape_from_three_points(const point3<T> &a, const point3<T> &b,
                                                               const point3<T> &c) {
  auto ab = b - a, ac = c - a;

  if (throttle::geometry::colinear(ab, ac)) { // Either a segment or a point
    if (is_roughly_equal(ab, vec3<T>::zero()) && is_roughly_equal(ac, vec3<T>::zero())) {
      return throttle::geometry::barycentric_average<T>(a, b, c);
    }
    // This is a segment. Project the the points onto the most closely alligned axis.
    auto max_index = ab.max_component().first;

    std::array<std::pair<point3<T>, T>, 3> arr = {std::make_pair(a, a[max_index]), std::make_pair(b, b[max_index]),
                                                  std::make_pair(c, c[max_index])};
    std::sort(arr.begin(), arr.end(),
              [](const auto &left, const auto &right) -> bool { return left.second < right.second; });
    return segment3<T>{arr[0].first, arr[2].first};
  }

  return triangle3<T>{a, b, c};
}

static unsigned apporoximate_optimal_depth(unsigned number) {
  constexpr unsigned max_depth = 6;
  unsigned           log_num = std::log10(float(number));
  return std::min(max_depth, log_num);
}

int main() {
  using point_type = throttle::geometry::point3<float>;

  unsigned n;
  if (!(std::cin >> n)) {
    std::cout << "Can't read number of triangles\n";
    return 1;
  }

  throttle::geometry::octree<float, indexed_geom> octree{apporoximate_optimal_depth(n)};
  for (unsigned i = 0; i < n; ++i) {
    point_type a, b, c;

    std::cin >> a[0] >> a[1] >> a[2] >> b[0] >> b[1] >> b[2] >> c[0] >> c[1] >> c[2];
    octree.add_collision_shape({i, shape_from_three_points(a, b, c)});
  }

  for (const auto v : octree.many_to_many())
    std::cout << v->index << " ";
}