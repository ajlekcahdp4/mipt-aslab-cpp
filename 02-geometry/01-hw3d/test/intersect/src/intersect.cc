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
  using collison_geom_type = throttle::geometry::collision_shape<float>;
  using triangle = throttle::geometry::triangle3<float>;
  std::vector<collison_geom_type> vec;

  throttle::geometry::bruteforce<float, indexed_geom> bruteforce;

  unsigned n;
  if (!(std::cin >> n)) {
    std::cout << "Can't read number of triangles\n";
    return 1;
  }

  vec.reserve(n);
  for (unsigned i = 0; i < n; ++i) {
    triangle temp;
    std::cin >> temp.a[0] >> temp.a[1] >> temp.a[2] >> temp.b[0] >> temp.b[1] >> temp.b[2] >> temp.c[0] >> temp.c[1] >>
        temp.c[2];
    bruteforce.add_collison_shape({i, temp});
    vec.push_back(temp);
  }

  std::set<unsigned> in_contact;
  for (unsigned i = 0; i < vec.size(); ++i) {
    for (unsigned j = i + 1; j < vec.size(); ++j) {
      bool intersect = vec[i].collide(vec[j]);
      if (intersect) {
        in_contact.insert(i);
        in_contact.insert(j);
      }
    }
  }

  for (const auto &v : in_contact)
    std::cout << v << " ";
}