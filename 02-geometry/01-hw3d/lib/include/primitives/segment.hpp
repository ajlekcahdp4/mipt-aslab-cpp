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

template <typename T> struct segment {
  using vec_type = vec3<T>;
  using point_type = point3<T>;

  point_type m_a;
  point_type m_b;

  segment(const point_type &p_start, const vec_type &p_dir) : m_a{p_start}, m_b{p_start + p_dir} {}
};

} // namespace geometry
} // namespace throttle
