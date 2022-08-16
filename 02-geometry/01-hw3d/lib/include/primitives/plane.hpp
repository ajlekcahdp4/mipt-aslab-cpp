/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#include <cmath>

#include "point3.hpp"
#include "vec3.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct plane {
  using vec_type = vec3<T>;
  using point_type = point3<T>;

  vec_type m_normal; // Normalized normal vector
  T m_dist;          // Distance from origin

  plane() : m_normal{}, m_dist{} {}
  plane(const point_type &p_point, const vec_type &p_u, const vec_type &p_v) {}

  plane(const point_type &p_a, const point_type &p_b, const point_type &p_c) {}
};

} // namespace geometry
} // namespace throttle
