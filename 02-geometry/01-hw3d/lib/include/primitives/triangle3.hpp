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

#include <cmath>

#include "primitives/plane.hpp"

#include "point3.hpp"
#include "vec3.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct triangle3 {
  using point_type = point3<T>;
  using vec_type = vec3<T>;
  using plane_type = plane<T>;

  point_type a;
  point_type b;
  point_type c;

  plane_type plane_of() const { return plane_type{a, b, c}; }

  bool lies_on_one_side(const plane_type &p_plane) const { return lie_on_the_same_side(p_plane, a, b, c); }
};

} // namespace geometry
} // namespace throttle
