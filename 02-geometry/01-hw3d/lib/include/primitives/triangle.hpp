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

#include "point3.hpp"
#include "vec3.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct triangle {
  using point_type = point3<T>;

  point_type m_a;
  point_type m_b;
  point_type m_c;

  triangle() : m_a{}, m_b{}, m_c{} {}
  triangle(const point_type &p_a, const point_type &p_b, const point_type &p_c) : m_a{p_a}, m_b{p_b}, m_c{p_c} {}

  T signed_distance(const point_type &p_point) {}
  
  T distance(const point_type &p_point) {
    return std::abs(signed_distance(p_point));
  }
};

} // namespace geometry
} // namespace throttle
