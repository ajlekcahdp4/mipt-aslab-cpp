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

#include "aabb.hpp"
#include "point3.hpp"
#include "primitives/segment3.hpp"
#include "primitives/triangle3.hpp"

#include <variant>

namespace throttle {
namespace geometry {

template <typename T> class collision_shape {
  using segment_type = segment3<T>;
  using point_type = point3<T>;
  using triangle_type = triangle3<T>;
  using aabb_type = aabb<T>;

  std::variant<segment_type, point_type, triangle_type> m_shape;
  aabb_type                                             m_aabb;

public:
  collision_shape(const segment_type &seg) : m_shape{seg}, m_aabb{seg.a, seg.b} {}
  collision_shape(const point_type &point) : m_shape{point}, m_aabb{point} {}
  collision_shape(const triangle_type &tri) : m_shape{tri}, m_aabb{tri} {}

private:
public:
  bool intersect(const collision_shape &other) const {}
};

} // namespace geometry
} // namespace throttle