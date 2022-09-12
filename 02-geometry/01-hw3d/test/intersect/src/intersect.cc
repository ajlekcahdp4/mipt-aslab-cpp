#include <chrono>
#include <iostream>

#include "vec3.hpp"
#include "primitives/plane.hpp"
#include "primitives/aabb.hpp"
#include "primitives/triangle3.hpp"

int main() {
  using triangle = throttle::geometry::triangle3<float>;
  triangle a{{0, 0, 0}, {0, 5, 0}, {5, 0, 0}};
  triangle b{{0, 3, 0}, {3, 0, 0}, {10, 10, 0}};
  throttle::geometry::triangle_triangle_intersect(a, b);
}