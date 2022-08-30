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
#include <type_traits>

#include "vec3.hpp"

namespace throttle {
namespace geometry {

// Precision to be used for floating point comparisons
template <typename T> struct default_precision { static constexpr T m_prec = 1.0e-6f; };

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool is_roughly_equal(T p_first, T p_second, T p_precision = default_precision<T>::m_prec) {
  using std::abs;
  using std::max;

  T epsilon = p_precision;
  return (abs(p_first - p_second) <= epsilon * max({abs(p_first), abs(p_second), T{1.0}}));
};

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool is_roughly_equal(vec3<T> p_first, vec3<T> p_second, T p_precision = default_precision<T>::m_prec) {
  return is_roughly_equal(p_first.m_x, p_second.m_x, p_precision) &&
         is_roughly_equal(p_first.m_y, p_second.m_y, p_precision) &&
         is_roughly_equal(p_first.m_z, p_second.m_z, p_precision);
};

} // namespace geometry
} // namespace throttle
