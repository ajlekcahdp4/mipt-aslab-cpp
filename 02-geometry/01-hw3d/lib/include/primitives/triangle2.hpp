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

#include <cmath>
#include <cassert>

#include "point2.hpp"
#include "vec2.hpp"
#include "equal.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct triangle2 {
  using point_type = point2<T>;
  using vec_type = vec2<T>;

  point_type a;
  point_type b;
  point_type c;

  bool operator==(const triangle2 &other) const { return (a == other.a && b == other.b && c == other.c); }
  bool operator!=(const triangle2 &other) const { return (*this == other); }

private:  
  T compute_distance_pair(const point_type& first, const point_type& second, const point_type &point) const {
    vec_type radius = point - first, dir = (second - first).norm(), norm = vec_type{dir.y, -dir.x};
    vec_type proj = dir * dot(dir, radius), perp_component = radius - proj;
    return (dot(norm, perp_component) > 0 ? perp_component.length() : -perp_component.length());
  }

public:
  bool point_in_triangle(const point_type &point) const {
    T dist_ab = compute_distance_pair(a, b, point);
    if (is_roughly_equal(dist_ab, T{0})) dist_ab = T{0};
    bool greater_eq = dist_ab >= 0;
    
    T dist_bc = compute_distance_pair(b, c, point);
    if (is_roughly_equal(dist_bc, T{0})) dist_ab = T{0};
    if ((greater_eq && dist_bc < 0) || (!greater_eq && dist_bc > 0)) return false;
    
    T dist_ca = compute_distance_pair(c, a, point);
    if (is_roughly_equal(dist_ca, T{0})) dist_ab = T{0};
    if ((greater_eq && dist_ca < 0) || (!greater_eq && dist_ca > 0)) return false;

    return true;
  }
};

} // namespace geometry
} // namespace throttle