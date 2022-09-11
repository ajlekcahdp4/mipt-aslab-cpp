/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "equal.hpp"
#include "point2.hpp"
#include "vec2.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct segment2 {
  using vec_type = vec2<T>;
  using point_type = point2<T>;

  point_type a;
  point_type b;

  segment2(const point_type &p_a, const point_type &p_b) : a{p_a}, b{p_b} {}

  T signed_distance(const point_type &p_point) const {
    vec_type radius = p_point - b, dir = (b - a).norm(), norm = dir.perp();
    vec_type proj = dir * dot(dir, radius), perp_component = radius - proj;
    return (dot(norm, perp_component) > 0 ? perp_component.length() : -perp_component.length());
  }

  bool contains(const point_type &point) const {
    vec_type segment_vec = b - a, vec = b - point;
    return (co_directional(segment_vec, vec) && is_roughly_greater_eq(segment_vec.length_sq(), vec.length_sq()));
  }
};

template <typename T> bool operator==(const segment2<T> &lhs, const segment2<T> &rhs) {
  return (lhs.a == rhs.a && lhs.b == rhs.b);
}

template <typename T> bool test_2d_segment_segment(const segment2<T> &seg1, const segment2<T> &seg2) {
  auto r = seg1.b - seg1.a;
  auto s = seg2.b - seg2.a;

  if (seg1 == seg2) return true;

  auto p = seg1.a - point2<T>::origin();
  auto q = seg2.a - point2<T>::origin();

  auto dir_cross = perp_dot(r, s);
  auto diff_start_cross = perp_dot(q - p, r);

  if (dir_cross == 0) {
    if (diff_start_cross == 0) {
      auto t0 = dot(q - p, r) / r.length_sq();
      auto t1 = t0 + dot(s, r) / r.length_sq();

      if ((is_roughly_greater_eq(t0, 0.0f) && is_roughly_less_eq(t0, 1.0f)) ||
          (is_roughly_greater_eq(t1, 0.0f) && is_roughly_less_eq(t1, 1.0f)))
        return true;
    }
    return false;
  }

  auto t = perp_dot(q - p, s) / perp_dot(r, s);
  auto u = perp_dot(q - p, r) / perp_dot(r, s);
  if ((is_roughly_greater_eq(t, 0.0f) && is_roughly_less_eq(t, 1.0f)) &&
      (is_roughly_greater_eq(u, 0.0f) && is_roughly_less_eq(u, 1.0f)))
    return true;

  return false;
}

} // namespace geometry
} // namespace throttle