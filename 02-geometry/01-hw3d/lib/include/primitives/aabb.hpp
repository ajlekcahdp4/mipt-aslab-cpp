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

template <typename T> struct aabb {
  using vec_type = vec3<T>;
  using point_type = point3<T>;

  point_type m_center;
  T m_halfwidth_x;
  T m_halfwidth_y;
  T m_halfwidth_z;
};

} // namespace geometry
} // namespace throttle
