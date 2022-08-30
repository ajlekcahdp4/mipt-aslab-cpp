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

private:
  vec_type m_normal; // Normalized normal vector
  T m_dist;          // Distance from origin

public:
  plane(const point_type &p_point, const vec_type &p_normal)
      : m_normal{p_normal.norm()}, m_dist{dot(p_point - point_type{0, 0, 0}, m_normal)} {}

  plane(const point_type &p_point, const vec_type &p_u, const vec_type &p_v) : plane{p_point, cross(p_u, p_v).norm()} {}
  plane(const point_type &p_a, const point_type &p_b, const point_type &p_c) : plane{p_a, p_b - p_a, p_c - p_a} {}

  T signed_distance_to(const point3<T> &p_point) const {
    return dot(p_point - typename plane<T>::point_type{0, 0, 0}, m_normal) - m_dist;
  }

  T distance_to(const point3<T> &p_point) const {
    return std::abs(signed_distance_to(p_point));
  }

  T distance_to_origin() const {
    return std::abs(m_dist);
  }

  vec_type normal() const {
    return m_normal;
  }
};

template <typename T> T signed_distance_from_plane(const plane<T> &p_plane, const point3<T> &p_point) {
  return p_plane.signed_distance_to(p_point);
}

template <typename T> T distance_from_plane(const plane<T> &p_plane, const point3<T> &p_point) {
  return p_plane.distance_to(p_point);
}

} // namespace geometry
} // namespace throttle
