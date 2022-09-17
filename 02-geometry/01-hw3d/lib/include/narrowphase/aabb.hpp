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
#include <type_traits>

#include "equal.hpp"
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

  aabb(const point_type &p_center, T half_x, T half_y, T half_z)
      : m_center{p_center}, m_halfwidth_x{half_x}, m_halfwidth_y{half_y}, m_halfwidth_z{half_z} {}

  aabb(const point_type &first, const point_type &second)
      : m_center{first + T{0.5f} * (second - first)}, m_halfwidth_x{std::abs(first.x - second.x) / T{2.0f}},
        m_halfwidth_y{std::abs(first.y - second.y) / T{2.0f}}, m_halfwidth_z{std::abs(first.z - second.z) / T{2.0f}} {}

  template <typename... Ts, typename = std::enable_if_t<std::conjunction_v<std::is_convertible<Ts, point_type>...>>>
  aabb(Ts... points)
      : aabb(point_type{vmin(points.x...), vmin(points.y...), vmin(points.z...)},
             point_type{vmax(points.x...), vmax(points.y...), vmax(points.z...)}) {}

  bool intersect(aabb a) const {
    if (is_definitely_greater(std::abs(m_center.x - a.m_center.x), (m_halfwidth_x + a.m_halfwidth_x))) return false;
    if (is_definitely_greater(std::abs(m_center.y - a.m_center.y), (m_halfwidth_y + a.m_halfwidth_y))) return false;
    if (is_definitely_greater(std::abs(m_center.z - a.m_center.z), (m_halfwidth_z + a.m_halfwidth_z))) return false;
    return true;
  }
};

} // namespace geometry
} // namespace throttle
