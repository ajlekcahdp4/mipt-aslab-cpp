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
  T          m_halfwidth_x;
  T          m_halfwidth_y;
  T          m_halfwidth_z;

  bool test_intersect(aabb a) const {
    if (std::abs(m_center.x - a.m_center.x) > (m_halfwidth_x + a.m_halfwidth_x)) return false;
    if (std::abs(m_center.y - a.m_center.y) > (m_halfwidth_y + a.m_halfwidth_y)) return false;
    if (std::abs(m_center.z - a.m_center.z) > (m_halfwidth_z + a.m_halfwidth_z)) return false;
    return true;
  }

  bool operator==(const aabb &b) {
    return (m_center == b.m_center && m_halfwidth_x == b.m_halfwidth_x && m_halfwidth_y == b.m_halfwidth_y &&
            m_halfwidth_z == b.m_halfwidth_z);
  }

  bool operator!=(const aabb &b) { return !(*this == b); }
};

// return true if AABBs intersect.
template <typename T> bool aabb_aabb_intersect(aabb<T> a, aabb<T> b) { return a.test_intersect(b); }

} // namespace geometry
} // namespace throttle
