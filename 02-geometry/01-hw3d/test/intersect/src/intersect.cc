#include <chrono>
#include <iostream>

#include "primitives/aabb.hpp"
#include "primitives/plane.hpp"
#include "primitives/triangle3.hpp"
#include "vec3.hpp"

#include <vector>
#include <set>

int main() {
  using triangle = throttle::geometry::triangle3<float>;
  std::vector<triangle> vec;

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
    vec.push_back(temp);
  }

  std::set<unsigned> in_contact;
  for (unsigned i = 0; i < vec.size(); ++i) {
    for (unsigned j = i; j < vec.size(); ++j) {
      bool intersect = throttle::geometry::triangle_triangle_intersect(vec[i], vec[j]);
      if (intersect) {
        in_contact.insert(i);
        in_contact.insert(j);
      }
    }
  }

  for (const auto &v: in_contact) std::cout << v << " ";
}