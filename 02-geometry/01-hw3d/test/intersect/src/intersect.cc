#include <chrono>
#include <iostream>

#include "vec3.hpp"
#include "primitives/plane.hpp"
#include "primitives/aabb.hpp"
#include "primitives/triangle3.hpp"

int main() {
  using triangle = throttle::geometry::triangle3<float>;
  triangle a{{-5, -5, 0}, {5, -5, 0}, {0, 10, 0}};
  triangle b{{-10, 0, -10}, {10, 0, 10}, {0, 0, 20}};
  std::cout << throttle::geometry::triangle_triangle_intersect(a, b);
}