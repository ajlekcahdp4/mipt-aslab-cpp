/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "primitives/plane.hpp"
#include "primitives/triangle3.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <utility>

namespace throttle {
namespace geometry {

namespace detail {
template <typename T> std::pair<T, T> compute_interval(T p_a, T p_b, T p_c, T d_a, T d_b, T d_c) {
  assert(!are_same_sign(d_a, d_b, d_c));
  if (are_same_sign(d_a, d_c)) {}
}

// Rearrange triangle vertices
template <typename T> triangle3<T> canonical_triangle(const triangle3<T> &p_tri, std::array<T, 3> &p_dist) {
#ifndef NDEBUG
  auto arr = p_dist;
  std::sort(arr.begin(), arr.end());
  assert(arr[0] != 0);
  assert(arr[1] != 0);
  assert(arr[2] != 0);
#endif

  auto greater_count = std::count_if(p_dist.begin(), p_dist.end(), [](T elem) { return elem > 0; });
  switch (greater_count) {
  case 1: break; // clang-format off
  case 2: { std::for_each(p_dist.begin(), p_dist.end(), [](T &elem) { elem *= -1; }); break; } // clang-format on
  default: throw std::invalid_argument("Elements of distance array should all be of different signs.");
  }

  auto max_index = std::distance(p_dist.begin(), std::max_element(p_dist.begin(), p_dist.end()));
  switch (max_index) {
  case 0: return triangle3<T>{p_tri.a, p_tri.b, p_tri.c};
  case 1: return triangle3<T>{p_tri.b, p_tri.a, p_tri.c};
  case 2: return triangle3<T>{p_tri.c, p_tri.a, p_tri.b};
  }
}
} // namespace detail

template <typename T> bool triangle_triangle_intersect(const triangle3<T> &t1, const triangle3<T> &t2) {
  // 1. Compute the plane pi1 of the first triangle
  auto pi1 = t1.plane_of();

  // 2. Compute djstances from t2 to pi1
  T d_2_a = pi1.signed_distance(t2.a);
  T d_2_b = pi1.signed_distance(t2.b);
  T d_2_c = pi1.signed_distance(t2.c);

  // 3. Rejection test. If none of the points lie on the plane and all distances have the same sign, then triangles
  // can't intersect.
  if (!are_all_roughly_zero(d_2_a, d_2_b, d_2_c) && are_same_sign(d_2_a, d_2_b, d_2_c)) {
    return false;
  }

  // 4. Same as 1
  auto pi2 = t2.plane_of();

  // 5. Compute djstances from t1 to pi2
  T d_1_a = pi1.signed_distance(t1.a);
  T d_1_b = pi1.signed_distance(t1.b);
  T d_1_c = pi1.signed_distance(t1.c);

  // 5. Rejection test. If none of the points lie on the plane and all distances have the same sign, then triangles
  // can't intersect.
  if (!are_all_roughly_zero(d_1_a, d_1_b, d_1_c) && are_same_sign(d_1_a, d_1_b, d_1_c)) {
    return false;
  }

  // 6. Same as 3
  if (are_all_roughly_zero(d_1_a, d_1_b, d_1_c)) {
    return false; // TODO[]: Placeholder
  }

  // 7. If we get here than all early rejection tests failed and 2 triangles intersect the plane of the other.
  auto d = cross(pi1.normal(), pi2.normal());
  auto [index, max] = d.max_component();

  T p_1_a = t1.a.get_at_index(index);
  T p_1_b = t1.b.get_at_index(index);
  T p_1_c = t1.c.get_at_index(index);

  T p_2_a = t2.a.get_at_index(index);
  T p_2_b = t2.b.get_at_index(index);
  T p_2_c = t2.c.get_at_index(index);
}

} // namespace geometry
} // namespace throttle