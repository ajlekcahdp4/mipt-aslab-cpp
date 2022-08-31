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

template <typename T> struct triangle {
  using point_type = point3<T>;
  using vec_type = vec3<T>;
  using plane_type = plane<T>;

  point_type m_a;
  point_type m_b;
  point_type m_c;

  plane_type plane() { return plane_type{m_a, m_b, m_c}; }
};

} // namespace geometry
} // namespace throttle
