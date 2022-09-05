#include <chrono>
#include <iostream>

#include "vec3.hpp"
#include "primitives/plane.hpp"
#include "narrowphase/triangletriangle.hpp"
#include "primitives/aabb.hpp"

int main() {
  using throttle::geometry::aabb;
  using throttle::geometry::point3;

  point3<float> a, b, c;
  std::cin >> a.x >> a.y >> a.z;
  std::cin >> b.x >> b.y >> b.z;
  std::cin >> c.x >> c.y >> c.z;

  aabb<float> box{a, b, c};
}