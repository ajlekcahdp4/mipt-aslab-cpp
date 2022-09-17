#include <chrono>
#include <iostream>

#include "broadphase/broadphase_structure.hpp"
#include "broadphase/bruteforce.hpp"

#include "narrowphase/collision_shape.hpp"
#include "primitives/plane.hpp"
#include "primitives/triangle3.hpp"
#include "vec3.hpp"

#include <set>
#include <vector>

struct indexed_geom : public throttle::geometry::collision_shape<float> {
  unsigned index;
  indexed_geom(unsigned idx, auto &&base) : collision_shape{base}, index{idx} {};
};

int main() {
  using triangle = throttle::geometry::triangle3<float>;

  unsigned n;
  if (!(std::cin >> n)) {
    std::cout << "Can't read number of triangles\n";
    return 1;
  }

  throttle::geometry::bruteforce<float, indexed_geom> bruteforce{n};
  for (unsigned i = 0; i < n; ++i) {
    triangle temp;
    std::cin >> temp.a[0] >> temp.a[1] >> temp.a[2] >> temp.b[0] >> temp.b[1] >> temp.b[2] >> temp.c[0] >> temp.c[1] >>
        temp.c[2];
    bruteforce.add_collision_shape({i, temp});
  }

  for (const auto v : bruteforce.many_to_many())
    std::cout << v->index << " ";
}